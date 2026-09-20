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

#ifndef CONFIRMED_CAUTION

#error USING REGISTRY EDITOR SERVICE LOCAL WITHOUT HOST MAY LOSE DATA IN CASE OF CRASH OR POWER FAILURE. USE WITH CAUTION. define "CONFIRMED_CAUTION" to bypass this warning

#endif

#pragma message(" *	Registry Editor Library")
#pragma message(" *")
#pragma message(" *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY EDITOR")
#pragma message(" *	All rights reserved.")
#pragma message(" ")
#pragma message(" ")
#pragma message(" ------------------------------------------------------ -")
#pragma message(" 12 / 20 / 2025 - 11:30 : 54PM")
#pragma message(" ------------------------------------------------------ -")
#pragma message(" ")
#pragma message(" LICENSE")
#pragma message(" ")
#pragma message(" ====================================================== =")
#pragma message(" ")
#pragma message(" Copyright(c) 2022 RANDOM ARMESE HITEMIT")
#pragma message(" All Rights Reserved")
#pragma message(" ")
#pragma message(" -- -")
#pragma message(" ")
#pragma message(" Permission is hereby granted, free of charge, to any person obtaining a copy")
#pragma message(" of this software and associated documentation files(the \"Software\"), to deal")
#pragma message(" in the Software without restriction, including without limitation the rights")
#pragma message(" to use, copy, modify, merge, publish, distribute, sublicense, and /or sell")
#pragma message(" copies of the Software, and to permit persons to whom the Software is")
#pragma message(" furnished to do so, subject to the following conditions :")
#pragma message(" ")
#pragma message(" The above copyright notice and this permission notice shall be included in all")
#pragma message(" copies or substantial portions of the Software.")
#pragma message(" ")
#pragma message(" THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR")
#pragma message(" IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,")
#pragma message(" FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL ")
#pragma message(" AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER")
#pragma message(" LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,")
#pragma message(" OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE")
#pragma message(" SOFTWARE.")
#pragma message(" ")
#pragma message(" -- -")

#pragma once

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
#include "../unordered_dense/include/ankerl/unordered_dense.h"

#if defined(_MSC_VER)
#define PACK_PUSH_1 __pragma(pack(push, 1))
#define PACK_POP    __pragma(pack(pop))
#define PACKED
#elif defined(__GNUC__) || defined(__clang__)
#define PACK_PUSH_1
#define PACK_POP
#define PACKED __attribute__((packed))
#else
#define PACK_PUSH_1
#define PACK_POP
#define PACKED
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

namespace registry_editor_service_local {
#ifndef NO_CHECK_VALID
    inline void check_name(std::string_view name) {
        QWORD s = static_cast<QWORD>(name.size());
        if (s > 255 || s == 0)
            throw platform_core::ecc{ 2, 14 };

        static const auto allowed = [] {
            std::array<bool, 256> table{};

            for (unsigned char c = 'a'; c <= 'z'; ++c) table[c] = true;
            for (unsigned char c = 'A'; c <= 'Z'; ++c) table[c] = true;
            for (unsigned char c = '0'; c <= '9'; ++c) table[c] = true;

            for (unsigned char c : std::string_view("_-{}[].@#!:?~"))
                table[c] = true;

            return table;
            }();

        for (unsigned char c : name) {
            if (!allowed[c])
                throw platform_core::ecc{ 2, 15 };
        }
    }
#endif
    inline std::vector<std::pair<std::string*, QWORD>> string_value;
    inline std::mutex string_value_mutex;

    inline std::string* retain_string_value(const std::string& value) {
        if (value.empty())
            return nullptr;

        std::lock_guard<std::mutex> lock(string_value_mutex);

        for (auto& data : string_value) {
            if (data.first && *data.first == value) {
                ++data.second;
                return data.first;
            }
        }

        std::string* allocated = new std::string(value);

        try {
            string_value.emplace_back(allocated, 1);
        }
        catch (...) {
            delete allocated;
            throw;
        }

        return allocated;
    }

    inline void release_string_value(std::string*& value) noexcept {
        if (!value)
            return;

        try {
            std::lock_guard<std::mutex> lock(string_value_mutex);

            for (auto it = string_value.begin(); it != string_value.end(); ++it) {
                if (it->first == value) {
                    if (it->second <= 1) {
                        delete it->first;
                        string_value.erase(it);
                    }
                    else {
                        --it->second;
                    }

                    value = nullptr;
                    return;
                }
            }

            delete value;
            value = nullptr;
        }
        catch (...) {
            value = nullptr;
        }
    }

    enum class type : BYTE {
        key = 0,
        string,
        byte,
        word,
        dword,
        qword,
        _uninit_key,
        _uninit_value
    };

    inline std::shared_mutex mtxs;

    PACK_PUSH_1
        struct file_system {
        using storage_map = ankerl::unordered_dense::map<std::string, file_system>;
        union {
            BYTE b;
            WORD w;
            DWORD dw;
            QWORD qw;
            std::string* value = nullptr;
            storage_map* storage;
        };
        type node_type = type::byte;
        file_system() : node_type(type::_uninit_key) {
            storage = nullptr;
        }

        explicit file_system(const std::string& data) {
            if (data.empty()) {
                node_type = type::_uninit_value;
                value = nullptr;
            }
            else {
                node_type = type::string;
                value = retain_string_value(data);
            }
        }

        explicit file_system(BYTE data) : node_type(type::byte) {
            b = data;
        }

        explicit file_system(WORD data) : node_type(type::word) {
            w = data;
        }

        explicit file_system(DWORD data) : node_type(type::dword) {
            dw = data;
        }

        explicit file_system(QWORD data) : node_type(type::qword) {
            qw = data;
        }

        inline bool is_key_like() const noexcept {
            return node_type == type::key || node_type == type::_uninit_key;
        }

        inline bool is_string_like() const noexcept {
            return node_type == type::string || node_type == type::_uninit_value;
        }

        inline storage_map& ensure_storage() {
            if (node_type == type::_uninit_key) {
                storage = new storage_map();
                node_type = type::key;
                return *storage;
            }

            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };

            if (!storage)
                storage = new storage_map();

            return *storage;
        }

        inline const storage_map* read_storage() const {
            if (node_type == type::_uninit_key)
                return nullptr;

            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };

