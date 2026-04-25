# Contributing to DHT-IPFS Simulator

Thank you for your interest in contributing! This document outlines the process
for reporting issues, proposing features, and submitting pull requests.

---

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How to Report a Bug](#how-to-report-a-bug)
- [How to Request a Feature](#how-to-request-a-feature)
- [Development Setup](#development-setup)
- [Submitting a Pull Request](#submitting-a-pull-request)
- [Coding Style](#coding-style)

---

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you agree to abide by its terms.

---

## How to Report a Bug

1. Search [existing issues](../../issues) to avoid duplicates.
2. Open a new issue and fill in the **Bug Report** template.
3. Include:
   - A minimal, reproducible example or steps to reproduce
   - Your OS, compiler version, and CMake version
   - Expected behaviour vs. actual behaviour

---

## How to Request a Feature

1. Open an issue with the label **enhancement**.
2. Describe the motivation and proposed API or behaviour.
3. Be open to discussion — the maintainer may suggest alternative approaches.

---

## Development Setup

### Prerequisites

| Tool     | Minimum Version |
|----------|----------------|
| CMake    | 3.16           |
| GCC/Clang| C++17 support  |
| Git      | 2.x            |

### Build and test

```bash
git clone https://github.com/AwaisKhan-01/Distributed-Hash-Tables.git
cd Distributed-Hash-Tables
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build && ctest --output-on-failure
```

Catch2 is fetched automatically via CMake `FetchContent` — no manual installation required.

---

## Submitting a Pull Request

1. **Fork** the repository and create a feature branch from `main`:
   ```bash
   git checkout -b feature/my-feature
   ```
2. Make your changes, add tests where appropriate.
3. Ensure all tests pass:
   ```bash
   cmake -B build && cmake --build build && cd build && ctest
   ```
4. Keep commits focused and descriptive (`git commit -m "feat: add chord lookup optimisation"`).
5. Open a Pull Request against `main` and fill in the PR template.
6. Address review comments promptly.

---

## Coding Style

- **C++17** standard.
- Prefer `nullptr` over `NULL`, `std::filesystem` over OS-specific APIs.
- Keep header-only implementations inside include guards (`#pragma once` + `#ifndef`).
- Use descriptive variable and function names; avoid single-letter names except for loop counters.
- Run your code through the existing CI before marking a PR as ready for review.
