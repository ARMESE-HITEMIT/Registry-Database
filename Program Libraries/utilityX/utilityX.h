/*
    *   Utility Library
    *
    *   Copyright (c) 2022 RANDOM ARMESE HITEMIT - Utility Library
    *   All rights reserved.
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

#pragma message(" *   Utility Library")
#pragma message(" *")
#pragma message(" *   Copyright (c) 2022 RANDOM ARMESE HITEMIT - Utility Library")
#pragma message(" *   All rights reserved.")
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

#include <algorithm>
#include <cerrno>
#include <charconv>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <string>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <limits>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#else
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#define NO_UAC
#define NO_CONNECTOR
#define NO_RECORDER
#define NO_ERROR_LOOKINGUP

#include "../system/system.h"

#undef NO_UAC
#undef NO_CONNECTOR
#undef NO_RECORDER
#undef NO_ERROR_LOOKINGUP

namespace utilityX {
    namespace detail {
        inline bool is_space(unsigned char c) noexcept
        {
            return std::isspace(c) != 0;
        }

        inline bool is_alpha_ascii(char c) noexcept
        {
            return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }

        inline char ascii_lower(char c) noexcept
        {
            if (c >= 'A' && c <= 'Z')
                return static_cast<char>(c - 'A' + 'a');

            return c;
        }

        inline bool is_path_separator(char c) noexcept
        {
            return c == '/' || c == '\\';
        }

        inline bool has_windows_drive_prefix(std::string_view s) noexcept
        {
            return s.size() >= 2 &&
                is_alpha_ascii(s[0]) &&
                s[1] == ':';
        }

        inline bool has_windows_unc_prefix(std::string_view s) noexcept
        {
            return s.size() >= 2 &&
                s[0] == '\\' &&
                s[1] == '\\';
        }

        inline std::string trim_path_edges(std::string_view s)
        {
            while (!s.empty() && is_space(static_cast<unsigned char>(s.front())))
                s.remove_prefix(1);

            while (!s.empty() && is_space(static_cast<unsigned char>(s.back())))
                s.remove_suffix(1);

            return std::string(s);
        }

        inline std::string strip_outer_quotes(std::string s)
        {
            s = trim_path_edges(s);

            if (s.size() >= 2)
            {
                const char first = s.front();
                const char last = s.back();

                if ((first == '"' && last == '"') ||
                    (first == '\'' && last == '\''))
                {
                    s.erase(s.begin());
                    s.pop_back();
                }
            }

            return s;
        }

        inline void replace_char(std::string& s, char from, char to)
        {
            for (char& c : s)
            {
                if (c == from)
                    c = to;
            }
        }

        inline std::string collapse_repeated_separators(std::string s, char separator)
        {
            if (s.empty())
                return s;

            std::string out;
            out.reserve(s.size());

            for (std::size_t i = 0; i < s.size(); ++i)
            {
                const char c = s[i];

                if (c != separator)
                {
                    out.push_back(c);
                    continue;
                }

                out.push_back(c);

                while (i + 1 < s.size() && s[i + 1] == separator)
                {
                    /*
                        Keep the first two leading separators on POSIX because a path
                        beginning with // can have implementation-defined semantics.
                        This also preserves UNC-like text after converting \\server\share
                        to //server/share on Linux.
                    */
                    if (separator == '/' && out.size() == 1 && i == 0)
                        break;

                    ++i;
                }
            }

            return out;
        }

#ifndef _WIN32
        inline std::string windows_drive_to_wsl_mount(std::string s)
        {
            if (!has_windows_drive_prefix(s))
                return s;

            const char drive = ascii_lower(s[0]);
            std::string tail = s.substr(2);

            replace_char(tail, '\\', '/');

            while (!tail.empty() && tail.front() == '/')
                tail.erase(tail.begin());

            std::string out = "/mnt/";
            out.push_back(drive);

            if (!tail.empty())
            {
                out.push_back('/');
                out += tail;
            }

            return out;
        }
#endif

        inline std::string translate_path_for_host(std::string input)
        {
            input = strip_outer_quotes(std::move(input));

            if (input.empty())
                return input;

#ifdef _WIN32
            /*
                Windows accepts both C:/dir/file and C:\dir\file in most APIs.
                Convert to native separators before std::filesystem normalizes it.
            */
            replace_char(input, '/', '\\');
            return input;
#else
            /*
                Linux / POSIX support added here:
                - C:\Users\me\file.txt  -> /mnt/c/Users/me/file.txt
                - C:/Users/me/file.txt  -> /mnt/c/Users/me/file.txt
                - \mnt\c\Users\me       -> /mnt/c/Users/me
                - /mnt/c/Users/me       -> /mnt/c/Users/me

                This is intentionally done before std::filesystem::path is built.
                Otherwise Linux treats "C:\..." as a relative path named literally
                with ':' and '\' characters.
            */
            input = windows_drive_to_wsl_mount(std::move(input));
            replace_char(input, '\\', '/');
            input = collapse_repeated_separators(std::move(input), '/');
            return input;
#endif
        }

        inline std::filesystem::path weakly_canonical_or_lexical(
            std::filesystem::path p)
        {
            try
            {
                return std::filesystem::weakly_canonical(p);
            }
            catch (...)
            {
                return p.lexically_normal();
            }
        }

        inline bool has_exe_extension_ci(const std::string& s)
        {
            if (s.size() < 4)
                return false;

            const std::size_t n = s.size();

            return std::tolower(static_cast<unsigned char>(s[n - 4])) == '.' &&
                std::tolower(static_cast<unsigned char>(s[n - 3])) == 'e' &&
                std::tolower(static_cast<unsigned char>(s[n - 2])) == 'x' &&
                std::tolower(static_cast<unsigned char>(s[n - 1])) == 'e';
        }

