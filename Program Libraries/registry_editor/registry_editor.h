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

#include "../unordered_dense/include/ankerl/unordered_dense.h"

#define NO_RECORDER
#define NO_ERROR_LOOKINGUP
#define NO_UAC

#include "../../Program Libraries/system/system.h"

#undef NO_UAC
#undef NO_RECORDER
#undef NO_ERROR_LOOKINGUP

#include "../utilityX/utilityX.h"

#include <string>
#include <filesystem>
#include <vector>
#include <variant>
#include <functional>
#include <mutex>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <array>
#include <string_view>

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define BYTE   WINSDK_BYTE
#define WORD   WINSDK_WORD
#define DWORD  WINSDK_DWORD
#define QWORD  WINSDK_QWORD
#define ULONG  WINSDK_ULONG
#define LONG   WINSDK_LONG
#define BOOL   WINSDK_BOOL

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#undef BYTE
#undef WORD
#undef DWORD
#undef QWORD
#undef ULONG
#undef LONG
#undef BOOL

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

using SOCKET = int;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#endif

namespace registry_editor {
    inline void CLOSE_SOCKET(SOCKET s) noexcept
    {
        if (s == INVALID_SOCKET)
            return;

    #ifdef _WIN32
        closesocket(s);
    #else
        close(s);
    #endif
    }

    enum class type : BYTE {
        key = 0,
        string,
        byte,
        word,
        dword,
        qword
    };

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

    namespace {
        enum class Op : BYTE {
            CREATE_REGISTRY_FILE = 1,
            DELETE_REGISTRY_FILE,
            REGISTRY_BEGIN,
            REGISTRY_END,
            CREATE_KEY,
            CREATE_VALUE,
            CREATE_BYTE,
            CREATE_WORD,
            CREATE_DWORD,
            CREATE_QWORD,
            MODIFY_VALUE,
            MODIFY_BYTE,
            MODIFY_WORD,
            MODIFY_DWORD,
            MODIFY_QWORD,
            READ_VALUE,
            READ_BYTE,
            READ_WORD,
            READ_DWORD,
            READ_QWORD,
            GET_TYPE,
            RENAME,
            CONTAINS,
            LIST,
            READ_ELEMENT,
            DELETE_ELEMENT,
            UPDATE,
            UNUPDATE,
            JSON_TREE,
            TREE,
            GET_KEYNAME_PATH,
            UPDATE_TRIGGER,
            SAVE,
            CREATE_MULTIPLE_KEY,
            CREATE_MULTIPLE_VALUE,
            CREATE_MULTIPLE_BYTE,
            CREATE_MULTIPLE_WORD,
            CREATE_MULTIPLE_DWORD,
            CREATE_MULTIPLE_QWORD,
            MODIFY_MULTIPLE_VALUE,
            MODIFY_MULTIPLE_BYTE,
            MODIFY_MULTIPLE_WORD,
            MODIFY_MULTIPLE_DWORD,
            MODIFY_MULTIPLE_QWORD,
            READ_MULTIPLE_VALUE,
            READ_MULTIPLE_BYTE,
            READ_MULTIPLE_WORD,
            READ_MULTIPLE_DWORD,
            READ_MULTIPLE_QWORD,
            GET_MULTIPLE_TYPE,
            RENAME_MULTIPLE,
            DELETE_MULTIPLE_ELEMENT,
            READ_MULTIPLE_ELEMENT,
            PROPERTIES,
            INITIALIZATION
        };

        struct WSAInit {
            WSAInit() {
    #ifdef _WIN32
                WSADATA wsa;
                if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) throw platform_core::ecc{0, 0};
    #endif
            }
            ~WSAInit() {
    #ifdef _WIN32
                WSACleanup();
    #endif
            }
        };

        inline WSAInit g_wsa_init;

