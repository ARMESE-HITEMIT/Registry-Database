# Registry Database

Registry Database is a hierarchical key-value data storage system designed for structured data management and inter-process communication. It utilizes a client-server architecture over TCP/IP to ensure reliable, thread-safe data access across distributed components or local services.

## Overview

The system organizes data into a tree-like structure, similar to the Windows Registry, allowing for complex, nested configurations and state management. It provides a robust communication protocol for reading, modifying, and monitoring data states in real time without exposing the underlying physical storage mechanics to the client applications.

## Core Features

*   **Hierarchical Structure:** Organizes information into logical paths and endpoints, supporting deep nesting and structured data grouping.
*   **Client-Server Architecture:** Operates over network sockets (TCP) with built-in multiplexing, allowing multiple clients to interface with the database service concurrently.
*   **Strict Data Typing:** Native implementation and enforcement of standard binary types, including:
    *   `VALUE` : (Variable length)
    *   `BYTE ` : (8-bit unsigned)
    *   `WORD ` : (16-bit unsigned)
    *   `DWORD` : (32-bit unsigned)
    *   `QWORD` : (64-bit unsigned)
*   **Real-time Synchronization:** Built-in event-driven update mechanisms allowing clients to subscribe to specific nodes and receive immediate state changes.
*   **Concurrency & Thread Safety:** Utilizes robust locking mechanisms to handle concurrent read/write operations and prevent data races.
*   **Data Serialization:** Supports structural extraction and structural views, including full JSON tree mapping for external integration and debugging.
*   **Batch Operations:** Capability to process multiple key and value operations (Create, Read, Modify, Delete) within a single execution cycle to minimize network overhead.

## Architecture

Registry Database is split into three primary components:
1.  **Host Service:** The central server that maintains the data tree in memory, handles disk I/O, processes incoming socket connections, and dispatches data updates to subscribed clients. (soon)
2.  **Client Library:** A lightweight interface that provides developers with direct, strictly-typed functions to interact with the database over the network layer.
3.  **Service Library:** A 

The communication layer handles automated error correction and status code lookups to maintain stable connectivity and predictable failure states during I/O operations.

## Remider/Notices

Since "Registry Database" (also known as "Registry Editor") originated from one of my private projects, it shares some core dependencies. Libraries like utilityX.h and system.h will be continuously maintained and updated in sync with my main project.

Regarding error handling, the system originally relied on a custom Error Correction Code (ECC) structure. I am keeping the ECC system and its associated error code files intact. However, standard std::exception will now be used as the default fallback to make integration easier, unless you explicitly define USING_ECC in your build.

Data safety is a strict priority: future version updates are guaranteed not to modify, corrupt, or cause any data loss in your existing legacy registry files.

For API stability, if you are including the static header files, I will ensure that the function call order, parameters, and return codes remain fully backward compatible. Please note that this compatibility guarantee does not apply to the DLL interface. I plan to introduce advanced features soon—such as file and key passwords, node-level encryption, and restricted access. Consequently, the DLL interface may undergo signature changes (like adding new optional or default parameters to read/write functions) to accommodate these upcoming security layers.

On a final note regarding the codebase itself: because this is a personal project where raw performance and structural stability are my absolute top priorities, I do not strictly follow "clean code" conventions or standard industry formatting. The source code might look quite "unfriendly" and hard to read at the moment, largely due to a lack of comments. I am aware of this and will try my best to document the code more thoroughly in the future.

Additionally, the current file manipulation interface relies on raw pointers. It is only safe to use if you strictly follow the provided lifecycle protocols (such as using the begin and end functions or other built-in library methods). Bypassing these to handle the data manually is not guaranteed to be memory-safe, though I may refactor this for better safety in the future. For now, please keep in mind that this library is tightly optimized and may only support C++ environments.

## License

Copyright (c) 2022 RANDOM ARMESE HITEMIT. 
All rights reserved.

* Software License:<br>
  HREF: `https://docs.google.com/document/d/1_BNwiYPVKE7-OBkRfHusiKhqvkmBwJFBXv6fG7i2ntk/edit?usp=sharing`
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/software_license_scancode.png" width="200">
------------------------------------------------------------------------------------------------------------------
* License Agreement - Global Version:<br>
  HREF: `https://docs.google.com/document/d/1s8TCbDmofW26iyK5n1O6HS57ldkwyr0_hbF-Z2FjVtw/edit?usp=sharing`
  SCAN CODE:<br>
  <img src="Program%20Datas/Resources/Documents/license_scancode.png" width="200">