# Netus

A minimal HTTP server and peer-to-peer networking framework written in C to understand the fundamentals of networking and systems programming.

Netus is built entirely from scratch — no external libraries, no frameworks. Every component, from the data structures to the thread pool to the socket server, is hand-written in C. The project explores how networking primitives work under the hood by implementing them directly on top of POSIX sockets.

## Features

- **TCP Server** — A bare-metal socket server with bind, listen, and accept
- **HTTP Server** — Route registration, request parsing, and template rendering built on top of the TCP server
- **Peer-to-Peer Node** — A P2P networking layer that combines a server and client, with automatic peer discovery and a binary block protocol
- **Thread Pool** — A work-stealing thread pool for handling concurrent connections
- **Custom Data Structures** — Linked lists, queues, binary search trees, and dictionaries implemented from scratch
- **Cryptography** — Arbitrarily precise integer arithmetic for future cryptographic operations
- **File I/O** — Utilities for reading, writing, and appending to files

## Project Structure

```
netus/
├── main.c                          # Entry point — launches the P2P node
├── CMakeLists.txt                  # Build configuration
├── LICENSE                         # MIT License
│
├── nodes/                          # Networking nodes
│   ├── server/                     # TCP server (socket, bind, listen, accept)
│   │   ├── server.c / server.h
│   │   └── test.c                  # Standalone server test
│   ├── http-server/                # HTTP layer (routing, request handling, templates)
│   │   └── http_server.c / http_server.h
│   ├── routes/                     # Route registration and HTTP method definitions
│   │   └── route.c / route.h
│   ├── clients/                    # TCP client (connect, send, receive)
│   │   └── client.c / client.h
│   └── p2p/                        # Peer-to-peer node (server + client + peer tracking)
│       └── PeerToPeer.c / PeerToPeer.h
│
├── protocols/                      # Protocol definitions
│   ├── http-request.c / http-request.h   # HTTP request parser
│   └── BlockHeaders.h              # Binary block protocol headers
│
├── dsa/                            # Data structures
│   ├── common/                     # Shared node type
│   │   └── node.c / node.h
│   ├── lists/                      # Linked list and queue
│   │   ├── linkedlist.c / linkedlist.h
│   │   └── queue.c / queue.h
│   ├── dictionary/                 # Key-value dictionary (BST-backed)
│   │   ├── dictionary.c / dictionary.h
│   │   └── entry.c / entry.h
│   └── trees/                      # Binary search tree
│       └── bst.c / bst.h
│
├── systems/                        # System utilities
│   ├── threadpool.c / threadpool.h # Thread pool with work queue
│   └── files.c / files.h          # File I/O helpers
│
├── cryptography/                   # Cryptographic primitives
│   └── multiprecision/             # Big integer arithmetic
│       └── ArbitrarilyPreciseInteger.c / ArbitrarilyPreciseInteger.h
│
└── lib/                            # Convenience headers (aggregate includes)
    ├── netus.h                     # Top-level include
    ├── dsa.h                       # All data structures
    ├── networking.h                # All networking components
    ├── systems.h                   # All system utilities
    └── cryptography.h              # All cryptography modules
```

## Architecture

The project follows a layered architecture where each layer builds on the one below it:

```
┌─────────────────────────────────────────────┐
│              main.c (P2P Application)       │
├─────────────────────────────────────────────┤
│              PeerToPeer Node                │
│         (server thread + client)            │
├──────────────────┬──────────────────────────┤
│   HTTP Server    │       Client             │
│  (routes, parse) │  (connect, send, recv)   │
├──────────────────┴──────────────────────────┤
│              TCP Server                     │
│       (socket, bind, listen, accept)        │
├─────────────────────────────────────────────┤
│  Thread Pool  │  Data Structures  │  Files  │
└───────────────┴───────────────────┴─────────┘
```

- The **TCP Server** wraps POSIX socket calls into a reusable struct with configurable domain, service, protocol, port, and backlog.
- The **HTTP Server** extends the TCP server with a route dictionary, request parsing, and a multithreaded connection handler via the thread pool.
- The **P2P Node** combines an HTTP server (for incoming requests) with a client (for outgoing requests), maintaining a linked list of known peers. It supports both text-based route queries and a binary block messaging protocol.

## Building

### Prerequisites

- GCC or Clang with C11 support
- CMake 4.1+
- POSIX-compatible system (Linux/macOS)

### Build

```bash
mkdir -p cmake-build-debug
cd cmake-build-debug
cmake ..
cmake --build . --target web_server
```

Or simply press **Build** (Ctrl+F9) in CLion.

### Run

```bash
./cmake-build-debug/web_server
```

You should see:

```
176, 176
Server running.
```

The P2P node is now listening on **port 8000**. The client reads from `stdin` — type a message and press Enter to broadcast it to all known peers.

## Usage

### Query Known Hosts

From another terminal, send a raw text request:

```bash
printf '/known_hosts\n' | nc -q 1 127.0.0.1 8000
```

Response:

```
127.0.0.1,
```

### Send a Message

Type directly into the running server's terminal. The message is packaged as a binary block with headers (previous hash, nonce, sender, timestamp, size) and broadcast to every known peer.

### Standalone HTTP Server Test

The `nodes/server/test.c` file contains a minimal standalone HTTP server that responds with a simple HTML page:

```bash
# Build and run separately (not included in the main build target)
gcc -o test_server nodes/server/server.c nodes/server/test.c -std=c11
./test_server
```

Then visit `http://127.0.0.1:8000` in your browser.

## Design Decisions

- **Object-oriented C** — Structs contain function pointers to emulate methods. Constructors return fully initialized structs. This mirrors how OOP patterns can be achieved in C.
- **No external dependencies** — Every data structure and system utility is written from scratch to maximize learning.
- **Void pointer generics** — Data structures use `void *` for type-agnostic storage, requiring the caller to cast appropriately on retrieval.
- **Thread pool** — The HTTP server uses a thread pool with a mutex-protected work queue and condition variables, rather than spawning a new thread per connection.

## License

[MIT](LICENSE) — Copyright (c) 2026 Fortunate
