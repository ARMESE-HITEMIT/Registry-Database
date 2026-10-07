/*
    *	Registry Editor Library
    *
    *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY EDITOR
    *	All rights reserved.
*/

// ------------------------------------------------------ -
// 12 / 20 / 2025 - 11:30 : 54PM
// ------------------------------------------------------ -
// 
// LICENSE
// 
// ====================================================== =
// 
// Copyright(c) 2022 RANDOM ARMESE HITEMIT
// All Rights Reserved
// 
// -- -
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// -- -

#pragma once

#define REGISTRY_DATABASE_VERSION       2.0A    // DEV NOTE: VERSION & MEAN
#define REGISTRY_DATABASE_VERSION_MAJOR 2       // DEV NOTE: THIS REPRESENT A HUGE UPDATE WHICH MAY CHANGE API/ABI OF THE LIBRARY
#define REGISTRY_DATABASE_VERSION_MINOR 0       // DEV NOTE: FEATURE UPDATE. THIS WILL NOT CHANGE API/ABI OF THE LIBRARY
#define REGISTRY_DATABASE_VERSION_PATCH A       // DEV NOTE: PATCHING VERSION

#if defined(_MSVC_LANG)
#    define REGISTRY_DATABASE_CPP_VERSION _MSVC_LANG
#else
#    define REGISTRY_DATABASE_CPP_VERSION __cplusplus
#endif

#if REGISTRY_DATABASE_CPP_VERSION < 201703L
#   error registry database require C++17 or higher
#endif

// settings
/*
    * NUMBER_OF_PAGE_CACHED:
    *   Number of pages loaded into the L1 cache per swap operation.
    *
    * MMAP_COPY_CHUNK_SIZE:
    *   Fixed allocation size used for copying and temporary data buffers.
    *
    * L2_MAP_MAX_SIZE_ZONES:
    *   Maximum number of string zones loaded into the L2 map for scanning.
    *
    * ALLOC_RESERVE_ZONES:
    *   Number of zones reserved for the allocation table when reallocating.
    *
    * ALLOC_CACHE_SIZE_MAX:
    *   Maximum number of allocation entries kept in the allocation cache.
    *
    * ARRAY_RESERVE_ZONES:
    *   Number of zones reserved for the element array.
    *
    * ARRAY_CACHE_SIZE_MAX:
    *   Maximum number of element entries kept in the array cache.
    *
    * BYTEMAP_RESERVE_ZONES:
    *   Number of zones reserved for the byte map.
    
    SETTING TABLE
    +----------------------------+----------+-----------------------+-----------+-------------------+---------------------+
    | NAME OF SETTING            | MINIMUM  | DEFAULT (Recommened)  | BALANCED  | BEST PERFORMANCE  | BEST MEMORY USUAGE  |
    +----------------------------+----------+-----------------------+-----------+-------------------+---------------------+
    | NUMBER_OF_PAGE_CACHED      | 1        | 8                     | 4         | 16                | 4                   |
    | MMAP_COPY_CHUNK_SIZE       | 1024     | 4096                  | 2048      | 8192 - 16384      | 1024                |
    | L2_MAP_MAX_SIZE_ZONES      | 8        | 32                    | 16        | 32 - 64           | 16                  |
    | ALLOC_RESERVE_ZONES        | 2        | 16                    | 8         | 16 - 24           | 8                   |
    | ALLOC_CACHE_SIZE_MAX       | 8        | 32                    | 16        | 64                | 16                  |
    | ARRAY_RESERVE_ZONES        | 2        | 16                    | 8         | 16 - 24           | 8                   |
    | ARRAY_CACHE_SIZE_MAX       | 8        | 32                    | 16        | 64                | 16                  |
    | BYTEMAP_RESERVE_ZONE       | 2        | 16                    | 8         | 16 - 24           | 8                   |
    +----------------------------+----------+-----------------------+-----------+-------------------+---------------------+
*/
#define NUMBER_OF_PAGE_CACHED      8
#define MMAP_COPY_CHUNK_SIZE       4096
#define L2_MAP_MAX_SIZE_ZONES      32

#define ALLOC_RESERVE_ZONES        16
#define ALLOC_CACHE_SIZE_MAX       32
#define ARRAY_RESERVE_ZONES        16
#define ARRAY_CACHE_SIZE_MAX       32
#define BYTEMAP_RESERVE_ZONES      16

//vars DO NOT CHANGE
#define DOUBLE_QWORD_SIZE          16
#define TRIPLE_QWORD_SIZE          24
#define MMAP_KEY_STRUCTURE_SIZE    202 

#if defined(_MSC_VER)
#   define PACK_PUSH_1 __pragma(pack(push, 1))
#   define PACK_POP    __pragma(pack(pop))
#   define PACKED
#elif defined(__GNUC__) || defined(__clang__)
#   define PACK_PUSH_1
#   define PACK_POP
#   define PACKED __attribute__((packed))
#else
#   define PACK_PUSH_1
#   define PACK_POP
#   define PACKED
#endif

// THIS ZONE CHECK SETTING

#if !defined(DEVELOPER_TEST)
#   if NUMBER_OF_PAGE_CACHED < 1
#      error SETTINGS ERROR: "NUMBER_OF_PAGE_CACHED" REQUIRE QWORD GREATER THAN 0
#   endif
#   if MMAP_COPY_CHUNK_SIZE < 1024
#      error SETTINGS ERROR: "MMAP_COPY_CHUNK_SIZE" REQUIRE QWORD GREATER THAN OR EQUAL 1024
#   endif
#   if L2_MAP_MAX_SIZE_ZONES < 1
#      error SETTINGS ERROR: "L2_MAP_MAX_SIZE_ZONES" REQUIRE QWORD GREATER THAN 0
#   elif L2_MAP_MAX_SIZE_ZONES < 16
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE L2 MAP USEING TO INDEX STRING DATA/KEYNAME REQUIRED 16 STRING ZONE READY IN MEMORY TO SCAN. THE "L2_MAP_MAX_SIZE_ZONES" SHOULD BE GREATER OR EQUAL TO 16. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#   if ALLOC_RESERVE_ZONES < 1
#      error SETTINGS ERROR: "ALLOC_RESERVE_ZONES" REQUIRE QWORD GREATER THAN 0
#   elif ALLOC_RESERVE_ZONES < 8
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE RESERVING ZONE FOR ALLOCATION TABLE. THE "ALLOC_RESERVE_ZONES" SHOULD BE GREATER OR EQUAL TO 8. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#   if ALLOC_CACHE_SIZE_MAX < 8
#      error SETTINGS ERROR: "ALLOC_CACHE_SIZE_MAX" REQUIRE QWORD GREATER THAN OR EQUAL TO 8
#   elif ALLOC_CACHE_SIZE_MAX < 32
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE ALLOCATION CACHE SHOULD CONTAIN AT LEAST 32 ELEMENTS. THE "ALLOC_CACHE_SIZE_MAX" SHOULD BE GREATER OR EQUAL TO 32. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#   if BYTEMAP_RESERVE_ZONES < 1
#      error SETTINGS ERROR: "BYTEMAP_RESERVE_ZONES" REQUIRE QWORD GREATER THAN 0
#   elif BYTEMAP_RESERVE_ZONES < 8
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE BYTEMAP SHOULD RESERVE AT LEAST 8 ZONES. THE "BYTEMAP_RESERVE_ZONES" SHOULD BE GREATER OR EQUAL TO 8. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#   if ARRAY_RESERVE_ZONES < 1
#      error SETTINGS ERROR: "ARRAY_RESERVE_ZONES" REQUIRE QWORD GREATER THAN 0
#   elif ARRAY_RESERVE_ZONES < 8
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE ARRAY SHOULD RESERVE AT LEAST 8 ZONES. THE "ARRAY_RESERVE_ZONES" SHOULD BE GREATER OR EQUAL TO 8. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#   if ARRAY_CACHE_SIZE_MAX < 8
#      error SETTINGS ERROR: "ARRAY_CACHE_SIZE_MAX" REQUIRE QWORD GREATER THAN OR EQUAL TO 8
#   elif ARRAY_CACHE_SIZE_MAX < 32
#      ifndef REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE
#          warning SETTINGS WARNING: DUE TO PERFORMANCE THE ARRAY CACHE SHOULD CONTAIN AT LEAST 32 ELEMENTS. THE "ARRAY_CACHE_SIZE_MAX" SHOULD BE GREATER OR EQUAL TO 32. DEFINE "REGISTRY_DATABASE_BYPASS_PERFORMANCE_ISSUE" TO BYPASS THIS WARNING
#      endif
#   endif
#endif

#define NO_CONNECTOR
#define NO_RECORDER
#define NO_ERROR_LOOKINGUP
#define NO_UAC

#include "../system/system.h"

#undef NO_UAC
#undef NO_CONNECTOR
#undef NO_RECORDER
#undef NO_ERROR_LOOKINGUP

#include "../utilityX/utilityX.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <atomic>
#include <thread>
#include <string>
#include <vector>
#include <functional>
#include <variant>
#include <sstream>
#include <shared_mutex>
#include <cstdint>
#include <algorithm>
#include <mutex>
#include <array>
#include <string_view>
#include <cstring>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <malloc.h>
#include <random>
#include "../unordered_dense/include/ankerl/unordered_dense.h"