#ifdef _WIN32
        inline std::string quote_windows_arg(const std::string& s)
        {
            std::string result;
            result.reserve(s.size() + 2);

            result.push_back('"');

            std::size_t backslashes = 0;

            for (const char c : s)
            {
                if (c == '\\')
                {
                    ++backslashes;
                }
                else if (c == '"')
                {
                    result.append(backslashes * 2 + 1, '\\');
                    result.push_back('"');
                    backslashes = 0;
                }
                else
                {
                    result.append(backslashes, '\\');
                    result.push_back(c);
                    backslashes = 0;
                }
            }

            result.append(backslashes * 2, '\\');
            result.push_back('"');

            return result;
        }
#endif
    }

    inline std::string trim(const std::string& inp)
    {
        std::string strings = inp;

        while (!strings.empty() && strings.front() == ' ')
        {
            strings.erase(0, 1);
        }

        while (!strings.empty() && strings.back() == ' ')
        {
            strings.pop_back();
        }

        bool in_string = false;

        for (QWORD i = 0; i + 1 < strings.size();)
        {
            if (strings[static_cast<size_t>(i)] == '"')
            {
                in_string = !in_string;
                ++i;
                continue;
            }

            if (!in_string && strings[static_cast<size_t>(i)] == ' ' && strings[static_cast<size_t>(i + 1)] == ' ')
            {
                strings.erase(static_cast<size_t>(i), 1);
                continue;
            }

            ++i;
        }

        return strings;
    }

    inline std::vector<std::string> parser(
        const std::string& input,
        char delimiter)
    {
        std::vector<std::string> result;

        if (input.empty())
            return result;

        bool in_string = false;
        QWORD start = 0;

        for (QWORD i = 0; i < input.size(); ++i)
        {
            if (input[static_cast<size_t>(i)] == '"')
            {
                in_string = !in_string;
                continue;
            }

            if (!in_string && input[static_cast<size_t>(i)] == delimiter)
            {
                result.emplace_back(input.substr(static_cast<size_t>(start), static_cast<size_t>(i) - static_cast<size_t>(start)));
                start = i + 1;
            }
        }

        result.emplace_back(input.substr(static_cast<size_t>(start)));
        return result;
    }

    inline std::string to_native_path_string(const std::string& raw_path)
    {
        return detail::translate_path_for_host(raw_path);
    }

    inline std::string normalize_path(const std::string& raw_path)
    {
        namespace fs = std::filesystem;

        std::string translated = detail::translate_path_for_host(raw_path);

        if (translated.empty())
            return translated;

        fs::path p(translated);

        if (p.is_relative())
        {
            try
            {
                p = fs::current_path() / p;
            }
            catch (...)
            {
            }
        }

        p = detail::weakly_canonical_or_lexical(std::move(p));

#ifdef _WIN32
        return p.string();
#else
        return p.generic_string();
#endif
    }

    inline std::filesystem::path normalize_filesystem_path(
        const std::string& raw_path)
    {
        return std::filesystem::path(normalize_path(raw_path));
    }

    inline std::string loop_character(char chr, QWORD ammount)
    {
        std::string s(static_cast<size_t>(ammount), chr);
        return s;
    }

    static inline long double to_real(std::string_view s) noexcept
    {
        if (s.empty())
            return 0.0L;

        const char* p = s.data();
        const char* e = p + s.size();

        bool neg = *p == '-';

        if (neg)
            ++p;

        long long ip = 0;

        while (p < e && *p != '.')
        {
            if (*p < '0' || *p > '9')
                break;

            ip = ip * 10 + (*p++ - '0');
        }

        long double v = static_cast<long double>(ip);

        if (p < e && *p == '.')
        {
            ++p;

            long long frac_int = 0;
            long long scale = 1;

            while (p < e)
            {
                if (*p < '0' || *p > '9')
                    break;

                frac_int = frac_int * 10 + (*p++ - '0');
                scale *= 10;
            }

            v += static_cast<long double>(frac_int) / scale;
        }

        return neg ? -v : v;
    }

    static inline QWORD to_unsigned(std::string_view s) noexcept
    {
        const char* p = s.data();
        QWORD v = 0;

        for (const char* e = p + s.size(); p < e; ++p)
        {
            if (*p < '0' || *p > '9')
                break;

            v = v * 10 + (*p - '0');
        }

        return v;
    }

    inline std::string registry_file_path_from_this_header()
    {
        namespace fs = std::filesystem;

        const fs::path registry_suffix =
            fs::path("Program Datas") /
            "Essentials" /
            "global.system_registry.regx";

        try {
            fs::path probe = fs::current_path();
            while (true) {
                const fs::path candidate = probe / registry_suffix;
                if (fs::exists(candidate)) {
                    return normalize_path(candidate.string());
                }

                if (!probe.has_parent_path() || probe == probe.parent_path()) {
                    break;
                }

                probe = probe.parent_path();
            }
        }
        catch (...) {}

        const fs::path header_path =
            fs::path(normalize_path(__FILE__)).parent_path();

        const fs::path registry_path =
            header_path /
            ".." /
            ".." /
            registry_suffix;

        return normalize_path(registry_path.string());
    }
    struct process_handle
    {
        enum class type_t : BYTE
        {
            thread = 0,
            task = 1
        };

        BYTE type;
        QWORD id;

        constexpr process_handle()
            : type(static_cast<BYTE>(type_t::thread)), id(0)
        {}

        constexpr process_handle(type_t type, QWORD id)
            : type(static_cast<BYTE>(type)), id(id)
        {}

        constexpr type_t get_type() const noexcept
        {
            return static_cast<type_t>(type);
        }

        constexpr bool is_thread() const noexcept
        {
            return get_type() == type_t::thread;
        }

        constexpr bool is_task() const noexcept
        {
            return get_type() == type_t::task;
        }

        constexpr bool valid() const noexcept
        {
            return id != 0;
        }
    };

    bool test_netconnection(const std::string& ip, DWORD port)
    {
#ifdef _WIN32

        static bool wsa_initialized = []()
            {
                WSADATA wsa{};
                return WSAStartup(MAKEWORD(2, 2), &wsa) == 0;
            }();

        if (!wsa_initialized)
            return false;
#endif

        if (port > 65535)
            return false;

        const int sock = static_cast<int>(
#ifdef _WIN32
            socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)
#else
            socket(AF_INET, SOCK_STREAM, 0)
#endif
            );

        if (sock < 0)
            return false;

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_port = htons(static_cast<std::uint16_t>(port));

        if (inet_pton(AF_INET, ip.c_str(), &address.sin_addr) != 1)
        {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }

        const int result = connect(
            sock,
            reinterpret_cast<const sockaddr*>(&address),
            sizeof(address)
        );

