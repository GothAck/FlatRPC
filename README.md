# ![FlatRPC](https://raw.githubusercontent.com/GothAck/FlatRPC/master/docs/flatrpc1.png)

## A C++ RPC library built upon FlatBuffers and ZeroMQ

### Why?
Because I needed an easy to use RPC library that was lightweight, simple, and didn't require building the world (shifty eyes at gRPC >_>).
This was born from inside of another project, please bear with me whilst I port all the features.

### What?
A pipe agnostic RPC library (anything that can transport ZeroMQ), using Flatbuffer reflection to generate RPC server stubs and clients.

### Can this be ported to language x?
Probably, feel free to contribute, I just need C++ though.

### But library xyz exists?!
Sure, can I give it a raw socket and make it dance in < 10 minutes?

### Documentation?
Coming, but for now please check out the example in the examples directory and my hasilty written [Quick Start](https://github.com/GothAck/FlatRPC/wiki/Quickstart) guide in the wiki

### TODO:
- Implement server side timeouts.
- Stop client crashing when server goes away.
- Reconnect clients
- Tunables
- Test performance.
- [Allow the?] use [of] non Native Table flatbuffers.
- Allow calling RPC methods with arguments instead of the request table.
- Tests
- Documentation
