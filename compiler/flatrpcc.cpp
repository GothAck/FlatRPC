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

array<string, 18> baseTypeNames = {
  "None",
  "UType",
  "Bool",
  "Byte",
  "UByte",
  "Short",
  "UShort",
  "Int",
  "UInt",
  "Long",
  "ULong",
  "Float",
  "Double",
  "String",
  "Vector",
  "Obj",     // Used for tables & structs.
  "Union",
  "Array"
};

// Simple types only at the moment
array<string, 18> baseTypeEquivalents = {
  "",
  "",
  "bool",
  "char",
  "unsigned char",
  "short",
  "unsigned short",
  "int",
  "unsigned int",
  "long",
  "unsigned long",
  "float",
  "double",
  "std::string",
  "", // "std::vector<[ELEMENT]>",
  "",     // Used for tables & structs.
  "",
  "", // "std::array<[ELEMENT], [FIXED_LENGTH]>"
};

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
        obj["base_type"] = baseTypeNames[static_cast<size_t>(type->base_type)];
        obj["base_type_equivalent"] = baseTypeEquivalents[static_cast<size_t>(type->base_type)];
        obj["element"] = baseTypeNames[static_cast<size_t>(type->element)];
        obj["element_equivalent"] = baseTypeEquivalents[static_cast<size_t>(type->element)];
        obj["index"] = type->index;
        obj["fixed_length"] = type->fixed_length;

        if (baseTypeEquivalents[static_cast<size_t>(type->base_type)] == "")
          simple = false;

        if (
          baseTypeNames[static_cast<size_t>(type->element)] != "None" &&
          baseTypeEquivalents[static_cast<size_t>(type->element)] == ""
        )
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

        if (responsesData.find(response) == responsesData.end()) {
          auto nsn = nsnamed(response);
          nsn["index"] = serviceData["responses"].size();
          serviceData["responses"].push_back(nsn);
          obj["response"] = responsesData[response] = nsn;
        } else {
          obj["response"] = responsesData[response];
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
