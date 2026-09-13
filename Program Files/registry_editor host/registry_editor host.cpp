/*
    *	Registry Editor Library Host Services
    *
    *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - Registry Editor Library Host Services
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

#pragma message(" *	Registry Editor Library Host Services")
#pragma message(" *")
#pragma message(" *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - Registry Editor Library Host Services")
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

#include "../../Program Libraries/system/system.h"
// #include "../../Program Libraries/system/recorder-service-local.h"
#include "../../Program Libraries/unordered_dense/include/ankerl/unordered_dense.h"
#include "../../Program Libraries/utilityX/utilityX.h"

#include <string>
#include <filesystem>
#include <vector>
#include <variant>
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <system_error>
#include <algorithm>
#include <string_view>
#include <limits>
#include <shared_mutex>
#include <fstream>

#define NO_CHECK_VALID

#include "../../Program Libraries/registry_editor/registry_editor-service-local.h"

#undef NO_CHECK_VALID
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

// platform_core::recorder::record* global_log;
// bool logcall = true;

static inline void CLOSE_SOCKET(SOCKET s) noexcept
{
    if (s == INVALID_SOCKET)
        return;

#ifdef _WIN32
    closesocket(s);
#else
    close(s);
#endif
}

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

std::shared_mutex editor_mutex;

inline bool net_read(SOCKET s, void* buf, QWORD len)
{
    char* p = static_cast<char*>(buf);

    while (len)
    {
        int n = recv(s, p, static_cast<int>(len), 0);

        if (n == 0)
            throw platform_core::ecc{ 2, 31 };

        if (n < 0)
            throw platform_core::ecc{ 2, 23 };

        p += n;
        len -= static_cast<QWORD>(n);
    }

    return true;
}

inline bool net_write(SOCKET s, const void* buf, QWORD len)
{
    const char* p = static_cast<const char*>(buf);

    while (len)
    {
        int n = send(s, p, static_cast<int>(len), 0);

        if (n <= 0)
            throw platform_core::ecc{ 2, 24 };

        p += n;
        len -= static_cast<QWORD>(n);
    }

    return true;
}

template<class T>
inline T read_raw_pod(SOCKET s)
{
    static_assert(std::is_trivially_copyable_v<T>);

    T value{};
    net_read(s, &value, sizeof(T));

    return value;
}

template<class T>
inline void write_raw_pod(SOCKET s, const T& value)
{
    static_assert(std::is_trivially_copyable_v<T>);

    net_write(s, &value, sizeof(T));
}
inline platform_core::VALUE_TYPE read_value_type(SOCKET s)
{
    BYTE b = read_raw_pod<BYTE>(s);

    if (b > static_cast<BYTE>(platform_core::VALUE_TYPE::ECC))
        throw platform_core::ecc{ 2,33 };

    return static_cast<platform_core::VALUE_TYPE>(b);
}

inline void write_value_type(SOCKET s, platform_core::VALUE_TYPE t)
{
    write_raw_pod(s, static_cast<BYTE>(t));
}

inline std::string read_string(SOCKET s)
{
    auto type = read_value_type(s);

    if (type == platform_core::VALUE_TYPE::ECC)
    {
        auto cls = read_raw_pod<QWORD>(s);
        auto err = read_raw_pod<QWORD>(s);

        throw platform_core::ecc{ cls,err };
    }

    if (type != platform_core::VALUE_TYPE::DATA)
        throw platform_core::ecc{ 2,30 };

    DWORD len = read_raw_pod<DWORD>(s);

    constexpr DWORD MAX_STRING = 64 * 1024 * 1024;

    if (len > MAX_STRING)
        throw platform_core::ecc{ 2,34 };

    std::string str(len, '\0');

    if (len)
        net_read(s, str.data(), len);

    return str;
}

inline void write_string(SOCKET s, const std::string& str)
{
    write_value_type(s, platform_core::VALUE_TYPE::DATA);

    DWORD len = static_cast<DWORD>(str.size());

    write_raw_pod(s, len);

    if (len)
        net_write(s, str.data(), len);
}

template<class T>
inline T read_pod(SOCKET s)
{
    auto type = read_value_type(s);

    if (type == platform_core::VALUE_TYPE::ECC)
    {
        auto cls = read_raw_pod<QWORD>(s);
        auto err = read_raw_pod<QWORD>(s);

        throw platform_core::ecc{ cls,err };
    }

    if constexpr (std::is_enum_v<T>)
    {
        if (type != platform_core::VALUE_TYPE::BYTE)
            throw platform_core::ecc{ 2,26 };

        return static_cast<T>(read_raw_pod<BYTE>(s));
    }

    else if constexpr (std::is_same_v<T, BYTE>)
    {
        if (type != platform_core::VALUE_TYPE::BYTE)
            throw platform_core::ecc{ 2,26 };

        return read_raw_pod<BYTE>(s);
    }

    else if constexpr (std::is_same_v<T, WORD>)
    {
        if (type != platform_core::VALUE_TYPE::WORD)
            throw platform_core::ecc{ 2,27 };

        return read_raw_pod<WORD>(s);
    }

    else if constexpr (std::is_same_v<T, DWORD>)
    {
        if (type != platform_core::VALUE_TYPE::DWORD)
            throw platform_core::ecc{ 2,28 };

        return read_raw_pod<DWORD>(s);
    }

    else if constexpr (std::is_same_v<T, QWORD>)
    {
        if (type != platform_core::VALUE_TYPE::QWORD)
            throw platform_core::ecc{ 2,29 };

        return read_raw_pod<QWORD>(s);
    }

    else
    {
        static_assert(sizeof(T) == 0, "Unsupported POD type");
    }
}

template<typename T>
inline void write_pod(SOCKET s, const T& val) {
    if constexpr (std::is_enum_v<T>) {
        write_value_type(s, platform_core::VALUE_TYPE::BYTE);
        write_raw_pod<BYTE>(s, static_cast<BYTE>(val));
    }
    else if constexpr (std::is_same_v<T, BYTE>) {
        write_value_type(s, platform_core::VALUE_TYPE::BYTE);
        write_raw_pod<BYTE>(s, val);
    }
    else if constexpr (std::is_same_v<T, WORD>) {
        write_value_type(s, platform_core::VALUE_TYPE::WORD);
        write_raw_pod<WORD>(s, val);
    }
    else if constexpr (std::is_same_v<T, DWORD>) {
        write_value_type(s, platform_core::VALUE_TYPE::DWORD);
        write_raw_pod<DWORD>(s, val);
    }
    else if constexpr (std::is_same_v<T, QWORD>) {
        write_value_type(s, platform_core::VALUE_TYPE::QWORD);
        write_raw_pod<QWORD>(s, val);
    }
}

inline void write_ecc(SOCKET s, QWORD err1, QWORD err2) {
    write_value_type(s, platform_core::VALUE_TYPE::ECC);
    write_raw_pod<QWORD>(s, err1);
    write_raw_pod<QWORD>(s, err2);
}

inline void check_ecc(SOCKET s) {
    platform_core::VALUE_TYPE t = read_value_type(s);

    if (t == platform_core::VALUE_TYPE::BYTE) {
        BYTE status = read_raw_pod<BYTE>(s);
        if (status == 0) return;
        if (status == 1) {
            QWORD err1 = read_pod<QWORD>(s);
            throw platform_core::ecc{ err1, read_pod<QWORD>(s) };
        }
        throw platform_core::ecc{2, 23};
    }

    if (t == platform_core::VALUE_TYPE::ECC) {
        QWORD err1 = read_raw_pod<QWORD>(s);
        throw platform_core::ecc{ err1, read_raw_pod<QWORD>(s) };
    }

    throw platform_core::ecc{2, 23};
}


inline void send_response_status(SOCKET s, QWORD err1, QWORD err2) {
    if (err1 == 0) write_pod<BYTE>(s, 0);
    else write_ecc(s, err1, err2);
}

static void handle_client(SOCKET client_socket) {
    while (true) {
        Op op;
        try {
            op = static_cast<Op>(read_pod<BYTE>(client_socket));
        }
        catch (...) {
            break;
        }
        try {
            switch (op) {
            case Op::CREATE_REGISTRY_FILE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_REGISTRY_FILE\"" });
                registry_editor_service_local::create_registry_file((read_string(client_socket)));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::DELETE_REGISTRY_FILE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"DELETE_REGISTRY_FILE\"" });
                registry_editor_service_local::delete_registry_file((read_string(client_socket)));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::REGISTRY_BEGIN: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"REGISTRY_BEGIN\"" });
                registry_editor_service_local::registry_editor_service_local* ed = registry_editor_service_local::registry_begin((read_string(client_socket)));
                send_response_status(client_socket, 0, { 0 });
                write_pod<QWORD>(client_socket, reinterpret_cast<QWORD>(ed));
                break;
            }
            case Op::REGISTRY_END: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"REGISTRY_END\"" });
                registry_editor_service_local::registry_end(reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket)));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_KEY: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_KEY\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string kn = read_string(client_socket);
                ed->create_key(kn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string vd = read_string(client_socket);
                ed->create_value(vn, vd, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                BYTE vd = read_pod<BYTE>(client_socket);
                ed->create_byte(vn, vd, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                WORD vd = read_pod<WORD>(client_socket);
                ed->create_word(vn, vd, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                DWORD vd = read_pod<DWORD>(client_socket);
                ed->create_dword(vn, vd, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                QWORD vd = read_pod<QWORD>(client_socket);
                ed->create_qword(vn, vd, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string kp = read_string(client_socket);
                ed->modify_value(vn, kp, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string kp = read_string(client_socket);
                ed->modify_byte(vn, kp, read_pod<BYTE>(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string kp = read_string(client_socket);
                ed->modify_word(vn, kp, read_pod<WORD>(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string kp = read_string(client_socket);
                ed->modify_dword(vn, kp, read_pod<DWORD>(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                std::string kp = read_string(client_socket);
                ed->modify_qword(vn, kp, read_pod<QWORD>(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::READ_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto s = ed->read_value(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_string(client_socket, s);;
                break;
            }
            case Op::READ_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto b = ed->read_byte(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<BYTE>(client_socket, b);
                break;
            }
            case Op::READ_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto w = ed->read_word(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<WORD>(client_socket, w);
                break;
            }
            case Op::READ_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto dw = ed->read_dword(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<DWORD>(client_socket, dw);
                break;
            }
            case Op::READ_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto qw = ed->read_qword(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<QWORD>(client_socket, qw);
                break;
            }
            case Op::GET_TYPE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"GET_TYPE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto t = ed->get_type(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<BYTE>(client_socket, static_cast<BYTE>(t));
                break;
            }
            case Op::RENAME: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"RENAME\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string kp = read_string(client_socket);
                std::string on = read_string(client_socket);
                ed->rename(kp, on, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CONTAINS: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CONTAINS\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto ct = ed->contains(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<BYTE>(client_socket, ct ? 1 : 0);
                break;
            }
            case Op::LIST: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"LIST\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> res = ed->list(read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<QWORD>(client_socket, res.size());
                for (const auto& s : res) write_string(client_socket, s);
                break;
            }
            case Op::READ_ELEMENT: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_ELEMENT\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                auto res = ed->read_element(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<BYTE>(client_socket, static_cast<BYTE>(res.index()));
                std::visit([client_socket](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, std::string>) {
                        write_string(client_socket, arg);
                    }
                    else {
                        write_pod<T>(client_socket, arg);
                    }
                    }, res);
                break;
            }
            case Op::DELETE_ELEMENT: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"DELETE_ELEMENT\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::string vn = read_string(client_socket);
                ed->delete_element(vn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::UPDATE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"UPDATE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                QWORD client_cb_id = read_pod<QWORD>(client_socket);
                auto update_mtx = std::make_shared<std::mutex>();
                QWORD reg_id;

                {
                    std::lock_guard<std::mutex> init_lock(*update_mtx);

                    reg_id = ed->update([client_socket, client_cb_id, update_mtx]() {
                        std::lock_guard<std::mutex> cb_lock(*update_mtx);
                        Op n_op = Op::UPDATE_TRIGGER;
                        write_pod(client_socket, n_op);
                        write_pod<QWORD>(client_socket, client_cb_id);
                        });
                    send_response_status(client_socket, 0, { 0 });
                    write_pod<QWORD>(client_socket, reg_id);
                }
                break;
            }
            case Op::UNUPDATE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"UNUPDATE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                ed->unupdate(read_pod<QWORD>(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::JSON_TREE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"JSON_TREE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                auto jst = ed->json_tree();
                send_response_status(client_socket, 0, { 0 });
                write_string(client_socket, jst);
                break;
            }
            case Op::TREE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"TREE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                auto tr = ed->tree();
                send_response_status(client_socket, 0, { 0 });
                write_string(client_socket, tr);
                break;
            }
            case Op::GET_KEYNAME_PATH: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"GET_KEYNAME_PATH\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                auto knp = ed->get_keyname_path();
                send_response_status(client_socket, 0, { 0 });
                write_string(client_socket, knp);
                break;
            }
            case Op::SAVE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"SAVE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                ed->save();
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_KEY: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_KEY\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> lokn;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    lokn.push_back(read_string(client_socket));
                }
                ed->create_multiple_key(lokn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, std::string>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, std::string>{first, read_string(client_socket)});
                }
                ed->create_multiple_value(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, BYTE>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, BYTE>{first, read_pod<BYTE>(client_socket)});
                }
                ed->create_multiple_byte(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, WORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, WORD>{first, read_pod<WORD>(client_socket)});
                }
                ed->create_multiple_word(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, DWORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, DWORD>{first, read_pod<DWORD>(client_socket)});
                }
                ed->create_multiple_dword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::CREATE_MULTIPLE_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"CREATE_MULTIPLE_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, QWORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, QWORD>{first, read_pod<QWORD>(client_socket)});
                }
                ed->create_multiple_qword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_MULTIPLE_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_MULTIPLE_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, std::string>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, std::string>{first, read_string(client_socket)});
                }
                ed->modify_multiple_value(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_MULTIPLE_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_MULTIPLE_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, BYTE>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, BYTE>{first, read_pod<BYTE>(client_socket)});
                }
                ed->modify_multiple_byte(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_MULTIPLE_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_MULTIPLE_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, WORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, WORD>{first, read_pod<WORD>(client_socket)});
                }
                ed->modify_multiple_word(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_MULTIPLE_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_MULTIPLE_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, DWORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, DWORD>{first, read_pod<DWORD>(client_socket)});
                }
                ed->modify_multiple_dword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::MODIFY_MULTIPLE_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"MODIFY_MULTIPLE_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, QWORD>> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string first = read_string(client_socket);
                    loknav.push_back(std::pair<std::string, QWORD>{first, read_pod<QWORD>(client_socket)});
                }
                ed->modify_multiple_qword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::READ_MULTIPLE_VALUE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_VALUE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_value(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_string(client_socket, vector[static_cast<size_t>(i)].second);
                }
                break;
            }
            case Op::READ_MULTIPLE_BYTE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_BYTE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_byte(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_pod<BYTE>(client_socket, vector[static_cast<size_t>(i)].second);
                }
                break;
            }
            case Op::READ_MULTIPLE_WORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_WORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_word(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_pod<WORD>(client_socket, vector[static_cast<size_t>(i)].second);
                }
                break;
            }
            case Op::READ_MULTIPLE_DWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_DWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_dword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_pod<DWORD>(client_socket, vector[static_cast<size_t>(i)].second);
                }
                break;
            }
            case Op::READ_MULTIPLE_QWORD: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_QWORD\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_qword(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_pod<QWORD>(client_socket, vector[static_cast<size_t>(i)].second);
                }
                break;
            }
            case Op::GET_MULTIPLE_TYPE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"GET_MULTIPLE_TYPE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->get_multiple_type(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (QWORD i = 0; i < repeat; i++) {
                    write_pod<BYTE>(client_socket, static_cast<BYTE>(vector[static_cast<size_t>(i)].second));
                }
                break;
            }
            case Op::RENAME_MULTIPLE: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"RENAME_MULTIPLE\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::pair<std::string, std::string>> lokn;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    std::string name = read_string(client_socket);
                    lokn.push_back(std::pair<std::string, std::string>{ name, read_string(client_socket)});
                }
                ed->rename_multiple(read_string(client_socket), lokn);
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::READ_MULTIPLE_ELEMENT: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"READ_MULTIPLE_ELEMENT\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> loknav;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    loknav.push_back(read_string(client_socket));
                }
                const auto& vector = ed->read_multiple_element(loknav, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                for (auto& [name, v] : vector) {
                    switch (v.index()) {
                    case 0: {
                        write_pod<BYTE>(client_socket, static_cast<BYTE>(registry_editor_service_local::type::string));
                        write_string(client_socket, std::get<0>(v));
                        break;
                    }

                    case 1: {
                        write_pod<BYTE>(client_socket, static_cast<BYTE>(registry_editor_service_local::type::byte));
                        write_pod<BYTE>(client_socket, std::get<1>(v));
                        break;
                    }

                    case 2: {
                        write_pod<BYTE>(client_socket, static_cast<BYTE>(registry_editor_service_local::type::word));
                        write_pod<WORD>(client_socket, std::get<2>(v));
                        break;
                    }

                    case 3: {
                        write_pod<BYTE>(client_socket, static_cast<BYTE>(registry_editor_service_local::type::dword));
                        write_pod<DWORD>(client_socket, std::get<3>(v));
                        break;
                    }

                    case 4: {
                        write_pod<BYTE>(client_socket, static_cast<BYTE>(registry_editor_service_local::type::qword));
                        write_pod<QWORD>(client_socket, std::get<4>(v));
                        break;
                    }
                    }
                }
                break;
            }
            case Op::DELETE_MULTIPLE_ELEMENT: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"DELETE_MULTIPLE_ELEMENT\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                std::vector<std::string> lokn;
                QWORD repeat = read_pod<QWORD>(client_socket);
                for (QWORD i = 0; i < repeat; i++) {
                    lokn.push_back(read_string(client_socket));
                }
                ed->delete_multiple_element(lokn, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            case Op::PROPERTIES: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"PROPERTIES\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                auto res = ed->properties(read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                write_pod<QWORD>(client_socket, res.size());
                for (const auto& [a, b] : res) {
                    write_string(client_socket, a);
                    write_pod<BYTE>(client_socket, static_cast<BYTE>(b));
                };
                break;
            }
            case Op::INITIALIZATION: {
                // if (logcall) global_log->writes({ "Info", "global.registry_editor", "", "", "", "Registry Editor received: \"INITIALIZATION\"" });
                registry_editor_service_local::registry_editor_service_local* ed = reinterpret_cast<registry_editor_service_local::registry_editor_service_local*>(read_pod<QWORD>(client_socket));
                QWORD repeat = read_pod<QWORD>(client_socket);
                std::vector<std::variant<registry_editor_service_local::INIT_KEY, registry_editor_service_local::INIT_VALUE, registry_editor_service_local::INIT_BYTE, registry_editor_service_local::INIT_WORD, registry_editor_service_local::INIT_DWORD, registry_editor_service_local::INIT_QWORD>> insert;
                for (QWORD i = 0; i < repeat; i++) {
                    switch (static_cast<registry_editor_service_local::type>(read_pod<BYTE>(client_socket))) {
                    case registry_editor_service_local::type::key: {
                        insert.push_back(registry_editor_service_local::INIT_KEY{ read_string(client_socket) });
                        break;
                    }
                    case registry_editor_service_local::type::string: {
                        std::string name = read_string(client_socket);
                        insert.push_back(registry_editor_service_local::INIT_VALUE{ name,  read_string(client_socket) });
                        break;
                    }
                    case registry_editor_service_local::type::byte: {
                        std::string name = read_string(client_socket);
                        insert.push_back(registry_editor_service_local::INIT_BYTE{ name, read_pod<BYTE>(client_socket) });
                        break;
                    }
                    case registry_editor_service_local::type::word: {
                        std::string name = read_string(client_socket);
                        insert.push_back(registry_editor_service_local::INIT_WORD{ name, read_pod<WORD>(client_socket) });
                        break;
                    }
                    case registry_editor_service_local::type::dword: {
                        std::string name = read_string(client_socket);
                        insert.push_back(registry_editor_service_local::INIT_DWORD{ name, read_pod<DWORD>(client_socket) });
                        break;
                    }
                    case registry_editor_service_local::type::qword: {
                        std::string name = read_string(client_socket);
                        insert.push_back(registry_editor_service_local::INIT_QWORD{ name, read_pod<QWORD>(client_socket) });
                        break;
                    }
                    }
                }
                ed->initialization(insert, read_string(client_socket));
                send_response_status(client_socket, 0, { 0 });
                break;
            }
            }
        }
        catch (platform_core::ecc& es) {
            // auto [e, ep] = platform_core::error_correction_handle::looked_up_error(es);
            // if (logcall) global_log->writes({ "Ecc", ep.services, [&] { std::ostringstream oss; oss << "0x" << std::hex << std::setw(16) << std::setfill('0') << static_cast<uint64_t>(e.class_id); return oss.str(); }(), [&] { std::ostringstream oss; oss << "0x" << std::hex << std::setw(16) << std::setfill('0') << static_cast<uint64_t>(e.error_code); return oss.str(); }(), ep.id, ep.msg });
            send_response_status(client_socket, es.class_id, es.error_code);
        }
    }
    CLOSE_SOCKET(client_socket);
}

template <typename T>
T to_numeric(std::string_view s)
{
    static_assert(
        std::is_same_v<T, BYTE> ||
        std::is_same_v<T, WORD> ||
        std::is_same_v<T, DWORD> ||
        std::is_same_v<T, QWORD>);
    T value = 0;
    constexpr T max_value = (std::numeric_limits<T>::max)();

    for (char c : s)
    {
        T digit = static_cast<T>(c - '0');
        value = static_cast<T>(value * 10 + digit);
    }

    return value;
}

int main() {
    {
        std::vector<std::string> files;
        {
            std::error_code ec;
            if (std::filesystem::exists("../../Program Datas/log/recovery-log/registry-editor-recovery-log", ec)) {
                for (const auto& entry : std::filesystem::directory_iterator("../../Program Datas/log/recovery-log/registry-editor-recovery-log"))
                {
                    files.push_back(entry.path().generic_string());
                }
            }
        }
        {
            for (const auto& file : files) {
                registry_editor_service_local::registry_editor_service_local* registry = nullptr;
                std::vector<std::vector<std::string>> lines;
                {
                    std::ifstream fil(file);
                    std::string line;
                    if (!std::getline(fil, line)) {
                        fil.close();
                        std::error_code remove_ec;
                        std::filesystem::remove(file, remove_ec);
                        continue;
                    }
                    if (!std::filesystem::exists(line)) {
                        fil.close();
                        std::error_code remove_ec;
                        std::filesystem::remove(file, remove_ec);
                        continue;
                    }
                    try {
                        registry = registry_editor_service_local::registry_begin(registry_editor_service_local::registry_normalize_path(line));
                    }
                    catch (...) {
                        continue;
                    }
                    while (std::getline(fil, line)) {
                        std::vector<std::string> stm = utilityX::parser(line, '|');
                        for (auto& chk : stm)
                        {
                            if (chk.size() >= 2)
                            {
                                chk = chk.substr(1, chk.size() - 2);
                            }
                        }
                        lines.push_back(stm);
                    }
                    fil.close();
                    std::error_code remove_ec;
                    std::filesystem::remove(file, remove_ec);
                }
                {
                    if (lines.size() != 0) {
                        for (const auto& argm : lines) {
                            try {
                                switch (to_numeric<BYTE>(argm[0])) {
                                case 1: {
                                    registry->create_key(argm[1], argm[2]);
                                    continue;
                                }
                                case 2: {
                                    registry->create_value(argm[1], argm[2], argm[3]);
                                    continue;
                                }
                                case 3: {
                                    registry->create_byte(argm[1], to_numeric<BYTE>(argm[2]), argm[3]);
                                    continue;
                                }
                                case 4: {
                                    registry->create_word(argm[1], to_numeric<WORD>(argm[2]), argm[3]);
                                    continue;
                                }
                                case 5: {
                                    registry->create_dword(argm[1], to_numeric<DWORD>(argm[2]), argm[3]);
                                    continue;
                                }
                                case 6: {
                                    registry->create_qword(argm[1], to_numeric<QWORD>(argm[2]), argm[3]);
                                    continue;
                                }
                                case 7: {
                                    registry->modify_value(argm[1], argm[2], argm[3]);
                                    continue;
                                }
                                case 8: {
                                    registry->modify_byte(argm[1], argm[2], to_numeric<BYTE>(argm[3]));
                                    continue;
                                }
                                case 9: {
                                    registry->modify_word(argm[1], argm[2], to_numeric<WORD>(argm[3]));
                                    continue;
                                }
                                case 10: {
                                    registry->modify_dword(argm[1], argm[2], to_numeric<DWORD>(argm[3]));
                                    continue;
                                }
                                case 11: {
                                    registry->modify_qword(argm[1], argm[2], to_numeric<QWORD>(argm[3]));
                                    continue;
                                }
                                case 12: {
                                    registry->rename(argm[1], argm[2], argm[3]);
                                    continue;
                                }
                                case 13: {
                                    registry->delete_element(argm[1], argm[2]);
                                    continue;
                                }
                                }
                            }
                            catch (...) {}
                        }
                    }
                }
                if (registry) {
                    registry_end(registry);
                }
            }
        }
    }
    //{
    //    try {
    //        registry_editor_service_local::registry_editor_service_local* registry = registry_editor_service_local::registry_begin(utilityX::registry_file_path_from_this_header());
    //        try {
    //            global_log = platform_core::recorder::recoding_log_begin(platform_core::recorder::log_format{
    //                platform_core::recorder::align::left,
    //                "System",
    //                registry->read_value("sys_log_file_path", "*__root__*>HKEY-CLASSES-ROOT>Storage>system>.cache"),
    //                "",
    //                6,
    //                2,
    //                1,
    //                {4, 24, 18, 18, 48}
    //                });
    //        }
    //        catch (...) {
    //            logcall = false;
    //        }
    //        registry_end(registry);
    //    }
    //    catch (...) {
    //        logcall = false;
    //    }
    //}
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        // if (logcall) platform_core::recorder::recording_log_end(global_log);
        return 1;
    }
#endif

    std::atomic_bool stop_requested{ false };
    std::atomic<SOCKET> server_socket{ INVALID_SOCKET };

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
#ifdef _WIN32
        WSACleanup();
#endif
        // if (logcall) platform_core::recorder::recording_log_end(global_log);
        return 1;
    }

    server_socket.store(server_fd);

    DWORD opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&opt), sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    address.sin_port = htons(41420);

    if (bind(server_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR) {
        SOCKET s = server_socket.exchange(INVALID_SOCKET);
        if (s != INVALID_SOCKET) {
            CLOSE_SOCKET(s);
        }
#ifdef _WIN32
        WSACleanup();
#endif
        // if (logcall) platform_core::recorder::recording_log_end(global_log);
        return 1;
    }

    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR) {
        SOCKET s = server_socket.exchange(INVALID_SOCKET);
        if (s != INVALID_SOCKET) {
            CLOSE_SOCKET(s);
        }
#ifdef _WIN32
        WSACleanup();
#endif
        // if (logcall) platform_core::recorder::recording_log_end(global_log);
        return 1;
    }
    {
        auto close_port = std::make_shared<platform_core::connector>(platform_core::connector::connector_begin("127.0.0.3", 2));
        try {
            close_port->connect("127.0.0.3", 1);

            if (close_port->is_connected()) {
                std::thread([close_port, &stop_requested, &server_socket] {
                    close_port->listener(1);
                    close_port->disconnect();
                    platform_core::connector::connector_end(*close_port);
                    stop_requested.store(true);

                    SOCKET s = server_socket.exchange(INVALID_SOCKET);
                    if (s != INVALID_SOCKET) {
#ifdef _WIN32
                        shutdown(s, SD_BOTH);
#else
                        shutdown(s, SHUT_RDWR);
#endif
                        CLOSE_SOCKET(s);
                    }
                    }).detach();
            }
            else {
                platform_core::connector::connector_end(*close_port);
            }
        }
        catch (...) {

        }
    }

    while (!stop_requested.load()) {
        sockaddr_in client_addr{};
#ifdef _WIN32
        int client_len = sizeof(client_addr);
#else
        socklen_t client_len = sizeof(client_addr);
#endif

        SOCKET current_server = server_socket.load();
        if (current_server == INVALID_SOCKET) {
            break;
        }

        SOCKET client_socket = accept(current_server, reinterpret_cast<struct sockaddr*>(&client_addr), &client_len);

        if (client_socket == INVALID_SOCKET) {
            if (stop_requested.load()) {
                break;
            }
            continue;
        }

        int tcp_no_delay = 1;
        setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&tcp_no_delay), sizeof(tcp_no_delay));
        std::thread(handle_client, client_socket).detach();
    }

    {
        SOCKET s = server_socket.exchange(INVALID_SOCKET);
        if (s != INVALID_SOCKET) {
#ifdef _WIN32
            shutdown(s, SD_BOTH);
#else
            shutdown(s, SHUT_RDWR);
#endif
            CLOSE_SOCKET(s);
        }
    }

#ifdef _WIN32
    WSACleanup();
#endif
    for (auto &[a, b] : registry_editor_service_local::returned) {
        b.second = 1;
        registry_end(b.first);
    }
    // if (logcall) platform_core::recorder::recording_log_end(global_log);
    return 0;
}