            return storage;
        }

        inline std::string read_own_value() const {
            if (node_type == type::_uninit_value)
                return "";

            if (node_type != type::string)
                throw platform_core::ecc{ 2, 2 };

            if (!value)
                throw platform_core::ecc{ 2, 14 };

            return *value;
        }

        inline void set_own_value(const std::string& data) {
            if (node_type == type::_uninit_value) {
                if (data.empty())
                    return;

                value = retain_string_value(data);
                node_type = type::string;
                return;
            }

            if (node_type != type::string)
                throw platform_core::ecc{ 2, 2 };

            if (data.empty()) {
                release_string_value(value);
                node_type = type::_uninit_value;
                return;
            }

            std::string* new_value = retain_string_value(data);
            release_string_value(value);
            value = new_value;
            node_type = type::string;
        }

        inline type public_type() const noexcept {
            if (node_type == type::_uninit_key)
                return type::key;
            if (node_type == type::_uninit_value)
                return type::string;
            return node_type;
        }

        void destroy() noexcept {
            switch (node_type) {
            case type::string:
                release_string_value(value);
                break;

            case type::key:
                delete storage;
                storage = nullptr;
                break;
            case type::_uninit_key:
            case type::_uninit_value:
            case type::byte:
            case type::word:
            case type::dword:
            case type::qword:
                break;
            }

            node_type = type::qword;
            qw = 0;
        }

        ~file_system() noexcept {
            destroy();
        }

        file_system(file_system&& other) noexcept : node_type(other.node_type) {
            switch (node_type) {
            case type::_uninit_key:
                storage = nullptr;
                break;
            case type::_uninit_value:
                value = nullptr;
                break;
            case type::string:
                value = other.value;
                break;

            case type::key:
                storage = other.storage;
                break;

            case type::byte:
                b = other.b;
                break;

            case type::word:
                w = other.w;
                break;

            case type::dword:
                dw = other.dw;
                break;

            case type::qword:
                qw = other.qw;
                break;
            }

            other.node_type = type::qword;
            other.qw = 0;
        }

        file_system& operator=(file_system&& other) noexcept {
            if (this == &other) return *this;

            destroy();

            node_type = other.node_type;

            switch (node_type) {
            case type::_uninit_key:
                storage = nullptr;
                break;
            case type::_uninit_value:
                value = nullptr;
                break;
            case type::string:
                value = other.value;
                break;

            case type::key:
                storage = other.storage;
                break;

            case type::byte:
                b = other.b;
                break;

            case type::word:
                w = other.w;
                break;

            case type::dword:
                dw = other.dw;
                break;

            case type::qword:
                qw = other.qw;
                break;
            }

            other.node_type = type::qword;
            other.qw = 0;

            return *this;
        }

        file_system(const file_system& other) : node_type(other.node_type) {
            switch (node_type) {
            case type::_uninit_key:
                storage = nullptr;
                break;
            case type::_uninit_value:
                value = nullptr;
                break;
            case type::string:
                if (!other.value) {
                    node_type = type::_uninit_value;
                    value = nullptr;
                }
                else {
                    value = retain_string_value(*other.value);
                }
                break;

            case type::key:
                storage = other.storage ? new storage_map(*other.storage) : nullptr;
                break;

            case type::byte:
                b = other.b;
                break;

            case type::word:
                w = other.w;
                break;

            case type::dword:
                dw = other.dw;
                break;

            case type::qword:
                qw = other.qw;
                break;
            }
        }

        file_system& operator=(const file_system& other) {
            if (this == &other) return *this;

            file_system tmp(other);
            *this = std::move(tmp);

            return *this;
        }

        inline bool exists(const std::string& name) const {
            if (node_type == type::_uninit_key)
                return false;
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                return false;
            return storage->contains(name);
        }

        inline void delete_child(const std::string& name) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            if (storage->erase(name) == 0)
                throw platform_core::ecc{ 2, 1 };
        }

        inline std::string readchild_value(const std::string& name) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            return it->second.read_own_value();
        }

        inline BYTE readchild_byte(const std::string& name)
        {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::byte)
                throw platform_core::ecc{ 2, 3 };
            return it->second.b;
        }

        inline WORD readchild_word(const std::string& name)
        {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::word)
                throw platform_core::ecc{ 2, 4 };
            return it->second.w;
        }

        inline DWORD readchild_dword(const std::string& name) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::dword)
                throw platform_core::ecc{ 2, 5 };
            return it->second.dw;
        }

        inline QWORD readchild_qword(const std::string& name) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::qword)
                throw platform_core::ecc{ 2, 6 };
            return it->second.qw;
        }

        inline void rename_child(const std::string& oldn, const std::string& newn) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(oldn);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (storage->contains(newn))
                throw platform_core::ecc{ 2, 9 };
            file_system temp_node = std::move(it->second);
            storage->erase(it);

            try {
                storage->emplace(newn, std::move(temp_node));
            }
            catch (...) {
                try {
                    storage->emplace(oldn, std::move(temp_node));
                }
                catch (...) {}
                throw;
            }
        }

        inline void modify_child_value(const std::string& name, const std::string& value) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            it->second.set_own_value(value);
        }
        inline void modify_child_byte(const std::string& name, BYTE byte) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::byte)
                throw platform_core::ecc{ 2, 3 };
            it->second.b = byte;
        }
        inline void modify_child_word(const std::string& name, WORD word) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::word)
                throw platform_core::ecc{ 2, 4 };
            it->second.w = word;
        }
        inline void modify_child_dword(const std::string& name, DWORD dword) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::dword)
                throw platform_core::ecc{ 2, 5 };
            it->second.dw = dword;
        }
        inline void modify_child_qword(const std::string& name, QWORD qword) {
            if (node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (node_type != type::key)
                throw platform_core::ecc{ 2, 0 };
            if (!storage || storage->empty())
                throw platform_core::ecc{ 2, 1 };
            auto it = storage->find(name);
            if (it == storage->end())
                throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::qword)
                throw platform_core::ecc{ 2, 6 };
            it->second.qw = qword;
        }
    };
    PACK_POP

        struct INIT_KEY {
        std::string id;
    };

    struct INIT_VALUE {
        std::string id;
        std::string value = "";
    };

    struct INIT_BYTE {
        std::string id;
        BYTE value = 0;
    };

    struct INIT_WORD {
        std::string id;
        WORD value = 0;
    };

    struct INIT_DWORD {
        std::string id;
        DWORD value = 0;
    };

    struct INIT_QWORD {
        std::string id;
        QWORD value = 0;
    };

    struct roots {
    private:
        using storage_map = ankerl::unordered_dense::map<std::string, file_system>;
        using string_table = std::vector<const std::string*>;
        using string_id_map = ankerl::unordered_dense::map<const std::string*, QWORD>;

        static constexpr QWORD MAX_ROOT_COUNT = -1;
        static constexpr QWORD MAX_CHILD_COUNT = -1;
        static constexpr QWORD MAX_KEY_LENGTH = -1;
        static constexpr QWORD MAX_STRING_LENGTH = -1;
        static constexpr QWORD MAX_STRING_SECTION_COUNT = -1;
        static constexpr QWORD INVALID_STRING_ID = -1;

        inline static constexpr char FORMAT_SIGNATURE[] =
            "Copyright (c) 2022 RANDOM ARMESE HITEMIT - REGISTRY EDITOR LIBRARY - REGISTRY EDITOR FORMAT DATA SYSTEM\n"
            "All rights reserved.\n\n\n"
            "-------------------------------------------------------------------------------------------------------\n\n"
            "This file cannot opened by Windows Registry Editor.\n\n"
            "-------------------------------------------------------------------------------------------------------\n";

        inline static constexpr char FORMAT_V2_MAGIC[] = {
            'R', 'A', 'H', 'R', 'E', 'G', 'X', 'D', 'A', 'T', 'A', '0', '0', '0', '2', '\n'
        };

        static constexpr QWORD SIGNATURE_SIZE = sizeof(FORMAT_SIGNATURE) - 1;
        static constexpr QWORD V2_MAGIC_SIZE = sizeof(FORMAT_V2_MAGIC);

        template<typename T>
        inline static void write_pod(std::ofstream& f, const T& value)
        {
            f.write(reinterpret_cast<const char*>(&value), sizeof(T));
            if (!f)
                throw platform_core::ecc{ 2, 12 };
        }

        template<typename T>
        inline static void read_pod(std::ifstream& f, T& value)
        {
            f.read(reinterpret_cast<char*>(&value), sizeof(T));
            if (!f)
                throw platform_core::ecc{ 2, 11 };
        }

        inline static void write_bytes(std::ofstream& f, const char* data, QWORD size)
        {
            if (size)
                f.write(data, static_cast<std::streamsize>(size));
            if (!f)
                throw platform_core::ecc{ 2, 12 };
        }

        inline static void read_bytes(std::ifstream& f, char* data, QWORD size)
        {
            if (size)
                f.read(data, static_cast<std::streamsize>(size));
            if (!f)
                throw platform_core::ecc{ 2, 11 };
        }

        inline static void write_string_blob(std::ofstream& f, const std::string& value)
        {
            QWORD len = static_cast<QWORD>(value.size());
            write_pod(f, len);
            write_bytes(f, value.data(), len);
        }

        inline static std::string read_string_blob(std::ifstream& f, QWORD max_len, bool allow_empty)
        {
            QWORD len;
            read_pod(f, len);

            if ((!allow_empty && len == 0) || len > max_len)
                throw platform_core::ecc{ 2, 11 };

            std::string value;
            value.resize(static_cast<size_t>(len));
            read_bytes(f, value.data(), len);
            return value;
        }

        inline static void collect_string_values(const file_system& node, string_table& table, string_id_map& ids)
        {
            if (node.node_type == type::string) {
                if (!node.value)
                    throw platform_core::ecc{ 2, 14 };

                if (!ids.contains(node.value)) {
                    ids.emplace(node.value, static_cast<QWORD>(table.size()));
                    table.push_back(node.value);
                }
            }

            if (node.node_type == type::key && node.storage) {
                for (const auto& [name, child] : *node.storage)
                    collect_string_values(child, table, ids);
            }
        }

        inline static void collect_string_values(const roots& root, string_table& table, string_id_map& ids)
        {
            if (!root.storage)
                return;

            for (const auto& [name, node] : *root.storage)
                collect_string_values(node, table, ids);
        }

        inline static void write_string_section(std::ofstream& f, const string_table& table)
        {
            QWORD count = static_cast<QWORD>(table.size());

            if (count > MAX_STRING_SECTION_COUNT)
                throw platform_core::ecc{ 2, 12 };

            write_pod(f, count);

            for (const std::string* value : table) {
                if (!value)
                    throw platform_core::ecc{ 2, 14 };

                if (value->empty() || static_cast<QWORD>(value->size()) > MAX_STRING_LENGTH)
                    throw platform_core::ecc{ 2, 12 };

                write_string_blob(f, *value);
            }
        }

        inline static std::vector<std::string> read_string_section(std::ifstream& f)
        {
            QWORD count;
            read_pod(f, count);

            if (count > MAX_STRING_SECTION_COUNT)
                throw platform_core::ecc{ 2, 11 };

            std::vector<std::string> table;
            table.reserve(static_cast<size_t>(count));

            for (QWORD i = 0; i < count; ++i)
                table.push_back(read_string_blob(f, MAX_STRING_LENGTH, false));

            return table;
        }

        inline static QWORD string_id_for_node(const file_system& node, const string_id_map& ids)
        {
            if (node.node_type == type::_uninit_value)
                return INVALID_STRING_ID;

            if (node.node_type != type::string)
                throw platform_core::ecc{ 2, 12 };

            if (!node.value)
                throw platform_core::ecc{ 2, 14 };

            auto it = ids.find(node.value);
            if (it == ids.end())
                throw platform_core::ecc{ 2, 12 };

            return it->second;
        }

        inline static void write_node(std::ofstream& f, const file_system& node, const string_id_map& ids)
        {
            type disk_type = node.node_type;
            if (disk_type == type::_uninit_key)
                disk_type = type::key;
            else if (disk_type == type::_uninit_value)
                disk_type = type::string;

            BYTE raw_type = static_cast<BYTE>(disk_type);
            write_pod(f, raw_type);

            switch (disk_type)
            {
            case type::string:
            {
                QWORD id = string_id_for_node(node, ids);
                write_pod(f, id);
                break;
            }

            case type::byte:
                write_pod(f, node.b);
                break;

            case type::word:
                write_pod(f, node.w);
                break;

            case type::dword:
                write_pod(f, node.dw);
                break;

            case type::qword:
                write_pod(f, node.qw);
                break;

            case type::key:
                break;

            default:
                throw platform_core::ecc{ 2, 12 };
            }

            QWORD child_count = 0;

            if (disk_type == type::key && node.node_type == type::key && node.storage)
                child_count = static_cast<QWORD>(node.storage->size());

            write_pod(f, child_count);

            if (disk_type == type::key && node.node_type == type::key && node.storage)
            {
                for (const auto& [name, child] : *node.storage)
                {
                    if (name.empty() || static_cast<QWORD>(name.size()) > MAX_KEY_LENGTH)
                        throw platform_core::ecc{ 2, 12 };

                    write_string_blob(f, name);
                    write_node(f, child, ids);
                }
            }
        }

        inline static file_system read_node(std::ifstream& f, const std::vector<std::string>& string_section)
        {
            try
            {
                BYTE raw_type;
                read_pod(f, raw_type);

                type node_type = static_cast<type>(raw_type);

                switch (node_type)
                {
                case type::string:
                {
                    QWORD id;
                    read_pod(f, id);

                    file_system node(id == INVALID_STRING_ID ? std::string() : [&]() -> const std::string& {
                        if (id >= static_cast<QWORD>(string_section.size()))
                            throw platform_core::ecc{ 2, 11 };
                        return string_section[static_cast<size_t>(id)];
                        }());

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::byte:
                {
                    BYTE value;
                    read_pod(f, value);

                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::word:
                {
                    WORD value;
                    read_pod(f, value);

                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::dword:
                {
                    DWORD value;
                    read_pod(f, value);

                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::qword:
                {
                    QWORD value;
                    read_pod(f, value);

                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::key:
                {
                    file_system node;

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count > MAX_CHILD_COUNT)
                        throw platform_core::ecc{ 2, 11 };

                    if (child_count == 0)
                        return node;

                    auto& children = node.ensure_storage();
                    children.reserve(static_cast<size_t>(child_count));

                    for (QWORD i = 0; i < child_count; ++i)
                    {
                        std::string key = read_string_blob(f, MAX_KEY_LENGTH, false);
                        file_system child = read_node(f, string_section);

                        auto [it, inserted] = children.emplace(std::move(key), std::move(child));

                        if (!inserted)
                            throw platform_core::ecc{ 2, 11 };
                    }

                    return node;
                }

                default:
                    throw platform_core::ecc{ 2, 11 };
                }
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 20 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 20 };
            }
        }

        inline static file_system read_node_legacy(std::ifstream& f)
        {
            try
            {
                BYTE raw_type;
                read_pod(f, raw_type);

                type node_type = static_cast<type>(raw_type);

                switch (node_type)
                {
                case type::string:
                {
                    std::string value = read_string_blob(f, MAX_STRING_LENGTH, true);
                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::byte:
                {
                    BYTE value;
                    read_pod(f, value);
                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::word:
                {
                    WORD value;
                    read_pod(f, value);
                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::dword:
                {
                    DWORD value;
                    read_pod(f, value);
                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::qword:
                {
                    QWORD value;
                    read_pod(f, value);
                    file_system node(value);

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count != 0)
                        throw platform_core::ecc{ 2, 11 };

                    return node;
                }

                case type::key:
                {
                    file_system node;

                    QWORD child_count;
                    read_pod(f, child_count);

                    if (child_count > MAX_CHILD_COUNT)
                        throw platform_core::ecc{ 2, 11 };

                    if (child_count == 0)
                        return node;

                    auto& children = node.ensure_storage();
                    children.reserve(static_cast<size_t>(child_count));

                    for (QWORD i = 0; i < child_count; ++i)
                    {
                        std::string key = read_string_blob(f, MAX_KEY_LENGTH, false);
                        file_system child = read_node_legacy(f);

                        auto [it, inserted] = children.emplace(std::move(key), std::move(child));

                        if (!inserted)
                            throw platform_core::ecc{ 2, 11 };
                    }

                    return node;
                }

                default:
                    throw platform_core::ecc{ 2, 11 };
                }
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 20 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 20 };
            }
        }

        inline static roots* load_body_legacy(std::ifstream& f)
        {
            roots* r = nullptr;

            try
            {
                r = new roots;

                QWORD root_count;
                read_pod(f, root_count);

                if (root_count > MAX_ROOT_COUNT)
                    throw platform_core::ecc{ 2, 11 };

                r->storage->reserve(static_cast<size_t>(root_count));

                for (QWORD i = 0; i < root_count; ++i)
                {
                    std::string key = read_string_blob(f, MAX_KEY_LENGTH, false);
                    file_system node = read_node_legacy(f);

                    auto [it, inserted] = r->storage->emplace(std::move(key), std::move(node));

                    if (!inserted)
                        throw platform_core::ecc{ 2, 11 };
                }

                return r;
            }
            catch (const std::bad_alloc&)
            {
                delete r;
                throw platform_core::ecc{ 2, 20 };
            }
            catch (const std::length_error&)
            {
                delete r;
                throw platform_core::ecc{ 2, 20 };
            }
            catch (...)
            {
                delete r;
                throw;
            }
        }

        inline static roots* load_body_v2(std::ifstream& f)
        {
            roots* r = nullptr;

            try
            {
                std::vector<std::string> string_section = read_string_section(f);

                r = new roots;

                QWORD root_count;
                read_pod(f, root_count);

                if (root_count > MAX_ROOT_COUNT)
                    throw platform_core::ecc{ 2, 11 };

                r->storage->reserve(static_cast<size_t>(root_count));

                for (QWORD i = 0; i < root_count; ++i)
                {
                    std::string key = read_string_blob(f, MAX_KEY_LENGTH, false);
                    file_system node = read_node(f, string_section);

                    auto [it, inserted] = r->storage->emplace(std::move(key), std::move(node));

                    if (!inserted)
                        throw platform_core::ecc{ 2, 11 };
                }

                return r;
            }
            catch (const std::bad_alloc&)
            {
                delete r;
                throw platform_core::ecc{ 2, 20 };
            }
            catch (const std::length_error&)
            {
                delete r;
                throw platform_core::ecc{ 2, 20 };
            }
            catch (...)
            {
                delete r;
                throw;
            }
        }

    public:
        storage_map* storage;

        roots() {
            storage = new storage_map();
        }

        ~roots() {
            delete storage;
            storage = nullptr;
        }

        roots(const roots& other) {
            if (other.storage)
                storage = new storage_map(*other.storage);
            else
                storage = new storage_map();
        }

        roots& operator=(const roots& other) {
            if (this == &other)
                return *this;

            storage_map* new_storage;

            if (other.storage)
                new_storage = new storage_map(*other.storage);
            else
                new_storage = new storage_map();

            delete storage;
            storage = new_storage;

            return *this;
        }

        roots(roots&& other) noexcept {
            storage = other.storage;
            other.storage = nullptr;
        }

        roots& operator=(roots&& other) noexcept {
            if (this == &other)
                return *this;

            delete storage;

            storage = other.storage;
            other.storage = nullptr;

            return *this;
        }

        inline std::string readchild_value(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(target_name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            return it->second.read_own_value();
        }

        inline BYTE readchild_byte(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(target_name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::byte) throw platform_core::ecc{ 2, 3 };
            return it->second.b;
        }

        inline WORD readchild_word(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(target_name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::word) throw platform_core::ecc{ 2, 4 };
            return it->second.w;
        }

        inline DWORD readchild_dword(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(target_name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::dword) throw platform_core::ecc{ 2, 5 };
            return it->second.dw;
        }

        inline QWORD readchild_qword(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(target_name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::qword) throw platform_core::ecc{ 2, 6 };
            return it->second.qw;
        }

        inline void modify_child_value(const std::string& name, const std::string& value) {
            if (!storage) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            it->second.set_own_value(value);
        }

        inline void modify_child_byte(const std::string& name, BYTE byte) {
            if (!storage) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::byte) throw platform_core::ecc{ 2, 3 };
            it->second.b = byte;
        }

        inline void modify_child_word(const std::string& name, WORD word) {
            if (!storage) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::word) throw platform_core::ecc{ 2, 4 };
            it->second.w = word;
        }

        inline void modify_child_dword(const std::string& name, DWORD dword) {
            if (!storage) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::dword) throw platform_core::ecc{ 2, 5 };
            it->second.dw = dword;
        }

        inline void modify_child_qword(const std::string& name, QWORD qword) {
            if (!storage) throw platform_core::ecc{ 2, 0 };
            auto it = storage->find(name);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (it->second.node_type != type::qword) throw platform_core::ecc{ 2, 6 };
            it->second.qw = qword;
        }

        inline bool exists(const std::string& target_name) {
            if (!storage || storage->empty()) return false;
            return storage->find(target_name) != storage->end();
        }

        inline void delete_child(const std::string& target_name) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };
            if (storage->erase(target_name) == 0) throw platform_core::ecc{ 2, 1 };
        }

        inline void rename_child(const std::string& oldn, const std::string& newn) {
            if (!storage || storage->empty()) throw platform_core::ecc{ 2, 0 };

            auto it = storage->find(oldn);
            if (it == storage->end()) throw platform_core::ecc{ 2, 1 };
            if (storage->contains(newn)) throw platform_core::ecc{ 2, 9 };

            file_system temp_node = std::move(it->second);
            storage->erase(it);

            auto backup = temp_node;
            try {
                storage->emplace(newn, std::move(temp_node));
            }
            catch (...) {
                storage->emplace(oldn, std::move(backup));
            }
        }

        static bool validate(std::ifstream& f) {
            char sig[static_cast<size_t>(SIGNATURE_SIZE)]{};

            f.read(sig, static_cast<std::streamsize>(SIGNATURE_SIZE));

            if (!f)
                return false;

            return std::memcmp(sig, FORMAT_SIGNATURE, static_cast<size_t>(SIGNATURE_SIZE)) == 0;
        }

        static void save_to_disk(const std::string& path, const roots& r)
        {
            std::ofstream f(path, std::ios::binary | std::ios::trunc);

            if (!f)
                throw platform_core::ecc{ 2, 12 };

            write_bytes(f, FORMAT_SIGNATURE, SIGNATURE_SIZE);
            write_bytes(f, FORMAT_V2_MAGIC, V2_MAGIC_SIZE);

            string_table table;
            string_id_map ids;
            collect_string_values(r, table, ids);
            write_string_section(f, table);

            QWORD root_count = 0;

            if (r.storage)
                root_count = static_cast<QWORD>(r.storage->size());

            write_pod(f, root_count);

            if (r.storage)
            {
                for (const auto& [name, node] : *r.storage)
                {
                    if (name.empty() || static_cast<QWORD>(name.size()) > MAX_KEY_LENGTH)
                        throw platform_core::ecc{ 2, 12 };

                    write_string_blob(f, name);
                    write_node(f, node, ids);
                }
            }

            f.flush();

            if (!f)
                throw platform_core::ecc{ 2, 12 };
        }

        static roots* load_from_disk(const std::string& path)
        {
            std::ifstream f(path, std::ios::binary);
            if (!f)
                throw platform_core::ecc{ 2, 12 };

            if (!validate(f))
                throw platform_core::ecc{ 2, 13 };

            char magic[static_cast<size_t>(V2_MAGIC_SIZE)]{};
            f.read(magic, static_cast<std::streamsize>(V2_MAGIC_SIZE));

            if (!f)
                throw platform_core::ecc{ 2, 11 };

            if (std::memcmp(magic, FORMAT_V2_MAGIC, static_cast<size_t>(V2_MAGIC_SIZE)) == 0)
                return load_body_v2(f);

            f.clear();
            f.seekg(static_cast<std::streamoff>(SIGNATURE_SIZE), std::ios::beg);

            if (!f)
                throw platform_core::ecc{ 2, 11 };

            return load_body_legacy(f);
        }
    };


    class registry_editor_service_local {
    private:
        std::shared_mutex mutex;
        std::shared_mutex clsmtx;
        ankerl::unordered_dense::map<QWORD, std::function<void()>> listeners;
        std::vector<QWORD> used_ids;
        std::string registry_file_name;
#ifdef RECOVERY
        std::ofstream files;
        std::string recovery_file_path;
#endif
    private:
        roots* root;
        friend registry_editor_service_local* registry_begin(const std::string& path_to_registry_file);
        friend void registry_end(registry_editor_service_local* editor);
#ifdef RECOVERY
        registry_editor_service_local(const std::string& path_to_registry_file, const std::string& recovery_file)
            : root(nullptr)
        {
            try {
                registry_file_name = path_to_registry_file;
                recovery_file_path = recovery_file;
                root = roots::load_from_disk(path_to_registry_file);
                files.open(recovery_file, std::ios::app);
                files << path_to_registry_file << "\n";
            }
            catch (...) {
                delete root;
                root = nullptr;
                throw;
            }
        }
#else
        registry_editor_service_local(const std::string& path_to_registry_file)
            : root(nullptr)
        {
            try {
                registry_file_name = path_to_registry_file;
                root = roots::load_from_disk(path_to_registry_file);
            }
            catch (...) {
                delete root;
                root = nullptr;
                throw;
            }
        }
#endif
        ~registry_editor_service_local() {
            delete root;
        };
        inline void notify_update()
        {
            std::vector<std::function<void()>> copy;

            {
                std::shared_lock<std::shared_mutex> lock(clsmtx);
                copy.reserve(listeners.size());

                for (auto& [id, fn] : listeners)
                    copy.push_back(fn);
            }

            for (auto& fn : copy) {
                try {
                    fn();
                }
                catch (...) {}
            }
        }

        inline file_system* to_node(const std::string& paths)
        {
            if (!root || !root->storage)
                throw platform_core::ecc{ 2, 0 };

            const QWORD n = static_cast<QWORD>(paths.size());

            if (n == 0)
                throw platform_core::ecc{ 2, 7 };

            const char* data = paths.data();

            static constexpr char root_name[] = "*__root__*";
            static constexpr QWORD root_len = sizeof(root_name) - 1;

            if (n < root_len || std::memcmp(data, root_name, root_len) != 0)
                throw platform_core::ecc{ 2, 8 };

            if (n == root_len)
                return nullptr;

            if (data[root_len] != '>')
                throw platform_core::ecc{ 2, 8 };

            thread_local std::string key;
            key.clear();

            if (key.capacity() < 256)
                key.reserve(256);

            QWORD start = root_len + 1;
            QWORD i = start;

            while (i < n && data[i] != '>')
                ++i;

            if (i == start)
                throw platform_core::ecc{ 2, 8 };

            key.assign(data + start, static_cast<size_t>(i - start));

            auto it = root->storage->find(key);

            if (it == root->storage->end())
                throw platform_core::ecc{ 2, 1 };

            file_system* current = &it->second;

            if (i == n)
                return current;

            start = i + 1;

            while (true)
            {
                if (current->node_type == type::_uninit_key)
                    throw platform_core::ecc{ 2, 1 };

                if (current->node_type != type::key)
                    throw platform_core::ecc{ 2, 0 };

                auto* storage = current->storage;
                if (!storage || storage->empty())
                    throw platform_core::ecc{ 2, 1 };

                i = start;

                while (i < n && data[i] != '>')
                    ++i;

                if (i == start)
                    throw platform_core::ecc{ 2, 8 };

                key.assign(data + start, static_cast<size_t>(i - start));

                auto child = storage->find(key);

                if (child == storage->end())
                    throw platform_core::ecc{ 2, 1 };

                current = &child->second;

                if (i == n)
                    return current;

                start = i + 1;
            }
        }

        using registry_storage = ankerl::unordered_dense::map<std::string, file_system>;

        inline registry_storage& storage_for_path(const std::string& keyname_path)
        {
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr)
                return *root->storage;
            if (parent->node_type == type::_uninit_key)
                throw platform_core::ecc{ 2, 1 };
            if (parent->node_type != type::key || !parent->storage)
                throw platform_core::ecc{ 2, 0 };
            return *parent->storage;
        }

        inline registry_storage& ensure_storage_for_path(const std::string& keyname_path)
        {
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr)
                return *root->storage;
            return parent->ensure_storage();
        }

        inline static file_system& require_child(registry_storage& storage, const std::string& name)
        {
            auto it = storage.find(name);
            if (it == storage.end())
                throw platform_core::ecc{ 2, 1 };
            return it->second;
        }

        inline static const file_system& require_child_const(const registry_storage& storage, const std::string& name)
        {
            auto it = storage.find(name);
            if (it == storage.end())
                throw platform_core::ecc{ 2, 1 };
            return it->second;
        }

        inline static void assign_value(file_system& node, const std::string& value)
        {
            try
            {
                node.set_own_value(value);
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
        }

        inline static void assign_byte(file_system& node, BYTE value)
        {
            if (node.node_type != type::byte)
                throw platform_core::ecc{ 2, 3 };
            node.b = value;
        }

        inline static void assign_word(file_system& node, WORD value)
        {
            if (node.node_type != type::word)
                throw platform_core::ecc{ 2, 4 };
            node.w = value;
        }

        inline static void assign_dword(file_system& node, DWORD value)
        {
            if (node.node_type != type::dword)
                throw platform_core::ecc{ 2, 5 };
            node.dw = value;
        }

        inline static void assign_qword(file_system& node, QWORD value)
        {
            if (node.node_type != type::qword)
                throw platform_core::ecc{ 2, 6 };
            node.qw = value;
        }
#ifdef RECOVERY
        template<typename Mutator, typename Logger>
        inline void atomic_batch(const std::string& keyname_path, Mutator mutator, Logger logger)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = ensure_storage_for_path(keyname_path);
            registry_storage backup = target;
            try {
                mutator(target);
            }
            catch (...) {
                target = std::move(backup);
                throw;
            }
            logger();
            files.flush();
        }