// namespace registry_editor_service_local {
//     //settings
//     static constexpr QWORD NUMBER_OF_PAGE_CACHED        = 8;
//     static constexpr QWORD MMAP_COPY_CHUNK_SIZE         = 4096;
//     static constexpr QWORD L2_MAP_MAX_SIZE_ZONES                  = 32;
// 
//     static constexpr QWORD ALLOC_RESERVE_ZONES          = 16;
//     static constexpr QWORD ALLOC_CACHE_SIZE_MAX         = 40;
//     static constexpr QWORD BYTEMAP_RESERVE_ZONES        = 16;
// 	static constexpr QWORD ARRAY_RESERVE_ZONES          = 16;
// 	static constexpr QWORD ARRAY_CACHE_SIZE_MAX         = 40;
// 
//     //vars DO NOT CHANGE
//     static constexpr QWORD DOUBLE_QWORD_SIZE            = 2 * sizeof(QWORD);
//     static constexpr QWORD TRIPLE_QWORD_SIZE            = 3 * sizeof(QWORD);
// 	static constexpr QWORD MMAP_KEY_STRUCTURE_SIZE      = (TRIPLE_QWORD_SIZE << 3) + sizeof(WORD) + sizeof(QWORD);
// 
//     //
// 
//     // Im tooo lazy for this
//     // or stoopid
//     // 
//     // struct setting {
//     //     // 0 mean default;
//     //     QWORD ALLOC_RESERVE_ZONES = 0;
//     // 	QWORD ALLOC_CACHE_SIZE_MAX = 0;
//     // 	QWORD element_array_reserved_zones = 0;
//     //     QWORD bytemap_reserved_zones = 0;
//     // };
// }

// THIS CODE USING AES-256 FOR EN/DECRYPTION

// NOTICE: EVERY KEY, BYTE, WORD, DWORD, QWORD, AND VALUE ASSOCIATED 
// WITH A KEY WILL BE ENCRYPTED USING THE SAME PASSWORD AS ITS PARENT.
// STRINGS CYPHERTEXT WILL BE TREAD AS STRING.
// 
// REGX Format
/*
    <MAGIC : "REGX">
    <metadata :
      "Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY EDITOR LIBRARY - REGISTRY EDITOR FORMAT DATA SYSTEM
      All rights reserved.

      -------------------------------------------------------------------------------------------------------

      This file cannot opened by Windows Registry Editor.

      -------------------------------------------------------------------------------------------------------"
    >

    <format version: "2.0A" : FORMAT :
    <QWORD : Number(s) of element(s) (*key structure & *array excluding root)>
      <QWORD : Number(s) of string(s)>*<QWORD | string id><QWORD | number(s) of element use this string><string : <QWORD | Size of string><byte(s)>>
      <WORD | BITMASK>
      <BITMASK 00 : <QWORD | ROOT key(s) size(s)><QWORD | offset to pointer array of key structure>>
      <BITMASK 01 : <QWORD | ROOT byte(s) size(s)><QWORD | offset to data array of byte>>
      <BITMASK 02 : <QWORD | ROOT word(s) size(s)><QWORD | offset to data array of word>>
      <BITMASK 03 : <QWORD | ROOT dword(s) size(s)><QWORD | offset to data array of dword>>
      <BITMASK 04 : <QWORD | ROOT qword(s) size(s)><QWORD | offset to data array of qword>>
      <BITMASK 05 : <QWORD | ROOT string(s) size(s)><QWORD | offset to pointer array of string>>
      <BITMASK 06 : <QWORD | ROOT lock_key(s) size(s)><QWORD | offset to encrypt pointer array of encrypt key structure>>
      <BITMASK 07 : <QWORD | ROOT lock_byte(s) size(s)><QWORD | offset to encrypt data array byte>>
      <BITMASK 08 : <QWORD | ROOT lock_word(s) size(s)><QWORD | offset to encrypt data array word>>
      <BITMASK 09 : <QWORD | ROOT lock_dword(s) size(s)><QWORD | offset to encrypt data array dword>>
      <BITMASK 10 : <QWORD | ROOT lock_qword(s) size(s)><QWORD | offset to encrypt data array of qword>>
      <BITMASK 11 : <QWORD | ROOT lock_string(s) size(s)><QWORD | offset to encrypt pointer array of encrypt string>>
      <key structure : <QWORD | string id (key name)>
        <WORD | BITMASK>
        <BITMASK 00 : <QWORD | key(s) size(s)><QWORD | offset to pointer array of key structure>>
        <BITMASK 01 : <QWORD | byte(s) size(s)><QWORD | offset to data array of byte>>
        <BITMASK 02 : <QWORD | word(s) size(s)><QWORD | offset to data array of word>>
        <BITMASK 03 : <QWORD | dword(s) size(s)><QWORD | offset to data array of dword>>
        <BITMASK 04 : <QWORD | qword(s) size(s)><QWORD | offset to data array of qword>>
        <BITMASK 05 : <QWORD | string(s) size(s)><QWORD | offset to pointer array of string>>
        <BITMASK 06 : <QWORD | lock_key(s) size(s)><QWORD | offset to encrypt pointer array of encrypt key structure>>
        <BITMASK 07 : <QWORD | lock_byte(s) size(s)><QWORD | offset to encrypt data array byte>>
        <BITMASK 08 : <QWORD | lock_word(s) size(s)><QWORD | offset to encrypt data array word>>
        <BITMASK 09 : <QWORD | lock_dword(s) size(s)><QWORD | offset to encrypt data array dword>>
        <BITMASK 10 : <QWORD | lock_qword(s) size(s)><QWORD | offset to encrypt data array of qword>>
        <BITMASK 11 : <QWORD | lock_string(s) size(s)><QWORD | offset to encrypt pointer array of encrypt string>>
      >
      <encrypt key structure : <QWORD | string id (key name)>
        <ENCRYPT : <WORD | BITMASK>
          <BITMASK 00 : <QWORD | key(s) size(s)><QWORD | offset to pointer array of key structure>>
          <BITMASK 01 : <QWORD | byte(s) size(s)><QWORD | offset to data array of byte>>
          <BITMASK 02 : <QWORD | word(s) size(s)><QWORD | offset to data array of word>>
          <BITMASK 03 : <QWORD | dword(s) size(s)><QWORD | offset to data array of dword>>
          <BITMASK 04 : <QWORD | qword(s) size(s)><QWORD | offset to data array of qword>>
          <BITMASK 05 : <QWORD | string(s) size(s)><QWORD | offset to pointer array of string>>
          <BITMASK 06 : <QWORD | lock_key(s) size(s)><QWORD | offset to encrypt pointer array of encrypt key structure>>
          <BITMASK 07 : <QWORD | lock_byte(s) size(s)><QWORD | offset to encrypt data array byte>>
          <BITMASK 08 : <QWORD | lock_word(s) size(s)><QWORD | offset to encrypt data array word>>
          <BITMASK 09 : <QWORD | lock_dword(s) size(s)><QWORD | offset to encrypt data array dword>>
          <BITMASK 10 : <QWORD | lock_qword(s) size(s)><QWORD | offset to encrypt data array of qword>>
          <BITMASK 11 : <QWORD | lock_string(s) size(s)><QWORD | offset to encrypt pointer array of encrypt string>>
        >
      >
      <pointer array : <QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><QWORD | offset to data>>>
      <data array : <QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><T bytes>>>
      <encrypt pointer array : <QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><QWORD | offset to encrypt data>>>
      <encrypt data array : <QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><encrypt of T bytes>>>
    >
    <OVER : "FIVE MINUTES OF AFTERNOON TEA IS THE SHORTEST SEVEN HOURS OF THE DAY!">
*/

// RUNTIME MEMORY
// MMAP Format
/*
    <QWORD | Address to Malloc table><QWORD | Address to bytes map><QWORD | Address to ROOT>
    <Malloc table : <QWORD | Number(s) of zone prealloc>
      <QWORD | Numbers of zone(s) use><QWORD | address start><QWORD | address stop>
    >
    <BYTES MAP : <QWORD | number(s) of string(s) prealloc>
      <QWORD : Number(s) of string(s) address>*<QWORD | string id><QWORD | number(s) of element use this string><QWORD | Address to string>
      <string : <QWORD | Size of string><byte(s)>>
    >
    <ROOT :
      <WORD | BITMASK>
      <QWORD | ROOT key(s) size(s)><QWORD | address to pointer array of key structure>
      <QWORD | ROOT byte(s) size(s)><QWORD | address to data array of byte>
      <QWORD | ROOT word(s) size(s)><QWORD | address to data array of word>
      <QWORD | ROOT dword(s) size(s)><QWORD | address to data array of dword>
      <QWORD | ROOT qword(s) size(s)><QWORD | address to data array of qword>
      <QWORD | ROOT string(s) size(s)><QWORD | address to pointer array of string>
      <QWORD | ROOT lock_key(s) size(s)><QWORD | address to encrypt pointer array of encrypt key structure>
      <QWORD | ROOT lock_byte(s) size(s)><QWORD | address to encrypt data array byte>
      <QWORD | ROOT lock_word(s) size(s)><QWORD | address to encrypt data array word>
      <QWORD | ROOT lock_dword(s) size(s)><QWORD | address to encrypt data array dword>
      <QWORD | ROOT lock_qword(s) size(s)><QWORD | address to encrypt data array of qword>
      <QWORD | ROOT lock_string(s) size(s)><QWORD | address to encrypt pointer array of encrypt string>
    >
    <key structure : <QWORD | string id (key name)>
      <WORD | BITMASK>
      <QWORD | key(s) size(s)><QWORD | address to pointer array of key structure>
      <QWORD | byte(s) size(s)><QWORD | address to data array of byte>
      <QWORD | word(s) size(s)><QWORD | address to data array of word>
      <QWORD | dword(s) size(s)><QWORD | address to data array of dword>
      <QWORD | qword(s) size(s)><QWORD | address to data array of qword>
      <QWORD | string(s) size(s)><QWORD | address to pointer array of string>
      <QWORD | lock_key(s) size(s)><QWORD | address to encrypt pointer array of encrypt key structure>
      <QWORD | lock_byte(s) size(s)><QWORD | address to encrypt data array byte>
      <QWORD | lock_word(s) size(s)><QWORD | address to encrypt data array word>
      <QWORD | lock_dword(s) size(s)><QWORD | address to encrypt data array dword>
      <QWORD | lock_qword(s) size(s)><QWORD | address to encrypt data array of qword>
      <QWORD | lock_string(s) size(s)><QWORD | address to encrypt pointer array of encrypt string>
    >
    <encrypt key structure : <QWORD | string id (key name)>
      <ENCRYPT : <WORD | BITMASK>
        <QWORD | key(s) size(s)><QWORD | address to pointer array of key structure>
        <QWORD | byte(s) size(s)><QWORD | address to data array of byte>
        <QWORD | word(s) size(s)><QWORD | address to data array of word>
        <QWORD | dword(s) size(s)><QWORD | address to data array of dword>
        <QWORD | qword(s) size(s)><QWORD | address to data array of qword>
        <QWORD | string(s) size(s)><QWORD | address to pointer array of string>
        <QWORD | lock_key(s) size(s)><QWORD | address to encrypt pointer array of encrypt key structure>
        <QWORD | lock_byte(s) size(s)><QWORD | address to encrypt data array byte>
        <QWORD | lock_word(s) size(s)><QWORD | address to encrypt data array word>
        <QWORD | lock_dword(s) size(s)><QWORD | address to encrypt data array dword>
        <QWORD | lock_qword(s) size(s)><QWORD | address to encrypt data array of qword>
        <QWORD | lock_string(s) size(s)><QWORD | address to encrypt pointer array of encrypt string>
      >
    >
    <pointer array : <QWORD | number(s) of element prealloc><QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><QWORD | address to data>>>
    <data array : <QWORD | number(s) of element prealloc><QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><T bytes>>>
    <encrypt pointer array : <QWORD | number(s) of element prealloc><QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><QWORD | address to encrypt data>>>
    <encrypt data array : <QWORD | number(s) of element prealloc><QWORD | number(s) of element in arrays> × <<QWORD | string id (element name)><encrypt of T bytes>>>
*/

