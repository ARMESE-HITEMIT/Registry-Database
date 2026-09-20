# Registry Database

> **Version:** 1.0A (Alpha Release)
> **Status:** Experimental Build — use with caution.

Registry Database is a hierarchical key-value data storage system designed for structured data management and inter-process communication. It utilizes a client-server architecture over TCP/IP to ensure reliable, thread-safe data access across distributed components or local services.

---

## Table of Contents

1. [Overview](#overview)
2. [Core Features](#core-features)
3. [Architecture](#architecture)
4. [Critical Notice: Version 1.0A Alpha and Data Safety](#critical-notice-version-10a-alpha-and-data-safety)
5. [Usage Guide](#usage-guide)
   1. [Header Inclusions and Namespaces](#header-inclusions-and-namespaces)
   2. [Host Service Deployment (Running as a Separate Task)](#host-service-deployment-running-as-a-separate-task)
   3. [Path Syntax and Naming Constraints](#path-syntax-and-naming-constraints)
   4. [Local Service Implementation (`registry_editor-service-local.h`)](#local-service-implementation-registry_editor-service-localh)
   5. [Network Client Implementation (`registry_editor.h`)](#network-client-implementation-registry_editorh)
   6. [Complete Function and API Reference](#complete-function-and-api-reference)
      1. [File and Database Lifecycle](#file-and-database-lifecycle)
      2. [Single Element CRUD Operations](#single-element-crud-operations)
      3. [Batch Operations](#batch-operations)
      4. [Schema Initialization](#schema-initialization)
      5. [Hierarchy Inspection and Node Information](#hierarchy-inspection-and-node-information)
      6. [Real-time Event Subscription](#real-time-event-subscription)
      7. [Hierarchy Visualization and Export](#hierarchy-visualization-and-export)
   7. [Error Handling and ECC Architecture](#error-handling-and-ecc-architecture)
      1. [ECC Exception Handling](#ecc-exception-handling)
      2. [Registry Editor Error Code Reference Table](#registry-editor-error-code-reference-table)
6. [Dependencies](#dependencies)
   1. [Third-Party Attribution](#third-party-attribution)
   2. [License Compliance](#license-compliance)
7. [Reminders / Developer Notices](#reminders--developer-notices)
8. [License](#license)

---

## Overview

The system organizes data into a tree-like structure, similar to the Windows Registry, allowing for complex, nested configurations and state management. It provides a robust communication protocol for reading, modifying, and monitoring data states in real time without exposing the underlying physical storage mechanics to the client applications.

## Core Features

- Hierarchical Structure: Organizes information into logical paths and endpoints, supporting deep nesting and structured data grouping.
- Client-Server Architecture: Operates over network sockets (TCP) with built-in multiplexing, allowing multiple clients to interface with the database service concurrently.
- Strict Data Typing: Native implementation and enforcement of standard binary types, including:
  - `VALUE` : Variable-length string data (`std::string`)
  - `BYTE`  : 8-bit unsigned integer (`uint8_t`)
  - `WORD`  : 16-bit unsigned integer (`uint16_t`)
  - `DWORD` : 32-bit unsigned integer (`uint32_t`)
  - `QWORD` : 64-bit unsigned integer (`uint64_t`)
- Real-time Synchronization: Built-in event-driven update mechanisms allowing clients to subscribe to specific nodes and receive immediate state changes.
- Concurrency and Thread Safety: Utilizes robust locking mechanisms to handle concurrent read/write operations and prevent data races.
- Data Serialization: Supports structural extraction and structural views, including full JSON tree mapping for external integration and debugging.
- Batch Operations: Capability to process multiple key and value operations (Create, Read, Modify, Delete) within a single execution cycle to minimize network overhead.

## Architecture

Registry Database is split into three primary components:
1. **Host Service:** The central server daemon that loads and maintains the registry tree in memory, orchestrates disk I/O persistence, manages TCP socket connections (default port `41420`), and dispatches event notifications to subscribed clients.
2. **Client Library (`registry_editor.h`):** A lightweight client interface that provides developers with direct, strictly-typed functions to interact with the database host service over the local network / loopback socket layer.
3. **Service Library (`registry_editor-service-local.h`):** A direct, in-process engine providing the same interface directly against memory structures without socket transport overhead.
The communication layer handles automated error correction and status code lookups to maintain stable connectivity and predictable failure states during I/O operations.

## Critical Notice: Version 1.0A Alpha and Data Safety

> **WARNING: EXPERIMENTAL ALPHA RELEASE (1.0A)**

1. **Alpha Status and Known Limitations:** This codebase is an experimental 1.0A Alpha preview. It contains actively developed routines and may encounter unforeseen bugs, edge-case failures, or unhandled states.
2. **In-Memory Storage Model and Data Loss Risk:** The database engine maintains its working dataset entirely in volatile RAM for high-throughput associative access. Modifications are written to disk only when `save()` or `registry_end()` is explicitly called. Any unhandled crash, sudden process abort, or hardware power failure before saving will result in unrecoverable data loss.
3. **Subsystem Origin and Standalone Environment:** This engine was extracted directly from an internal operating environment where data safety, persistent transaction logging, process supervision, and replication were handled by higher-level supervisory layers. As a standalone library, it currently lacks built-in automated journaling, auto-recovery daemons, and transactional rollback mechanisms. Comprehensive self-protection features will be integrated in future releases.
4. **Local Service Safety Guard (`CONFIRMED_CAUTION`):** When using the local in-process service header (`registry_editor-service-local.h`), developers must explicitly `#define CONFIRMED_CAUTION` before inclusion. Failing to define this macro causes a compilation `#error`, enforcing developer awareness that local mode operates without host process isolation.

## Usage Guide

### Header Inclusions and Namespaces

The library provides two equivalent headers:

- **Network Client Interface:** Used when communicating with the separate host service over TCP/IP:

```cpp
  #include "Registry-Editor/include/registry_editor.h"

  // API resides in namespace: registry_editor
```

- **Local In-Process Interface:** Used for direct memory operations within a single application process
```cpp
  #define CONFIRMED_CAUTION // Mandatory safety acknowledgment macro

  #include "Registry-Editor/include/registry_editor-service-local.h"

  // API resides in namespace: registry_editor_service_local
```

Both interfaces expose identical method signatures, error semantics, and data types.
---

### Host Service Deployment (Running as a Separate Task)

When using `registry_editor.h`, the Host Service must run as an independent background task, service, or daemon prior to client connection.

- **Default Endpoint:** `127.0.0.1:41420` (TCP/IP).

- **Launch Example:**

```bash
  # Execute the Host Service binary as a dedicated task or service

  ./registry_database_host
```
- Once active, client calls to `registry_begin(...)` establish a socket connection automatically.

---

### Path Syntax and Naming Constraints

1. **Root Prefix Requirement:** Every registry path must begin with `*__root__*`. Passing a path without this prefix triggers an invalid path exception.

2. **Path Delimiter:** Sub-keys are delimited strictly by the `>` character.

   - Example: `*__root__*>Software>MyCompany>Application`

3. **Identifier Constraints:**

   - Length: Must be between 1 and 255 characters inclusive.

   - Permitted Characters: `a-z`, `A-Z`, `0-9`, `_`, `-`, `{`, `}`, `[`, `]`, `.`, `@`, `#`, `!`, `:`, `?`, `\~`.

   - Prohibited: Spaces, slashes (`/`, `\\`), backticks, quotes, and control characters are strictly forbidden.

---

### Local Service Implementation (registry_editor-service-local.h)

This example demonstrates direct local memory manipulation:

```cpp

#define CONFIRMED_CAUTION // Required before header inclusion

#include "Registry-Editor/include/registry_editor-service-local.h"

#include <iostream>

int main() {

    const std::string db_file = "local_storage.regx";

    try {

        // Step 1: Initialize a new physical file on disk

        registry_editor_service_local::create_registry_file(db_file);

        // Step 2: Open session and load structure into RAM

        auto* editor = registry_editor_service_local::registry_begin(db_file);

        // Step 3: Create key hierarchy

        editor->create_key("Hardware", "*__root__*");

        editor->create_key("GPU", "*__root__*>Hardware");

        // Step 4: Write typed attributes

        editor->create_value("DeviceName", "RTX 4090", "*__root__*>Hardware>GPU");

        editor->create_dword("VRAM_MB", 24576, "*__root__*>Hardware>GPU");

        editor->create_byte("IsPCIe5", 1, "*__root__*>Hardware>GPU");

        // Step 5: Read attributes back

        std::string name = editor->read_value("DeviceName", "*__root__*>Hardware>GPU");

        DWORD vram = editor->read_dword("VRAM_MB", "*__root__*>Hardware>GPU");

        std::cout << "Device: " << name << " (" << vram << " MB)\\\n";

        // Step 6: Flush changes from RAM to physical disk

        editor->save();

        // Step 7: Close session and release allocated memory

        registry_editor_service_local::registry_end(editor);

    }

    catch (const platform_core::ecc& e) {

        std::cerr << "Local Registry ECC Error: Service=" << e.service_id

                  << ", Code=" << e.error_code << "\\\n";

    }

    return 0;

}
```

---

### Network Client Implementation (registry_editor.h)

This example connects over TCP to the separate Host Service process:

```cpp

#include "Registry-Editor/include/registry_editor.h"

#include <iostream>

int main() {

    const std::string db_file = "network_storage.regx";

    try {

        // Step 1: Instruct Host to format a new registry file

        registry_editor::create_registry_file(db_file);

        // Step 2: Connect over TCP (127.0.0.1:41420) and begin session

        registry_editor::registry_editor* client = registry_editor::registry_begin(db_file);

        // Step 3: Perform operations through TCP protocol

        client->create_key("ServerSettings", "*__root__*");

        client->create_word("Port", 41420, "*__root__*>ServerSettings");

        client->create_qword("MaxConnections", 10000ULL, "*__root__*>ServerSettings");

        // Step 4: Query server state

        WORD port = client->read_word("Port", "*__root__*>ServerSettings");

        std::cout << "Server Listening on Port: " << port << "\\\n";

        // Step 5: Request Host to commit in-memory tree to disk

        client->save();

        // Step 6: Terminate network session and free client resources

        registry_editor::registry_end(client);

    }

    catch (const platform_core::ecc& e) {

        std::cerr << "Client TCP ECC Error: Service=" << e.service_id

                  << ", Code=" << e.error_code << "\\\n";

    }

    return 0;

}
```

---

### Complete Function and API Reference

Both interfaces provide the following member functions on the editor pointer:

#### File and Database Lifecycle

- `create_registry_file(path)`: Writes an empty binary database format with signature headers.

- `registry_begin(path)`: Loads file into memory, manages reference counting, and returns instance pointer.

- `save()`: Flushes the in-memory tree state to the persistent `.regx` disk file.

- `registry_end(handle)`: Saves pending state, decrements reference counter, closes sockets/files, and frees memory.

- `delete_registry_file(path)`: Removes the database file from disk.

```cpp
// Lifecycle example

registry_editor::create_registry_file("config.regx");

auto* db = registry_editor::registry_begin("config.regx");

// Commit in-memory data to disk

db->save();

// Clean termination and resource deallocation

registry_editor::registry_end(db);
```

#### Single Element CRUD Operations

- `create_key(key_name, path)`: Creates a new child key under target path.

- `create_value(name, str_val, path)`: Creates a string `VALUE` node.

- `create_byte(name, byte_val, path)`: Creates an 8-bit unsigned `BYTE` node.

- `create_word(name, word_val, path)`: Creates a 16-bit unsigned `WORD` node.

- `create_dword(name, dword_val, path)`: Creates a 32-bit unsigned `DWORD` node.

- `create_qword(name, qword_val, path)`: Creates a 64-bit unsigned `QWORD` node.

- `modify_value(name, path, new_str)`: Updates existing string value.

- `modify_byte(name, path, new_byte)`: Updates existing byte value.

- `modify_word(name, path, new_word)`: Updates existing word value.

- `modify_dword(name, path, new_dword)`: Updates existing dword value.

- `modify_qword(name, path, new_qword)`: Updates existing qword value.

- `read_value(name, path)`: Reads `std::string`.

- `read_byte(name, path)`: Reads `BYTE` (`uint8_t`).

- `read_word(name, path)`: Reads `WORD` (`uint16_t`).

- `read_dword(name, path)`: Reads `DWORD` (`uint32_t`).

- `read_qword(name, path)`: Reads `QWORD` (`uint64_t`).

- `read_element(name, path)`: Reads node into `std::variant<std::string, BYTE, WORD, DWORD, QWORD>`.

- `delete_element(name, path)`: Deletes specified child key or value.

- `rename(path, old_name, new_name)`: Renames an existing key or value within the path.

```cpp
// Creation

db->create_key("Security", "*__root__*");

db->create_dword("MaxAttempts", 5, "*__root__*>Security");

db->create_value("AdminUser", "root", "*__root__*>Security");

// Modification

db->modify_dword("MaxAttempts", "*__root__*>Security", 3);

// Generic read via variant

auto elem = db->read_element("AdminUser", "*__root__*>Security");

if (std::holds_alternative<std::string>(elem)) {

    std::cout << "User: " << std::get<std::string>(elem) << "\\\n";

}

// Rename and Delete

db->rename("*__root__*>Security", "AdminUser", "SuperUser");

db->delete_element("SuperUser", "*__root__*>Security");
```

#### Batch Operations

Batch operations process multiple items within a single lock acquisition or network packet to maximize performance:

- `create_multiple_key(vector<string>, path)`

- `create_multiple_value(vector<pair<string, string>>, path)`

- `create_multiple_byte(vector<pair<string, BYTE>>, path)`

- `create_multiple_word(vector<pair<string, WORD>>, path)`

- `create_multiple_dword(vector<pair<string, DWORD>>, path)`

- `create_multiple_qword(vector<pair<string, QWORD>>, path)`

- `modify_multiple_value(...)`, `modify_multiple_byte(...)`, etc.

- `read_multiple_value(...)`, `read_multiple_byte(...)`, etc.

- `delete_multiple_element(vector<string>, path)`

- `rename_multiple(path, vector<pair<string, string>>)`

```cpp
// Batch creation of keys

db->create_multiple_key({"Core", "Display", "Audio"}, "*__root__*");

// Batch creation of numerical metrics

std::vector<std::pair<std::string, DWORD>> display_cfg = {

    {"Width", 2560},

    {"Height", 1440},

    {"RefreshRate", 165}

};

db->create_multiple_dword(display_cfg, "*__root__*>Display");

// Batch reading

auto results = db->read_multiple_dword({"Width", "Height"}, "*__root__*>Display");
```

#### Schema Initialization

Heterogeneous batches containing mixed keys, strings, and integer types can be created together using `initialization`:

```cpp
using namespace registry_editor;

std::vector<std::variant<INIT_KEY, INIT_VALUE, INIT_BYTE, INIT_WORD, INIT_DWORD, INIT_QWORD>> schema = {

    INIT_KEY{"SubEngine"},

    INIT_VALUE{"Renderer", "DirectX12"},

    INIT_BYTE{"EnableVSync", 1},

    INIT_WORD{"FOV", 90},

    INIT_DWORD{"TextureQuality", 3},

    INIT_QWORD{"MemoryBudget", 8589934592ULL}

};

db->initialization(schema, "*__root__*>Display");
```

#### Hierarchy Inspection and Node Information

- `contains(name, path)`: Returns boolean indicating whether child exists.

- `get_type(name, path)`: Returns node type (`type::key`, `type::string`, `type::byte`, etc.).

- `list(path)`: Returns `std::vector<std::string>` containing all immediate child names.

- `properties(path)`: Returns a map/collection of pairs containing child names and their respective types.

```cpp
if (db->contains("Width", "*__root__*>Display")) {

    type t = db->get_type("Width", "*__root__*>Display");

    if (t == type::dword) {

        std::cout << "Width is a 32-bit unsigned integer\\\n";

    }

}

// List all children

std::vector<std::string> children = db->list("*__root__*>Display");

for (const auto& name : children) {

    std::cout << "Child Node: " << name << "\\\n";

}
```

#### Real-time Event Subscription

Clients can register callbacks invoked asynchronously when data changes occur:

```cpp
// Register notification listener

QWORD listener_token = db->update([]() {

    std::cout << "Event: Database node modified or created\\\n";

});

// Trigger change

db->create_byte("Flag", 1, "*__root__*");

// Unregister listener

db->unupdate(listener_token);
```

#### Hierarchy Visualization and Export

- `tree()`: Generates a formatted ASCII tree structure string.

- `json_tree()`: Generates a full JSON document mapping keys, values, and schema hierarchy.

```cpp
// Print ASCII representation

std::cout << db->tree() << "\\\n";

// Export JSON representation

std::string json_output = db->json_tree();

std::cout << json_output << "\\\n";
```

---

### Error Handling and ECC Architecture

#### ECC Exception Handling

In Version 1.0A Alpha, the database utilizes custom Error Correction Code structures (`platform_core::ecc`). Violations of constraints, corrupted files, and socket drops throw `platform_core::ecc`.

```cpp
try {

    // Attempting invalid operation

    db->read_dword("NonExistent", "*__root__*");

}

catch (const platform_core::ecc& err) {

    std::cerr << "Service ID: " << err.service_id << "\\\n";

    std::cerr << "Error Code: " << err.error_code << "\\\n";

}
```

#### Registry Editor Error Code Reference Table

For exceptions where `service_id == 2` (`global.registry_editor`):

| Error Code | Constant ID                        | Description / Cause                                                   |
| :--------: | :--------------------------------- | :-------------------------------------------------------------------- |
|      0     | `exception.invalid_path`           | Target node is not a key container or has no child storage.           |
|      1     | `exception.invalid_path`           | Target element name does not exist at path.                           |
|      2     | `exception.invalid_path`           | Target element is not a string/value.                                 |
|      3     | `exception.invalid_path`           | Target element is not a byte.                                         |
|      4     | `exception.invalid_path`           | Target element is not a word.                                         |
|      5     | `exception.invalid_path`           | Target element is not a dword.                                        |
|      6     | `exception.invalid_path`           | Target element is not a qword.                                        |
|      7     | `exception.invalid_path`           | Path string is empty.                                                 |
|      8     | `exception.invalid_path`           | Path is missing required `*__root__*` prefix or delimiter formatting. |
|      9     | `exception.invalid_path`           | Key, value, or element already exists.                                |
|     10     | `exception.file_system`            | File not found.                                                       |
|     11     | `exception.file_system`            | Corrupted file or decode mismatch.                                    |
|     12     | `exception.file_system`            | Cannot open file for reading or writing.                              |
|     13     | `exception.file_system`            | Invalid file signature or header magic.                               |
|     14     | `exception.invalid_path`           | Element is a key when a value was expected, or null pointer error.    |
|     15     | `exception.invalid_name`           | Name contains characters outside the permitted character set.         |
|     16     | `exception.invalid_name`           | Name length is outside the valid range of 1 to 255 characters.        |
|     17     | `exception.invalid_object`         | Target editor session handle does not exist or double-free detected.  |
|     18     | `critical.connection_closed`       | IPC communication drop.                                               |
|     19     | `critical.connection_closed`       | Cannot connect to remote host.                                        |
|     20     | `exception.memory_allocation`      | Memory allocation failure: cannot allocate buffer during load.        |
|     21     | `exception.memory_allocation`      | Memory allocation failure: cannot allocate buffer during modify.      |
|     22     | `exception.memory_allocation`      | Memory allocation failure: cannot allocate buffer during create.      |
|     23     | `critical.socket_receive_failed`   | Socket receive failed or communication timeout.                       |
|     24     | `critical.socket_send_failed`      | Socket send failed.                                                   |
|     25     | `critical.unexpected_value_type`   | Socket packet encountered unexpected `VALUE_TYPE` enum.               |
|     26     | `critical.expected_byte`           | Socket packet expected `BYTE` data payload.                           |
|     27     | `critical.expected_word`           | Socket packet expected `WORD` data payload.                           |
|     28     | `critical.expected_dword`          | Socket packet expected `DWORD` data payload.                          |
|     29     | `critical.expected_qword`          | Socket packet expected `QWORD` data payload.                          |
|     30     | `critical.expected_string`         | Socket packet expected `DATA` (string) payload.                       |
|     31     | `critical.packet_truncated`        | Incomplete packet received over network socket.                       |
|     32     | `critical.packet_corrupted`        | Malformed or corrupted network packet.                                |
|     33     | `critical.invalid_enum_value`      | Network operation decoded an invalid operation enum.                  |
|     34     | `critical.string_length_too_large` | Decoded string length exceeds maximum allowable boundary.             |
|     35     | `exception.host_controller`        | Host server instance is already running.                              |
|     36     | `exception.host_controller`        | Host server instance is already running.                              |
|     37     | `exception.host_controller`        | Cannot find server host.                                              |


---

## Dependencies

Registry Database is implemented primarily as a native C++ system and may rely on a combination of third-party libraries, internal project libraries, platform APIs, and facilities provided by the C++ standard library. Dependencies are used internally to provide specific implementation capabilities such as high-performance associative storage, network communication, synchronization, system interaction, error handling, and data processing.

The dependency set is not intended to expose the internal implementation of the database to client applications. In particular, the physical storage representation, internal containers, memory management strategy, synchronization mechanisms, and platform-specific operations may change between releases as long as the supported public API and registry-file compatibility requirements are preserved.

| Dependency                                                                                                     | Type                        | Purpose / Usage                                                                                                                                                                                                   | Integration                                                                                                                                   |
| -------------------------------------------------------------------------------------------------------------- | --------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------- |
| `ankerl::unordered_dense::map` — Martin Leitner-Ankerl ([GitHub](https://github.com/martinus/unordered_dense)) | External library            | High-performance hash map used for fast key-based lookup and in-memory indexing of registry data, client/session information, subscriptions, and other internal structures where hash-based access is beneficial. | Integrated directly into the C++ implementation; used as an internal container and is not intended to define the public registry data format. |
| `system.h` — Hitemit-Armese                                                                                    | Internal library            | Provides system-level functionality required by the project, such as platform-specific operations and low-level runtime facilities used by the Registry Database implementation.                                  | Internal project dependency; may be updated together with the main codebase.                                                                  |
| `utilityX.h` — Hitemit-Armese                                                                                  | Internal library            | Provides common low-level utilities and supporting functionality used throughout the implementation, including internal helpers required by the Registry Database components.                                     | Internal project dependency; version changes may occur together with the main project.                                                        |
| C++ Standard Library                                                                                           | Standard library            | Provides fundamental language/runtime facilities, containers, synchronization primitives, strings, exceptions, and other standard functionality required by the implementation.                                   | Required by the C++ build environment.                                                                                                        |
| TCP/IP Socket API                                                                                              | System / platform interface | Provides the communication transport between the Registry Database Host Service and connected clients or local services.                                                                                          | Platform-dependent implementation behind the communication layer.                                                                             |


### Third-Party Attribution

Registry Database uses `ankerl::unordered_dense`, an open-source C++ hash map/set library developed and maintained by Martin Leitner-Ankerl.

Repository: [GitHub repository](https://github.com/martinus/unordered_dense)

The library is distributed under the MIT License. The original copyright and license notice contained in the library's source files should be retained when the library is redistributed in source form or otherwise included as required by the applicable license terms.

The use of this third-party library does not imply that its author or maintainers endorse, sponsor, or are otherwise affiliated with Registry Database.

### License Compliance

Third-party components remain subject to their respective licenses. Registry Database does not claim ownership of third-party libraries or code incorporated from external projects.

When redistributing Registry Database together with third-party components, the applicable copyright notices, license texts, and attribution requirements for those components must be preserved as required by their respective licenses.

---

## Reminders / Developer Notices

Since "Registry Database" (also known as "Registry Editor") originated from one of my private projects, it shares some core dependencies. Libraries like utilityX.h and system.h will be continuously maintained and updated in sync with my main project.

Regarding error handling, the system originally relied on a custom Error Correction Code (ECC) structure. I am keeping the ECC system and its associated error code files intact. In this 1.0A Alpha version, the system actively uses the ECC error architecture (`platform_core::ecc`) across both local service and network client layers. Standard std::exception will be added as the default fallback in later updates to make integration easier, unless you explicitly define USING_ECC in your build.

Data safety is a strict priority: future version updates are guaranteed not to modify, corrupt, or cause any data loss in your existing legacy registry files.

For API stability, if you are including the static header files, I will ensure that the function call order, parameters, and return codes remain fully backward compatible. Please note that this compatibility guarantee does not apply to the DLL interface. I plan to introduce advanced features soon—such as file and key passwords, node-level encryption, and restricted access. Consequently, the DLL interface may undergo signature changes (like adding new optional or default parameters to read/write functions) to accommodate these upcoming security layers.

On a final note regarding the codebase itself: because this is a personal project where raw performance and structural stability are my absolute top priorities, I do not strictly follow "clean code" conventions or standard industry formatting. The source code might look quite "unfriendly" and hard to read at the moment, largely due to a lack of comments. I am aware of this and will try my best to document the code more thoroughly in the future.

Additionally, the current file manipulation interface relies on raw pointers. It is only safe to use if you strictly follow the provided lifecycle protocols (such as using the begin and end functions or other built-in library methods). Bypassing these to handle the data manually is not guaranteed to be memory-safe, though I may refactor this for better safety in the future. For now, please keep in mind that this library is tightly optimized and may only support C++ environments.

---

## License

Copyright (c) 2022 RANDOM ARMESE HITEMIT. <br>
All rights reserved.

* Software License:<br>
  HREF: `https://docs.google.com/document/d/1_BNwiYPVKE7-OBkRfHusiKhqvkmBwJFBXv6fG7i2ntk/edit?usp=sharing`<br>
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/software_license_scancode.png" width="200">
------------------------------------------------------------------------------------------------------------------
* License Agreement - Global Version:<br>
  HREF: `https://docs.google.com/document/d/1s8TCbDmofW26iyK5n1O6HS57ldkwyr0_hbF-Z2FjVtw/edit?usp=sharing`<br>
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/license_scancode.png" width="200">
