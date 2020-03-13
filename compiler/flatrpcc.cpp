/**
 * Copyright (c) Greg "GothAck" Miell <flatrpc@greg.gothack.ninja>
 * Licensed under the MIT License. See LICENSE file in the project root.
 */
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>
#include <set>
#include <map>

#include <docopt/docopt.h>
#include <inja/inja.hpp>

// Just for convenience
using namespace inja;
using json = nlohmann::json;

#include "reflection_generated.h"
#include "flatrpcc_inja/flatrpc.hpp.inja.h"
#include "flatrpcc_inja/flatrpc.cpp.inja.h"

#include "flatrpc/rpcversion.hpp"

using namespace std;
namespace fs = std::filesystem;

static const char USAGE[] =
R"(Flatrpc service generator

    Usage:
      flatrpc [options] <FILE>
      flatrpc (-h | --help)

    Options:
      <FILE>                    .fbs file containing the service(s) to generate.
      -o <dir> --output=<dir>   Output directory [Default: .].
      -h --help                 Show this screen.
)";

struct BaseTypeInfo {
  string name;
  string equivalent;
  bool direct;
  bool flatter_supported;
};

const array<const BaseTypeInfo, 18> baseTypes = {
  BaseTypeInfo {
    .name = "None",
    .equivalent = "",
    .direct = false,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "UType",
    .equivalent = "",
    .direct = false,
    .flatter_supported = false,
  },
  BaseTypeInfo {
    .name = "Bool",
    .equivalent = "bool",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Byte",
    .equivalent = "char",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "UByte",
    .equivalent = "unsigned char",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Short",
    .equivalent = "short",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "UShort",
    .equivalent = "unsigned short",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Int",
    .equivalent = "int",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "UInt",
    .equivalent = "unsigned int",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Long",
    .equivalent = "long",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "ULong",
    .equivalent = "unsigned long",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Float",
    .equivalent = "float",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Double",
    .equivalent = "double",
    .direct = true,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "String",
    .equivalent = "std::string",
    .direct = false,
    .flatter_supported = true,
  },
  BaseTypeInfo {
    .name = "Vector",
    .equivalent = "",
    .direct = false,
    .flatter_supported = false,
  },
  BaseTypeInfo {
    .name = "Obj",
    .equivalent = "",
    .direct = false,
    .flatter_supported = false,
  },
  BaseTypeInfo {
    .name = "Union",
    .equivalent = "",
    .direct = false,
    .flatter_supported = false,
  },
  BaseTypeInfo {
    .name = "Array",
    .equivalent = "",
    .direct = false,
    .flatter_supported = false,
  },
};

const string flatrpc_hpp_inja_tmpl(flatrpcc_inja_flatrpc_hpp_inja, flatrpcc_inja_flatrpc_hpp_inja + flatrpcc_inja_flatrpc_hpp_inja_len);
const string flatrpc_cpp_inja_tmpl(flatrpcc_inja_flatrpc_cpp_inja, flatrpcc_inja_flatrpc_cpp_inja + flatrpcc_inja_flatrpc_cpp_inja_len);

const string flatrpc_gitrev(to_string(FLATRPC_GITREV));

string tmpPath = fs::temp_directory_path() / "flatrpc-XXXXXX";

void atexitDeleteTmp() {
  fs::remove_all(tmpPath);
}

string cppnamespace(string fb_namespaced) {
  while (fb_namespaced.find(".") != string::npos)
    fb_namespaced.replace(fb_namespaced.find("."), 1, "::");
  return fb_namespaced;
}

string denamespace(string fb_namespaced) {
  if (fb_namespaced.rfind(".") != string::npos)
    fb_namespaced.replace(0, fb_namespaced.rfind(".") + 1, "");
  return fb_namespaced;
}

json nsnamed(string fb_namespaced) {
  json data = json::object();
  data["name"] = fb_namespaced;
  data["cpp"] = cppnamespace(fb_namespaced);
  data["local"] = denamespace(fb_namespaced);
  return data;
}