#else
        template<typename Mutator>
        inline void atomic_batch(const std::string& keyname_path, Mutator mutator)
        {
            std::unique_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = ensure_storage_for_path(keyname_path);
            registry_storage backup = target;
            try {
                mutator(target);
            }
            catch (...) {
                target = std::move(backup);
                throw;
            }
        }
#endif
    public:
        void save()
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            if (root) roots::save_to_disk(registry_file_name, *root);
#ifdef RECOVERY
            files.close();
            {
                std::ofstream out(recovery_file_path, std::ios::out | std::ios::trunc);
                out << registry_file_name << '\n';
                out.close();
            }
            files.open(recovery_file_path, std::ios::out | std::ios::app);
#endif
        }
        inline void create_key(const std::string& key_name, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(key_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                auto [it, inserted] = target.try_emplace(key_name);
                if (!inserted)
                    throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                files << "\"1\"|" << '"' << key_name << '"' << "|" << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void create_value(const std::string& value_name, const std::string& value_data, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            try
            {
                {
                    std::unique_lock<std::shared_mutex> lock(mutex);
                    registry_storage& target = ensure_storage_for_path(keyname_path);
                    auto [it, inserted] = target.try_emplace(value_name, value_data);
                    if (!inserted)
                        throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                    files << "\"2\"|" << '"' << value_name << '"' << '|' << '"' << value_data << '"' << '|' << '"' << keyname_path << '"' << '\n';
                    files.flush();
#endif
                }
                notify_update();
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 22 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 22 };
            }
        }

        inline void create_byte(const std::string& value_name, BYTE value_data, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                auto [it, inserted] = target.try_emplace(value_name, value_data);
                if (!inserted)
                    throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                files << "\"3\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void create_word(const std::string& value_name, WORD value_data, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                auto [it, inserted] = target.try_emplace(value_name, value_data);
                if (!inserted)
                    throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                files << "\"4\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void create_dword(const std::string& value_name, DWORD value_data, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                auto [it, inserted] = target.try_emplace(value_name, value_data);
                if (!inserted)
                    throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                files << "\"5\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void create_qword(const std::string& value_name, QWORD value_data, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                auto [it, inserted] = target.try_emplace(value_name, value_data);
                if (!inserted)
                    throw platform_core::ecc{ 2, 9 };
#ifdef RECOVERY
                files << "\"6\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void modify_value(const std::string& value_name, const std::string& keyname_path, const std::string& new_value) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            try
            {
                {
                    std::unique_lock<std::shared_mutex> lock(mutex);
                    registry_storage& target = ensure_storage_for_path(keyname_path);
                    assign_value(require_child(target, value_name), new_value);
#ifdef RECOVERY
                    files << "\"7\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << new_value << '"' << '\n';
                    files.flush();
#endif
                }
                notify_update();
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
        }

        inline void modify_byte(const std::string& value_name, const std::string& keyname_path, BYTE new_byte) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                assign_byte(require_child(target, value_name), new_byte);
#ifdef RECOVERY
                files << "\"8\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(new_byte) << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void modify_word(const std::string& value_name, const std::string& keyname_path, WORD new_word) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                assign_word(require_child(target, value_name), new_word);
#ifdef RECOVERY
                files << "\"9\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(new_word) << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }
        inline void modify_dword(const std::string& value_name, const std::string& keyname_path, DWORD new_dword) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                assign_dword(require_child(target, value_name), new_dword);
#ifdef RECOVERY
                files << "\"10\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(new_dword) << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline void modify_qword(const std::string& value_name, const std::string& keyname_path, QWORD new_qword) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                assign_qword(require_child(target, value_name), new_qword);
#ifdef RECOVERY
                files << "\"11\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(new_qword) << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline std::string read_value(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr) {
                return root->readchild_value(value_name);
            }
            else {
                return parent->readchild_value(value_name);
            }
        }

        inline BYTE read_byte(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr) {
                return root->readchild_byte(value_name);
            }
            else {
                return parent->readchild_byte(value_name);
            }
        }

        inline WORD read_word(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr) {
                return root->readchild_word(value_name);
            }
            else {
                return parent->readchild_word(value_name);
            }
        }

        inline DWORD read_dword(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr) {
                return root->readchild_dword(value_name);
            }
            else {
                return parent->readchild_dword(value_name);
            }
        }

        inline QWORD read_qword(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr) {
                return root->readchild_qword(value_name);
            }
            else {
                return parent->readchild_qword(value_name);
            }
        }

        inline type get_type(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* parent = to_node(keyname_path);
            if (parent == nullptr)
            {
                auto it = root->storage->find(value_name);
                if (it == root->storage->end())
                    throw platform_core::ecc{ 2, 1 };
                return it->second.public_type();
            }
            else
            {
                if (parent->node_type == type::_uninit_key)
                    throw platform_core::ecc{ 2, 1 };
                if (parent->node_type != type::key || !parent->storage)
                    throw platform_core::ecc{ 2, 0 };
                auto it = parent->storage->find(value_name);
                if (it == parent->storage->end())
                    throw platform_core::ecc{ 2, 1 };
                return it->second.public_type();
            }
        }

        inline void rename(const std::string& keyname_path, const std::string& old_name, const std::string& new_name)
        {
#ifndef NO_CHECK_VALID
            check_name(old_name);
            check_name(new_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = ensure_storage_for_path(keyname_path);
                registry_storage backup = target;
                try {
                    auto it = target.find(old_name);
                    if (it == target.end())
                        throw platform_core::ecc{ 2, 1 };
                    if (target.contains(new_name))
                        throw platform_core::ecc{ 2, 9 };
                    file_system temp_node = std::move(it->second);
                    target.erase(it);
                    target.emplace(new_name, std::move(temp_node));
                }
                catch (...) {
                    target = std::move(backup);
                    throw;
                }
#ifdef RECOVERY
                files << "\"12\"|" << '"' << keyname_path << '"' << '|' << '"' << old_name << '"' << '|' << '"' << new_name << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline bool contains(const std::string& value_name, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            try {
                file_system* parent = to_node(keyname_path);
                if (parent == nullptr)
                    return root->storage->find(value_name) != root->storage->end();
                if (parent->node_type != type::key || !parent->storage)
                    return false;
                return parent->storage->find(value_name) != parent->storage->end();
            }
            catch (...) {
                return false;
            }
        }

        inline void create_multiple_key(const std::vector<std::string>& list_of_key_name, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            for (const auto& key_name : list_of_key_name)
                check_name(key_name);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& key_name : list_of_key_name) {
                        auto [it, inserted] = target.try_emplace(key_name);
                        if (!inserted)
                            throw platform_core::ecc{ 2, 9 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& key_name : list_of_key_name)
                        files << "\"1\"|" << '"' << key_name << '"' << "|" << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void create_multiple_value(const std::vector<std::pair<std::string, std::string>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            try
            {
                atomic_batch(keyname_path,
                    [&](registry_storage& target) {
                        for (const auto& [value_name, value_data] : list_of_value_name_and_data) {
                            auto [it, inserted] = target.try_emplace(value_name, value_data);
                            if (!inserted)
                                throw platform_core::ecc{ 2, 9 };
                        }
                    }
#ifdef RECOVERY
                    , [&]() {
                        for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                            files << "\"2\"|" << '"' << value_name << '"' << '|' << '"' << value_data << '"' << '|' << '"' << keyname_path << '"' << '\n';
                    }
#endif
                );
                notify_update();
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 22 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 22 };
            }
        }

        inline void create_multiple_byte(const std::vector<std::pair<std::string, BYTE>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data) {
                        auto [it, inserted] = target.try_emplace(value_name, value_data);
                        if (!inserted)
                            throw platform_core::ecc{ 2, 9 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"3\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void create_multiple_word(const std::vector<std::pair<std::string, WORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data) {
                        auto [it, inserted] = target.try_emplace(value_name, value_data);
                        if (!inserted)
                            throw platform_core::ecc{ 2, 9 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"4\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void create_multiple_dword(const std::vector<std::pair<std::string, DWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data) {
                        auto [it, inserted] = target.try_emplace(value_name, value_data);
                        if (!inserted)
                            throw platform_core::ecc{ 2, 9 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"5\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void create_multiple_qword(const std::vector<std::pair<std::string, QWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data) {
                        auto [it, inserted] = target.try_emplace(value_name, value_data);
                        if (!inserted)
                            throw platform_core::ecc{ 2, 9 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"6\"|" << '"' << value_name << '"' << '|' << '"' << std::to_string(value_data) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void modify_multiple_value(const std::vector<std::pair<std::string, std::string>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            try
            {
                atomic_batch(keyname_path,
                    [&](registry_storage& target) {
                        for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                            assign_value(require_child(target, value_name), value_data);
                    }
#ifdef RECOVERY
                    , [&]() {
                        for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                            files << "\"7\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << value_data << '"' << '\n';
                    }
#endif
                );
                notify_update();
            }
            catch (const std::bad_alloc&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
            catch (const std::length_error&)
            {
                throw platform_core::ecc{ 2, 21 };
            }
        }

        inline void modify_multiple_byte(const std::vector<std::pair<std::string, BYTE>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        assign_byte(require_child(target, value_name), value_data);
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"8\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(value_data) << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void modify_multiple_word(const std::vector<std::pair<std::string, WORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        assign_word(require_child(target, value_name), value_data);
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"9\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(value_data) << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void modify_multiple_dword(const std::vector<std::pair<std::string, DWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        assign_dword(require_child(target, value_name), value_data);
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"10\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(value_data) << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline void modify_multiple_qword(const std::vector<std::pair<std::string, QWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data)
                check_name(item.first);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        assign_qword(require_child(target, value_name), value_data);
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [value_name, value_data] : list_of_value_name_and_data)
                        files << "\"11\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '|' << '"' << std::to_string(value_data) << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline std::vector<std::pair<std::string, std::string>> read_multiple_value(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, std::string>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                returned.emplace_back(value_name, node.read_own_value());
            }
            return returned;
        }

        inline std::vector<std::pair<std::string, BYTE>> read_multiple_byte(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, BYTE>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                if (node.node_type != type::byte)
                    throw platform_core::ecc{ 2, 3 };
                returned.emplace_back(value_name, node.b);
            }
            return returned;
        }

        inline std::vector<std::pair<std::string, WORD>> read_multiple_word(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, WORD>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                if (node.node_type != type::word)
                    throw platform_core::ecc{ 2, 4 };
                returned.emplace_back(value_name, node.w);
            }
            return returned;
        }

        inline std::vector<std::pair<std::string, DWORD>> read_multiple_dword(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, DWORD>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                if (node.node_type != type::dword)
                    throw platform_core::ecc{ 2, 5 };
                returned.emplace_back(value_name, node.dw);
            }
            return returned;
        }

        inline std::vector<std::pair<std::string, QWORD>> read_multiple_qword(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, QWORD>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                if (node.node_type != type::qword)
                    throw platform_core::ecc{ 2, 6 };
                returned.emplace_back(value_name, node.qw);
            }
            return returned;
        }

        inline std::vector<std::pair<std::string, type>> get_multiple_type(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, type>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name)
                returned.emplace_back(value_name, require_child_const(target, value_name).public_type());
            return returned;
        }

        inline void rename_multiple(const std::string& keyname_path, const std::vector<std::pair<std::string, std::string>>& list_of_name)
        {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_name) {
                check_name(item.first);
                check_name(item.second);
            }
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& [old_name, new_name] : list_of_name) {
                        auto it = target.find(old_name);
                        if (it == target.end())
                            throw platform_core::ecc{ 2, 1 };
                        if (target.contains(new_name))
                            throw platform_core::ecc{ 2, 9 };
                        file_system temp_node = std::move(it->second);
                        target.erase(it);
                        target.emplace(new_name, std::move(temp_node));
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& [old_name, new_name] : list_of_name)
                        files << "\"12\"|" << '"' << keyname_path << '"' << '|' << '"' << old_name << '"' << '|' << '"' << new_name << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        inline std::vector<std::string> list(const std::string& keyname_path)
        {
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* node = to_node(keyname_path);
            if (node == nullptr)
            {
                std::vector<std::string> result;
                result.reserve(root->storage->size());
                for (const auto& [name, _] : *root->storage)
                    result.push_back(name);
                return result;
            }
            else
            {
                if (node->node_type == type::_uninit_key)
                    return {};
                if (node->node_type != type::key || !node->storage)
                    throw platform_core::ecc{ 2, 0 };
                std::vector<std::string> result;
                result.reserve(node->storage->size());
                for (const auto& [name, _] : *node->storage)
                    result.push_back(name);
                return result;
            }
        }

        inline std::variant<std::string, BYTE, WORD, DWORD, QWORD> read_element(const std::string& value_name, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            const file_system& node = require_child_const(target, value_name);
            switch (node.node_type)
            {
            case type::byte:
                return node.b;
            case type::word:
                return node.w;
            case type::dword:
                return node.dw;
            case type::qword:
                return node.qw;
            case type::string:
                if (!node.value)
                    throw platform_core::ecc{ 2, 14 };
                return *node.value;
            case type::_uninit_value:
                return std::string();
            default:
                throw platform_core::ecc{ 2, 14 };
            }
        }

        void delete_element(const std::string& value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            check_name(value_name);
#endif
            {
                std::unique_lock<std::shared_mutex> lock(mutex);
                registry_storage& target = storage_for_path(keyname_path);
                if (target.erase(value_name) == 0)
                    throw platform_core::ecc{ 2, 1 };
#ifdef RECOVERY
                files << "\"13\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '\n';
                files.flush();
#endif
            }
            notify_update();
        }

        inline std::vector<std::pair<std::string, std::variant<std::string, BYTE, WORD, DWORD, QWORD>>> read_multiple_element(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path)
        {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            std::shared_lock<std::shared_mutex> lock(mutex);
            registry_storage& target = storage_for_path(keyname_path);
            std::vector<std::pair<std::string, std::variant<std::string, BYTE, WORD, DWORD, QWORD>>> returned;
            returned.reserve(list_of_value_name.size());
            for (const auto& value_name : list_of_value_name) {
                const file_system& node = require_child_const(target, value_name);
                switch (node.node_type)
                {
                case type::byte:
                    returned.emplace_back(value_name, node.b);
                    break;
                case type::word:
                    returned.emplace_back(value_name, node.w);
                    break;
                case type::dword:
                    returned.emplace_back(value_name, node.dw);
                    break;
                case type::qword:
                    returned.emplace_back(value_name, node.qw);
                    break;
                case type::string:
                    if (!node.value)
                        throw platform_core::ecc{ 2, 14 };
                    returned.emplace_back(value_name, *node.value);
                    break;
                case type::_uninit_value:
                    returned.emplace_back(value_name, std::string());
                    break;
                default:
                    throw platform_core::ecc{ 2, 14 };
                }
            }
            return returned;
        }

        void delete_multiple_element(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& value_name : list_of_value_name)
                check_name(value_name);
