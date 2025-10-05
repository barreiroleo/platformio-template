# Agent Guidelines for PlatformIO C++ Project

## Build Commands
- Build debug: `make build` or `pio run -e atmega328p_debug`
- Build release: `make build-release` or `pio run -e atmega328p_release`
- Run all tests: `make test` or `pio test --without-uploading`
- Run single test env: `pio test -e test_native` (environments: test_native, test_native_gtest, test_common, test_simavr)
- Install dependencies: `make gen` or `pio pkg install`
- Clean: `make clean` or `pio run --target clean`
- Coverage report: `make coverage` (generates HTML/JSON in `.coverage/`)

## Code Style
- C++17 standard required (`-std=c++17`)
- Use `#pragma once` for header guards
- Namespace structure: `namespace core::module` with closing comment `// namespace core::module`
- Types: PascalCase structs/classes, snake_case variables/functions, UPPER_CASE constants
- Includes: Local headers first (`"core/span.hpp"`), then system headers (`<Arduino.h>`)
- Use `constexpr` for compile-time wherever is feasible
- Prefer structured bindings and designated initializers when available
- Use Doxygen comments (`///` or `/** */`) for public API documentation
- Test functions: `test_descriptive_name()` for Unity, `TEST(Suite, Name)` for GTest.
- Use `auto` for complex types, explicit types when declaring variables