namespace registry_editor_service_local {
    const QWORD page_size = NUMBER_OF_PAGE_CACHED * utilityX::filesystem::page_size();
    namespace encryption {
        namespace detail_aes {
            static const uint8_t sbox[256] = {
                0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
            };

            static const uint32_t rcon[11] = {
                0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000,
                0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000
            };

            inline uint8_t gmul(uint8_t a, uint8_t b) {
                uint8_t p = 0;
                for (int i = 0; i < 8; ++i) {
                    if (b & 1) p ^= a;
                    uint8_t hi = a & 0x80;
                    a <<= 1;
                    if (hi) a ^= 0x1b;
                    b >>= 1;
                }
                return p;
            }

            inline void key_expansion_256(const uint8_t* key, uint32_t* w) {
                for (int i = 0; i < 8; ++i) {
                    w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16) | (key[4 * i + 2] << 8) | key[4 * i + 3];
                }
                for (int i = 8; i < 60; ++i) {
                    uint32_t temp = w[i - 1];
                    if (i % 8 == 0) {
                        temp = (temp << 8) | (temp >> 24);
                        temp = (sbox[(temp >> 24) & 0xff] << 24) | (sbox[(temp >> 16) & 0xff] << 16) |
                            (sbox[(temp >> 8) & 0xff] << 8) | sbox[temp & 0xff];
                        temp ^= rcon[i / 8];
                    }
                    else if (i % 8 == 4) {
                        temp = (sbox[(temp >> 24) & 0xff] << 24) | (sbox[(temp >> 16) & 0xff] << 16) |
                            (sbox[(temp >> 8) & 0xff] << 8) | sbox[temp & 0xff];
                    }
                    w[i] = w[i - 8] ^ temp;
                }
            }

            inline void aes256_encrypt_block(const uint32_t* w, const uint8_t in[16], uint8_t out[16]) {
                uint8_t state[4][4];
                for (int r = 0; r < 4; ++r) {
                    for (int c = 0; c < 4; ++c) state[r][c] = in[r + 4 * c];
                }
                for (int c = 0; c < 4; ++c) {
                    uint32_t kw = w[c];
                    state[0][c] ^= (kw >> 24) & 0xff;
                    state[1][c] ^= (kw >> 16) & 0xff;
                    state[2][c] ^= (kw >> 8) & 0xff;
                    state[3][c] ^= kw & 0xff;
                }
                for (int round = 1; round <= 14; ++round) {
                    for (int r = 0; r < 4; ++r)
                        for (int c = 0; c < 4; ++c) state[r][c] = sbox[state[r][c]];
                    uint8_t tmp = state[1][0];
                    state[1][0] = state[1][1]; state[1][1] = state[1][2]; state[1][2] = state[1][3]; state[1][3] = tmp;
                    tmp = state[2][0]; uint8_t tmp2 = state[2][1];
                    state[2][0] = state[2][2]; state[2][1] = state[2][3]; state[2][2] = tmp; state[2][3] = tmp2;
                    tmp = state[3][3];
                    state[3][3] = state[3][2]; state[3][2] = state[3][1]; state[3][1] = state[3][0]; state[3][0] = tmp;

                    if (round < 14) {
                        for (int c = 0; c < 4; ++c) {
                            uint8_t a0 = state[0][c], a1 = state[1][c], a2 = state[2][c], a3 = state[3][c];
                            state[0][c] = gmul(a0, 2) ^ gmul(a1, 3) ^ a2 ^ a3;
                            state[1][c] = a0 ^ gmul(a1, 2) ^ gmul(a2, 3) ^ a3;
                            state[2][c] = a0 ^ a1 ^ gmul(a2, 2) ^ gmul(a3, 3);
                            state[3][c] = gmul(a0, 3) ^ a1 ^ a2 ^ gmul(a3, 2);
                        }
                    }
                    for (int c = 0; c < 4; ++c) {
                        uint32_t kw = w[round * 4 + c];
                        state[0][c] ^= (kw >> 24) & 0xff;
                        state[1][c] ^= (kw >> 16) & 0xff;
                        state[2][c] ^= (kw >> 8) & 0xff;
                        state[3][c] ^= kw & 0xff;
                    }
                }
                for (int r = 0; r < 4; ++r) {
                    for (int c = 0; c < 4; ++c) out[r + 4 * c] = state[r][c];
                }
            }

            inline void sha256_transform(uint32_t state[8], const uint8_t data[64]) {
                static const uint32_t k[64] = {
                    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
                };
                uint32_t w[64];
                for (int i = 0; i < 16; ++i)
                    w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | data[i * 4 + 3];
                for (int i = 16; i < 64; ++i) {
                    uint32_t s0 = ((w[i - 15] >> 7) | (w[i - 15] << 25)) ^ ((w[i - 15] >> 18) | (w[i - 15] << 14)) ^ (w[i - 15] >> 3);
                    uint32_t s1 = ((w[i - 2] >> 17) | (w[i - 2] << 15)) ^ ((w[i - 2] >> 19) | (w[i - 2] << 13)) ^ (w[i - 2] >> 10);
                    w[i] = w[i - 16] + s0 + w[i - 7] + s1;
                }
                uint32_t a = state[0], b = state[1], c = state[2], d = state[3], e = state[4], f = state[5], g = state[6], h = state[7];
                for (int i = 0; i < 64; ++i) {
                    uint32_t S1 = ((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7));
                    uint32_t ch = (e & f) ^ ((~e) & g);
                    uint32_t temp1 = h + S1 + ch + k[i] + w[i];
                    uint32_t S0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
                    uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                    uint32_t temp2 = S0 + maj;
                    h = g; g = f; f = e; e = d + temp1; d = c; c = b; b = a; a = temp1 + temp2;
                }
                state[0] += a; state[1] += b; state[2] += c; state[3] += d;
                state[4] += e; state[5] += f; state[6] += g; state[7] += h;
            }

            inline std::array<uint8_t, 32> sha256(const uint8_t* data, size_t len) {
                uint32_t state[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };
                size_t i = 0;
                for (; i + 64 <= len; i += 64) sha256_transform(state, data + i);
                uint8_t pad[128] = { 0 };
                size_t rem = len - i;
                std::memcpy(pad, data + i, rem);
                pad[rem] = 0x80;
                size_t pad_len = (rem < 56) ? 64 : 128;
                uint64_t bits = static_cast<uint64_t>(len) * 8;
                for (int j = 0; j < 8; ++j) pad[pad_len - 1 - j] = static_cast<uint8_t>(bits >> (j * 8));
                for (size_t p = 0; p < pad_len; p += 64) sha256_transform(state, pad + p);
                std::array<uint8_t, 32> out;
                for (int j = 0; j < 8; ++j) {
                    out[j * 4] = (state[j] >> 24) & 0xff; out[j * 4 + 1] = (state[j] >> 16) & 0xff;
                    out[j * 4 + 2] = (state[j] >> 8) & 0xff; out[j * 4 + 3] = state[j] & 0xff;
                }
                return out;
            }

            inline std::array<uint8_t, 32> hmac_sha256(const uint8_t* key, size_t klen, const uint8_t* data, size_t dlen) {
                uint8_t k0[64] = { 0 };
                if (klen > 64) {
                    auto h = sha256(key, klen);
                    std::memcpy(k0, h.data(), 32);
                }
                else {
                    std::memcpy(k0, key, klen);
                }
                uint8_t ipad[64], opad[64];
                for (int i = 0; i < 64; ++i) {
                    ipad[i] = k0[i] ^ 0x36;
                    opad[i] = k0[i] ^ 0x5c;
                }
                std::vector<uint8_t> inner(64 + dlen);
                std::memcpy(inner.data(), ipad, 64);
                if (dlen > 0) std::memcpy(inner.data() + 64, data, dlen);
                auto ihash = sha256(inner.data(), inner.size());

                uint8_t outer[64 + 32];
                std::memcpy(outer, opad, 64);
                std::memcpy(outer + 64, ihash.data(), 32);
                return sha256(outer, sizeof(outer));
            }

        }