#endif
            atomic_batch(keyname_path,
                [&](registry_storage& target) {
                    for (const auto& value_name : list_of_value_name) {
                        if (target.erase(value_name) == 0)
                            throw platform_core::ecc{ 2, 1 };
                    }
                }
#ifdef RECOVERY
                , [&]() {
                    for (const auto& value_name : list_of_value_name)
                        files << "\"13\"|" << '"' << value_name << '"' << '|' << '"' << keyname_path << '"' << '\n';
                }
#endif
            );
            notify_update();
        }

        std::vector<std::pair<std::string, type>> properties(const std::string& keyname_path) {
            std::shared_lock<std::shared_mutex> lock(mutex);
            file_system* node = to_node(keyname_path);
            std::vector<std::pair<std::string, type>> returned;
            if (node == nullptr)
            {
                returned.reserve(root->storage->size());
                for (const auto& [name, filesystem] : *root->storage)
                    returned.emplace_back(name, filesystem.public_type());
            }
            else
            {
                if (node->node_type == type::_uninit_key)
                    return returned;
                if (node->node_type != type::key || !node->storage)
                    throw platform_core::ecc{ 2, 0 };
                returned.reserve(node->storage->size());
                for (const auto& [name, filesystem] : *node->storage)
                    returned.emplace_back(name, filesystem.public_type());
            }
            return returned;
        }

        void initialization(const std::vector<std::variant<INIT_KEY, INIT_VALUE, INIT_BYTE, INIT_WORD, INIT_DWORD, INIT_QWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
#ifndef NO_CHECK_VALID
            for (const auto& item : list_of_value_name_and_data) {
                std::visit([](const auto& data) {
                    check_name(data.id);
                    }, item);
            }
#endif
            auto to_file_string = [](const auto& value) -> std::string {
                if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
                    return value;
                }
                else {
                    return std::to_string(value);
                }
                };

            auto type_id = [](const auto& data) -> const char* {
                if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_KEY>)
                    return "1";
                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_VALUE>)
                    return "2";
                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_BYTE>)
                    return "3";
                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_WORD>)
                    return "4";
                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_DWORD>)
                    return "5";
                else // INIT_QWORD
                    return "6";
                };

            bool has_string_value = false;

            for (const auto& value_data : list_of_value_name_and_data)
            {
                if (std::holds_alternative<INIT_VALUE>(value_data))
                {
                    has_string_value = true;
                    break;
                }
            }

            try
            {
                atomic_batch(keyname_path,
                    [&](registry_storage& target) {
                        for (const auto& value_data : list_of_value_name_and_data) {
                            std::visit([&](const auto& data) {
                                bool inserted = false;
                                if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_KEY>) {
                                    auto result = target.try_emplace(data.id);
                                    inserted = result.second;
                                }
                                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_VALUE>) {
                                    auto result = target.try_emplace(data.id, data.value);
                                    inserted = result.second;
                                }
                                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_BYTE>) {
                                    auto result = target.try_emplace(data.id, data.value);
                                    inserted = result.second;
                                }
                                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_WORD>) {
                                    auto result = target.try_emplace(data.id, data.value);
                                    inserted = result.second;
                                }
                                else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_DWORD>) {
                                    auto result = target.try_emplace(data.id, data.value);
                                    inserted = result.second;
                                }
                                else {
                                    auto result = target.try_emplace(data.id, data.value);
                                    inserted = result.second;
                                }
                                if (!inserted)
                                    throw platform_core::ecc{ 2, 9 };
                                }, value_data);
                        }
                    }
