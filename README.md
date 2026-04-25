# DHT-IPFS Simulator

[![CI](https://github.com/AwaisKhan-01/Distributed-Hash-Tables/actions/workflows/ci.yml/badge.svg)](https://github.com/AwaisKhan-01/Distributed-Hash-Tables/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)

> **Suggested repository name:** `dht-ipfs-simulator`
> (Rename in **GitHub → Settings → Repository name** when ready.)

A command-line simulator of a **Chord-style Distributed Hash Table (DHT)**
inspired by IPFS file-addressing semantics. Nodes are arranged on a logical
ring; files are hashed with SHA-1 and stored on the responsible node.
Routing uses **finger tables** (O(log N) lookup hops).

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build Instructions](#build-instructions)
- [Run Instructions](#run-instructions)
- [Usage Examples](#usage-examples)
- [Running Tests](#running-tests)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

Distributed Hash Tables are the backbone of peer-to-peer systems such as
BitTorrent, IPFS, and Ethereum's discovery layer. This simulator lets you
experiment with a **Chord ring** locally:

- Add/remove virtual nodes with configurable identifier-space sizes.
- Store and retrieve files by content hash.
- Inspect finger tables for each node.
- Observe how files migrate when nodes join or leave.

---

## Features

| Feature | Status |
|---------|--------|
| Chord ring with circular linked list | ✅ |
| SHA-1 content addressing | ✅ |
| Finger table (routing table) per node | ✅ |
| B-tree file index per node | ✅ |
| BigInt arithmetic (arbitrary-precision node IDs) | ✅ |
| Cross-platform (Linux / macOS / Windows) | ✅ |
| CMake build system | ✅ |
| Unit tests via Catch2 | ✅ |
| GitHub Actions CI | ✅ |
| Network simulation / actual P2P | 🔲 planned |
| Persistent storage | 🔲 planned |

---

## Architecture

```
┌─────────────────── DHT Ring (Chord) ───────────────────┐
│                                                         │
│   Node 3 ──► Node 10 ──► Node 20 ──► (back to Node 3) │
│     │            │            │                         │
│  BTree        BTree        BTree    ← file index        │
│  FingerTbl  FingerTbl  FingerTbl   ← O(log N) routing  │
└─────────────────────────────────────────────────────────┘
         ▲
         │  SHA-1(file content) mod 2^m
         │
      File input (txt / mp3)
```

**Key components:**

| Component | File | Description |
|-----------|------|-------------|
| `DHT_Ring` | `include/DHT_Ring.h` | Ring management, add/delete nodes, finger table updates |
| `machine` | `include/DHT_Ring.h` | Individual node holding a B-tree index and routing table |
| `BTree` | `include/Btree.h` | B-tree for storing file records on each node |
| `BigInt` | `include/BigInt.h` | Arbitrary-precision integer for node/file IDs |
| `SHA1` | `include/SHA1.h` | Pure C++ SHA-1 hash implementation |
| `Hash` | `include/Hash.h` | Helpers: hash string, text file, audio file |
| `Conversion` | `include/BigIntToDec.h` | Convert SHA-1 hex digest to decimal BigInt |
| `main` | `src/main.cpp` | Interactive CLI entry point |

**Lookup flow (Chord finger table):**
1. Hash the file content → decimal ID modulo 2^m.
2. Starting from the head node, walk the finger table entries.
3. Jump to the closest preceding node and repeat until the responsible node is found (O(log N) hops).

---

## Project Structure

```
.
├── .github/
│   └── workflows/
│       └── ci.yml          # GitHub Actions CI (Ubuntu + macOS)
├── include/                # Header-only library code
│   ├── BigInt.h
│   ├── BigIntToDec.h
│   ├── Btree.h
│   ├── DHT_Ring.h
│   ├── Hash.h
│   └── SHA1.h
├── src/
│   └── main.cpp            # Interactive CLI
├── tests/
│   └── test_dht.cpp        # Catch2 unit tests
├── CMakeLists.txt
├── .gitignore
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── LICENSE
├── README.md
└── SECURITY.md
```

---

## Prerequisites

| Tool | Minimum Version |
|------|----------------|
| CMake | 3.16 |
| GCC or Clang | C++17 support |
| Git | 2.x |

> Catch2 is fetched automatically via CMake `FetchContent` — no manual
> installation required.

---

## Build Instructions

```bash
# 1. Clone
git clone https://github.com/AwaisKhan-01/Distributed-Hash-Tables.git
cd Distributed-Hash-Tables

# 2. Configure (Release build)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build everything
cmake --build build --parallel
```

The build produces two artefacts inside `build/`:

| Artefact | Purpose |
|----------|---------|
| `dht_ipfs` | Interactive CLI simulator |
| `dht_tests` | Catch2 test runner |

---

## Run Instructions

```bash
./build/dht_ipfs
```

You will be prompted interactively:

```
Welcome to DHT-IPFS Simulator
Enter the number of bits for the identifier space:
> 5
Enter the number of machines you want to add:
> 3
1. Manually assign each ID
2. Auto-assign IDs (hash from machine name)
> 1
Enter the order of BTree:
> 3
...
```

---

## Usage Examples

### Add three nodes and store a file

```
Identifier space bits: 5  (ring size = 32)
Machines: 3
Assignment: manual

  Node IDs: 3, 10, 20

Ring:  3 -> 10 -> 20 -> (3)

Action: 2 (Add a file)
  Name: hello
  Type: txt
  Path: /home/user/docs

File hash mapped to ring position → stored on Node 10
```

### Auto-assign node IDs from name hashes

```
Assignment: 2 (auto)
Machine name: alpha
  SHA-1("alpha") → hex → decimal → mod 32 = 17
  Node 17 created.
```

### Print the DHT ring

```
Action: 1
DHT is: 3 -> 10 -> 17 -> 20 ->
```

### Print routing (finger) tables

```
Action: 6
machine 3  routing table values
  index 1 is machine: 10
  index 2 is machine: 10
  index 3 is machine: 17
  index 4 is machine: 20
  index 5 is machine: 3
```

---

## Running Tests

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
cd build && ctest --output-on-failure
```

Expected output:

```
All tests passed (27 assertions in 15 test cases)
```

---

## Roadmap

- [ ] **Networking layer** – replace in-memory ring with actual TCP sockets so
      nodes can run on separate machines or Docker containers.
- [ ] **Persistent storage** – serialize the B-tree and ring state to disk so
      the simulator can be stopped and resumed.
- [ ] **Replication** – store file replicas on successor nodes for fault tolerance.
- [ ] **Consistent hashing improvements** – virtual nodes for better load balance.
- [ ] **REST API** – expose DHT operations over HTTP for easy integration testing.
- [ ] **Visualization** – simple terminal or web-based ring diagram.
- [ ] **Rename repository** to `dht-ipfs-simulator` for clarity
      (Settings → General → Repository name).

---

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) before
opening a pull request.

---

## License

This project is licensed under the **MIT License** — see [LICENSE](LICENSE) for
details.