        inline void registry_check_name_view(std::string_view name) {
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

        inline void registry_check_name(const std::string& name) {
            registry_check_name_view(std::string_view(name.data(), name.size()));
        }

        inline void registry_check_path(const std::string& path) {
            const QWORD n = static_cast<QWORD>(path.size());
            if (n == 0)
                throw platform_core::ecc{ 2, 7 };

            static constexpr char root_name[] = "*__root__*";
            static constexpr QWORD root_len = sizeof(root_name) - 1;

            if (n < root_len)
                throw platform_core::ecc{ 2, 8 };

            for (QWORD i = 0; i < root_len; ++i) {
                if (path[static_cast<size_t>(i)] != root_name[static_cast<size_t>(i)])
                    throw platform_core::ecc{ 2, 8 };
            }

            if (n == root_len)
                return;

            if (path[static_cast<size_t>(root_len)] != '>')
                throw platform_core::ecc{ 2, 8 };

            QWORD start = root_len + 1;
            while (true) {
                QWORD i = start;
                while (i < n && path[static_cast<size_t>(i)] != '>')
                    ++i;

                registry_check_name_view(std::string_view(path.data() + static_cast<size_t>(start), static_cast<size_t>(i - start)));

                if (i == n)
                    break;

                start = i + 1;
            }
        }


        inline SOCKET connect_host() {
    #ifdef _WIN32
            constexpr int max_attempts = 1;
    #else
            constexpr int max_attempts = 100;
    #endif
            for (int attempt = 0; attempt < max_attempts; ++attempt) {
                SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
                if (s == INVALID_SOCKET) throw platform_core::ecc{ 2, 23 };

                int tcp_no_delay = 1;
                setsockopt(s, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&tcp_no_delay), sizeof(tcp_no_delay));

                sockaddr_in addr;
                addr.sin_family = AF_INET;
                inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
                addr.sin_port = htons(41420);
                if (connect(s, (sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
                    return s;
                }

                ::registry_editor::CLOSE_SOCKET(s);
    #ifndef _WIN32
                if (attempt + 1 < max_attempts) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
    #endif
            }
            throw platform_core::ecc{ 2, 23 };
        }

        inline bool net_read(SOCKET s, char* buf, QWORD len) {
            QWORD total = 0;
            while (total < len) {
                int n = recv(s, buf + total, static_cast<int>(len - total), 0);
                if (n <= 0) return false;
                total += static_cast<QWORD>(n);
            }
            return true;
        }

        inline bool net_write(SOCKET s, const char* buf, QWORD len) {
            QWORD total = 0;
            while (total < len) {
                int n = send(s, buf + total, static_cast<int>(len - total), 0);
                if (n <= 0) return false;
                total += static_cast<QWORD>(n);
            }
            return true;
        }

        template<typename T>
        inline T read_raw_pod(SOCKET s) {
            T val;
            if (!net_read(s, reinterpret_cast<char*>(&val), sizeof(T))) throw platform_core::ecc{ 2, 23 };
            return val;
        }

        template<typename T>
        inline void write_raw_pod(SOCKET s, const T& val) {
            if (!net_write(s, reinterpret_cast<const char*>(&val), sizeof(T))) throw platform_core::ecc{ 2, 23 };
        }

        inline platform_core::VALUE_TYPE read_VALUE_TYPE(SOCKET s) {
            return static_cast<platform_core::VALUE_TYPE>(read_raw_pod<BYTE>(s));
        }

        inline void write_VALUE_TYPE(SOCKET s, platform_core::VALUE_TYPE t) {
            write_raw_pod<BYTE>(s, static_cast<BYTE>(t));
        }

        inline std::string read_string(SOCKET s) {
            platform_core::VALUE_TYPE t = read_VALUE_TYPE(s);
            if (t == platform_core::VALUE_TYPE::ECC) {
                QWORD err1 = read_raw_pod<QWORD>(s);
                throw platform_core::ecc{ err1, read_raw_pod<QWORD>(s) };
            }
            if (t != platform_core::VALUE_TYPE::DATA) throw platform_core::ecc{ 2, 23 };

            DWORD len = read_raw_pod<DWORD>(s);
            if (len == 0) return "";
            std::string str(len, '\0');
            if (!net_read(s, str.data(), len)) throw platform_core::ecc{ 2, 23 };
            return str;
        }

        inline void write_string(SOCKET s, const std::string& str) {
            write_VALUE_TYPE(s, platform_core::VALUE_TYPE::DATA);
            DWORD len = static_cast<DWORD>(str.size());
            write_raw_pod<DWORD>(s, len);
            if (len > 0 && !net_write(s, str.data(), len)) throw platform_core::ecc{ 2, 23 };
        }

        template<typename T>
        inline T read_pod(SOCKET s) {
            platform_core::VALUE_TYPE t = read_VALUE_TYPE(s);
            if (t == platform_core::VALUE_TYPE::ECC) {
                QWORD err1 = read_raw_pod<QWORD>(s);
                throw platform_core::ecc{ err1, read_raw_pod<QWORD>(s) };
            }

            if constexpr (std::is_enum_v<T>) {
                if (t != platform_core::VALUE_TYPE::BYTE) throw platform_core::ecc{ 2, 23 };
                return static_cast<T>(read_raw_pod<BYTE>(s));
            }
            else if constexpr (std::is_same_v<T, BYTE>) {
                if (t != platform_core::VALUE_TYPE::BYTE) throw platform_core::ecc{ 2, 23 };
                return read_raw_pod<BYTE>(s);
            }
            else if constexpr (std::is_same_v<T, WORD>) {
                if (t != platform_core::VALUE_TYPE::WORD) throw platform_core::ecc{ 2, 23 };
                return read_raw_pod<WORD>(s);
            }
            else if constexpr (std::is_same_v<T, DWORD>) {
                if (t != platform_core::VALUE_TYPE::DWORD) throw platform_core::ecc{ 2, 23 };
                return read_raw_pod<DWORD>(s);
            }
            else if constexpr (std::is_same_v<T, QWORD>) {
                if (t != platform_core::VALUE_TYPE::QWORD) throw platform_core::ecc{ 2, 23 };
                return read_raw_pod<QWORD>(s);
            }
        }

        template<typename T>
        inline void write_pod(SOCKET s, const T& val) {
            if constexpr (std::is_enum_v<T>) {
                write_VALUE_TYPE(s, platform_core::VALUE_TYPE::BYTE);
                write_raw_pod<BYTE>(s, static_cast<BYTE>(val));
            }
            else if constexpr (std::is_same_v<T, BYTE>) {
                write_VALUE_TYPE(s, platform_core::VALUE_TYPE::BYTE);
                write_raw_pod<BYTE>(s, val);
            }
            else if constexpr (std::is_same_v<T, WORD>) {
                write_VALUE_TYPE(s, platform_core::VALUE_TYPE::WORD);
                write_raw_pod<WORD>(s, val);
            }
            else if constexpr (std::is_same_v<T, DWORD>) {
                write_VALUE_TYPE(s, platform_core::VALUE_TYPE::DWORD);
                write_raw_pod<DWORD>(s, val);
            }
            else if constexpr (std::is_same_v<T, QWORD>) {
                write_VALUE_TYPE(s, platform_core::VALUE_TYPE::QWORD);
                write_raw_pod<QWORD>(s, val);
            }
        }

        inline void write_ecc(SOCKET s, QWORD err1, QWORD err2) {
            write_VALUE_TYPE(s, platform_core::VALUE_TYPE::ECC);
            write_raw_pod<QWORD>(s, err1);
            write_raw_pod<QWORD>(s, err2);
        }

        inline void check_ecc(SOCKET s) {
            platform_core::VALUE_TYPE t = read_VALUE_TYPE(s);

            if (t == platform_core::VALUE_TYPE::BYTE) {
                BYTE status = read_raw_pod<BYTE>(s);
                if (status == 0) return;
                if (status == 1) {
                    QWORD err1 = read_pod<QWORD>(s);
                    throw platform_core::ecc{ err1, read_pod<QWORD>(s) };
                }
                throw platform_core::ecc{ 2, 23 };
            }

            if (t == platform_core::VALUE_TYPE::ECC) {
                QWORD err1 = read_raw_pod<QWORD>(s);
                throw platform_core::ecc{ err1, read_raw_pod<QWORD>(s) };
            }

            throw platform_core::ecc{ 2, 23 };
        }
    }

    class registry_editor {
    public:
        QWORD m_ptr;
        SOCKET m_sock;
        std::mutex m_mtx;
        std::unordered_map<QWORD, SOCKET> m_update_socks;
        std::mutex m_update_mtx;

        registry_editor(QWORD ptr, SOCKET sock) : m_ptr(ptr), m_sock(sock) {}

        ~registry_editor() {
            {
                std::lock_guard<std::mutex> lock(m_update_mtx);
                for (auto& pair : m_update_socks) {
                    ::registry_editor::CLOSE_SOCKET(pair.second);
                }
                m_update_socks.clear();
            }
            if (m_sock != INVALID_SOCKET) {
                ::registry_editor::CLOSE_SOCKET(m_sock);
            }
        }

        void create_key(const std::string& key_name, const std::string& keyname_path) {
            registry_check_name(key_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_KEY);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, key_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void create_value(const std::string& value_name, const std::string& vd, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_VALUE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, vd);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void create_byte(const std::string& value_name, BYTE vd, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_BYTE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_pod(m_sock, vd);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void create_word(const std::string& value_name, WORD vd, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_WORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_pod(m_sock, vd);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void create_dword(const std::string& value_name, DWORD vd, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_DWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_pod(m_sock, vd);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void create_qword(const std::string& value_name, QWORD vd, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_QWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_pod(m_sock, vd);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        void modify_value(const std::string& value_name, const std::string& keyname_path, const std::string& vd) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_VALUE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            write_string(m_sock, vd);
            check_ecc(m_sock);
        }

        void modify_byte(const std::string& value_name, const std::string& keyname_path, BYTE vd) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_BYTE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            write_pod(m_sock, vd);
            check_ecc(m_sock);
        }

        void modify_word(const std::string& value_name, const std::string& keyname_path, WORD vd) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_WORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            write_pod(m_sock, vd);
            check_ecc(m_sock);
        }

        void modify_dword(const std::string& value_name, const std::string& keyname_path, DWORD vd) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_DWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            write_pod(m_sock, vd);
            check_ecc(m_sock);
        }

        void modify_qword(const std::string& value_name, const std::string& keyname_path, QWORD vd) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_QWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            write_pod(m_sock, vd);
            check_ecc(m_sock);
        }

        std::string read_value(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_VALUE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_string(m_sock);
        }

        BYTE read_byte(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_BYTE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_pod<BYTE>(m_sock);
        }

        WORD read_word(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_WORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_pod<WORD>(m_sock);
        }

        DWORD read_dword(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_DWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_pod<DWORD>(m_sock);
        }

        QWORD read_qword(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_QWORD);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_pod<QWORD>(m_sock);
        }

        ::registry_editor::type get_type(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::GET_TYPE);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return static_cast<::registry_editor::type>(read_pod<BYTE>(m_sock));
        }