#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif

        return result == 0;
    }

    inline bool terminate(const process_handle& handle)
    {
        if (!handle.valid())
        {
            return false;
        }

#ifdef _WIN32

        if (handle.is_thread())
        {
            HANDLE thread_handle = ::OpenThread(
                THREAD_TERMINATE | SYNCHRONIZE,
                FALSE,
                static_cast<DWORD>(handle.id)
            );

            if (thread_handle == nullptr)
            {
                return false;
            }

            const bool result = (::TerminateThread(thread_handle, 1) == TRUE);

            ::CloseHandle(thread_handle);

            return result;
        }

        if (handle.is_task())
        {
            HANDLE process_handle = ::OpenProcess(
                PROCESS_TERMINATE | SYNCHRONIZE,
                FALSE,
                static_cast<DWORD>(handle.id)
            );

            if (process_handle == nullptr)
            {
                return false;
            }

            const bool result =
                (::TerminateProcess(process_handle, 1) != FALSE);

            ::CloseHandle(process_handle);

            return result;
        }

        return false;

#else

        if (handle.is_thread())
        {
#if defined(__linux__)

            const pid_t process_id =
                static_cast<pid_t>(::getpid());

            const pid_t thread_id =
                static_cast<pid_t>(handle.id);

            return static_cast<long>(
                ::syscall(
                    SYS_tgkill,
                    process_id,
                    thread_id,
                    SIGKILL
                )
                ) == 0;

#else

            return false;

#endif
        }

        if (handle.is_task())
        {
            return ::kill(
                static_cast<pid_t>(handle.id),
                SIGKILL
            ) == 0;
        }

        return false;

#endif
    }

    template <typename F, typename... Args>
    inline process_handle initiate(F&& fn, Args&&... args)
    {
        if constexpr (
            std::is_invocable_v<
            std::decay_t<F>,
            std::decay_t<Args>...
            >
            )
        {
            std::thread t(
                [
                    callable = std::decay_t<F>(
                        std::forward<F>(fn)
                    ),
                        ...params = std::decay_t<Args>(
                            std::forward<Args>(args)
                        )
                ]() mutable
                {
                    try
                    {
                        std::invoke(
                            std::move(callable),
                            std::move(params)...
                        );
                    }
                    catch (...)
                    {
                    }
                }
           );

#ifdef _WIN32

            const HANDLE native_handle =
                reinterpret_cast<HANDLE>(
                    t.native_handle()
                    );

            const DWORD tid =
                ::GetThreadId(native_handle);

            if (tid == 0)
            {
                t.detach();

                return process_handle(
                    process_handle::type_t::thread,
                    0
                );
            }

            t.detach();

            return process_handle(
                process_handle::type_t::thread,
                static_cast<QWORD>(tid)
            );

#elif defined(__linux__)

            auto thread_id =
                std::make_shared<
                std::atomic<pid_t>
                >(0);

            t.detach();

            return process_handle(
                process_handle::type_t::thread,
                static_cast<QWORD>(0)
            );

#else

            t.detach();

            return process_handle(
                process_handle::type_t::thread,
                static_cast<QWORD>(0)
            );

#endif
        }
        else
        {
            auto make_string =
                [](auto&& value) -> std::string
                {
                    using T =
                        std::decay_t<
                        decltype(value)
                        >;

                    if constexpr (
                        std::is_same_v<
                        T,
                        std::string
                        >
                        )
                    {
                        return std::forward<
                            decltype(value)
                        >(value);
                    }
                    else if constexpr (
                        std::is_same_v<
                        T,
                        std::filesystem::path
                        >
                        )
                    {
                        return value.string();
                    }
                    else if constexpr (
                        std::is_convertible_v<
                        T,
                        std::string
                        >
                        )
                    {
                        return std::string(
                            std::forward<
                            decltype(value)
                            >(value)
                        );
                    }
                    else if constexpr (
                        std::is_arithmetic_v<T>
                        )
                    {
                        return std::to_string(value);
                    }
                    else
                    {
                        std::ostringstream out;

                        out << value;

                        return out.str();
                    }
                };

            std::string file =
                normalize_path(
                    make_string(
                        std::forward<F>(fn)
                    )
                );

#ifdef _WIN32

            if (!detail::has_exe_extension_ci(file))
            {
                file += ".exe";
            }

#endif

            if (
                file.empty() ||
                !std::filesystem::exists(file) ||
                !std::filesystem::is_regular_file(file)
                )
            {
                throw platform_core::ecc{ 0, 34 };
            }

            std::vector<std::string> values;

            values.reserve(
                1 + sizeof...(Args)
            );

            values.emplace_back(file);

            (
                values.emplace_back(
                    make_string(
                        std::forward<Args>(args)
                    )
                ),
                ...
                );

#ifdef _WIN32

            std::string command_line;

            for (
                std::size_t i = 0;
                i < values.size();
                ++i
                )
            {
                if (i != 0)
                {
                    command_line.push_back(' ');
                }

                command_line +=
                    detail::quote_windows_arg(
                        values[i]
                    );
            }

            std::vector<char> mutable_command_line(
                command_line.begin(),
                command_line.end()
            );

            mutable_command_line.push_back('\0');

            STARTUPINFOA startup_info{};

            PROCESS_INFORMATION process_info{};

            startup_info.cb =
                sizeof(startup_info);

            const bool created =
                (::CreateProcessA(
                    file.c_str(),
                    mutable_command_line.data(),
                    nullptr,
                    nullptr,
                    FALSE,
                    0,
                    nullptr,
                    nullptr,
                    &startup_info,
                    &process_info
                ) != FALSE);

            if (!created)
            {
                throw platform_core::ecc{ 0, 34 };
            }

            ::CloseHandle(
                process_info.hThread
            );

            const QWORD pid =
                static_cast<QWORD>(
                    process_info.dwProcessId
                    );

            const HANDLE native_process_handle =
                process_info.hProcess;

            try
            {
                std::thread(
                    [native_process_handle]
                    {
                        ::WaitForSingleObject(
                            native_process_handle,
                            INFINITE
                        );

                        ::CloseHandle(
                            native_process_handle
                        );
                    }
                ).detach();
            }
            catch (...)
            {
                ::TerminateProcess(
                    native_process_handle,
                    1
                );

                ::CloseHandle(
                    native_process_handle
                );

                throw;
            }

            return process_handle(
                process_handle::type_t::task,
                pid
            );

#else

            if (::access(file.c_str(), X_OK) != 0)
            {
                throw platform_core::ecc{ 0, 34 };
            }

            std::vector<char*> raw_args;

            raw_args.reserve(
                values.size() + 1
            );

            for (std::string& value : values)
            {
                raw_args.push_back(
                    value.data()
                );
            }

            raw_args.push_back(nullptr);

            const pid_t pid = ::fork();

            if (pid < 0)
            {
                throw platform_core::ecc{ 0, 34 };
            }

            if (pid == 0)
            {
                ::execv(
                    file.c_str(),
                    raw_args.data()
                );

                ::_exit(127);
            }

            try
            {
                std::thread(
                    [pid]
                    {
                        int status = 0;

                        while (
                            ::waitpid(
                                pid,
                                &status,
                                0
                            ) == -1
                            )
                        {
                            if (errno != EINTR)
                            {
                                return;
                            }
                        }
                    }
                ).detach();
            }
            catch (...)
            {
                ::kill(
                    pid,
                    SIGKILL
                );

                int status = 0;

                while (
                    ::waitpid(
                        pid,
                        &status,
                        0
                    ) == -1 &&
                    errno == EINTR
                    )
                {
                }

                throw;
            }

            return process_handle(
                process_handle::type_t::task,
                static_cast<QWORD>(pid)
            );

#endif
        }
    }

    namespace coder {

        class compact_ascii_reference;

        struct compact_ascii_lut {
            uint8_t table[256]{};

            constexpr compact_ascii_lut() {
                for (int i = 0; i < 256; ++i) {
                    if (i >= 'a' && i <= 'z')
                        table[i] = static_cast<uint8_t>(i - 'a');

                    else if (i >= '0' && i <= '9')
                        table[i] = static_cast<uint8_t>(i - '0' + 26);

                    else if (i == '-')
                        table[i] = 36;

                    else if (i == '_')
                        table[i] = 37;

                    else if (i == '#')
                        table[i] = 38;

                    else if (i == '%')
                        table[i] = 39;

                    else if (i == '.')
                        table[i] = 40;

                    else if (i == ',')
                        table[i] = 41;

                    else if (i == ';')
                        table[i] = 42;

                    else if (i == ':')
                        table[i] = 43;

                    else if (i == '/')
                        table[i] = 44;

                    else if (i == '\\')
                        table[i] = 45;

                    else if (i == '*')
                        table[i] = 46;

                    else if (i == '+')
                        table[i] = 47;

                    else if (i == '@')
                        table[i] = 48;

                    else if (i == '!')
                        table[i] = 49;

                    else if (i == '&')
                        table[i] = 50;

                    else if (i == '=')
                        table[i] = 51;

                    else if (i == '(')
                        table[i] = 52;

                    else if (i == ')')
                        table[i] = 53;

                    else if (i == '[')
                        table[i] = 54;

                    else if (i == ']')
                        table[i] = 55;

                    else if (i == '{')
                        table[i] = 56;

                    else if (i == '}')
                        table[i] = 57;

                    else if (i == '"')
                        table[i] = 58;

                    else if (i == '\'')
                        table[i] = 59;

                    else if (i == '<')
                        table[i] = 60;

                    else if (i == '>')
                        table[i] = 61;

                    else if (i == '~')
                        table[i] = 62;

                    else if (i == ' ')
                        table[i] = 63;

                    else
                        table[i] = 63;
                }
            }
        };

        inline constexpr compact_ascii_lut ascii_val_lut;

        class compact_ascii {
            private:
                std::string data;

                static constexpr char table[] =
                    "abcdefghijklmnopqrstuvwxyz"
                    "0123456789"
                    "-_#%"
                    ".,;:/\\*+@!&="
                    "()[]{}"
                    "\"'"
                    "<>~ ";

            public:
                compact_ascii() {
                    data.resize(sizeof(size_t), '\0');
                }

                explicit compact_ascii(std::string packed) noexcept
                    : data(std::move(packed)) {}

                operator compact_ascii_reference() const noexcept;

                bool operator==(const compact_ascii& other) const noexcept {
                    return this->data == other.data;
                }

                bool operator==(const compact_ascii_reference& other) const noexcept;

                size_t size() const noexcept {
                    if (data.size() < sizeof(size_t))
                        return 0;

                    size_t sz = 0;
                    std::memcpy(&sz, data.data(), sizeof(size_t));

                    return sz;
                }

                bool empty() const noexcept {
                    return size() == 0;
                }

                char operator[](size_t i) const noexcept {
                    size_t bit_pos = i * 6;

                    size_t byte_offset =
                        sizeof(size_t) + (bit_pos >> 3);

                    int bit_shift =
                        static_cast<int>(bit_pos & 7);

                    if (byte_offset >= data.size())
                        return table[0];

                    uint64_t chunk = 0;

                    size_t available =
                        data.size() - byte_offset;

                    if (available >= 8) {
                        std::memcpy(
                            &chunk,
                            data.data() + byte_offset,
                            8
                        );
                    }
                    else {
                        std::memcpy(
                            &chunk,
                            data.data() + byte_offset,
                            available
                        );
                    }

                    return table[
                        (chunk >> bit_shift) & 0x3F
                    ];
                }



                static inline compact_ascii encode(
                    const std::string& text
                ) {
                    const size_t text_len = text.size();

                    const size_t packed_bytes =
                        sizeof(size_t) +
                        (text_len * 6 + 7) / 8;

                    std::string packed(
                        packed_bytes,
                        '\0'
                    );

                    std::memcpy(
                        packed.data(),
                        &text_len,
                        sizeof(size_t)
                    );

                    uint32_t buffer = 0;
                    int bits = 0;

                    size_t write_idx =
                        sizeof(size_t);

                    char* dest =
                        packed.data();

                    for (char c : text) {
                        const uint8_t uc = static_cast<uint8_t>(c);

                        c = static_cast<char>(
                            uc >= 'A' && uc <= 'Z'
                            ? uc + ('a' - 'A')
                            : uc
                            );

                        buffer |=
                            static_cast<uint32_t>(
                                ascii_val_lut.table[
                                    static_cast<uint8_t>(c)
                                ]
                                ) << bits;

                        bits += 6;

                        while (bits >= 8) {
                            dest[write_idx++] =
                                static_cast<char>(buffer);

                            buffer >>= 8;
                            bits -= 8;
                        }
                    }

                    if (
                        bits > 0 &&
                        write_idx < packed_bytes
                        ) {
                        dest[write_idx] =
                            static_cast<char>(buffer);
                    }

                    return compact_ascii(
                        std::move(packed)
                    );
                }

                std::string decode() const {
                    size_t length = size();

                    std::string out;
                    out.reserve(length);

                    uint32_t buffer = 0;
                    int bits = 0;

                    const size_t byte_len = data.size();
                    const char* src = data.data();

                    for (
                        size_t i = sizeof(size_t);
                        i < byte_len && out.size() < length;
                        ++i
                        ) {
                        buffer |=
                            static_cast<uint32_t>(
                                static_cast<uint8_t>(src[i])
                                ) << bits;

                        bits += 8;

                        while (
                            bits >= 6 &&
                            out.size() < length
                            ) {
                            out.push_back(
                                table[buffer & 0x3F]
                            );

                            buffer >>= 6;
                            bits -= 6;
                        }
                    }

                    return out;
                }

                const std::string& raw() const noexcept {
                    return data;
                }

                friend compact_ascii encode(
                    const std::string& text
                );

                friend class compact_ascii_reference;
        };

        constexpr char compact_ascii::table[];

        class compact_ascii_reference {
        private:
            const char* ptr_{ nullptr };
            size_t byte_size_{ 0 };

        public:
            compact_ascii_reference() noexcept = default;

            compact_ascii_reference(
                const char* data,
                size_t size_bytes
            ) noexcept
                : ptr_(data),
                byte_size_(size_bytes) {}

            compact_ascii_reference(
                const compact_ascii& ca
            ) noexcept
                : ptr_(ca.raw().data()),
                byte_size_(ca.raw().size()) {}

            size_t size() const noexcept {
                if (
                    ptr_ == nullptr ||
                    byte_size_ < sizeof(size_t)
                    )
                    return 0;

                size_t sz = 0;

                std::memcpy(
                    &sz,
                    ptr_,
                    sizeof(size_t)
                );

                return sz;
            }

            bool empty() const noexcept {
                return size() == 0;
            }

            char operator[](size_t i) const noexcept {
                size_t bit_pos = i * 6;

                size_t byte_offset =
                    sizeof(size_t) + (bit_pos >> 3);

                int bit_shift =
                    static_cast<int>(bit_pos & 7);

                if (
                    ptr_ == nullptr ||
                    byte_offset >= byte_size_
                    )
                    return compact_ascii::table[0];

                uint64_t chunk = 0;

                size_t available =
                    byte_size_ - byte_offset;

                if (available >= 8) {
                    std::memcpy(
                        &chunk,
                        ptr_ + byte_offset,
                        8
                    );
                }
                else {
                    std::memcpy(
                        &chunk,
                        ptr_ + byte_offset,
                        available
                    );
                }

                return compact_ascii::table[
                    (chunk >> bit_shift) & 0x3F
                ];
            }

            std::string decode() const {
                size_t length = size();

                std::string out;
                out.reserve(length);

                uint32_t buffer = 0;
                int bits = 0;

                for (
                    size_t i = sizeof(size_t);
                    i < byte_size_ &&
                    out.size() < length;
                    ++i
                    ) {
                    buffer |=
                        static_cast<uint32_t>(
                            static_cast<uint8_t>(ptr_[i])
                            ) << bits;

                    bits += 8;

                    while (
                        bits >= 6 &&
                        out.size() < length
                        ) {
                        out.push_back(
                            compact_ascii::table[
                                buffer & 0x3F
                            ]
                        );

                        buffer >>= 6;
                        bits -= 6;
                    }
                }

                return out;
            }

            std::string_view raw_view() const noexcept {
                return std::string_view(
                    ptr_,
                    byte_size_
                );
            }

            bool operator==(
                const compact_ascii_reference& other
                ) const noexcept {
                return
                    byte_size_ == other.byte_size_ &&
                    (
                        ptr_ == other.ptr_ ||
                        std::memcmp(
                            ptr_,
                            other.ptr_,
                            byte_size_
                        ) == 0
                        );
            }

            bool operator==(
                const compact_ascii& other
                ) const noexcept {
                return
                    *this ==
                    compact_ascii_reference(other);
            }
        };

        inline compact_ascii::operator compact_ascii_reference()
            const noexcept {
            return compact_ascii_reference(*this);
        }

        inline bool compact_ascii::operator==(
            const compact_ascii_reference& other
            ) const noexcept {
            return
                compact_ascii_reference(*this) == other;
        }

        class uuid_reference;

        class uuid {
        private:
            union {
                struct {
                    uint64_t qword[4];
                } q;

                struct {
                    uint32_t dword[8];
                } d;

                struct {
                    uint16_t word[16];
                } w;

                struct {
                    uint8_t byte[32];
                } b;
            } data{};

            static constexpr char hex[] =
                "0123456789abcdef";

            static constexpr bool is_hex(char c) noexcept {
                return
                    (c >= '0' && c <= '9') ||
                    (c >= 'a' && c <= 'f') ||
                    (c >= 'A' && c <= 'F');
            }

            static constexpr uint8_t hex_value(char c) noexcept {
                if (c >= '0' && c <= '9')
                    return static_cast<uint8_t>(c - '0');

                if (c >= 'a' && c <= 'f')
                    return static_cast<uint8_t>(c - 'a' + 10);

                return static_cast<uint8_t>(c - 'A' + 10);
            }

            static constexpr size_t encoded_size =
                2 +                 // {}
                64 +                // 32 bytes -> 64 hex
                7;                  // 7 separators

        public:
            uuid() noexcept = default;

            uuid(
                uint64_t a,
                uint64_t b,
                uint64_t c,
                uint64_t d
            ) noexcept {
                data.q.qword[0] = a;
                data.q.qword[1] = b;
                data.q.qword[2] = c;
                data.q.qword[3] = d;
            }

            uuid(const uuid&) noexcept = default;
            uuid(uuid&&) noexcept = default;

            uuid& operator=(const uuid&) noexcept = default;
            uuid& operator=(uuid&&) noexcept = default;

            uuid(const uuid_reference& ref);

            uuid& operator=(const uuid_reference& ref);

            static uuid encode(const std::string& value) {
                return encode(std::string_view(value));
            }

            static uuid encode(std::string_view value) {
                if (value.size() > size())
                    throw platform_core::ecc{ 4, 18 };

                uuid result;

                std::memcpy(
                    result.data.b.byte,
                    value.data(),
                    value.size()
                );

                return result;
            }

            static uuid encode(const char* value) {
                return encode(std::string_view(value));
            }

            std::string decode() const {
                size_t length = size();

                while (
                    length != 0 &&
                    data.b.byte[length - 1] == 0
                    ) {
                    --length;
                }

                return std::string(
                    reinterpret_cast<const char*>(data.b.byte),
                    length
                );
            }

            std::string get() const {
                std::string result(encoded_size, '-');

                result[0] = '{';
                result[encoded_size - 1] = '}';

                size_t output = 1;

                for (size_t i = 0; i < size(); ++i) {
                    if (i != 0 && (i % 4) == 0)
                        ++output;

                    const uint8_t value = data.b.byte[i];

                    result[output++] =
                        hex[value >> 4];

                    result[output++] =
                        hex[value & 0x0f];
                }

                return result;
            }

            void assign(const std::string& value) {
                assign(std::string_view(value));
            }

            void assign(std::string_view value) {
                if (value.size() != encoded_size)
                    throw platform_core::ecc{ 4, 19 };

                if (
                    value[0] != '{' ||
                    value[encoded_size - 1] != '}'
                    ) {
                    throw platform_core::ecc{ 4, 19 };
                }

                size_t input = 1;

                for (size_t i = 0; i < size(); ++i) {
                    if (i != 0 && (i % 4) == 0) {
                        if (value[input] != '-')
                            throw platform_core::ecc{ 4, 19 };

                        ++input;
                    }

                    const char high = value[input++];
                    const char low = value[input++];

                    if (
                        !is_hex(high) ||
                        !is_hex(low)
                        ) {
                        throw platform_core::ecc{ 4, 19 };
                    }

                    data.b.byte[i] =
                        static_cast<uint8_t>(
                            (hex_value(high) << 4) |
                            hex_value(low)
                            );
                }
            }

            void assign(const char* value) {
                assign(std::string_view(value));
            }

            const uint8_t* raw() const noexcept {
                return data.b.byte;
            }

            uint8_t* raw() noexcept {
                return data.b.byte;
            }

            static constexpr size_t size() noexcept {
                return 32;
            }

            static constexpr size_t string_size() noexcept {
                return encoded_size;
            }

            bool operator==(const uuid&) const noexcept = default;

            auto operator<=>(const uuid&) const noexcept = default;

            bool operator==(const uuid_reference& ref) const noexcept;
        };


        class uuid_reference {
        private:
            const uint8_t* ptr = nullptr;

            static constexpr char hex[] =
                "0123456789abcdef";

            static constexpr size_t encoded_size =
                73;

        public:
            uuid_reference() noexcept = default;

            explicit uuid_reference(
                const uuid& value
            ) noexcept :
                ptr(value.raw()) {}

            explicit uuid_reference(
                const uint8_t* value
            ) noexcept :
                ptr(value) {}

            const uint8_t* raw() const noexcept {
                return ptr;
            }

            std::string_view raw_view() const noexcept {
                return std::string_view(
                    reinterpret_cast<const char*>(ptr),
                    uuid::size()
                );
            }

            std::string decode() const {
                size_t length = uuid::size();

                while (
                    length != 0 &&
                    ptr[length - 1] == 0
                    ) {
                    --length;
                }

                return std::string(
                    reinterpret_cast<const char*>(ptr),
                    length
                );
            }

            std::string get() const {
                std::string result(encoded_size, '-');

                result[0] = '{';
                result[encoded_size - 1] = '}';

                size_t output = 1;

                for (size_t i = 0; i < uuid::size(); ++i) {
                    if (i != 0 && (i % 4) == 0)
                        ++output;

                    const uint8_t value = ptr[i];

                    result[output++] =
                        hex[value >> 4];

                    result[output++] =
                        hex[value & 0x0f];
                }

                return result;
            }

            static constexpr size_t size() noexcept {
                return uuid::size();
            }

            static constexpr size_t string_size() noexcept {
                return encoded_size;
            }

            bool operator==(
                const uuid_reference& ref
                ) const noexcept {
                return std::memcmp(
                    ptr,
                    ref.ptr,
                    uuid::size()
                ) == 0;
            }

            bool operator==(
                const uuid& value
                ) const noexcept {
                return std::memcmp(
                    ptr,
                    value.raw(),
                    uuid::size()
                ) == 0;
            }
        };


        inline uuid::uuid(
            const uuid_reference& ref
        ) {
            std::memcpy(
                data.b.byte,
                ref.raw(),
                size()
            );
        }


        inline uuid& uuid::operator=(
            const uuid_reference& ref
            ) {
            std::memcpy(
                data.b.byte,
                ref.raw(),
                size()
            );

            return *this;
        }


        inline bool uuid::operator==(
            const uuid_reference& ref
            ) const noexcept {
            return std::memcmp(
                data.b.byte,
                ref.raw(),
                size()
            ) == 0;
        }
        namespace encryptor
        {
            namespace detail
            {
                constexpr QWORD c1 = 0x9E3779B97F4A7C15ull;
                constexpr QWORD c2 = 0xBF58476D1CE4E5B9ull;
                constexpr QWORD c3 = 0x94D049BB133111EBull;

                QWORD rotl(QWORD x, unsigned r)
                {
                    return (x << r) | (x >> (64 - r));
                }

                QWORD mix(QWORD x)
                {
                    x ^= x >> 30;
                    x *= c2;
                    x ^= x >> 27;
                    x *= c3;
                    x ^= x >> 31;
                    return x;
                }

                QWORD hash_key(const std::string& key)
                {
                    QWORD h = 0xCBF29CE484222325ull;

                    for (std::size_t i = 0; i < key.size(); ++i)
                    {
                        h ^= static_cast<BYTE>(key[i]);
                        h *= 0x100000001B3ull;
                        h ^= rotl(h, 17);
                        h += c1 ^ (static_cast<QWORD>(i) * c2);
                        h = mix(h);
                    }

                    return mix(h ^ static_cast<QWORD>(key.size()));
                }

                QWORD init_state(const std::string& key, QWORD nonce)
                {
                    QWORD state = hash_key(key) ^ nonce ^ 0x243F6A8885A308D3ull;
                    state = mix(state);

                    for (std::size_t i = 0; i < key.size(); ++i)
                    {
                        state ^= static_cast<BYTE>(key[i]);
                        state += c1 + static_cast<QWORD>(i) * c2;
                        state = rotl(state, 23);
                        state = mix(state);
                    }

                    return mix(state ^ nonce);
                }

                QWORD next(QWORD& state)
                {
                    state += c1;
                    state ^= state >> 12;
                    state *= c2;
                    state ^= state >> 25;
                    state *= c3;
                    state ^= state >> 27;
                    return mix(state);
                }

                BYTE next_byte(QWORD& state, QWORD& buffer, unsigned& count)
                {
                    if (count == 0)
                    {
                        buffer = next(state);
                        count = 8;
                    }

                    BYTE result = static_cast<BYTE>(buffer & 0xff);
                    buffer >>= 8;
                    --count;

                    return result;
                }

                void write_qword(std::string& output, QWORD value)
                {
                    for (unsigned i = 0; i < 8; ++i)
                    {
                        output.push_back(static_cast<char>((value >> (i * 8)) & 0xff));
                    }
                }

                QWORD read_qword(const std::string& input, std::size_t offset)
                {
                    QWORD value = 0;

                    for (unsigned i = 0; i < 8; ++i)
                    {
                        value |= static_cast<QWORD>(static_cast<BYTE>(input[offset + i])) << (i * 8);
                    }

                    return value;
                }
            }

            std::string encode(const std::string& input, const std::string& key)
            {
                if (key.empty())
                    throw platform_core::ecc{ 4, 20 };

                std::random_device rd;
                QWORD nonce = (static_cast<QWORD>(rd()) << 32) ^ static_cast<QWORD>(rd());
                nonce = detail::mix(nonce);

                QWORD state = detail::init_state(key, nonce);
                QWORD buffer = 0;
                unsigned count = 0;

                std::string output;
                output.reserve(8 + input.size());
                detail::write_qword(output, nonce);

                for (std::size_t i = 0; i < input.size(); ++i)
                {
                    const BYTE stream = detail::next_byte(state, buffer, count);
                    const BYTE a = static_cast<BYTE>(i * 0x37u);
                    const BYTE b = static_cast<BYTE>((i >> 8) ^ (i >> 16));
                    const BYTE c = static_cast<BYTE>(state >> ((i & 7) * 8));

                    BYTE x = static_cast<BYTE>(input[i]);
                    x ^= stream;
                    x ^= a;
                    x ^= b;
                    x ^= c;

                    output.push_back(static_cast<char>(x));
                }

                return output;
            }

            std::string decode(const std::string& input, const std::string& key)
            {
                if (key.empty())
                    throw platform_core::ecc{ 4, 20 };

                if (input.size() < 8)
                    throw platform_core::ecc{ 4, 21 };

                const QWORD nonce = detail::read_qword(input, 0);
                QWORD state = detail::init_state(key, nonce);
                QWORD buffer = 0;
                unsigned count = 0;

                std::string output;
                output.resize(input.size() - 8);

                for (std::size_t i = 0; i < output.size(); ++i)
                {
                    const BYTE stream = detail::next_byte(state, buffer, count);
                    const BYTE a = static_cast<BYTE>(i * 0x37u);
                    const BYTE b = static_cast<BYTE>((i >> 8) ^ (i >> 16));
                    const BYTE c = static_cast<BYTE>(state >> ((i & 7) * 8));

                    BYTE x = static_cast<BYTE>(input[i + 8]);
                    x ^= c;
                    x ^= b;
                    x ^= a;
                    x ^= stream;

                    output[i] = static_cast<char>(x);
                }

                return output;
            }
        }
    }
}