int main(int argc, char *argv[]) {
  std::map<std::string, docopt::value> args
    = docopt::docopt(USAGE, { argv + 1, argv + argc }, true, "", true);

  fs::path fileName = args["<FILE>"].asString();

  if (!fs::exists(fileName)) {
    cerr << "File " << fileName << " does not exist" << endl;
    return -1;
  }

  if (!mkdtemp(tmpPath.data())) {
    cerr << "Failed to create temporary directory";
  }
  atexit(atexitDeleteTmp);

  system((string("flatc --binary --schema -o ") + tmpPath + " " + args["<FILE>"].asString()).c_str());

  fs::current_path(args["--output"].asString());

  Environment env;

  std::ifstream t(tmpPath + "/" + fileName.stem().string() + ".bfbs", std::ios::binary);

  std::string str((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());

  flatbuffers::Verifier verifier ((const uint8_t *)str.data(), str.size());
  if (!verifier.VerifyBuffer<reflection::Schema>(0)) {
    cerr << "Verification of bfbs failed" << endl;
  }

  auto schema = reflection::UnPackSchema(str.data());
  json schemaData;

  {
    schemaData["filename"] = fileName.stem().string();
    schemaData["services"] = json::array();
    schemaData["flatrpc_gitrev"] = flatrpc_gitrev;
    schemaData["objects"] = json();

    for (auto object : schema->objects) {
      json objectData;
      json requestsData;
      json responsesData;
      objectData["name"] = object->name;
      objectData["local"] = denamespace(object->name);
      objectData["cpp"] = cppnamespace(object->name);
      objectData["fields"] = json::array();

      bool simple = true;

      for (auto field : object->fields) {
        auto name = field->name;
        auto type = field->type;
        map<string, string> attributes;

        for (auto &kv : field->attributes) {
          attributes[kv->key] = kv->value;
        }

        auto obj = json::object();
        obj["name"] = name;
        obj["local"] = denamespace(name);
        obj["cpp"] = cppnamespace(name);

        const auto &baseType = baseTypes[static_cast<size_t>(type->base_type)];
        const auto &elementType = baseTypes[static_cast<size_t>(type->element)];

        obj["base_type"] = baseType.name;
        obj["base_type_equivalent"] = baseType.equivalent;
        obj["base_type_direct"] = baseType.direct;
        obj["element"] = elementType.name;
        obj["element_equivalent"] = elementType.equivalent;
        obj["element_direct"] = elementType.direct;
        obj["index"] = type->index;
        obj["fixed_length"] = type->fixed_length;

        if (!(baseType.flatter_supported && elementType.flatter_supported))
          simple = false;

        obj["local"] = denamespace(name);
        obj["attributes"] = attributes;

        objectData["fields"].push_back(obj);
      }

      objectData["simple"] = simple;
      schemaData["objects"][object->name] = objectData;
    }

    for (auto service : schema->services) {
      json serviceData;
      json requestsData;
      json responsesData;
      json tablesData;
      bool abstractClient = false;
      for (auto &kv : service->attributes) {
        if (kv->key == "abstract_client") {
          abstractClient = true;
        }
      }
      serviceData["filename"] = fileName.stem().string();
      serviceData["name"] = service->name;
      serviceData["local"] = denamespace(service->name);
      serviceData["nameClient"] = denamespace(service->name) + "Client" + (abstractClient ? "Base" : "");
      serviceData["nameServer"] = denamespace(service->name) + "ServerBase";
      serviceData["calls"] = json::array();
      serviceData["requests"] = json::array();
      serviceData["responses"] = json::array();
      serviceData["tables"] = json::array();

      for (auto call : service->calls) {
        auto name = call->name;
        auto request = call->request->name;
        auto response = call->response->name;
        map<string, string> attributes;

        for (auto &kv : call->attributes) {
          attributes[kv->key] = kv->value;
        }

        auto obj = json::object();
        obj["name"] = name;
        obj["local"] = denamespace(name);
        obj["attributes"] = attributes;

        if (requestsData.find(request) == requestsData.end()) {
          auto nsn = nsnamed(request);
          nsn["index"] = serviceData["requests"].size();
          serviceData["requests"].push_back(nsn);
          obj["request"] = requestsData[request] = nsn;
        } else {
          obj["request"] = requestsData[request];
        }

        if (tablesData.find(request) == tablesData.end()) {
          serviceData["tables"].push_back(
            tablesData[request] = requestsData[request]);
        }

        if (responsesData.find(response) == responsesData.end()) {
          auto nsn = nsnamed(response);
          nsn["index"] = serviceData["responses"].size();
          serviceData["responses"].push_back(nsn);
          obj["response"] = responsesData[response] = nsn;
        } else {
          obj["response"] = responsesData[response];
        }

        if (tablesData.find(response) == tablesData.end()) {
          serviceData["tables"].push_back(
            tablesData[response] = responsesData[response]);
        }

        obj["request"]["object"] = schemaData["objects"][request];
        obj["response"]["object"] = schemaData["objects"][response];

        serviceData["calls"].push_back(obj);
      }

      cerr << service->name << endl;
      schemaData["services"].push_back(serviceData);
    }
  }

  {
    auto name = fileName.stem().string() + "_flatrpc.hpp";
    cerr << "Generating " << name << endl;
    std::ofstream out(name);
    out << env.render(flatrpc_hpp_inja_tmpl, schemaData);
  }

  {
    auto name = fileName.stem().string() + "_flatrpc.cpp";
    cerr << "Generating " << name << endl;
    std::ofstream out(name);
    out << env.render(flatrpc_cpp_inja_tmpl, schemaData);
  }

}