        void rename(const std::string& keyname_path, const std::string& old_name, const std::string& new_name) {
            registry_check_path(keyname_path);
            registry_check_name(old_name);
            registry_check_name(new_name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::RENAME);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, keyname_path);
            write_string(m_sock, old_name);
            write_string(m_sock, new_name);
            check_ecc(m_sock);
        }

        bool contains(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CONTAINS);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            return read_pod<BYTE>(m_sock) != 0;
        }

        std::vector<std::string> list(const std::string& keyname_path) {
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::LIST);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            QWORD count = read_pod<QWORD>(m_sock);
            std::vector<std::string> res;
            res.reserve(static_cast<size_t>(count));
            for (QWORD i = 0; i < count; ++i) {
                res.push_back(read_string(m_sock));
            }
            return res;
        }

        std::variant<std::string, BYTE, WORD, DWORD, QWORD> read_element(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::READ_ELEMENT);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            switch (static_cast<::registry_editor::type>(read_pod<BYTE>(m_sock))) {
            case ::registry_editor::type::string: return read_string(m_sock);
            case ::registry_editor::type::byte:   return read_pod<BYTE>(m_sock);
            case ::registry_editor::type::word:   return read_pod<WORD>(m_sock);
            case ::registry_editor::type::dword:  return read_pod<DWORD>(m_sock);
            case ::registry_editor::type::qword:  return read_pod<QWORD>(m_sock);
            default: throw platform_core::ecc{ 2, 23 };
            }
        }

        void delete_element(const std::string& value_name, const std::string& keyname_path) {
            registry_check_name(value_name);
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::DELETE_ELEMENT);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, value_name);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }

        QWORD update(std::function<void()> fn) {
            SOCKET ls = connect_host();
            write_pod(ls, Op::UPDATE);
            write_pod(ls, m_ptr);
            QWORD dummy_cb = 0;
            write_pod(ls, dummy_cb);
            check_ecc(ls);
            QWORD reg_id = read_pod<QWORD>(ls);

            {
                std::lock_guard<std::mutex> lk(m_update_mtx);
                m_update_socks[reg_id] = ls;
            }

            std::thread([ls, fn]() {
                while (true) {
                    try {
                        Op op = static_cast<Op>(read_pod<BYTE>(ls));
                        if (op == Op::UPDATE_TRIGGER) {
                            read_pod<QWORD>(ls);
                            fn();
                        }
                    }
                    catch (...) {
                        break;
                    }
                }
                }).detach();

            return reg_id;
        }

        void unupdate(QWORD id) {
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::UNUPDATE);
            write_pod(m_sock, m_ptr);
            write_pod(m_sock, id);
            check_ecc(m_sock);

            std::lock_guard<std::mutex> ulk(m_update_mtx);
            auto it = m_update_socks.find(id);
            if (it != m_update_socks.end()) {
                ::registry_editor::CLOSE_SOCKET(it->second);
                m_update_socks.erase(it);
            }
        }

        std::string json_tree() {
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::JSON_TREE);
            write_pod(m_sock, m_ptr);
            check_ecc(m_sock);
            return read_string(m_sock);
        }

        std::string tree() {
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::TREE);
            write_pod(m_sock, m_ptr);
            check_ecc(m_sock);
            return read_string(m_sock);
        }

        std::string get_keyname_path() {
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::GET_KEYNAME_PATH);
            write_pod(m_sock, m_ptr);
            check_ecc(m_sock);
            return read_string(m_sock);
        }
        void save() {
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::SAVE);
            write_pod(m_sock, m_ptr);
            check_ecc(m_sock);
        }
        inline void create_multiple_key(const std::vector<std::string> list_of_key_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_key_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_KEY);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_key_name.size());
            for (const auto& s : list_of_key_name) write_string(m_sock, s);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void create_multiple_value(const std::vector<std::pair<std::string, std::string>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_VALUE);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, value] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_string(m_sock, value);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void create_multiple_byte(const std::vector<std::pair<std::string, BYTE>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_BYTE);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, byte] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<BYTE>(m_sock, byte);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void create_multiple_word(const std::vector<std::pair<std::string, WORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_WORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, word] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<WORD>(m_sock, word);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void create_multiple_dword(const std::vector<std::pair<std::string, DWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_DWORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, dword] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<DWORD>(m_sock, dword);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void create_multiple_qword(const std::vector<std::pair<std::string, QWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::CREATE_MULTIPLE_QWORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, qword] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<QWORD>(m_sock, qword);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        inline void modify_multiple_value(const std::vector<std::pair<std::string, std::string>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_MULTIPLE_VALUE);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, value] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_string(m_sock, value);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline void modify_multiple_byte(const std::vector<std::pair<std::string, BYTE>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_MULTIPLE_BYTE);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, byte] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<BYTE>(m_sock, byte);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline void modify_multiple_word(const std::vector<std::pair<std::string, WORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_MULTIPLE_WORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, word] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<WORD>(m_sock, word);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline void modify_multiple_dword(const std::vector<std::pair<std::string, DWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_MULTIPLE_DWORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, dword] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<DWORD>(m_sock, dword);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline void modify_multiple_qword(const std::vector<std::pair<std::string, QWORD>>& list_of_value_name_and_data, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& [name, value] : list_of_value_name_and_data) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::MODIFY_MULTIPLE_QWORD);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& [value_name, qword] : list_of_value_name_and_data) {
                write_string(m_sock, value_name);
                write_pod<QWORD>(m_sock, qword);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline ankerl::unordered_dense::map<std::string, std::string> read_multiple_value(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, std::string> returned;

            write_pod(m_sock, Op::READ_MULTIPLE_VALUE);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_string(m_sock));
            }
            return returned;
        };
        inline ankerl::unordered_dense::map<std::string, BYTE> read_multiple_byte(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, BYTE> returned;

            write_pod(m_sock, Op::READ_MULTIPLE_BYTE);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<BYTE>(m_sock));
            }
            return returned;
        };
        inline ankerl::unordered_dense::map<std::string, WORD> read_multiple_word(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, WORD> returned;

            write_pod(m_sock, Op::READ_MULTIPLE_WORD);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<WORD>(m_sock));
            }
            return returned;
        };
        inline ankerl::unordered_dense::map<std::string, DWORD> read_multiple_dword(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, DWORD> returned;

            write_pod(m_sock, Op::READ_MULTIPLE_DWORD);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<DWORD>(m_sock));
            }
            return returned;
        };
        inline ankerl::unordered_dense::map<std::string, QWORD> read_multiple_qword(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, QWORD> returned;

            write_pod(m_sock, Op::READ_MULTIPLE_QWORD);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<QWORD>(m_sock));
            }
            return returned;
        };
        inline ankerl::unordered_dense::map<std::string, ::registry_editor::type> get_multiple_type(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, ::registry_editor::type> returned;

            write_pod(m_sock, Op::GET_MULTIPLE_TYPE);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                returned.emplace(list_of_value_name[static_cast<size_t>(i)], static_cast<::registry_editor::type>(read_pod<BYTE>(m_sock)));
            }
            return returned;
        };
        inline void rename_multiple(const std::string& keyname_path, const std::vector<std::pair<std::string, std::string>>& list_of_name) {
            registry_check_path(keyname_path);
            for (const auto& [old_name, new_name] : list_of_name) { registry_check_name(old_name); registry_check_name(new_name); }
            std::lock_guard<std::mutex> lock(m_mtx);

            write_pod(m_sock, Op::RENAME_MULTIPLE);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_name.size());
            for (const auto& [old_name, new_name] : list_of_name) {
                write_string(m_sock, old_name);
                write_string(m_sock, new_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        };
        inline ankerl::unordered_dense::map<std::string, std::variant<std::string, BYTE, WORD, DWORD, QWORD>> read_multiple_element(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            ankerl::unordered_dense::map<std::string, std::variant<std::string, BYTE, WORD, DWORD, QWORD>> returned;
            write_pod(m_sock, Op::READ_MULTIPLE_ELEMENT);
            write_pod(m_sock, m_ptr);
            QWORD repeat = list_of_value_name.size();
            write_pod<QWORD>(m_sock, repeat);
            for (const auto& value_name : list_of_value_name) {
                write_string(m_sock, value_name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            for (QWORD i = 0; i < repeat; i++) {
                switch (static_cast<::registry_editor::type>(read_pod<BYTE>(m_sock))) {
                case ::registry_editor::type::string: {
                    returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_string(m_sock));
                    break;
                }
                case ::registry_editor::type::byte: {
                    returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<BYTE>(m_sock));
                    break;
                }
                case ::registry_editor::type::word: {
                    returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<WORD>(m_sock));
                    break;
                }
                case ::registry_editor::type::dword: {
                    returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<DWORD>(m_sock));
                    break;
                }
                case ::registry_editor::type::qword: {
                    returned.emplace(list_of_value_name[static_cast<size_t>(i)], read_pod<QWORD>(m_sock));
                    break;
                }
                default: throw platform_core::ecc{ 2, 23 };
                }
            }
            return returned;
        }
        void delete_multiple_element(const std::vector<std::string>& list_of_value_name, const std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& name : list_of_value_name) registry_check_name(name);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::DELETE_MULTIPLE_ELEMENT);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name.size());
            for (const auto& name : list_of_value_name) {
                write_string(m_sock, name);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
        ankerl::unordered_dense::map<std::string, ::registry_editor::type> properties(const std::string& keyname_path) {
            registry_check_path(keyname_path);
            std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::PROPERTIES);
            write_pod(m_sock, m_ptr);
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
            QWORD count = read_pod<QWORD>(m_sock);
            ankerl::unordered_dense::map<std::string, ::registry_editor::type> res;
            res.reserve(static_cast<size_t>(count));
            for (QWORD i = 0; i < count; ++i) {
                std::string name = read_string(m_sock);
                res.emplace(name, static_cast<::registry_editor::type>(read_pod<BYTE>(m_sock)));
            }
            return res;
        }
        void initialization(const std::vector<std::variant<::registry_editor::INIT_KEY, ::registry_editor::INIT_VALUE, ::registry_editor::INIT_BYTE, ::registry_editor::INIT_WORD, ::registry_editor::INIT_DWORD, ::registry_editor::INIT_QWORD>>& list_of_value_name_and_data, const ::std::string& keyname_path) {
            registry_check_path(keyname_path);
            for (const auto& item : list_of_value_name_and_data) {
                ::std::visit([](const auto& x) { registry_check_name(x.id); }, item);
            }
            ::std::lock_guard<std::mutex> lock(m_mtx);
            write_pod(m_sock, Op::INITIALIZATION);
            write_pod(m_sock, m_ptr);
            write_pod<QWORD>(m_sock, list_of_value_name_and_data.size());
            for (const auto& item : list_of_value_name_and_data) {
                ::std::visit([this](const auto& x) {
                    if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ::registry_editor::INIT_KEY>) {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::key));
                        write_string(m_sock, x.id);
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ::registry_editor::INIT_VALUE>) {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::string));
                        write_string(m_sock, x.id);
                        write_string(m_sock, x.value);
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ::registry_editor::INIT_BYTE>) {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::byte));
                        write_string(m_sock, x.id);
                        write_pod<BYTE>(m_sock, x.value);
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ::registry_editor::INIT_WORD>) {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::word));
                        write_string(m_sock, x.id);
                        write_pod<WORD>(m_sock, x.value);
                    }
                    else if constexpr (std::is_same_v<std::decay_t<decltype(x)>, ::registry_editor::INIT_DWORD>) {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::dword));
                        write_string(m_sock, x.id);
                        write_pod<DWORD>(m_sock, x.value);
                    }
                    else {
                        write_pod<BYTE>(m_sock, static_cast<BYTE>(::registry_editor::type::qword));
                        write_string(m_sock, x.id);
                        write_pod<QWORD>(m_sock, x.value);
                    }
                }, item);
            }
            write_string(m_sock, keyname_path);
            check_ecc(m_sock);
        }
    };

    inline void create_registry_file(const std::string& path_to_registry_file) {
        ::SOCKET s = connect_host();
        write_pod(s, ::registry_editor::Op::CREATE_REGISTRY_FILE);
        write_string(s, path_to_registry_file);
        check_ecc(s);
        ::registry_editor::CLOSE_SOCKET(s);
    }

    inline void delete_registry_file(const std::string& path_to_registry_file) {
        ::SOCKET s = connect_host();
        write_pod(s, Op::DELETE_REGISTRY_FILE);
        write_string(s, path_to_registry_file);
        check_ecc(s);
        ::registry_editor::CLOSE_SOCKET(s);
    }

    inline ::registry_editor::registry_editor* registry_begin(const ::std::string& path_to_registry_file)
    {
        SOCKET s = connect_host();
        write_pod(s, Op::REGISTRY_BEGIN);
        write_string(s, utilityX::normalize_path(path_to_registry_file));
        check_ecc(s);
        QWORD ptr = read_pod<QWORD>(s);
        return new ::registry_editor::registry_editor(ptr, s);
    }
    inline void registry_end(::registry_editor::registry_editor* ed) {
        if (!ed) return;
        try {
            std::lock_guard<std::mutex> lock(ed->m_mtx);
            if (ed->m_sock != INVALID_SOCKET) {
                write_pod(ed->m_sock, ::registry_editor::Op::REGISTRY_END);
                write_pod(ed->m_sock, ed->m_ptr);
                check_ecc(ed->m_sock);
            }
        }
        catch (...) {}
        delete ed;
    }
}