#ifdef RECOVERY
                    , [&]() {
                        for (const auto& value_data : list_of_value_name_and_data) {
                            std::visit([&](const auto& data) {
                                if constexpr (std::is_same_v<std::decay_t<decltype(data)>, INIT_KEY>) {
                                    files << '"' << type_id(data) << '"' << '|' << '"' << data.id << '"' << '|' << '"' << keyname_path << '"' << '\n';
                                }
                                else {
                                    files << '"' << type_id(data) << '"' << '|' << '"' << data.id << '"' << '|' << '"' << to_file_string(data.value) << '"' << '|' << '"' << keyname_path << '"' << '\n';
                                }
                                }, value_data);
                        }
                    }
#endif
                );
                notify_update();
            }
            catch (const std::bad_alloc&)
            {
                if (has_string_value)
                    throw platform_core::ecc{ 2, 22 };
                throw;
            }
            catch (const std::length_error&)
            {
                if (has_string_value)
                    throw platform_core::ecc{ 2, 22 };
                throw;
            }
        }
    private:
        inline static std::string json_escape(const std::string& s)
        {
            std::string out;
            out.reserve(s.size());

            for (char c : s)
            {
                switch (c)
                {
                case '"':  out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n";  break;
                case '\t': out += "\\t";  break;
                case '\r': out += "\\r";  break;
                default:    out += c;      break;
                }
            }
            return out;
        }
        inline static void build_json_node(
            std::ostringstream& out,
            const std::string& name,
            const file_system& node,
            bool is_last)
        {
            out << "{";
            out << "\"type\":\"node_type::"
                << (node.is_key_like() ? "key" : "value")
                << "\",";
            out << "\"name\":\"" << json_escape(name) << "\"";
            if (node.is_key_like())
            {
                out << ",\"permutation\":[";

                if (node.storage && !node.storage->empty())
                {
                    QWORD i = 0;
                    QWORD n = node.storage->size();

                    for (const auto& [child_name, child] : *node.storage)
                    {
                        build_json_node(out, child_name, child, i + 1 == n);
                        if (++i != n) out << ",";
                    }
                }

                out << "]";
            }
            else
            {
                out << ",\"value\":";

                switch (node.node_type)
                {
                case type::string:
                    if (!node.value)
                        throw platform_core::ecc{ 2, 14 };
                    out << "\"" << json_escape(*node.value) << "\"";
                    break;
                case type::_uninit_value:
                    out << "\"\"";
                    break;
                case type::byte:
                    out << +node.b;
                    break;
                case type::word:
                    out << node.w;
                    break;
                case type::dword:
                    out << node.dw;
                    break;
                case type::qword:
                    out << node.qw;
                    break;
                default:
                    out << "null";
                    break;
                }
            }

            out << "}";
        }

        static const char* type_name(type t)
        {
            switch (t)
            {
            case type::key:
            case type::_uninit_key:
                return "key";
            case type::string:
            case type::_uninit_value:
                return "string";
            case type::byte:   return "byte";
            case type::word:   return "word";
            case type::dword:  return "dword";
            case type::qword:  return "qword";
            default:           return "unknown";
            }
        }

        inline static void build_tree(
            const std::string& name,
            const file_system& node,
            std::ostringstream& out,
            const std::string& prefix,
            bool is_last)
        {
            out << prefix
                << (is_last ? "└───" : "├───");

            out << '[' << type_name(node.node_type) << "] "
                << name;

            switch (node.node_type)
            {
            case type::string:
                if (!node.value)
                    throw platform_core::ecc{ 2, 14 };
                out << " : \"" << *node.value << '"';
                break;

            case type::_uninit_value:
                out << " : \"\"";
                break;

            case type::byte:
                out << " : " << (DWORD)node.b;
                break;

            case type::word:
                out << " : " << node.w;
                break;

            case type::dword:
                out << " : " << node.dw;
                break;

            case type::qword:
                out << " : " << node.qw;
                break;

            default:
                break;
            }

            out << '\n';

            if (!node.is_key_like())
                return;

            if (node.node_type == type::_uninit_key || !node.storage || node.storage->empty())
                return;

            const std::string next_prefix =
                prefix + (is_last ? "    " : "│   ");

            QWORD index = 0;
            const QWORD count = node.storage->size();

            for (const auto& [child_name, child] : *node.storage)
            {
                build_tree(
                    child_name,
                    child,
                    out,
                    next_prefix,
                    ++index == count);
            }
        }

    public:
        inline QWORD update(std::function<void()> fn)
        {
            std::unique_lock lock(clsmtx);

            QWORD id = 0;
            for (auto v : used_ids)
            {
                if (v != id)
                    break;
                ++id;
            }

            used_ids.insert(
                std::lower_bound(used_ids.begin(), used_ids.end(), id),
                id
            );

            listeners.emplace(id, std::move(fn));
            return id;
        }
        inline void unupdate(QWORD id)
        {
            std::unique_lock lock(clsmtx);

            listeners.erase(id);

            auto it = std::lower_bound(
                used_ids.begin(),
                used_ids.end(),
                id
            );

            if (it != used_ids.end() && *it == id)
                used_ids.erase(it);
        }
        inline std::string json_tree()
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::ostringstream out;

            out << "{";
            out << "\"format_version\":\"1.0A\",";

            out << "\"section_data\":{";
            out << "\"description\":{";
            out << "\"identifier\":\"global.registry_json_tree_export_format\",";
            out << "\"properties\":[]";
            out << "},";

            out << "\"components\":{";
            out << "\"registry_tree\":[";
            out << "{";
            out << "\"type\":\"node_type::key\",";
            out << "\"name\":\"*__root__*\",";
            out << "\"permutation\":[";

            QWORD i = 0;
            QWORD n = root && root->storage ? root->storage->size() : 0;

            if (root && root->storage) for (const auto& [name, node] : *root->storage)
            {
                build_json_node(out, name, node, i + 1 == n);
                if (++i != n) out << ",";
            }

            out << "]";
            out << "}";

            out << "]";
            out << "}";
            out << "}";
            out << "}";

            std::string result = out.str();
            return result;
        }
        inline std::string tree()
        {
            std::shared_lock<std::shared_mutex> lock(mutex);

            std::ostringstream out;
            out << "*__root__*\n";

            QWORD index = 0;
            const QWORD count = root && root->storage ? root->storage->size() : 0;

            if (root && root->storage) for (const auto& [name, node] : *root->storage) {
                build_tree(name, node, out, "", ++index == count);
            }

            return out.str();
        }
        inline std::string get_keyname_path() { return registry_file_name; };
    };

    inline ankerl::unordered_dense::map<std::string_view, std::pair<registry_editor_service_local*, QWORD>> returned;

    inline void create_registry_file(const std::string& path) {
        roots r;
        roots::save_to_disk(path, r);
    }

    inline void delete_registry_file(const std::string& path) {
        if (!std::filesystem::exists(path)) throw platform_core::ecc{ 2, 10 };

        std::filesystem::remove(path);
    }

    inline std::string registry_normalize_path(const std::string& path)
    {
        std::filesystem::path p(path);
        try
        {
            if (p.is_relative())
                p = std::filesystem::current_path() / p;

            p = std::filesystem::weakly_canonical(p);

            return p.generic_string();
        }
        catch (...)
        {
            if (p.is_relative())
                p = std::filesystem::current_path() / p;

            p = p.lexically_normal();

            return p.generic_string();
        }
    }

    registry_editor_service_local* registry_begin(const std::string& path_to_registry_file) {
        std::unique_lock<std::shared_mutex> lock(mtxs);
        {
            auto a = returned.find(path_to_registry_file);
            if (a != returned.end()) {
                a->second.second += 1;
                return a->second.first;
            }
        }

        registry_editor_service_local* returneds = nullptr;
        namespace fs = std::filesystem;
#ifdef RECOVERY
        std::string rcvaffr;
#endif
        try {
#ifdef RECOVERY
            {
                fs::path folder = "../Program Datas/recovery-log";
                std::string base = "recovery";
                std::string ext = ".rcvaffr";
                QWORD index = 0;
                while (true)
                {
                    std::string name = index == 0
                        ? base + ext
                        : base + "_" + std::to_string(index) + ext;
                    if (!fs::exists(folder / name))
                    {
                        rcvaffr = registry_normalize_path(("../Program Datas/recovery-log/" + name));
                        break;
                    }
                    ++index;
                }
            }
            returneds = new registry_editor_service_local(path_to_registry_file, rcvaffr);
#else
            returneds = new registry_editor_service_local(path_to_registry_file);
#endif
            returned.emplace(
                returneds->registry_file_name,
                std::pair<registry_editor_service_local*, QWORD>(
                    returneds,
                    1ULL
                )
            );
            return returneds;
        }
        catch (const std::bad_alloc&) {
            delete returneds;
            throw platform_core::ecc{ 2, 20 };
        }
        catch (const std::length_error&) {
            delete returneds;
            throw platform_core::ecc{ 2, 20 };
        }
        catch (platform_core::ecc& e) {
            throw e;
        }
        catch (...) {
            delete returneds;
            throw;
        }
    }

    void registry_end(registry_editor_service_local* editor) {
        if (!editor)
            throw platform_core::ecc{ 2, 17 };

        std::unique_lock<std::shared_mutex> lock(mtxs);
        auto a = returned.find(editor->registry_file_name);
        if (a == returned.end())
            throw platform_core::ecc{ 2, 17 };

        if (a->second.second == 0)
            throw platform_core::ecc{ 2, 17 };

        if (a->second.second > 1) {
            a->second.second -= 1;
            return;
        }

        editor->save();
#ifdef RECOVERY
        editor->files.close();
        std::filesystem::remove(editor->recovery_file_path);
#endif
        returned.erase(editor->registry_file_name);
        delete editor;
    }
}