namespace std {

    template <>
    struct hash<utilityX::coder::compact_ascii> {
        size_t operator()(const utilityX::coder::compact_ascii& value) const noexcept {
            const auto& r = value.raw();
            return std::hash<std::string_view>{}(std::string_view(r.data(), r.size()));
        }
    };

    template <>
    struct hash<utilityX::coder::compact_ascii_reference> {
        size_t operator()(const utilityX::coder::compact_ascii_reference& value) const noexcept {
            return std::hash<std::string_view>{}(value.raw_view());
        }
    };

    template <>
    struct hash<utilityX::coder::uuid> {
        size_t operator()(const utilityX::coder::uuid& value) const noexcept {
            return std::hash<std::string_view>{}(
                std::string_view(reinterpret_cast<const char*>(value.raw()), utilityX::coder::uuid::size())
                );
        }
    };

    template <>
    struct hash<utilityX::coder::uuid_reference> {
        size_t operator()(const utilityX::coder::uuid_reference& value) const noexcept {
            return std::hash<std::string_view>{}(value.raw_view());
        }
    };

}

namespace ankerl::unordered_dense {

    template <>
    struct hash<utilityX::coder::compact_ascii> {
        using is_transparent = void;

        size_t operator()(const utilityX::coder::compact_ascii& value) const noexcept {
            const auto& r = value.raw();
            return static_cast<size_t>(ankerl::unordered_dense::hash<std::string_view>{}(
                std::string_view(r.data(), r.size())
                )
                );
        }
    };

    template <>
    struct hash<utilityX::coder::compact_ascii_reference> {
        using is_transparent = void;

        size_t operator()(const utilityX::coder::compact_ascii_reference& value) const noexcept {
            return static_cast<size_t>(ankerl::unordered_dense::hash<std::string_view>{}(
                value.raw_view()
                )
                );
        }
    };

    template <>
    struct hash<utilityX::coder::uuid> {
        using is_transparent = void;

        size_t operator()(const utilityX::coder::uuid& value) const noexcept {
            return static_cast<size_t>(ankerl::unordered_dense::hash<std::string_view>{}(
                std::string_view(reinterpret_cast<const char*>(value.raw()), utilityX::coder::uuid::size())
                )
                );
        }
    };

    template <>
    struct hash<utilityX::coder::uuid_reference> {
        using is_transparent = void;

        size_t operator()(const utilityX::coder::uuid_reference& value) const noexcept {
            return static_cast<size_t>(ankerl::unordered_dense::hash<std::string_view>{}(
                value.raw_view()
                )
                );
        }
    };
}