        inline void encrypt(const std::string& master_key, std::string* data) {
            std::array<uint8_t, 32> aes_key = detail_aes::sha256(reinterpret_cast<const uint8_t*>(master_key.data()), master_key.size());

            uint8_t iv[16];
            std::random_device rd;
            for (int i = 0; i < 16; ++i) iv[i] = static_cast<uint8_t>(rd() & 0xFF);

            uint32_t w[60];
            detail_aes::key_expansion_256(aes_key.data(), w);

            size_t data_len = data->size();
            std::string result;
            result.resize(16 + 32 + data_len);

            uint8_t* out_ptr = reinterpret_cast<uint8_t*>(&result[0]);
            std::memcpy(out_ptr, iv, 16);

            uint8_t counter[16];
            std::memcpy(counter, iv, 16);
            uint8_t stream_block[16];

            const uint8_t* in_ptr = reinterpret_cast<const uint8_t*>(data->data());
            uint8_t* cipher_ptr = out_ptr + 16 + 32;

            for (size_t i = 0; i < data_len; i += 16) {
                detail_aes::aes256_encrypt_block(w, counter, stream_block);
                size_t block_size = (data_len - i < 16) ? (data_len - i) : 16;
                for (size_t j = 0; j < block_size; ++j) cipher_ptr[i + j] = in_ptr[i + j] ^ stream_block[j];
                for (int k = 15; k >= 0; --k) {
                    if (++counter[k] != 0) break;
                }
            }

            auto mac = detail_aes::hmac_sha256(aes_key.data(), 32, cipher_ptr, data_len);
            std::memcpy(out_ptr + 16, mac.data(), 32);

            *data = std::move(result);
        }

        inline std::string decrypt(const std::string& master_key, const std::string& cybertext) {
            if (cybertext.size() < 48) {
                throw std::runtime_error("Wrong password");
            }

            std::array<uint8_t, 32> aes_key = detail_aes::sha256(reinterpret_cast<const uint8_t*>(master_key.data()), master_key.size());

            const uint8_t* in_ptr = reinterpret_cast<const uint8_t*>(cybertext.data());
            const uint8_t* iv = in_ptr;
            const uint8_t* expected_mac = in_ptr + 16;
            const uint8_t* cipher_ptr = in_ptr + 48;
            size_t cipher_len = cybertext.size() - 48;

            auto calc_mac = detail_aes::hmac_sha256(aes_key.data(), 32, cipher_ptr, cipher_len);

            uint8_t diff = 0;
            for (int i = 0; i < 32; ++i) diff |= (calc_mac[i] ^ expected_mac[i]);
            if (diff != 0) {
                throw std::runtime_error("Wrong password");
            }

            uint32_t w[60];
            detail_aes::key_expansion_256(aes_key.data(), w);

            std::string plaintext;
            plaintext.resize(cipher_len);
            uint8_t* out_ptr = reinterpret_cast<uint8_t*>(&plaintext[0]);

            uint8_t counter[16];
            std::memcpy(counter, iv, 16);
            uint8_t stream_block[16];

            for (size_t i = 0; i < cipher_len; i += 16) {
                detail_aes::aes256_encrypt_block(w, counter, stream_block);
                size_t block_size = (cipher_len - i < 16) ? (cipher_len - i) : 16;
                for (size_t j = 0; j < block_size; ++j) out_ptr[i + j] = cipher_ptr[i + j] ^ stream_block[j];
                for (int k = 15; k >= 0; --k) {
                    if (++counter[k] != 0) break;
                }
            }

            return plaintext;
        }
    }
    //enum class OpenMode : BYTE {
    //    ReadWrite = 0,
    //    ReadOnly = 1,
    //    WriteOnly = 2,
    //};

    class registry_database;
    class registry_editor_service_local {
    private:
        BYTE* L1_cache = nullptr;
        QWORD L1_sector = 0;
        QWORD L1_max_sector = 1;
        QWORD L2_sector = 0;
        ankerl::unordered_dense::map<std::string, QWORD>* L2_map = nullptr;

        //OpenMode function_mode = OpenMode::ReadOnly;
        char* registry_file_path;
        FileHandle cache_handle;
        FileHandle recovery_file;
    private:
        // there are 2 version of this access_memory, move__mempry & write_memory function which provide
        // the same function but different return/input value & state.
        // 
        // I need to avoid two arithmetic operation for address
        // to get the same index

        void access_memory_address(BYTE* DESTINATION, QWORD address, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            QWORD q = address / page_size;
            QWORD offset = address % page_size;

            if (this->L1_sector != q) {
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                this->L1_sector = q;
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
            }

            QWORD remaining = SIZE;
            while (remaining > 0) {
                QWORD copy_size;
                {
                    QWORD available = page_size - offset;
                    copy_size = (remaining < available) ? remaining : available;
                }
                memcpy(DESTINATION, this->L1_cache + offset, copy_size);
                DESTINATION += copy_size;
                remaining -= copy_size;
                if (remaining == 0) break;
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, ++this->L1_sector);
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                offset = 0;
            }
        }
        void access_memory_index(BYTE* DESTINATION, QWORD index, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            QWORD remaining = SIZE;
            while (remaining > 0) {
                QWORD copy_size;
                {
                    QWORD available = page_size - index;
                    copy_size = (remaining < available) ? remaining : available;
                }
                memcpy(DESTINATION, this->L1_cache + index, copy_size);
                DESTINATION += copy_size;
                remaining -= copy_size;
                if (remaining == 0) break;
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, ++this->L1_sector);
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                index = 0;
            }
        }
        void write_memory_address(const BYTE* SOURCE, QWORD address, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            QWORD q = address / page_size;
            QWORD offset = address % page_size;

            if (this->L1_sector != q) {
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                this->L1_sector = q;
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
            }
            QWORD remaining = SIZE;
            while (remaining > 0) {
                QWORD copy_size;
                {
                    QWORD available = page_size - offset;
                    copy_size = (remaining < available) ? remaining : available;
                }
                memcpy(this->L1_cache + offset, SOURCE, copy_size);
                SOURCE += copy_size;
                remaining -= copy_size;

                if (remaining == 0) break;

                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, ++this->L1_sector);
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                offset = 0;
            }
        }
        void write_memory_index(const BYTE* SOURCE, QWORD index, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            QWORD remaining = SIZE;
            while (remaining > 0) {
                QWORD copy_size;
                {
                    QWORD available = page_size - index;
                    copy_size = (remaining < available) ? remaining : available;
                }
                memcpy(this->L1_cache + index, SOURCE, copy_size);

                SOURCE += copy_size;
                remaining -= copy_size;

                if (remaining == 0) break;

                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                // std::memset(this->L1_cache, 0, page_size);
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, ++this->L1_sector);
                if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                index = 0;
            }
        }
        void move_memory_address(QWORD sources, QWORD destination, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            BYTE* buffer = (BYTE*)malloc(MMAP_COPY_CHUNK_SIZE);
            if (!buffer) return;

            QWORD remaining = SIZE;

            while (remaining > 0) {
                QWORD source_offset = sources % page_size;
                QWORD destination_offset = destination % page_size;
                QWORD copy_size = (remaining < page_size - source_offset) ? remaining : page_size - source_offset;
                if (copy_size > page_size - destination_offset) copy_size = page_size - destination_offset;

                QWORD source_sector = sources / page_size;
                QWORD destination_sector = destination / page_size;

                if (this->L1_sector != source_sector) {
                    utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                    this->L1_sector = source_sector;
                    if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                    // std::memset(this->L1_cache, 0, page_size);
                    utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                }

                memcpy(buffer, this->L1_cache + source_offset, copy_size);

                if (this->L1_sector != destination_sector) {
                    utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                    this->L1_sector = destination_sector;
                    if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                    // std::memset(this->L1_cache, 0, page_size);
                    utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                }

                memcpy(this->L1_cache + destination_offset, buffer, copy_size);

                sources += copy_size;
                destination += copy_size;
                remaining -= copy_size;
            }
            free(buffer);
        }
        void move_memory_index(QWORD sources, QWORD destination, QWORD SIZE) {
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */\

                BYTE* buffer = (BYTE*)malloc(MMAP_COPY_CHUNK_SIZE);
            if (!buffer) return;

            QWORD remaining = SIZE;

            while (remaining > 0) {
                QWORD copy_size = (remaining < page_size - sources) ? remaining : page_size - sources;
                if (copy_size > page_size - destination) copy_size = page_size - destination;

                memcpy(buffer, this->L1_cache + sources, copy_size);

                QWORD destination_sector = this->L1_sector;

                if (destination != sources) {
                    if (destination >= page_size) {
                        utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                        this->L1_sector++;
                        if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                        // std::memset(this->L1_cache, 0, page_size);
                        utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                        destination -= page_size;
                    }
                }

                memcpy(this->L1_cache + destination, buffer, copy_size);

                sources += copy_size;
                destination += copy_size;
                remaining -= copy_size;

                if (remaining == 0) break;

                if (sources >= page_size) {
                    utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                    this->L1_sector++;
                    if (this->L1_sector > this->L1_max_sector) this->L1_max_sector = this->L1_sector;
                    // std::memset(this->L1_cache, 0, page_size);
                    utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                    sources -= page_size;
                }
            }
            free(buffer);
        }

        // QWORD disk_realloc(QWORD address, QWORD size) {
        //     // alloc for disk (address)
        //     /*
        //         this function may change current L1 sector and L1_cache
        //         notice that this version get address instead of index
        //     */
        // 
        // 
        // }
        // QWORD disk_malloc(QWORD size) {
        //     // malloc for disk (return address)
        //     /*
        //         this function may change current L1 sector and L1_cache
        //         notice that this version return address instead of index
        //     */
        // 
        //     if (L1_sector != 0) {
        //         utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
        //         this->L1_sector = 0;
        //         utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, 0);
        //     }
        //     QWORD repeat;
        //     QWORD prealloc_size;
        //     QWORD* malloc_map_temp = nullptr;
        //     QWORD address;
        //     memcpy(&address, this->L1_cache, sizeof(address));
        //     access_memory_address((BYTE*)&prealloc_size, address, sizeof(prealloc_size));
        //     access_memory_address((BYTE*)&repeat, address+sizeof(prealloc_size), sizeof(repeat));
        // 
        //     if (prealloc_size == 0) {
        //         prealloc_size = ALLOC_RESERVE_ZONES + 3;
        //         malloc_map_temp = (QWORD*)malloc(prealloc_size * DOUBLE_QWORD_SIZE);
        //     }
        //     address += DOUBLE_QWORD_SIZE;
        // 
        //     QWORD return_address[2] = { 0 , 0 };
        //     QWORD zone[2] = { 1, 1 };
        //     if (prealloc_size >= ALLOC_CACHE_SIZE_MAX) {
        //         malloc_map_temp = (QWORD*)malloc(ALLOC_CACHE_SIZE_MAX * DOUBLE_QWORD_SIZE);
        //         access_memory_address((BYTE*)(malloc_map_temp), address, ALLOC_CACHE_SIZE_MAX * DOUBLE_QWORD_SIZE);
        // 
        //         if (repeat == 0 || repeat == 1) {
        //             repeat = 2;
        //             malloc_map_temp[0] = static_cast<QWORD>(0x00000000);
        //             malloc_map_temp[1] = static_cast<QWORD>(0x00000018);
        //             malloc_map_temp[2] = static_cast<QWORD>(address - DOUBLE_QWORD_SIZE);
        //             malloc_map_temp[3] = static_cast<QWORD>(address + (prealloc_size * DOUBLE_QWORD_SIZE));
        //             write_memory_address((BYTE*)malloc_map_temp, address, ALLOC_CACHE_SIZE_MAX * DOUBLE_QWORD_SIZE);
        //         }
        //         {
        //             QWORD flush[2] = {
        //                 prealloc_size,
        //                 repeat + 1
        //             };
        //             write_memory_address((BYTE*)flush, address - DOUBLE_QWORD_SIZE, DOUBLE_QWORD_SIZE);
        //         }
        //         {
        //             QWORD current_chunk_start = 0;
        //             QWORD prev_stop = malloc_map_temp[1];
        // 
        //             for (; zone[0] < repeat; zone[0]++) {
        //                 if (zone[0] >= current_chunk_start + ALLOC_CACHE_SIZE_MAX) {
        //                     current_chunk_start += ALLOC_CACHE_SIZE_MAX;
        //                     QWORD fetch_zones = (repeat - current_chunk_start < ALLOC_CACHE_SIZE_MAX) ? (repeat - current_chunk_start) : ALLOC_CACHE_SIZE_MAX;
        //                     access_memory_address((BYTE*)malloc_map_temp, address + (current_chunk_start * DOUBLE_QWORD_SIZE), fetch_zones * DOUBLE_QWORD_SIZE);
        //                 }
        // 
        //                 QWORD idx = zone[0] - current_chunk_start;
        //                 QWORD current_start = malloc_map_temp[idx * 2];
        //                 QWORD current_stop = malloc_map_temp[idx * 2 + 1];
        // 
        //                 if (current_start - prev_stop == 1) {
        //                     prev_stop = current_stop;
        //                     continue;
        //                 }
        // 
        //                 if (((current_start - prev_stop) - 1) >= size) {
        //                     return_address[0] = prev_stop + 1;
        //                     break;
        //                 }
        // 
        //                 prev_stop = current_stop;
        //             }
        // 
        //             if (return_address[0] == 0) {
        //                 zone[0] = repeat;
        //                 return_address[0] = prev_stop + 1;
        //             }
        //             if (return_address[1] == 0) {
        //                 zone[1] = zone[1] + 1;
        //                 return_address[1] = return_address[0] + size + 1;
        //             }
        //         }
        //     }
        //     else {
        //         malloc_map_temp = (QWORD*)malloc(prealloc_size * DOUBLE_QWORD_SIZE);
        //         access_memory_address((BYTE*)(malloc_map_temp), address, prealloc_size * DOUBLE_QWORD_SIZE);
        // 
        //         if (repeat == 0 || repeat == 1) {
        //             repeat = 2;
        //             malloc_map_temp[0] = static_cast<QWORD>(0x00000000);
        //             malloc_map_temp[1] = static_cast<QWORD>(0x00000018);
        //             malloc_map_temp[2] = static_cast<QWORD>(address - DOUBLE_QWORD_SIZE);
        //             malloc_map_temp[3] = static_cast<QWORD>(address + (prealloc_size * DOUBLE_QWORD_SIZE));
        //             write_memory_address((BYTE*)malloc_map_temp, address, prealloc_size * DOUBLE_QWORD_SIZE);
        //         }
        //         {
        //             QWORD flush[2] = {
        //                 prealloc_size,
        //                 repeat + 1
        //             };
        //             write_memory_address((BYTE*)flush, address - DOUBLE_QWORD_SIZE, DOUBLE_QWORD_SIZE);
        //         }
        //         {
        //             QWORD* parameter = malloc_map_temp + 1;
        //             // if (prealloc_size == repeat) {
        //             //     prealloc_size += ALLOC_RESERVE_ZONES + 1;
        //             //     
        //             //     // enum state : BYTE {
        //             //     //     DONE_MALLOC     = 0b00000001,
        //             //     //     DONE_REALLOC    = 0b00000010,
        //             //     //     MALLOC_FIRST    = 0b00000100,
        //             //     //     REALLOC_FIRST   = 0b00001000,
        //             //     //     FOUND_ADDRESS   = 0b00010000
        //             //     // };
        //             //     BYTE flags = 0b00000000;
        //             // 
        //             //     for (; zone[0] < repeat; zone[0]++) {
        //             //         if (parameter[1] - parameter[0] == 1) {
        //             //             parameter += 2;
        //             //             continue;
        //             //         }
        //             //         if (!(flags & 0b00000010)) {
        //             //             if (((parameter[1] - parameter[0]) - 1) >= prealloc_size) {
        //             //                 return_address[1] = parameter[0] + 1;
        //             //                 flags |= 0b00000010;
        //             //                 if (flags & 0b00000100) zone[1] = zone[0] + 1;
        //             //                 else {
        //             //                     zone[1] = zone[0];
        //             //                     flags != 0b00001000;
        //             //                 }
        //             //             }
        //             //         }
        //             //         if (!(flags & 0b00000001)) {
        //             //             if (((parameter[1] - parameter[0]) - 1) >= )
        //             //         }
        //             //         parameter += 2;
        //             //     }
        //             // }
        //             // else {
        //                 for (; zone[0] < repeat; zone[0]++) {
        //                     if (parameter[1] - parameter[0] == 1) {
        //                         parameter += 2;
        //                         continue;
        //                     }
        //                     if (((parameter[1] - parameter[0]) - 1) >= size) {
        //                         return_address[0] = parameter[0] + 1;
        //                         break;
        //                     }
        //                     parameter += 2;
        //                 }
        //             // }
        //             if (return_address[0] == 0) {
        //                 zone[0] = repeat;
        //                 return_address[0] = parameter[0] + 1;
        //             }
        //             if (return_address[1] == 0) {
        //                 zone[1] = zone[1] + 1;
        //                 return_address[1] = return_address[0] + size + 1;
        //             }
        //             // zone = (zone * DOUBLE_QWORD_SIZE) + address;
        //         }
        //     }
        //     if (prealloc_size < repeat + 1) {
        //         QWORD addr = disk_realloc(address - DOUBLE_QWORD_SIZE, (prealloc_size + (ALLOC_RESERVE_ZONES + 1)) * DOUBLE_QWORD_SIZE);
        //         if (addr != (address - DOUBLE_QWORD_SIZE)) {
        //             write_memory_address((BYTE*)(&addr), 0x0000000000000000, sizeof(QWORD));
        //             address = addr + DOUBLE_QWORD_SIZE;
        //         }
        //     }
        //     free(malloc_map_temp);
        //     return_address[1] = return_address[0] + size;
        //     address = address + (zone[0] * DOUBLE_QWORD_SIZE);
        //     if (zone[0] == repeat) {
        //         write_memory_address((BYTE*)(&return_address), address, DOUBLE_QWORD_SIZE);
        //     }
        //     else {
        //         move_memory_address(address, address + DOUBLE_QWORD_SIZE, ((repeat - zone[0]) + 1) * DOUBLE_QWORD_SIZE);
        //         write_memory_address((BYTE*)(&return_address), address, DOUBLE_QWORD_SIZE);
        //     }
        //     return return_address[0];
        // }
        // void disk_free(QWORD address) {
        //     // free malloc for disk (address)
        //     /*
        //         this function may change current L1 sector
        //         notice that this version get address instead of index
        //     */
        // }

        QWORD disk_realloc(QWORD address, QWORD size) {
            // alloc for disk (address)
            /*
                this function may change current L1 sector and L1_cache
                notice that this version get address instead of index
            */

            // if (address == 0 || size == 0) return 0;

            if (L1_sector != 0) {
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                this->L1_sector = 0;
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, 0);
            }

            QWORD table_address, repeat, prealloc_size;
            QWORD* malloc_map_temp = nullptr;
            memcpy(&table_address, this->L1_cache, sizeof(table_address));

            // if (table_address == 0 || address == table_address) return 0;

            access_memory_address((BYTE*)&prealloc_size, table_address, sizeof(prealloc_size));
            access_memory_address((BYTE*)&repeat, table_address + sizeof(prealloc_size), sizeof(repeat));
            // if (prealloc_size == 0 || repeat < 2 || repeat > prealloc_size) return 0;

            QWORD map_size = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : prealloc_size;
            malloc_map_temp = (QWORD*)malloc(map_size * DOUBLE_QWORD_SIZE);
            if (malloc_map_temp) {
                // NOTE: IM NOT CLEANCODER. NO FLATTENED & EARLY RETURN TO REMOVE A "NOT" OP. NO COMLAIN
                access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE, map_size * DOUBLE_QWORD_SIZE);

                QWORD current_chunk_start = 0, prev_stop = malloc_map_temp[1];
                QWORD target_zone = repeat, target_stop = 0, table_zone = repeat;
                QWORD data_candidate = 0, data_candidate_zone = repeat, next_zone_start = 0, zone_index = 1;

                for (; zone_index < repeat; zone_index++) {
                    if (prealloc_size >= ALLOC_CACHE_SIZE_MAX && zone_index >= current_chunk_start + ALLOC_CACHE_SIZE_MAX) {
                        current_chunk_start += ALLOC_CACHE_SIZE_MAX;
                        QWORD fetch_zones = (repeat - current_chunk_start < ALLOC_CACHE_SIZE_MAX) ? (repeat - current_chunk_start) : ALLOC_CACHE_SIZE_MAX;
                        access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + (current_chunk_start * DOUBLE_QWORD_SIZE), fetch_zones * DOUBLE_QWORD_SIZE);
                    }

                    QWORD idx = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? zone_index - current_chunk_start : zone_index;
                    QWORD current_start = malloc_map_temp[idx * 2];
                    QWORD current_stop = malloc_map_temp[idx * 2 + 1];

                    if (current_start == table_address) table_zone = zone_index;

                    QWORD free_size = current_start - prev_stop;
                    if (data_candidate == 0 && free_size > size) {
                        data_candidate = prev_stop;
                        data_candidate_zone = zone_index;
                    }

                    if (current_start == address) {
                        target_zone = zone_index;
                        target_stop = current_stop;
                    }
                    else if (target_zone != repeat && zone_index == target_zone + 1) {
                        next_zone_start = current_start;
                    }

                    prev_stop = current_stop;
                }

                // if (target_zone == repeat || target_zone == table_zone) {
                //     free(malloc_map_temp);
                //     return 0;
                // }

                QWORD old_size = target_stop - address;
                QWORD target_table_offset = table_address + DOUBLE_QWORD_SIZE + (target_zone * DOUBLE_QWORD_SIZE) + sizeof(QWORD);

                if (size == old_size) {
                    free(malloc_map_temp);
                    return address;
                }

                if (size < old_size) {
                    QWORD new_stop = address + size;
                    write_memory_address((BYTE*)&new_stop, target_table_offset, sizeof(QWORD));
                    free(malloc_map_temp);
                    return address;
                }

                QWORD new_stop = address + size;
                if (next_zone_start == 0 || new_stop <= next_zone_start) {
                    write_memory_address((BYTE*)&new_stop, target_table_offset, sizeof(QWORD));
                    free(malloc_map_temp);
                    return address;
                }

                if (data_candidate == 0) {
                    data_candidate = prev_stop;
                    data_candidate_zone = repeat;
                }

                QWORD new_address = data_candidate;
                move_memory_address(address, new_address, old_size);

                if (target_zone + 1 < repeat) {
                    QWORD shift_zone = target_zone + 1;
                    while (shift_zone < repeat) {
                        QWORD shift_count = (repeat - shift_zone < ALLOC_CACHE_SIZE_MAX) ? (repeat - shift_zone) : ALLOC_CACHE_SIZE_MAX;
                        access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        write_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + ((shift_zone - 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        shift_zone += shift_count;
                    }
                }

                QWORD used_after_remove = repeat - 1;
                QWORD insert_zone = (data_candidate_zone > target_zone) ? data_candidate_zone - 1 : data_candidate_zone;
                if (insert_zone > used_after_remove) insert_zone = used_after_remove;

                if (insert_zone < used_after_remove) {
                    QWORD shift_end = used_after_remove;
                    while (shift_end > insert_zone) {
                        QWORD shift_count = (shift_end - insert_zone > ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : (shift_end - insert_zone);
                        QWORD shift_zone = shift_end - shift_count;
                        access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        write_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + ((shift_zone + 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        shift_end = shift_zone;
                    }
                }

                QWORD new_zone[2] = { new_address, new_stop };
                write_memory_address((BYTE*)new_zone, table_address + DOUBLE_QWORD_SIZE + (insert_zone * DOUBLE_QWORD_SIZE), DOUBLE_QWORD_SIZE);

                free(malloc_map_temp);
                return new_address;
            }
            else {
                return 0;
            }
        }


        QWORD disk_malloc(QWORD size) {
            // malloc for disk (return address)
            /*
                this function may change current L1 sector and L1_cache
                notice that this version return address instead of index
            */

            // if (size == 0) return 0;

            if (L1_sector != 0) {
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                this->L1_sector = 0;
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, 0);
            }

            QWORD repeat, prealloc_size, address;
            QWORD* malloc_map_temp = nullptr;
            memcpy(&address, this->L1_cache, sizeof(address));

            // if (address == 0) return 0;

            access_memory_address((BYTE*)&prealloc_size, address, sizeof(prealloc_size));
            access_memory_address((BYTE*)&repeat, address + sizeof(prealloc_size), sizeof(repeat));

            // if (repeat > prealloc_size && prealloc_size != 0) return 0;

            BYTE map_initialized = 0;
            if (prealloc_size == 0) {
                prealloc_size = ALLOC_RESERVE_ZONES + 3;
                repeat = 2;

                malloc_map_temp = (QWORD*)malloc(prealloc_size * DOUBLE_QWORD_SIZE);
                if (malloc_map_temp) {
                    // std::memset(malloc_map_temp, 0, prealloc_size * DOUBLE_QWORD_SIZE);

                    malloc_map_temp[0] = static_cast<QWORD>(0x00000000);
                    malloc_map_temp[1] = static_cast<QWORD>(0x00000018);
                    malloc_map_temp[2] = static_cast<QWORD>(address);
                    malloc_map_temp[3] = static_cast<QWORD>(address + DOUBLE_QWORD_SIZE + (prealloc_size * DOUBLE_QWORD_SIZE));

                    write_memory_address((BYTE*)&prealloc_size, address, sizeof(prealloc_size));
                    write_memory_address((BYTE*)&repeat, address + sizeof(prealloc_size), sizeof(repeat));
                    write_memory_address((BYTE*)malloc_map_temp, address + DOUBLE_QWORD_SIZE, prealloc_size * DOUBLE_QWORD_SIZE);
                    map_initialized = 1;
                }
                else {
                    return 0;
                }
            }

            QWORD map_size = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : prealloc_size;
            if (!malloc_map_temp) {
                malloc_map_temp = (QWORD*)malloc(map_size * DOUBLE_QWORD_SIZE);
                if (!malloc_map_temp) return 0;
            }

            if (!map_initialized) {
                access_memory_address((BYTE*)malloc_map_temp, address + DOUBLE_QWORD_SIZE, map_size * DOUBLE_QWORD_SIZE);
            }

            QWORD grow_bytes = (ALLOC_RESERVE_ZONES + 1) * DOUBLE_QWORD_SIZE;
            QWORD table_start = address, table_stop = 0;
            QWORD table_old_bytes = DOUBLE_QWORD_SIZE + (prealloc_size * DOUBLE_QWORD_SIZE);
            QWORD table_new_bytes = table_old_bytes + grow_bytes;
            QWORD table_zone = repeat, table_candidate = 0, table_candidate_zone = repeat, table_candidate_end = 0;
            QWORD data_candidate = 0, data_candidate_zone = repeat;
            QWORD prev_stop = malloc_map_temp[1], current_chunk_start = 0;
            BYTE table_resize_done = 0;
            QWORD zone_index = 1;

            for (; zone_index < repeat; zone_index++) {
                if (prealloc_size >= ALLOC_CACHE_SIZE_MAX && zone_index >= current_chunk_start + ALLOC_CACHE_SIZE_MAX) {
                    current_chunk_start += ALLOC_CACHE_SIZE_MAX;
                    QWORD fetch_zones = (repeat - current_chunk_start < ALLOC_CACHE_SIZE_MAX) ? (repeat - current_chunk_start) : ALLOC_CACHE_SIZE_MAX;
                    access_memory_address((BYTE*)malloc_map_temp, address + DOUBLE_QWORD_SIZE + (current_chunk_start * DOUBLE_QWORD_SIZE), fetch_zones * DOUBLE_QWORD_SIZE);
                }

                QWORD idx = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? zone_index - current_chunk_start : zone_index;
                QWORD current_start = malloc_map_temp[idx * 2];
                QWORD current_stop = malloc_map_temp[idx * 2 + 1];

                if (current_start == table_start && table_zone == repeat) {
                    table_zone = zone_index;
                    table_stop = current_stop;
                    table_old_bytes = table_stop - table_start;
                    table_new_bytes = table_old_bytes + grow_bytes;
                }

                if (prealloc_size < repeat + 1 && !table_resize_done && current_start == table_start) {
                    QWORD gap_before_table = current_start - prev_stop;
                    if (gap_before_table >= grow_bytes) {
                        table_start = current_start - grow_bytes;
                        move_memory_address(current_start, table_start, table_old_bytes);
                        table_stop = current_stop;
                        table_resize_done = 1;
                        prealloc_size += (ALLOC_RESERVE_ZONES + 1);

                        write_memory_address((BYTE*)&prealloc_size, table_start, sizeof(prealloc_size));
                        write_memory_address((BYTE*)&table_start, 0x0000000000000000, sizeof(QWORD));

                        QWORD new_table_zone_data[2] = { table_start, table_stop };
                        write_memory_address((BYTE*)new_table_zone_data, table_start + DOUBLE_QWORD_SIZE + (zone_index * DOUBLE_QWORD_SIZE), DOUBLE_QWORD_SIZE);

                        if (data_candidate != 0 && data_candidate_zone == zone_index && (data_candidate + size) >= table_start) {
                            data_candidate = 0;
                            data_candidate_zone = repeat;
                        }

                        current_start = table_start;
                    }
                }

                if (prealloc_size < repeat + 1 && !table_resize_done && table_zone != repeat && zone_index == table_zone + 1) {
                    QWORD gap_after_table = current_start - table_stop;
                    if (gap_after_table >= grow_bytes) {
                        table_stop += grow_bytes;
                        prealloc_size += (ALLOC_RESERVE_ZONES + 1);
                        table_resize_done = 1;

                        write_memory_address((BYTE*)&prealloc_size, table_start, sizeof(prealloc_size));
                        QWORD new_table_zone_data[2] = { table_start, table_stop };
                        write_memory_address((BYTE*)new_table_zone_data, table_start + DOUBLE_QWORD_SIZE + (table_zone * DOUBLE_QWORD_SIZE), DOUBLE_QWORD_SIZE);
                        prev_stop = table_stop;
                    }
                }

                QWORD free_size = current_start - prev_stop;
                if (free_size >= size && data_candidate == 0) {
                    data_candidate = prev_stop;
                    data_candidate_zone = zone_index;
                }

                if (prealloc_size < repeat + 1 && !table_resize_done) {
                    if (free_size >= table_new_bytes && table_candidate == 0) {
                        table_candidate = prev_stop;
                        table_candidate_zone = zone_index;
                        table_candidate_end = current_start;
                    }
                }

                prev_stop = current_stop;
            }

            // if (table_zone == repeat || table_stop == 0) {
            //     free(malloc_map_temp);
            //     return 0;
            // }

            if (prealloc_size < repeat + 1 && !table_resize_done && table_zone == repeat - 1) {
                table_stop += grow_bytes;
                prealloc_size += (ALLOC_RESERVE_ZONES + 1);
                table_resize_done = 1;

                write_memory_address((BYTE*)&prealloc_size, table_start, sizeof(prealloc_size));
                QWORD new_table_zone_data[2] = { table_start, table_stop };
                write_memory_address((BYTE*)new_table_zone_data, table_start + DOUBLE_QWORD_SIZE + (table_zone * DOUBLE_QWORD_SIZE), DOUBLE_QWORD_SIZE);
            }

            if (prealloc_size < repeat + 1 && !table_resize_done) {
                if (table_candidate == 0) {
                    table_candidate = prev_stop;
                    table_candidate_zone = repeat;
                    table_candidate_end = 0;
                }

                QWORD old_table_zone = table_zone;
                QWORD new_table_start = table_candidate;
                QWORD new_table_stop = new_table_start + table_new_bytes;
                QWORD used_after_remove = repeat - 1;

                move_memory_address(address, new_table_start, table_old_bytes);
                write_memory_address((BYTE*)&prealloc_size, new_table_start, sizeof(prealloc_size));
                write_memory_address((BYTE*)&new_table_start, 0x0000000000000000, sizeof(QWORD));

                if (old_table_zone + 1 < repeat) {
                    QWORD shift_zone = old_table_zone + 1;
                    while (shift_zone < repeat) {
                        QWORD shift_count = (repeat - shift_zone < ALLOC_CACHE_SIZE_MAX) ? (repeat - shift_zone) : ALLOC_CACHE_SIZE_MAX;
                        access_memory_address((BYTE*)malloc_map_temp, new_table_start + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        write_memory_address((BYTE*)malloc_map_temp, new_table_start + DOUBLE_QWORD_SIZE + ((shift_zone - 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        shift_zone += shift_count;
                    }
                }

                QWORD new_table_zone = (table_candidate_zone > old_table_zone) ? table_candidate_zone - 1 : table_candidate_zone;
                if (new_table_zone < used_after_remove) {
                    QWORD shift_end = used_after_remove;
                    while (shift_end > new_table_zone) {
                        QWORD shift_count = (shift_end - new_table_zone > ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : (shift_end - new_table_zone);
                        QWORD shift_zone = shift_end - shift_count;
                        access_memory_address((BYTE*)malloc_map_temp, new_table_start + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        write_memory_address((BYTE*)malloc_map_temp, new_table_start + DOUBLE_QWORD_SIZE + ((shift_zone + 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        shift_end = shift_zone;
                    }
                }

                QWORD new_table_zone_data[2] = { new_table_start, new_table_stop };
                write_memory_address((BYTE*)new_table_zone_data, new_table_start + DOUBLE_QWORD_SIZE + (new_table_zone * DOUBLE_QWORD_SIZE), DOUBLE_QWORD_SIZE);

                table_start = new_table_start;
                table_stop = new_table_stop;
                table_zone = new_table_zone;
                table_resize_done = 1;
                prealloc_size += (ALLOC_RESERVE_ZONES + 1);
                write_memory_address((BYTE*)&prealloc_size, table_start, sizeof(prealloc_size));

                if (data_candidate != 0 && data_candidate_zone == table_candidate_zone) {
                    if (table_candidate_end == 0 || table_candidate_end - new_table_stop >= size) {
                        data_candidate = new_table_stop;
                        data_candidate_zone = new_table_zone + 1;
                    }
                    else {
                        data_candidate = 0;
                        data_candidate_zone = repeat;
                    }
                }

                if (data_candidate != 0 && data_candidate_zone != repeat) {
                    if (data_candidate_zone > old_table_zone) data_candidate_zone--;
                    if (new_table_zone <= data_candidate_zone) data_candidate_zone++;
                }

                if (data_candidate == 0) {
                    if (table_candidate_end == 0) {
                        data_candidate = new_table_stop;
                        data_candidate_zone = new_table_zone + 1;
                    }
                    else {
                        data_candidate = prev_stop;
                        data_candidate_zone = repeat;
                    }
                }
            }

            // if (prealloc_size < repeat + 1 && !table_resize_done) {
            //     free(malloc_map_temp);
            //     return 0;
            // }

            if (data_candidate == 0) {
                data_candidate = (table_zone == repeat - 1) ? table_stop : prev_stop;
                data_candidate_zone = repeat;
            }

            QWORD return_address[2] = { data_candidate, data_candidate + size };
            QWORD zone_address = table_start + DOUBLE_QWORD_SIZE + (data_candidate_zone * DOUBLE_QWORD_SIZE);

            if (data_candidate_zone >= repeat) {
                write_memory_address((BYTE*)return_address, zone_address, DOUBLE_QWORD_SIZE);
            }
            else {
                QWORD shift_end = repeat;
                while (shift_end > data_candidate_zone) {
                    QWORD shift_count = (shift_end - data_candidate_zone > ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : (shift_end - data_candidate_zone);
                    QWORD shift_zone = shift_end - shift_count;
                    access_memory_address((BYTE*)malloc_map_temp, table_start + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                    write_memory_address((BYTE*)malloc_map_temp, table_start + DOUBLE_QWORD_SIZE + ((shift_zone + 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                    shift_end = shift_zone;
                }
                write_memory_address((BYTE*)return_address, zone_address, DOUBLE_QWORD_SIZE);
            }

            repeat++;
            write_memory_address((BYTE*)&repeat, table_start + sizeof(QWORD), sizeof(QWORD));
            free(malloc_map_temp);
            return return_address[0];
        }
        void disk_free(QWORD address) {
            // free malloc for disk (address)
            /*
                this function may change current L1 sector
                notice that this version get address instead of index
            */

            // if (address == 0) return;

            if (L1_sector != 0) {
                utilityX::filesystem::flush_chunk(this->cache_handle, this->L1_cache, page_size, this->L1_sector);
                this->L1_sector = 0;
                utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, 0);
            }

            QWORD table_address, repeat, prealloc_size;
            QWORD* malloc_map_temp = nullptr;
            memcpy(&table_address, this->L1_cache, sizeof(table_address));

            // if (table_address == 0 || address == table_address) return;

            access_memory_address((BYTE*)&prealloc_size, table_address, sizeof(prealloc_size));
            access_memory_address((BYTE*)&repeat, table_address + sizeof(prealloc_size), sizeof(repeat));
            // if (prealloc_size == 0 || repeat < 2 || repeat > prealloc_size) return;

            QWORD map_size = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? ALLOC_CACHE_SIZE_MAX : prealloc_size;
            malloc_map_temp = (QWORD*)malloc(map_size * DOUBLE_QWORD_SIZE);
            if (malloc_map_temp) {
                access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE, map_size * DOUBLE_QWORD_SIZE);

                QWORD current_chunk_start = 0, table_zone = repeat, target_zone = repeat, zone_index = 1;

                for (; zone_index < repeat; zone_index++) {
                    if (prealloc_size >= ALLOC_CACHE_SIZE_MAX && zone_index >= current_chunk_start + ALLOC_CACHE_SIZE_MAX) {
                        current_chunk_start += ALLOC_CACHE_SIZE_MAX;
                        QWORD fetch_zones = (repeat - current_chunk_start < ALLOC_CACHE_SIZE_MAX) ? (repeat - current_chunk_start) : ALLOC_CACHE_SIZE_MAX;
                        access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + (current_chunk_start * DOUBLE_QWORD_SIZE), fetch_zones * DOUBLE_QWORD_SIZE);
                    }

                    QWORD idx = (prealloc_size >= ALLOC_CACHE_SIZE_MAX) ? zone_index - current_chunk_start : zone_index;
                    QWORD current_start = malloc_map_temp[idx * 2];

                    if (current_start == table_address) table_zone = zone_index;
                    if (current_start == address) {
                        target_zone = zone_index;
                        break;
                    }
                }

                if (target_zone == repeat || target_zone == table_zone) {
                    free(malloc_map_temp);
                    return;
                }

                if (target_zone + 1 < repeat) {
                    QWORD shift_zone = target_zone + 1;
                    while (shift_zone < repeat) {
                        QWORD shift_count = (repeat - shift_zone < ALLOC_CACHE_SIZE_MAX) ? (repeat - shift_zone) : ALLOC_CACHE_SIZE_MAX;
                        access_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + (shift_zone * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        write_memory_address((BYTE*)malloc_map_temp, table_address + DOUBLE_QWORD_SIZE + ((shift_zone - 1) * DOUBLE_QWORD_SIZE), shift_count * DOUBLE_QWORD_SIZE);
                        shift_zone += shift_count;
                    }
                }

                repeat--;
                write_memory_address((BYTE*)&repeat, table_address + sizeof(QWORD), sizeof(QWORD));
                free(malloc_map_temp);
                {
                    QWORD last_zone_end = 0;
                    access_memory_address((BYTE*)&last_zone_end, table_address + (repeat * DOUBLE_QWORD_SIZE) + sizeof(QWORD), sizeof(QWORD));

                    QWORD last_sector = last_zone_end / page_size;
                    if (last_sector < this->L1_max_sector) {
                        QWORD pages_to_pop = this->L1_max_sector - last_sector;
                        if (pages_to_pop > 0) {
                            utilityX::filesystem::pop_page(this->cache_handle, page_size, pages_to_pop);
                            this->L1_max_sector = last_sector;
                        }
                    }
                }
            }
            else {
                return;
            }
        }
    private:
        inline static constexpr char FORMAT_SIGNATURE[] = "Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY EDITOR LIBRARY - REGISTRY EDITOR FORMAT DATA SYSTEM\n"
            "All rights reserved.\n\n\n"
            "-------------------------------------------------------------------------------------------------------\n\n"
            "This file cannot opened by Windows Registry Editor.\n\n"
            "-------------------------------------------------------------------------------------------------------\n";
        inline static constexpr char MAGIC_HEADER[] = "REGX\0\n";

        FileHandle load_from_disk(const char* path_to_regx_file) {
            std::ifstream main(path_to_regx_file, std::ios::binary);
            if (!main.is_open()) {
#ifdef USING_ECC
                throw platform_core::ecc{ 2, 13 };
#else
                throw std::exception("invalid_file");
#endif
            }
            {
                size_t magic_len = 0;
                char* magic = (char*)malloc(1);
                if (magic) magic[0] = '\0';

                char ch;
                while (main.get(ch) && ch != '\n') {
                    if (ch == '\r') continue;
                    magic = (char*)realloc(magic, magic_len + 2);
                    magic[magic_len] = ch;
                    magic[magic_len + 1] = '\0';
                    magic_len++;
                }

                if (magic) {
                    if (strcmp(magic, MAGIC_HEADER) == 0) {
                        QWORD length = strlen(FORMAT_SIGNATURE) + 1;
                        // signature check
                        {
                            magic = (char*)realloc(magic, length);
                            char* signature = magic;
                            if (signature) {
                                main.read(signature, length - 1);
                                signature[length - 1] = '\0';

                                if (strcmp(signature, FORMAT_SIGNATURE) != 0) {
#ifdef USING_ECC
                                    throw platform_core::ecc{ 2, 11 };
#else
                                    throw std::exception("corrupted_file");
#endif
                                }
                            }
                            else {
#ifdef USING_ECC
                                throw platform_core::ecc{ 2, 20 };
#else
                                throw std::exception("fileop_memlen_exceeded");
#endif
                            }
                        }
                        {
                            // format version check

                            size_t version_len = 0;
                            magic = (char*)realloc(magic, 1);
                            if (magic) magic[0] = '\0';

                            while (main.get(ch) && ch != '\n') {
                                if (ch == '\r') continue;
                                magic = (char*)realloc(magic, version_len + 2);
                                magic[version_len] = ch;
                                magic[version_len + 1] = '\0';
                                version_len++;
                            }

                            char* format_version = magic;
                            if (format_version) {
                                if (strcmp(format_version, "2.0A\0") == 0) {
                                    free(magic);
                                    goto format_version_2e0A;
                                }
                                else {
                                    free(magic);
#ifdef USING_ECC
                                    throw platform_core::ecc{ 2, 11 };
#else
                                    throw std::exception("corrupted_file");
#endif
                                }
                            }
                            else {
#ifdef USING_ECC
                                throw platform_core::ecc{ 2, 20 };
#else
                                throw std::exception("fileop_memlen_exceeded");
#endif
                            }
                        }
                    }
                    else {
                        if (magic) free(magic);
                        //check other old template then if not
#ifdef USING_ECC
                        throw platform_core::ecc{ 2, 13 };
#else
                        throw std::exception("invalid_file");
#endif
                    }
                }
                else {
#ifdef USING_ECC
                    throw platform_core::ecc{ 2, 20 };
#else
                    throw std::exception("fileop_memlen_exceeded");
#endif
                }
            }
            {
            format_version_2e0A:
                {
                    try {
                        // format_version 2.0A parser
                        {
                            std::string path = std::string(path_to_regx_file) + ".mmap";
                            std::ofstream file(path, std::ios::trunc);
                            file.close();
                            this->cache_handle = utilityX::filesystem::open_file(path.c_str());
                        }
                        this->L1_sector = 0;
                        utilityX::filesystem::fetch_chunk(this->cache_handle, this->L1_cache, page_size, 0);
                        QWORD buffered_pointer = sizeof(QWORD);

                        QWORD elements_count;
                        QWORD malloctablezone;
                        QWORD now_address;
						BYTE* copy_page = (BYTE*)malloc(MMAP_COPY_CHUNK_SIZE);
                        {
                            //load section .data

                            QWORD endofmalloctable;
                            QWORD endofbytemap;
                            QWORD bytemapzone;
                            QWORD bytemap_input[4] = {
                                0x00000000000000000,
                                0x00000000000000000,
                                0x00000000000000000,
                                0x00000000000000000
                            };

                            QWORD REP;

                            main.read((char*)&elements_count, sizeof(QWORD));
                            main.read((char*)&REP, sizeof(QWORD));
                            {
                                QWORD malloc_table_prealloc_size = 4 + ALLOC_RESERVE_ZONES + REP;
                                {
                                    endofmalloctable = (malloc_table_prealloc_size + elements_count + 1) * DOUBLE_QWORD_SIZE + 0x0000000000000018;
                                    endofbytemap = endofmalloctable + DOUBLE_QWORD_SIZE + ((BYTEMAP_RESERVE_ZONES + REP) * TRIPLE_QWORD_SIZE) + 8;
                                    malloctablezone = 0x0000000000000058;
                                    bytemapzone = endofmalloctable + DOUBLE_QWORD_SIZE;
                                    bytemap_input[2] = endofbytemap;

                                    QWORD address[11] = {
                                        // header
                                        0x0000000000000018, // address to malloc table
                                        endofmalloctable,   // address to bytes map
                                        0xFFFFFFFFFFFFFFFF, // address to root key (temp)

                                        // malloc table
                                        malloc_table_prealloc_size, 3 + REP + elements_count, // malloc table prealloc size, repeat

                                        0x0000000000000000, 0x0000000000000018, // defining 3 fixed address for reader
                                        0x0000000000000018, endofmalloctable,   // defining of malloc table
                                        endofmalloctable, endofbytemap,       // defining of bytes map
                                    };
                                    write_memory_address((BYTE*)&address, 0x0000000000000000, sizeof(address));
                                }
                                {
                                    QWORD address[2] = {
                                        REP + BYTEMAP_RESERVE_ZONES + 1,   // bytemap prealloc size
                                        REP                                 // bytemap repeat
                                    };
                                    write_memory_address((BYTE*)&address, endofmalloctable, sizeof(address));
                                }
                            }
                            for (; REP > 0; REP--) {
                                main.read((char*)&bytemap_input, DOUBLE_QWORD_SIZE);
                                write_memory_address((BYTE*)&bytemap_input, bytemapzone, TRIPLE_QWORD_SIZE);

                                QWORD string_size = 0;
                                main.read((char*)&string_size, sizeof(QWORD));
                                write_memory_address((BYTE*)&string_size, bytemap_input[2], sizeof(QWORD));
                                bytemap_input[2] += sizeof(QWORD);

                                if (string_size > MMAP_COPY_CHUNK_SIZE) {
									QWORD remaining = string_size;

                                    while (remaining >= MMAP_COPY_CHUNK_SIZE) {
                                        main.read((char*)(copy_page), MMAP_COPY_CHUNK_SIZE);
                                        write_memory_address((BYTE*)copy_page, bytemap_input[2], MMAP_COPY_CHUNK_SIZE);
                                        bytemap_input[2] += MMAP_COPY_CHUNK_SIZE;
                                        remaining -= MMAP_COPY_CHUNK_SIZE;
                                    }
                                    if (remaining > 0) {
                                        main.read((char*)(copy_page), remaining);
                                        write_memory_address((BYTE*)copy_page, bytemap_input[2], remaining);
                                        bytemap_input[2] += remaining;
                                    }
                                }
                                else {
                                    main.read((char*)copy_page, string_size);

                                    bytemap_input[3] = bytemap_input[2] + string_size + sizeof(QWORD);
                                    write_memory_address((BYTE*)copy_page, bytemap_input[2], string_size);
                                    bytemap_input[2] += string_size;
                                }
                                write_memory_address((((BYTE*)&bytemap_input) + DOUBLE_QWORD_SIZE), malloctablezone, DOUBLE_QWORD_SIZE);
                                malloctablezone += DOUBLE_QWORD_SIZE;
                                bytemapzone += TRIPLE_QWORD_SIZE;
                            }
                            now_address = bytemap_input[2];
                        }
                        {
                            // load section .text

                            BYTE keyobject[MMAP_KEY_STRUCTURE_SIZE];

                            QWORD* object = (QWORD*)&keyobject;
                            WORD* mask = (WORD*)&object[1];
                            {
                                // root route

                                main.read((char*)mask, sizeof(WORD));

                            }
                        }
                        free(copy_page);
                        goto end_processor;
                    }
                    catch (...) {
#ifdef USING_ECC
                        throw platform_core::ecc{ 2, 11 };
#else
                        throw std::exception("corrupted_file");
#endif
                    }
                }
            }
        end_processor:
            {
                // end
            }
        }
        registry_editor_service_local() = default;
        registry_editor_service_local(const char* path_to_registry_file) {
            this->L1_cache = (BYTE*)_aligned_malloc(page_size, utilityX::filesystem::page_size());
            // std::memset(L1_cache, 0, page_size);
            this->registry_file_path = new char[std::strlen(path_to_registry_file) + 1];
            strncpy_s(this->registry_file_path, sizeof(this->registry_file_path), path_to_registry_file, _TRUNCATE);
            this->cache_handle = load_from_disk(path_to_registry_file);
            this->L2_map = new ankerl::unordered_dense::map<std::string, QWORD>();
        }
        ~registry_editor_service_local() {
            _aligned_free(this->L1_cache);
            delete this->L2_map;
            delete[] this->registry_file_path;
        }

    private:
        friend registry_database registry_begin(const std::string& path_to_registry_file);
        friend void registry_end(registry_database registry_handle);
    };

    // container for handle;
    ankerl::unordered_dense::map<std::string_view, registry_database> lookup_table;

    class registry_database {
        // this is a handle
        /*
			PLEASE NOTE THAT THIS VERSION DOES NOT BACKWARD COMPATIBLE WITH THE PREVIOUS VERSION OF REGISTRY EDITOR SERVICE LOCAL
        */
    private:
		registry_editor_service_local* service;

        registry_database() = default;
        ~registry_database() {
            if (service) {
                

                service = nullptr;
            }
            else return;
        }
    private:
        friend registry_database registry_begin(const std::string& path_to_registry_file);
        friend void registry_end(registry_database registry_handle);
    public:

    };

    registry_database registry_begin(const std::string& path_to_registry_file) {

    }
    void registry_end(registry_database registry_handle) {

    }
}