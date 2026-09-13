/*
    *	System Library
    *
    *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - System Library
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

#pragma message(" *	System Library")
#pragma message(" *")
#pragma message(" *	Copyright (c) 2022 RANDOM ARMESE HITEMIT - System Library")
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

#include "../../Program Libraries/unordered_dense/include/ankerl/unordered_dense.h"

#include <array>
#include <atomic>
#include <chrono>
#include <concepts>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#if defined(_MSC_VER)
__pragma(pack(push, 8))
#endif

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#ifndef SIO_LOOPBACK_FAST_PATH
#define SIO_LOOPBACK_FAST_PATH 0x98000010
#endif

using QWORD = std::uint64_t;

using socket_t = SOCKET;
constexpr socket_t invalid_socket = INVALID_SOCKET;
constexpr int socket_error = SOCKET_ERROR;

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <signal.h>

using socket_t = int;
constexpr socket_t invalid_socket = -1;
constexpr int socket_error = -1;

using BYTE = std::uint8_t;
using WORD = std::uint16_t;
using DWORD = std::uint32_t;
using QWORD = std::uint64_t;
#endif

#if defined(_MSC_VER)
__pragma(pack(pop))
#endif

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
#endif

namespace platform_core {
    struct ecc {
        QWORD class_id = 0;
        QWORD error_code = 0;
    };
}

#ifndef NO_CONNECTOR
#include "../utilityX/utilityX.h"

namespace platform_core {
    PACK_PUSH_1
        enum class VALUE_TYPE : BYTE {
        DATA = 0,
        BYTE = 1,
        WORD = 2,
        DWORD = 3,
        QWORD = 4,
        ECC = 5
    };

    class connector;

    namespace connector_detail {
        template<class T>
        struct value_type_of;

        template<>
        struct value_type_of<std::string> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::DATA;
        };

        template<>
        struct value_type_of<BYTE> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::BYTE;
        };

        template<>
        struct value_type_of<WORD> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::WORD;
        };

        template<>
        struct value_type_of<DWORD> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::DWORD;
        };

        template<>
        struct value_type_of<QWORD> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::QWORD;
        };

        template<>
        struct value_type_of<ecc> {
            static constexpr VALUE_TYPE value = VALUE_TYPE::ECC;
        };

        template<class T>
        inline constexpr VALUE_TYPE value_type_of_v = value_type_of<std::decay_t<T>>::value;

#ifdef _WIN32
        using socket_t = SOCKET;
#else
        using socket_t = int;
#endif

        enum class frame_kind : BYTE {
            HELLO = 1,
            HELLO_OK = 2,
            HELLO_ERROR = 3,
            WRITE = 4,
            NOTIFY = 5,
            DISCONNECT = 6
        };

        struct frame {
            static constexpr size_t SBO_CAP = 16;

            frame_kind kind = frame_kind::DISCONNECT;
            VALUE_TYPE type = VALUE_TYPE::DATA;
            QWORD code = 0;
            uint32_t size = 0;

            BYTE sbo[SBO_CAP]{};
            std::unique_ptr<BYTE[]> heap;

            frame() noexcept = default;

            ~frame() = default;

            frame(frame&& other) noexcept
                : kind(other.kind),
                type(other.type),
                code(other.code),
                size(other.size),
                heap(std::move(other.heap))
            {
                if (other.size <= SBO_CAP) {
                    std::memcpy(sbo, other.sbo, other.size);
                    heap.reset();
                }

                other.size = 0;
            }

            frame& operator=(frame&& other) noexcept {
                if (this != &other) {
                    kind = other.kind;
                    type = other.type;
                    code = other.code;
                    size = other.size;

                    heap = std::move(other.heap);

                    if (other.size <= SBO_CAP) {
                        std::memcpy(sbo, other.sbo, other.size);
                        heap.reset();
                    }

                    other.size = 0;
                }

                return *this;
            }

            frame(const frame&) = delete;
            frame& operator=(const frame&) = delete;

            void reset() noexcept {
                heap.reset();
                size = 0;
            }

            void allocate(uint32_t sz) {
                reset();
                size = sz;

                if (sz > SBO_CAP) {
                    heap = std::make_unique<BYTE[]>(sz);
                }
            }

            BYTE* data() noexcept {
                return size <= SBO_CAP ? sbo : heap.get();
            }

            const BYTE* data() const noexcept {
                return size <= SBO_CAP ? sbo : heap.get();
            }
        };

        [[noreturn]] inline void fail(QWORD class_id, QWORD error_code) {
            throw ecc{ class_id, error_code };
        }

        [[noreturn]] inline void fail(const ecc& e) {
            throw e;
        }

        inline socket_t invalid_socket() noexcept {
#ifdef _WIN32
            return INVALID_SOCKET;
#else
            return -1;
#endif
        }

        inline bool socket_invalid(socket_t s) noexcept {
#ifdef _WIN32
            return s == INVALID_SOCKET;
#else
            return s < 0;
#endif
        }

        inline void socket_close(socket_t s) noexcept {
            if (socket_invalid(s)) return;
#ifdef _WIN32
            ::closesocket(s);
#else
            ::close(s);
#endif
        }

        inline void socket_shutdown(socket_t s) noexcept {
            if (socket_invalid(s)) return;
#ifdef _WIN32
            ::shutdown(s, SD_BOTH);
#else
            ::shutdown(s, SHUT_RDWR);
#endif
        }

        struct runtime {
            runtime() {
#ifdef _WIN32
                WSADATA wsa{};
                if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0) fail(0, 0);
#else
                ::signal(SIGPIPE, SIG_IGN);
#endif
            }
            ~runtime() {
#ifdef _WIN32
                ::WSACleanup();
#endif
            }
        };

        inline void ensure_runtime() {
            static runtime r;
            (void)r;
        }

        inline QWORD hton64(QWORD v) noexcept {
#if defined(_MSC_VER)
            return _byteswap_uint64(v);
#elif defined(__GNUC__) || defined(__clang__)
            return __builtin_bswap64(v);
#else
            uint32_t lo = static_cast<uint32_t>(v & 0xffffffffu);
            uint32_t hi = static_cast<uint32_t>(v >> 32);
            return (static_cast<QWORD>(::htonl(lo)) << 32) | ::htonl(hi);
#endif
        }

        inline QWORD ntoh64(QWORD v) noexcept {
            return hton64(v);
        }

        inline void configure_socket(socket_t s) noexcept {
            int yes = 1;
#ifdef _WIN32
            ::setsockopt(s, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&yes), sizeof(yes));
            DWORD bytes = 0;
            ::WSAIoctl(s, SIO_LOOPBACK_FAST_PATH, &yes, sizeof(yes), nullptr, 0, &bytes, nullptr, nullptr);
#else
            ::setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes));
#ifdef TCP_QUICKACK
            ::setsockopt(s, IPPROTO_TCP, TCP_QUICKACK, &yes, sizeof(yes));
#endif
#endif
        }

        inline void reuse_socket(socket_t s) noexcept {
            int yes = 1;
#ifdef _WIN32
            ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&yes), sizeof(yes));
#else
            ::setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
#ifdef SO_REUSEPORT
            ::setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
#endif
#endif
        }

        inline bool send_all(socket_t s, const BYTE* p, size_t n) noexcept {
            while (n > 0) {
#ifdef _WIN32
                int chunk = static_cast<int>(n > 1048576 ? 1048576 : n);
                int sent = ::send(s, reinterpret_cast<const char*>(p), chunk, 0);
#else
                size_t chunk = n > 1048576 ? 1048576 : n;
#ifdef MSG_NOSIGNAL
                ssize_t sent = ::send(s, p, chunk, MSG_NOSIGNAL);
#else
                ssize_t sent = ::send(s, p, chunk, 0);
#endif
#endif
                if (sent <= 0) return false;
                p += sent;
                n -= static_cast<size_t>(sent);
            }
            return true;
        }

        inline bool recv_all(socket_t s, BYTE* p, size_t n) noexcept {
            while (n > 0) {
#ifdef _WIN32
                int chunk = static_cast<int>(n > 1048576 ? 1048576 : n);
                int got = ::recv(s, reinterpret_cast<char*>(p), chunk, 0);
#else
                size_t chunk = n > 1048576 ? 1048576 : n;
                ssize_t got = ::recv(s, p, chunk, 0);
#endif
                if (got <= 0) return false;
                p += got;
                n -= static_cast<size_t>(got);
            }
            return true;
        }

        inline bool send_frame(socket_t s, frame_kind kind, VALUE_TYPE type, QWORD code, const void* payload = nullptr, size_t size = 0) {
            if (size > 64u * 1024u * 1024u) fail(0, 2);

            if (size <= 44) {
                BYTE packet[64];
                uint32_t mg = ::htonl(0x43495043u);
                std::memcpy(packet, &mg, 4);
                packet[4] = 1;
                packet[5] = static_cast<BYTE>(kind);
                packet[6] = static_cast<BYTE>(type);
                packet[7] = 0;
                QWORD c = hton64(code);
                std::memcpy(packet + 8, &c, 8);
                uint32_t sz = ::htonl(static_cast<uint32_t>(size));
                std::memcpy(packet + 16, &sz, 4);
                if (size > 0 && payload) {
                    std::memcpy(packet + 20, payload, size);
                }
                return send_all(s, packet, 20 + size);
            }

            BYTE h[20];
            uint32_t mg = ::htonl(0x43495043u);
            std::memcpy(h, &mg, 4);
            h[4] = 1;
            h[5] = static_cast<BYTE>(kind);
            h[6] = static_cast<BYTE>(type);
            h[7] = 0;
            QWORD c = hton64(code);
            std::memcpy(h + 8, &c, 8);
            uint32_t sz = ::htonl(static_cast<uint32_t>(size));
            std::memcpy(h + 16, &sz, 4);
            if (!send_all(s, h, 20)) return false;
            return send_all(s, static_cast<const BYTE*>(payload), size);
        }

        inline frame recv_frame(socket_t s) {
            BYTE raw[20];
            if (!recv_all(s, raw, 20)) fail(0, 3);
            uint32_t mg;
            std::memcpy(&mg, raw, 4);
            if (::ntohl(mg) != 0x43495043u) fail(0, 4);
            if (raw[4] != 1) fail(0, 5);
            uint32_t size;
            std::memcpy(&size, raw + 16, 4);
            size = ::ntohl(size);
            if (size > 64u * 1024u * 1024u) fail(0, 2);

            frame f;
            f.kind = static_cast<frame_kind>(raw[5]);
            f.type = static_cast<VALUE_TYPE>(raw[6]);
            QWORD code;
            std::memcpy(&code, raw + 8, 8);
            f.code = ntoh64(code);
            f.allocate(size);
            if (size && !recv_all(s, f.data(), size)) fail(0, 3);
            return f;
        }

        inline std::vector<BYTE> encode_endpoint(const std::string& ip, WORD port) {
            if (ip.size() > 65535) fail(0, 6);
            std::vector<BYTE> b(ip.size() + 4);
            uint16_t len = ::htons(static_cast<uint16_t>(ip.size()));
            std::memcpy(b.data(), &len, 2);
            if (!ip.empty()) std::memcpy(b.data() + 2, ip.data(), ip.size());
            uint16_t p = ::htons(port);
            std::memcpy(b.data() + 2 + ip.size(), &p, 2);
            return b;
        }

        inline std::pair<std::string, WORD> decode_endpoint(const frame& f) {
            if (f.size < 4) fail(0, 7);
            uint16_t len;
            std::memcpy(&len, f.data(), 2);
            len = ::ntohs(len);
            if (static_cast<size_t>(len) + 4 != f.size) fail(0, 7);
            std::string ip(reinterpret_cast<const char*>(f.data() + 2), len);
            uint16_t port;
            std::memcpy(&port, f.data() + 2 + len, 2);
            return { std::move(ip), static_cast<WORD>(::ntohs(port)) };
        }

        inline ecc decode_ecc(const frame& f) {
            if (f.size != 16) fail(0, 8);
            ecc e;
            QWORD c, ec;
            std::memcpy(&c, f.data(), 8);
            std::memcpy(&ec, f.data() + 8, 8);
            e.class_id = ntoh64(c);
            e.error_code = ntoh64(ec);
            return e;
        }

        template<VALUE_TYPE type>
        inline auto decode_value(const frame& f) {
            const BYTE* ptr = f.data();
            size_t sz = f.size;
            if constexpr (type == VALUE_TYPE::DATA) {
                return std::string(reinterpret_cast<const char*>(ptr), sz);
            }
            else if constexpr (type == VALUE_TYPE::BYTE) {
                if (sz != 1) fail(0, 8);
                return ptr[0];
            }
            else if constexpr (type == VALUE_TYPE::WORD) {
                if (sz != 2) fail(0, 8);
                uint16_t n;
                std::memcpy(&n, ptr, 2);
                return static_cast<WORD>(::ntohs(n));
            }
            else if constexpr (type == VALUE_TYPE::DWORD) {
                if (sz != 4) fail(0, 8);
                uint32_t n;
                std::memcpy(&n, ptr, 4);
                return static_cast<DWORD>(::ntohl(n));
            }
            else if constexpr (type == VALUE_TYPE::QWORD) {
                if (sz != 8) fail(0, 8);
                QWORD n;
                std::memcpy(&n, ptr, 8);
                return ntoh64(n);
            }
            else if constexpr (type == VALUE_TYPE::ECC) {
                if (sz != 16) fail(0, 8);
                ecc value;
                QWORD c, e;
                std::memcpy(&c, ptr, 8);
                std::memcpy(&e, ptr + 8, 8);
                value.class_id = ntoh64(c);
                value.error_code = ntoh64(e);
                return value;
            }
            else {
                fail(0, 8);
            }
        }

        inline uint32_t ip_to_addr(const std::string& ip) {
#ifdef _WIN32
            IN_ADDR a{};
            if (::InetPtonA(AF_INET, ip.c_str(), &a) != 1) fail(0, 9);
            return static_cast<uint32_t>(a.S_un.S_addr);
#else
            in_addr a{};
            if (::inet_pton(AF_INET, ip.c_str(), &a) != 1) fail(0, 9);
            return static_cast<uint32_t>(a.s_addr);
#endif
        }

        inline WORD runtime_port(const std::string&, WORD port) noexcept {
            return (port < 1024) ? static_cast<WORD>(41000 + port) : port;
        }

        inline socket_t open_server(const std::string& ip, WORD port) {
            ensure_runtime();
            socket_t s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (socket_invalid(s)) fail(0, 10);
            reuse_socket(s);
            configure_socket(s);
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = ::htons(static_cast<uint16_t>(runtime_port(ip, port)));
            addr.sin_addr.s_addr = ip.empty() ? 0u : ip_to_addr(ip);
            if (::bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
                socket_close(s);
                fail(0, 11);
            }
            if (::listen(s, SOMAXCONN) != 0) {
                socket_close(s);
                fail(0, 12);
            }
            return s;
        }

        inline socket_t open_client(const std::string& ip, WORD port) {
            ensure_runtime();
            socket_t s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (socket_invalid(s)) fail(0, 10);
            configure_socket(s);
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = ::htons(static_cast<uint16_t>(runtime_port(ip, port)));
            addr.sin_addr.s_addr = ip_to_addr(ip);
            if (::connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
                socket_close(s);
                fail(0, 13);
            }
            return s;
        }

        struct state {
            std::string ip;
            WORD port = 0;
            socket_t server = invalid_socket();
            socket_t sock = invalid_socket();
            std::thread accept_thread;
            std::thread recv_thread;
            std::mutex mx;
            std::mutex send_mx;
            std::condition_variable read_cv;
            std::condition_variable life_cv;
            bool server_running = false;
            bool connected = false;
            bool connecting = false;
            bool closed = false;
            std::string peer_ip;
            WORD peer_port = 0;
            ankerl::unordered_dense::map<QWORD, std::unique_ptr<std::deque<frame>>> inbox;
            ankerl::unordered_dense::map<QWORD, QWORD> notifications;
            std::optional<ecc> async_error;
        };

        inline std::mutex& registry_mutex() noexcept {
            static std::mutex m;
            return m;
        }

        inline ankerl::unordered_dense::map<std::string, std::weak_ptr<state>>& registry() noexcept {
            static ankerl::unordered_dense::map<std::string, std::weak_ptr<state>> r;
            return r;
        }

        inline std::string key_of(const std::string& ip, WORD port) {
            return ip + ":" + std::to_string(port);
        }

        //inline void join_thread(std::thread& t) noexcept {
        //    if (!t.joinable()) return;
        //    if (t.get_id() == std::this_thread::get_id()) {
        //        t.detach();
        //        return;
        //    }
        //    t.join();
        //}
        inline void join_thread(std::thread& t) noexcept {
            if (t.joinable()) {
                t.detach();
            }
        }

        inline void set_async_error(const std::shared_ptr<state>& s, const ecc& e) noexcept {
            {
                std::lock_guard<std::mutex> lock(s->mx);
                s->async_error = e;
            }
            s->read_cv.notify_all();
            s->life_cv.notify_all();
        }

        inline void mark_down(const std::shared_ptr<state>& s, socket_t x) noexcept {
            bool own = false;
            {
                std::lock_guard<std::mutex> lock(s->mx);
                if (s->sock == x) {
                    s->sock = invalid_socket();
                    s->connected = false;
                    s->connecting = false;
                    s->peer_ip.clear();
                    s->peer_port = 0;
                    s->async_error = ecc{ 0, 18 };
                    own = true;
                }
            }
            if (own) {
                socket_shutdown(x);
                socket_close(x);
                s->read_cv.notify_all();
                s->life_cv.notify_all();
            }
        }

        inline void recv_loop(std::shared_ptr<state> s, socket_t x) {
            for (;;) {
                try {
                    frame f = recv_frame(x);
                    if (f.kind == frame_kind::DISCONNECT) break;
                    if (f.kind == frame_kind::WRITE) {
                        {
                            std::lock_guard<std::mutex> lock(s->mx);
                            auto [it, inserted] = s->inbox.try_emplace(f.code, std::make_unique<std::deque<frame>>());
                            it->second->emplace_back(std::move(f));
                        }
                        s->read_cv.notify_one();
                        continue;
                    }
                    if (f.kind == frame_kind::NOTIFY) {
                        {
                            std::lock_guard<std::mutex> lock(s->mx);
                            ++s->notifications[f.code];
                        }
                        s->life_cv.notify_all();
                        continue;
                    }
                    break;
                }
                catch (const ecc& e) {
                    set_async_error(s, e);
                    break;
                }
                catch (...) {
                    set_async_error(s, ecc{ 0, 3 });
                    break;
                }
            }
            mark_down(s, x);
        }

        inline void start_recv_thread(const std::shared_ptr<state>& s, socket_t x) {
            join_thread(s->recv_thread);
            s->recv_thread = std::thread([s, x]() { recv_loop(s, x); });
        }

        inline void accept_loop(std::shared_ptr<state> s) {
            for (;;) {
                socket_t srv = invalid_socket();
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    if (!s->server_running || socket_invalid(s->server)) break;
                    srv = s->server;
                }

                sockaddr_in addr{};
#ifdef _WIN32
                int addr_len = sizeof(addr);
#else
                socklen_t addr_len = sizeof(addr);
#endif
                socket_t client = ::accept(srv, reinterpret_cast<sockaddr*>(&addr), &addr_len);
                if (socket_invalid(client)) {
                    bool run = false;
                    {
                        std::lock_guard<std::mutex> lock(s->mx);
                        run = s->server_running && !socket_invalid(s->server);
                    }
                    if (!run) break;
                    continue;
                }

                configure_socket(client);

                try {
                    frame hello = recv_frame(client);
                    if (hello.kind != frame_kind::HELLO) {
                        socket_shutdown(client);
                        socket_close(client);
                        continue;
                    }

                    auto peer = decode_endpoint(hello);
                    bool busy = false;
                    {
                        std::lock_guard<std::mutex> lock(s->mx);
                        busy = s->closed || !s->server_running || s->connected || s->connecting || !socket_invalid(s->sock);
                    }

                    if (busy) {
                        QWORD err[2] = { hton64(0), hton64(17) };
                        send_frame(client, frame_kind::HELLO_ERROR, VALUE_TYPE::ECC, 0, err, 16);
                        socket_shutdown(client);
                        socket_close(client);
                        continue;
                    }

                    std::vector<BYTE> ep = encode_endpoint(s->ip, s->port);
                    if (!send_frame(client, frame_kind::HELLO_OK, VALUE_TYPE::DATA, 0, ep.data(), ep.size())) {
                        socket_shutdown(client);
                        socket_close(client);
                        continue;
                    }

                    bool re_busy = false;
                    {
                        std::lock_guard<std::mutex> lock(s->mx);
                        if (s->closed || !s->server_running || s->connected || !socket_invalid(s->sock)) {
                            re_busy = true;
                        }
                        else {
                            s->sock = client;
                            s->connected = true;
                            s->connecting = false;
                            s->peer_ip = std::move(peer.first);
                            s->peer_port = peer.second;
                            s->async_error.reset();
                        }
                    }

                    if (re_busy) {
                        QWORD err[2] = { hton64(0), hton64(17) };
                        send_frame(client, frame_kind::HELLO_ERROR, VALUE_TYPE::ECC, 0, err, 16);
                        socket_shutdown(client);
                        socket_close(client);
                        continue;
                    }

                    s->life_cv.notify_all();
                    start_recv_thread(s, client);
                }
                catch (const ecc& e) {
                    set_async_error(s, e);
                    socket_shutdown(client);
                    socket_close(client);
                }
                catch (...) {
                    set_async_error(s, ecc{ 0, 13 });
                    socket_shutdown(client);
                    socket_close(client);
                }
            }
        }
    }

    class connector {
    private:
        std::shared_ptr<connector_detail::state> st;

        explicit connector(std::shared_ptr<connector_detail::state> s) noexcept : st(std::move(s)) {}

        std::shared_ptr<connector_detail::state> require_state(QWORD code) const {
            auto s = st;
            if (!s) connector_detail::fail(0, code);
            return s;
        }

        connector_detail::socket_t require_socket(const std::shared_ptr<connector_detail::state>& s, QWORD code) const {
            std::lock_guard<std::mutex> lock(s->mx);
            if (s->closed || !s->connected || connector_detail::socket_invalid(s->sock)) {
                connector_detail::fail(0, code);
            }
            return s->sock;
        }

        void send_checked(connector_detail::frame_kind kind, VALUE_TYPE type, QWORD code, const void* payload, size_t size, QWORD err_code) {
            auto s = require_state(15);
            std::lock_guard<std::mutex> send_lock(s->send_mx);
            connector_detail::socket_t x = require_socket(s, err_code);
            if (!connector_detail::send_frame(x, kind, type, code, payload, size)) {
                connector_detail::mark_down(s, x);
                connector_detail::fail(0, 13);
            }
        }

    public:
        connector() = default;

        static connector begin(const std::string& ip, WORD port) {
            std::string k = connector_detail::key_of(ip, port);
            std::lock_guard<std::mutex> reg_lock(connector_detail::registry_mutex());
            auto& r = connector_detail::registry();
            auto it = r.find(k);
            if (it != r.end()) {
                if (auto old = it->second.lock()) {
                    std::lock_guard<std::mutex> old_lock(old->mx);
                    if (!old->closed && old->server_running && !connector_detail::socket_invalid(old->server)) {
                        connector_detail::fail(0, 11);
                    }
                }
                r.erase(it);
            }
            auto s = std::make_shared<connector_detail::state>();
            s->ip = ip;
            s->port = port;
            s->server = connector_detail::open_server(ip, port);
            s->server_running = true;
            s->connected = false;
            s->connecting = false;
            s->closed = false;
            s->accept_thread = std::thread([s]() { connector_detail::accept_loop(s); });
            r[std::move(k)] = s;
            return connector(s);
        }

        connector& connect(const std::string& ip, WORD port) {
            auto s = require_state(15);
            {
                std::lock_guard<std::mutex> lock(s->mx);
                if (s->closed || !s->server_running || connector_detail::socket_invalid(s->server)) {
                    connector_detail::fail(0, 16);
                }
                if (s->connected || s->connecting || !connector_detail::socket_invalid(s->sock)) {
                    connector_detail::fail(0, 17);
                }
                s->connecting = true;
                s->async_error.reset();
            }

            connector_detail::socket_t x = connector_detail::invalid_socket();
            try {
                x = connector_detail::open_client(ip, port);
                std::vector<BYTE> ep = connector_detail::encode_endpoint(s->ip, s->port);
                if (!connector_detail::send_frame(x, connector_detail::frame_kind::HELLO, VALUE_TYPE::DATA, 0, ep.data(), ep.size())) {
                    connector_detail::fail(0, 13);
                }

                connector_detail::frame reply = connector_detail::recv_frame(x);
                if (reply.kind == connector_detail::frame_kind::HELLO_ERROR) {
                    if (reply.type == VALUE_TYPE::ECC) {
                        connector_detail::fail(connector_detail::decode_ecc(reply));
                    }
                    connector_detail::fail(0, 13);
                }
                if (reply.kind != connector_detail::frame_kind::HELLO_OK) {
                    connector_detail::fail(0, 13);
                }

                auto peer = connector_detail::decode_endpoint(reply);
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    if (s->closed || !s->server_running) {
                        connector_detail::fail(0, 16);
                    }
                    if (s->connected || !connector_detail::socket_invalid(s->sock)) {
                        connector_detail::fail(0, 17);
                    }
                    s->sock = x;
                    s->connected = true;
                    s->connecting = false;
                    s->peer_ip = std::move(peer.first);
                    s->peer_port = peer.second;
                    s->async_error.reset();
                }

                s->life_cv.notify_all();
                connector_detail::start_recv_thread(s, x);
                return *this;
            }
            catch (const ecc&) {
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    s->connecting = false;
                    if (s->sock == x) {
                        s->sock = connector_detail::invalid_socket();
                        s->connected = false;
                    }
                }
                connector_detail::socket_shutdown(x);
                connector_detail::socket_close(x);
                s->read_cv.notify_all();
                s->life_cv.notify_all();
                throw;
            }
            catch (...) {
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    s->connecting = false;
                    if (s->sock == x) {
                        s->sock = connector_detail::invalid_socket();
                        s->connected = false;
                    }
                }
                connector_detail::socket_shutdown(x);
                connector_detail::socket_close(x);
                s->read_cv.notify_all();
                s->life_cv.notify_all();
                connector_detail::fail(0, 13);
            }
        }

        void end_connect() {
            auto s = require_state(15);
            connector_detail::socket_t x = connector_detail::invalid_socket();
            {
                std::lock_guard<std::mutex> send_lock(s->send_mx);
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    if (!s->connected || connector_detail::socket_invalid(s->sock)) return;
                    x = s->sock;
                    s->sock = connector_detail::invalid_socket();
                    s->connected = false;
                    s->connecting = false;
                    s->peer_ip.clear();
                    s->peer_port = 0;
                    s->async_error.reset();
                }
                //connector_detail::send_frame(x, connector_detail::frame_kind::DISCONNECT, VALUE_TYPE::DATA, 0, nullptr, 0);
                connector_detail::socket_shutdown(x);
                connector_detail::socket_close(x);
            }
            s->read_cv.notify_all();
            s->life_cv.notify_all();
            connector_detail::join_thread(s->recv_thread);
        }

        void disconnect() {
            end_connect();
        }

        void end() {
            auto s = st;
            if (!s) return;
            connector_detail::socket_t srv = connector_detail::invalid_socket();
            connector_detail::socket_t x = connector_detail::invalid_socket();
            {
                std::lock_guard<std::mutex> send_lock(s->send_mx);
                {
                    std::lock_guard<std::mutex> lock(s->mx);
                    if (s->closed) {
                        st.reset();
                        return;
                    }
                    s->closed = true;
                    s->server_running = false;
                    s->connecting = false;
                    srv = s->server;
                    x = s->sock;
                    s->server = connector_detail::invalid_socket();
                    s->sock = connector_detail::invalid_socket();
                    s->connected = false;
                    s->peer_ip.clear();
                    s->peer_port = 0;
                    s->async_error.reset();
                }
                //if (!connector_detail::socket_invalid(x)) {
                //    connector_detail::send_frame(x, connector_detail::frame_kind::DISCONNECT, VALUE_TYPE::DATA, 0, nullptr, 0);
                //}
                connector_detail::socket_shutdown(x);
                connector_detail::socket_close(x);
                connector_detail::socket_shutdown(srv);
                connector_detail::socket_close(srv);
            }
            s->read_cv.notify_all();
            s->life_cv.notify_all();
            connector_detail::join_thread(s->accept_thread);
            connector_detail::join_thread(s->recv_thread);
            {
                std::lock_guard<std::mutex> reg_lock(connector_detail::registry_mutex());
                auto& r = connector_detail::registry();
                auto it = r.find(connector_detail::key_of(s->ip, s->port));
                if (it != r.end()) {
                    if (auto p = it->second.lock()) {
                        if (p == s) r.erase(it);
                    }
                    else {
                        r.erase(it);
                    }
                }
            }
            st.reset();
        }

        template<VALUE_TYPE type, class T>
        void write(QWORD to, const T& data) {
            if constexpr (type == VALUE_TYPE::DATA) {
                std::string_view s(data);
                send_checked(connector_detail::frame_kind::WRITE, type, to, s.data(), s.size(), 18);
            }
            else if constexpr (type == VALUE_TYPE::BYTE) {
                BYTE v = static_cast<BYTE>(data);
                send_checked(connector_detail::frame_kind::WRITE, type, to, &v, 1, 18);
            }
            else if constexpr (type == VALUE_TYPE::WORD) {
                uint16_t v = ::htons(static_cast<uint16_t>(data));
                send_checked(connector_detail::frame_kind::WRITE, type, to, &v, 2, 18);
            }
            else if constexpr (type == VALUE_TYPE::DWORD) {
                uint32_t v = ::htonl(static_cast<uint32_t>(data));
                send_checked(connector_detail::frame_kind::WRITE, type, to, &v, 4, 18);
            }
            else if constexpr (type == VALUE_TYPE::QWORD) {
                QWORD v = connector_detail::hton64(static_cast<QWORD>(data));
                send_checked(connector_detail::frame_kind::WRITE, type, to, &v, 8, 18);
            }
            else if constexpr (type == VALUE_TYPE::ECC) {
                QWORD buf[2] = { connector_detail::hton64(data.class_id), connector_detail::hton64(data.error_code) };
                send_checked(connector_detail::frame_kind::WRITE, type, to, buf, 16, 18);
            }
            else {
                connector_detail::fail(0, 8);
            }
        }

        template<VALUE_TYPE type>
        auto read(QWORD to) {
            auto s = require_state(15);
            std::unique_lock<std::mutex> lock(s->mx);
            for (;;) {
                if (s->closed) connector_detail::fail(0, 18);
                auto it = s->inbox.find(to);

                if (it != s->inbox.end() && !it->second->empty()) {
                    connector_detail::frame frame = std::move(it->second->front());
                    it->second->pop_front();
                    if (it->second->empty()) {
                        s->inbox.erase(it);
                    }
                    lock.unlock();
                    if (frame.type != type) {
                        connector_detail::fail(0, 8);
                    }
                    return connector_detail::decode_value<type>(frame);
                }
                if (s->async_error.has_value()) {
                    ecc e = *s->async_error;
                    s->async_error.reset();
                    connector_detail::fail(e);
                }
                if (!s->connected || connector_detail::socket_invalid(s->sock)) {
                    connector_detail::fail(0, 18);
                }
                s->read_cv.wait(lock);
            }
        }

        void notify(QWORD code) {
            send_checked(connector_detail::frame_kind::NOTIFY, VALUE_TYPE::DATA, code, nullptr, 0, 18);
        }
        void listener(QWORD code, std::atomic_bool* failsafe) {
            auto s = require_state(15);

            auto should_exit = [&]() noexcept -> bool {
                return failsafe &&
                    failsafe->load(std::memory_order_relaxed);
                };

            auto has_connection = [&]() noexcept -> bool {
                return s->connected &&
                    !connector_detail::socket_invalid(s->sock);
                };

            auto has_notification = [&]() noexcept -> bool {
                auto it = s->notifications.find(code);
                return it != s->notifications.end() && it->second > 0;
                };

            std::unique_lock<std::mutex> lock(s->mx);

            bool last_connection = false;

            for (;;) {
                if (should_exit()) {
                    break;
                }

                if (s->closed) {
                    connector_detail::fail(0, 18);
                }
                if (s->async_error.has_value()) {
                    ecc e = *s->async_error;
                    s->async_error.reset();

                    if (e.class_id != 0 || e.error_code != 18) {
                        lock.unlock();
                        connector_detail::fail(e);
                    }
                }

                const bool connected = has_connection();
                const bool notified = has_notification();

                if (connected && notified) {
                    auto it = s->notifications.find(code);

                    if (it != s->notifications.end()) {
                        if (--it->second == 0) {
                            s->notifications.erase(it);
                        }
                    }

                    return;
                }
                last_connection = connected;

                if (failsafe) {
                    s->life_cv.wait_for(
                        lock,
                        std::chrono::milliseconds(250),
                        [&]() noexcept {
                            return should_exit() ||
                                s->closed ||
                                s->async_error.has_value() ||
                                has_notification() ||
                                has_connection() != last_connection;
                        }
                    );
                }
                else {
                    s->life_cv.wait(
                        lock,
                        [&]() noexcept {
                            return s->closed ||
                                s->async_error.has_value() ||
                                has_notification() ||
                                has_connection() != last_connection;
                        }
                    );
                }
            }
        }

        void listener(QWORD code, std::atomic_bool& failsafe) {
            listener(code, &failsafe);
        }

        void listener(QWORD code) {
            listener(code, nullptr);
        }

        bool is_connected() const {
            auto s = st;
            if (!s) return false;
            std::lock_guard<std::mutex> lock(s->mx);
            return !s->closed && s->connected && !connector_detail::socket_invalid(s->sock);
        }

        std::pair<std::string, WORD> self_endpoint() const {
            auto s = require_state(15);
            std::lock_guard<std::mutex> lock(s->mx);
            return { s->ip, s->port };
        }

        std::pair<std::string, WORD> peer_endpoint() const {
            auto s = require_state(15);
            std::lock_guard<std::mutex> lock(s->mx);
            if (s->closed || !s->connected || connector_detail::socket_invalid(s->sock)) {
                connector_detail::fail(0, 18);
            }
            return { s->peer_ip, s->peer_port };
        }

        void clean(QWORD channel) {
            auto s = require_state(15);
            std::lock_guard<std::mutex> lock(s->mx);
            s->inbox.erase(channel);
            s->notifications.erase(channel);
        }

        void clear() {
            auto s = require_state(15);
            std::lock_guard<std::mutex> lock(s->mx);
            s->inbox.clear();
            s->notifications.clear();
        }

        static inline connector connector_begin(const std::string& ip, WORD port) {
            return connector::begin(ip, port);
        }

        static inline void connector_end(connector& c) {
            c.end();
        }

        static inline void connector_clean(connector& c, QWORD channel) {
            c.clean(channel);
        }

        static inline void connector_clear(connector& c) {
            c.clear();
        }
    };
}
PACK_POP
#endif

//Disable unuse scode
#if 0
#ifndef NO_RECORDER
/*
    port number identification:
        name   | host | client
    * recorder     0 <-> 1
    * client       x <-> x
*/

namespace platform_core {
    namespace recorder {
        constexpr const char* RECORDER_IP = "127.0.0.2";
        constexpr WORD RECORDER_PORT = 41422;

        enum class align : BYTE { left = 0, right = 1, center = 2 };

        struct log_format {
            align alignment = align::left;
            std::string services_name;
            std::string locate_file;
            std::string header_format = "";
            QWORD segment_count = 1;
            BYTE horizontal_margin = 4;
            BYTE vertical_margin = 0;
            std::vector<QWORD> segment_max_size;
        };

        enum class recorder_send_object : BYTE { BEGIN = 0, END = 1, SEND = 2, GET_SETTING = 3 };

        inline socket_t global_socket = invalid_socket;
        inline std::mutex global_mutex;
        inline QWORD count = 0;
        inline bool socket_initialized = false;

        inline void close_socket(socket_t s) noexcept {
            if (s == invalid_socket) return;
#ifdef _WIN32
            closesocket(s);
#else
            close(s);
#endif
        }

        inline void shutdown_socket(socket_t s) noexcept {
            if (s == invalid_socket) return;
#ifdef _WIN32
            shutdown(s, SD_BOTH);
#else
            shutdown(s, SHUT_RDWR);
#endif
        }

        inline void net_read(socket_t s, void* buffer, QWORD size) {
            char* ptr = static_cast<char*>(buffer);
            while (size != 0) {
                const int received = recv(s, ptr, static_cast<int>(size), 0);
                if (received == 0) throw platform_core::ecc{ 2, 31 };
                if (received < 0) throw platform_core::ecc{ 2, 23 };
                ptr += received;
                size -= static_cast<QWORD>(received);
            }
        }

        inline void net_write(socket_t s, const void* buffer, QWORD size) {
            const char* ptr = static_cast<const char*>(buffer);
            while (size != 0) {
                const int sent = send(s, ptr, static_cast<int>(size), 0);
                if (sent <= 0) throw platform_core::ecc{ 2, 24 };
                ptr += sent;
                size -= static_cast<QWORD>(sent);
            }
        }

        template<class T>
        inline T read_raw(socket_t s) {
            static_assert(std::is_trivially_copyable_v<T>);
            T value{};
            net_read(s, &value, sizeof(T));
            return value;
        }

        template<class T>
        inline void write_raw(socket_t s, const T& value) {
            static_assert(std::is_trivially_copyable_v<T>);
            net_write(s, &value, sizeof(T));
        }

        inline std::string read_string(socket_t s) {
            const QWORD length = read_raw<QWORD>(s);
            constexpr QWORD MAX_STRING = 64ull * 1024ull * 1024ull;
            if (length > MAX_STRING) throw platform_core::ecc{ 2, 34 };
            std::string result(static_cast<size_t>(length), '\0');
            if (length != 0) net_read(s, result.data(), length);
            return result;
        }

        inline void write_string(socket_t s, const std::string& value) {
            const QWORD length = static_cast<QWORD>(value.size());
            write_raw<QWORD>(s, length);
            if (length != 0) net_write(s, value.data(), length);
        }

        inline void read_error_or_ok(socket_t s) {
            const BYTE status = read_raw<BYTE>(s);
            if (status == 0) return;
            const QWORD class_id = read_raw<QWORD>(s);
            const QWORD error_code = read_raw<QWORD>(s);
            throw platform_core::ecc{ class_id, error_code };
        }

        inline socket_t connect_host() {
#ifdef _WIN32
            constexpr int MAX_ATTEMPTS = 1;
#else
            constexpr int MAX_ATTEMPTS = 100;
#endif
            for (int attempt = 0; attempt < MAX_ATTEMPTS; ++attempt) {
                socket_t socket_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (socket_handle == invalid_socket) throw platform_core::ecc{ 2, 23 };

                int no_delay = 1;
                setsockopt(socket_handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&no_delay), sizeof(no_delay));

                sockaddr_in address{};
                address.sin_family = AF_INET;
                if (inet_pton(AF_INET, RECORDER_IP, &address.sin_addr) != 1) {
                    close_socket(socket_handle);
                    throw platform_core::ecc{ 2, 23 };
                }
                address.sin_port = htons(RECORDER_PORT);

                if (connect(socket_handle, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != socket_error) {
                    return socket_handle;
                }
                close_socket(socket_handle);

#ifndef _WIN32
                if (attempt + 1 < MAX_ATTEMPTS) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
#endif
            }
            throw platform_core::ecc{ 2, 23 };
        }

        inline void ensure_connected() {
            if (global_socket != invalid_socket) return;
#ifdef _WIN32
            static std::once_flag wsa_once;
            std::call_once(wsa_once, [] {
                WSADATA data{};
                if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
                    throw platform_core::ecc{ 2, 23 };
                }
                });
#endif
            global_socket = connect_host();
            socket_initialized = true;
        }

        class record {
        public:
            socket_t socket_handle = invalid_socket;
            QWORD address = 0;
            std::mutex selfmutex;

            record(socket_t socket_input, QWORD address_input)
                : socket_handle(socket_input), address(address_input) {}
            record(const record&) = delete;
            record& operator=(const record&) = delete;

            void writes(const std::vector<std::string>& elements) {
                std::lock_guard<std::mutex> lock(selfmutex);
                if (socket_handle == invalid_socket) throw platform_core::ecc{ 0, 28 };

                write_raw<BYTE>(socket_handle, static_cast<BYTE>(recorder_send_object::SEND));
                write_raw<QWORD>(socket_handle, address);
                write_raw<QWORD>(socket_handle, static_cast<QWORD>(elements.size()));

                for (const std::string& value : elements) {
                    write_string(socket_handle, value);
                }
                read_error_or_ok(socket_handle);
            }

            log_format get_format() {
                std::lock_guard<std::mutex> lock(selfmutex);
                if (socket_handle == invalid_socket) throw platform_core::ecc{ 0, 28 };

                write_raw<BYTE>(socket_handle, static_cast<BYTE>(recorder_send_object::GET_SETTING));
                write_raw<QWORD>(socket_handle, address);

                const BYTE alignment = read_raw<BYTE>(socket_handle);
                if (alignment == 0xFF) {
                    const QWORD class_id = read_raw<QWORD>(socket_handle);
                    const QWORD error_code = read_raw<QWORD>(socket_handle);
                    throw platform_core::ecc{ class_id, error_code };
                }

                log_format result;
                result.alignment = static_cast<align>(alignment);
                result.services_name = read_string(socket_handle);
                result.locate_file = read_string(socket_handle);
                result.header_format = read_string(socket_handle);
                result.segment_count = read_raw<QWORD>(socket_handle);
                result.horizontal_margin = read_raw<BYTE>(socket_handle);
                result.vertical_margin = read_raw<BYTE>(socket_handle);

                const QWORD seg_count = read_raw<QWORD>(socket_handle);
                if (seg_count > 1024 * 1024) throw platform_core::ecc{ 2, 34 };

                result.segment_max_size.reserve(static_cast<size_t>(seg_count));
                for (QWORD i = 0; i < seg_count; ++i) {
                    result.segment_max_size.push_back(read_raw<QWORD>(socket_handle));
                }
                return result;
            }
        };

        inline record* recoding_log_begin(const log_format& format_inputs) {
            std::lock_guard<std::mutex> global_lock(global_mutex);
            try {
                ensure_connected();
                write_raw<BYTE>(global_socket, static_cast<BYTE>(recorder_send_object::BEGIN));
                write_raw<BYTE>(global_socket, static_cast<BYTE>(format_inputs.alignment));
                write_string(global_socket, format_inputs.services_name);
                write_string(global_socket, utilityX::normalize_path(format_inputs.locate_file));
                write_string(global_socket, format_inputs.header_format);
                write_raw<QWORD>(global_socket, format_inputs.segment_count);
                write_raw<BYTE>(global_socket, format_inputs.horizontal_margin);
                write_raw<BYTE>(global_socket, format_inputs.vertical_margin);
                write_raw<QWORD>(global_socket, static_cast<QWORD>(format_inputs.segment_max_size.size()));

                for (const QWORD value : format_inputs.segment_max_size) {
                    write_raw<QWORD>(global_socket, value);
                }

                const BYTE result_type = read_raw<BYTE>(global_socket);
                if (result_type == 0xFF) {
                    const QWORD class_id = read_raw<QWORD>(global_socket);
                    const QWORD error_code = read_raw<QWORD>(global_socket);
                    throw platform_core::ecc{ class_id, error_code };
                }
                if (result_type != 0x00) throw platform_core::ecc{ 2, 30 };

                const QWORD address = read_raw<QWORD>(global_socket);
                record* output = new record(global_socket, address);
                ++count;
                return output;
            }
            catch (...) {
                if (count == 0 && global_socket != invalid_socket) {
                    shutdown_socket(global_socket);
                    close_socket(global_socket);
                    global_socket = invalid_socket;
                    socket_initialized = false;
                }
                throw;
            }
        }

        inline void recording_log_end(record* object) {
            if (object == nullptr) throw platform_core::ecc{ 0, 22 };
            std::lock_guard<std::mutex> lock(object->selfmutex);

            if (object->socket_handle == invalid_socket) {
                delete object;
                throw platform_core::ecc{ 0, 28 };
            }

            try {
                write_raw<BYTE>(object->socket_handle, static_cast<BYTE>(recorder_send_object::END));
                write_raw<QWORD>(object->socket_handle, object->address);
                const BYTE status = read_raw<BYTE>(object->socket_handle);

                if (status == 0xFF) {
                    const QWORD class_id = read_raw<QWORD>(object->socket_handle);
                    const QWORD error_code = read_raw<QWORD>(object->socket_handle);
                    throw platform_core::ecc{ class_id, error_code };
                }
                if (status != 0) throw platform_core::ecc{ 0, 22 };

                delete object;
                if (count == 0) throw platform_core::ecc{ 0, 22 };

                --count;
                if (count == 0) {
                    shutdown_socket(global_socket);
                    close_socket(global_socket);
                    global_socket = invalid_socket;
                    socket_initialized = false;
                }
            }
            catch (...) {
                if (global_socket != invalid_socket) {
                    shutdown_socket(global_socket);
                    close_socket(global_socket);
                    global_socket = invalid_socket;
                    socket_initialized = false;
                }
                delete object;
                throw;
            }
        }
    }
}

#include "recorder-service-local.h"
#endif

#ifndef NO_ERROR_LOOKINGUP
#include "../format_json_processor_editor/format_json_processor_editor.h"

namespace platform_core {
    namespace error_correction_handle {
        struct ecc_reprocessed {
            std::string services;
            std::string id;
            std::string msg;
            std::vector<std::string> stack_call;
        };
        namespace SUPPORTED_FORMAT_VERSION {
            ecc_reprocessed LOAD_1_0_A(const platform_core::ecc& err, ::format_json::format_json* instruction_document) {
                try {
                    ::format_json::zone access = *instruction_document >> "components" >> "services_identification";
                    ::platform_core::error_correction_handle::ecc_reprocessed ecch;
                    goto execute_class;
                trigger_search_class:
                    {
                        auto& mems = access.get<::format_json::datatype::array_t>();
                        for (QWORD i = 0; i < mems.size(); i++) {
                            if ((mems[static_cast<size_t>(i)] >> "numerical" >> "dec").get<::format_json::datatype::int_t>() == err.class_id) {
                                access = access >> static_cast<size_t>(i);
                                ecch.services = (access >> "id").get<::format_json::datatype::string_t>();
                                goto pass_class;
                            }
                        }
                    }
                trigger_search_id:
                    {
                        auto& mems = access.get<::format_json::datatype::array_t>();
                        for (QWORD i = 0; i < mems.size(); i++) {
                            if ((mems[static_cast<size_t>(i)] >> "numerical" >> "dec").get<::format_json::datatype::int_t>() == err.error_code) {
                                access = access >> static_cast<size_t>(i);
                                ecch.id = (access >> "id").get<::format_json::datatype::string_t>();
                                ecch.msg = (access >> "msg").get<::format_json::datatype::string_t>();
                                const auto& mems = (access >> "stack_call").get<::format_json::datatype::array_t>();
                                for (const auto& mem : mems) ecch.stack_call.emplace_back(mem.get<::format_json::datatype::string_t>());
                                goto pass_id;
                            }
                        }
                    }
                execute_class:
                    {
                        try { access = access >> err.class_id; }
                        catch (...) { goto trigger_search_class; }
                        if ((access >> "numerical" >> "dec").get<::format_json::datatype::int_t>() != err.class_id) {
                            access = access <<= 1;
                            goto trigger_search_class; 
                        }
                        else { ecch.services = (access >> "id").get<::format_json::datatype::string_t>(); }
                    }
                pass_class:
                    {
                        {
                            bool first = false;
                            const auto& mems = (access >> "to").get<::format_json::datatype::array_t>();
                            for (const auto& mem : mems) {
                                if (first) {
                                    switch (mem.get_type()) {
                                    case ::format_json::datatype::int_t:
                                        access = access >> mem.get<::format_json::datatype::int_t>();
                                        break;
                                    case ::format_json::datatype::string_t:
                                        access = access >> mem.get<::format_json::datatype::string_t>();
                                        break;
                                    default:
                                        throw platform_core::ecc{ 0, 31 };
                                        break;
                                    }
                                    continue;
                                }
                                switch (mem.get_type()) {
                                case ::format_json::datatype::int_t:
                                    access = *instruction_document >> mem.get<::format_json::datatype::int_t>();
                                    break;
                                case ::format_json::datatype::string_t:
                                    access = *instruction_document >> mem.get<::format_json::datatype::string_t>();
                                    break;
                                default:
                                    throw platform_core::ecc{ 0, 31 };
                                    break;
                                }
                                first = true;
                            }
                        }
                        if ((access >> "description" >> "identifier").get<::format_json::datatype::string_t>() != ecch.services) throw ecc{0, 31};
                        access = access >> "components" >> "error_code_checking_and_correction";
                        try { access = access >> err.error_code; }
                        catch (...) { goto trigger_search_id; }
                        if ((access >> "numerical" >> "dec").get<::format_json::datatype::int_t>() != err.error_code) { 
                            access = access <<= 1;
                            goto trigger_search_id; 
                        }
                        else { 
                            ecch.id = (access >> "id").get<::format_json::datatype::string_t>(); 
                            ecch.msg = (access >> "msg").get<::format_json::datatype::string_t>();

                            const auto& mems = (access >> "stack_call").get<::format_json::datatype::array_t>();
                            for (const auto& mem : mems) ecch.stack_call.emplace_back(mem.get<::format_json::datatype::string_t>());
                        }
                    }
                pass_id:
                    return ecch;
                }
                catch (...) {
                    throw ::platform_core::ecc{ 0, 31 };
                }
            }
        }
        std::pair<::platform_core::ecc, ecc_reprocessed> looked_up_error(const ecc& error, const std::string& path_to_file = "../../Program Datas/Essentials/Formatted/error_checking_and_correction_instruction.json") {
            static const ::ankerl::unordered_dense::map<std::string, ecc_reprocessed(*)(const platform_core::ecc&, format_json::format_json*)> FUNCTIONS_POINTER_DEFINITION_FOR_DIFFERENT_FORMAT_VERSION = {
                { "1.0A",  ::platform_core::error_correction_handle::SUPPORTED_FORMAT_VERSION::LOAD_1_0_A }
            };
            ::format_json::format_json* ec = nullptr;
            try {
                ec = format_json::json_processor_begin(path_to_file);
            }
            catch (...) {
                throw platform_core::ecc{ 0, 30 };
            }
            {
                auto it = FUNCTIONS_POINTER_DEFINITION_FOR_DIFFERENT_FORMAT_VERSION.find(ec->get_format().format_version);
                if (it == FUNCTIONS_POINTER_DEFINITION_FOR_DIFFERENT_FORMAT_VERSION.end()) {
                    format_json::json_processor_end(ec);
                    throw platform_core::ecc{ 0, 31 };
                }
                std::pair<::platform_core::ecc, ecc_reprocessed> temp;
                try {
                    temp = std::pair<::platform_core::ecc, ecc_reprocessed>{ error, it->second(error, ec) };
                }
                catch (...) {
                    format_json::json_processor_end(ec);
                    throw;
                }
                format_json::json_processor_end(ec);
                return temp;
            }
        }
    }
}
#endif

#ifndef NO_UAC
#include "../registry_editor/registry_editor.h"
#include "../utilityX/utilityX.h"

namespace platform_core {
    namespace user_account_control {

        enum class priorities : BYTE {
            user = 0,
            administrator = 1
        };

        struct user_role {
            utilityX::coder::uuid user_uuid;
            std::string _internal_password;

            struct _name_proxy {
                const user_role* parent;
                operator std::string() const;
                friend std::ostream& operator<<(std::ostream& os, const _name_proxy& np) { return os << static_cast<std::string>(np); }
                bool operator==(const std::string& o) const { return static_cast<std::string>(*this) == o; }
                bool operator!=(const std::string& o) const { return static_cast<std::string>(*this) != o; }
            } user_display_name;

            struct _prio_proxy {
                const user_role* parent;
                operator priorities() const;
                bool operator==(priorities p) const { return static_cast<priorities>(*this) == p; }
                bool operator!=(priorities p) const { return static_cast<priorities>(*this) != p; }
            } user_priorities;

            user_role() { user_display_name.parent = this; user_priorities.parent = this; }
            user_role(const utilityX::coder::uuid& uuid, const std::string& pass = "") : user_uuid(uuid), _internal_password(pass) { user_display_name.parent = this; user_priorities.parent = this; }
            user_role(const user_role& other) : user_uuid(other.user_uuid), _internal_password(other._internal_password) { user_display_name.parent = this; user_priorities.parent = this; }
            user_role& operator=(const user_role& other) { user_uuid = other.user_uuid; _internal_password = other._internal_password; return *this; }
        };

        struct user_meta {
            user_role role;
            user_meta() = default;
            user_meta(const utilityX::coder::uuid& uuid, const std::string& pass) : role(uuid, pass) {}
        };

        namespace detail {
            inline constexpr const char* USERS_PATH = "*__root__*>HKEY-USERS";
            inline constexpr const char* SYSTEM_PATH = "*__root__*>HKEY-CLASSES-ROOT>Storage>system";
            inline constexpr const char* ADMIN_COMPARATOR_PATH = "*__root__*>HKEY-CLASSES-ROOT>Storage>system>uac_concept_comparator_credential";
            inline constexpr const char* require_metadatata[2] = { "encryptor_trusted_module_mono", "encryptor_trusted_module_stereo" };

            struct registry_guard {
                ::registry_editor::registry_editor* registry = nullptr;
                explicit registry_guard(::registry_editor::registry_editor* ptr) : registry(ptr) {}
                registry_guard(const registry_guard&) = delete;
                registry_guard& operator=(const registry_guard&) = delete;
                registry_guard(registry_guard&& other) noexcept : registry(other.registry) { other.registry = nullptr; }
                registry_guard& operator=(registry_guard&& other) noexcept {
                    if (this != &other) {
                        if (registry != nullptr) ::registry_editor::registry_end(registry);
                        registry = other.registry;
                        other.registry = nullptr;
                    }
                    return *this;
                }
                ~registry_guard() { if (registry != nullptr) ::registry_editor::registry_end(registry); }
                ::registry_editor::registry_editor* operator->() { return registry; }
                const ::registry_editor::registry_editor* operator->() const { return registry; }
            };

            inline bool check_id(const std::string& identifier) {
                if (identifier.empty()) return false;
                static const auto allowed = [] {
                    std::array<bool, 256> table{};
                    for (unsigned char c = 'a'; c <= 'z'; ++c) table[c] = true;
                    for (unsigned char c = 'A'; c <= 'Z'; ++c) table[c] = true;
                    for (unsigned char c = '0'; c <= '9'; ++c) table[c] = true;
                    constexpr char chars[] = "_-.";
                    for (unsigned char c : chars) table[c] = true;
                    return table;
                    }();
                for (unsigned char c : identifier) if (!allowed[c]) return false;
                return true;
            }

            inline std::string create_rng() {
                std::random_device rd;
                std::array<unsigned char, 32> key{};
                for (std::size_t i = 0; i < key.size(); ++i) key[i] = static_cast<unsigned char>(rd() & 0xFFu);
                return std::string(reinterpret_cast<const char*>(key.data()), key.size());
            }

            inline void encrypt(const std::string& master_key, std::string* data) {
                std::string rng = create_rng();
                *data = ::utilityX::coder::encryptor::encode(rng, master_key) + ::utilityX::coder::encryptor::encode(*data, rng);
            }

            inline std::string decrypt(const std::string& master_key, const std::string& cybertext) {
                if (cybertext.size() < 48) return "";
                return ::utilityX::coder::encryptor::decode(cybertext.substr(40), ::utilityX::coder::encryptor::decode(cybertext.substr(0, 40), master_key));
            }

            inline bool verify_password_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& password) {
                const std::string user_path = std::string(USERS_PATH) + ">" + user_uuid.get();
                if (!registry->contains(user_uuid.get(), USERS_PATH)) throw platform_core::ecc{ 0, 38 };
                if (registry->read_byte("pass_flag", user_path) == 1) throw platform_core::ecc{ 0, 38 };
                const std::string encryption_path = user_path + ">encryption";
                if (registry->get_type("encryption", user_path) != ::registry_editor::type::key) throw platform_core::ecc{ 0, 39 };

                auto property = registry->properties(encryption_path);
                for (const auto& name : require_metadatata) {
                    const auto it = property.find(name);
                    if (it == property.end() || it->second != ::registry_editor::type::string) throw platform_core::ecc{ 0, 39 };
                }
                try {
                    const std::string mono = decrypt(password, registry->read_value("encryptor_trusted_module_mono", encryption_path));
                    const std::string stereo = decrypt(password, registry->read_value("encryptor_trusted_module_stereo", encryption_path));
                    return mono == stereo;
                }
                catch (...) { return false; }
            }

            inline priorities get_priority_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid) {
                const std::string user_path = std::string(USERS_PATH) + ">" + user_uuid.get();
                try {
                    const std::string key = registry->read_value("aunothrize", user_path);
                    if (key.size() != 32) return priorities::user;
                    if (!registry->contains("uac_concept_comparator_credential", SYSTEM_PATH)) return priorities::user;

                    const std::string comparator_path = std::string(ADMIN_COMPARATOR_PATH) + ">" + user_uuid.get();
                    const std::string lean = decrypt(key, registry->read_value("aunothorize_lean", comparator_path));
                    const std::string mean = decrypt(key, registry->read_value("aunothorize_mean", comparator_path));
                    return lean == mean ? priorities::administrator : priorities::user;
                }
                catch (...) { return priorities::user; }
            }

            inline void check_account_live(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid) {
                if (!registry->contains(user_uuid.get(), USERS_PATH)) throw platform_core::ecc{ 0, 47 };
            }

            inline void check_account_live(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& password) {
                if (!registry->contains(user_uuid.get(), USERS_PATH)) throw platform_core::ecc{ 0, 47 };
                try { if (!verify_password_implement(registry, user_uuid, password)) throw platform_core::ecc{ 0, 47 }; }
                catch (...) { throw platform_core::ecc{ 0, 47 }; }
            }

            inline void create_account_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& name, const std::string& password) {
                const std::string uuid_string = user_uuid.get();
                std::vector<std::string> users = registry->list(USERS_PATH);
                if (std::find(users.begin(), users.end(), uuid_string) != users.end()) throw platform_core::ecc{ 0, 41 };

                const bool is_first_account = users.empty();
                registry->create_key(uuid_string, USERS_PATH);
                const std::string user_path = std::string(USERS_PATH) + ">" + uuid_string;
                registry->create_byte("pass_flag", 0, user_path);
                registry->create_value("name", name, user_path);

                if (is_first_account) {
                    std::string key = create_rng();
                    registry->create_value("aunothrize", key, user_path);
                    if (!registry->contains("uac_concept_comparator_credential", SYSTEM_PATH)) throw platform_core::ecc{ 0, 42 };

                    registry->create_key(uuid_string, ADMIN_COMPARATOR_PATH);
                    const std::string comparator_path = std::string(ADMIN_COMPARATOR_PATH) + ">" + uuid_string;
                    const std::string new_lean = create_rng();

                    std::string lean1 = new_lean;
                    encrypt(key, &lean1);
                    registry->create_value("aunothorize_lean", lean1, comparator_path);

                    std::string lean2 = new_lean;
                    encrypt(key, &lean2);
                    registry->create_value("aunothorize_mean", lean2, comparator_path);

                    if (!registry->contains("first_admin_uuid", SYSTEM_PATH)) registry->create_value("first_admin_uuid", uuid_string, SYSTEM_PATH);
                    else registry->modify_value("first_admin_uuid", SYSTEM_PATH, uuid_string);
                }
                else registry->create_value("aunothrize", "", user_path);

                registry->create_key("encryption", user_path);
                const std::string encryption_path = user_path + ">encryption";
                const std::string rng = create_rng();

                std::string key_mono = rng;
                encrypt(password, &key_mono);
                registry->create_value("encryptor_trusted_module_mono", key_mono, encryption_path);

                std::string key_stereo = rng;
                encrypt(password, &key_stereo);
                registry->create_value("encryptor_trusted_module_stereo", key_stereo, encryption_path);
            }

            inline void remove_account_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& password, const std::string& transfer_uuid_str) {
                const std::string uuid_string = user_uuid.get();
                const std::string user_path = std::string(USERS_PATH) + ">" + uuid_string;
                if (!registry->contains(uuid_string, USERS_PATH)) throw platform_core::ecc{ 0, 38 };
                if (!verify_password_implement(registry, user_uuid, password)) throw platform_core::ecc{ 0, 40 };

                bool has_first_admin_key = registry->contains("first_admin_uuid", SYSTEM_PATH);
                std::string first_admin;

                if (has_first_admin_key) {
                    try { first_admin = registry->read_value("first_admin_uuid", SYSTEM_PATH); }
                    catch (...) { throw platform_core::ecc{ 0, 43 }; }
                }

                const bool is_first_admin = (first_admin == uuid_string);
                if (!is_first_admin && !transfer_uuid_str.empty()) throw platform_core::ecc{ 0, 44 };

                if (is_first_admin) {
                    const auto users = registry->list(USERS_PATH);
                    if (users.size() <= 1) {
                        if (has_first_admin_key) registry->delete_element("first_admin_uuid", SYSTEM_PATH);
                    }
                    else {
                        std::string target_admin;
                        if (!transfer_uuid_str.empty()) {
                            if (std::find(users.begin(), users.end(), transfer_uuid_str) == users.end()) throw platform_core::ecc{ 0, 45 };
                            if (transfer_uuid_str == uuid_string) throw platform_core::ecc{ 0, 46 };
                            target_admin = transfer_uuid_str;
                        }
                        else {
                            for (const auto& u : users) {
                                if (u != uuid_string) {
                                    target_admin = u;
                                    break;
                                }
                            }
                        }

                        const std::string target_path = std::string(USERS_PATH) + ">" + target_admin;
                        std::string newkey = create_rng();
                        const std::string target_comparator = std::string(ADMIN_COMPARATOR_PATH) + ">" + target_admin;

                        if (!registry->contains(target_admin, ADMIN_COMPARATOR_PATH)) {
                            registry->create_key(target_admin, ADMIN_COMPARATOR_PATH);
                            const std::string lean1 = create_rng();

                            std::string enc_lean = lean1;
                            encrypt(newkey, &enc_lean);
                            registry->create_value("aunothorize_lean", enc_lean, target_comparator);

                            std::string enc_mean = lean1;
                            encrypt(newkey, &enc_mean);
                            registry->create_value("aunothorize_mean", enc_mean, target_comparator);
                        }
                        else {
                            const std::string lean1 = create_rng();

                            std::string enc_lean = lean1;
                            encrypt(newkey, &enc_lean);
                            registry->modify_value("aunothorize_lean", target_comparator, enc_lean);

                            std::string enc_mean = lean1;
                            encrypt(newkey, &enc_mean);
                            registry->modify_value("aunothorize_mean", target_comparator, enc_mean);
                        }
                        registry->modify_value("aunothrize", target_path, newkey);
                        registry->modify_value("first_admin_uuid", SYSTEM_PATH, target_admin);
                    }
                }

                if (registry->contains(uuid_string, ADMIN_COMPARATOR_PATH)) registry->delete_element(uuid_string, ADMIN_COMPARATOR_PATH);
                registry->delete_element(uuid_string, USERS_PATH);
            }

            inline void change_password_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& old_password, const std::string& new_password) {
                if (!verify_password_implement(registry, user_uuid, old_password)) throw platform_core::ecc{ 0, 40 };
                const std::string encryption_path = std::string(USERS_PATH) + ">" + user_uuid.get() + ">encryption";
                const std::string rng = create_rng();
                std::string key_mono = rng;
                encrypt(new_password, &key_mono);
                registry->modify_value("encryptor_trusted_module_mono", encryption_path, key_mono);
                std::string key_stereo = rng;
                encrypt(new_password, &key_stereo);
                registry->modify_value("encryptor_trusted_module_stereo", encryption_path, key_stereo);
            }

            inline void rename_account_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& password, const std::string& new_name) {
                if (!verify_password_implement(registry, user_uuid, password)) throw platform_core::ecc{ 0, 40 };
                const std::string user_path = std::string(USERS_PATH) + ">" + user_uuid.get();
                registry->modify_value("name", user_path, new_name);
            }

            inline void change_user_id_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& old_uuid, const utilityX::coder::uuid& new_uuid, const std::string& password) {
                const std::string old_id = old_uuid.get();
                const std::string new_id = new_uuid.get();
                if (old_id == new_id) throw platform_core::ecc{ 0, 41 };
                if (registry->contains(new_id, USERS_PATH)) throw platform_core::ecc{ 0, 41 };
                if (!verify_password_implement(registry, old_uuid, password)) throw platform_core::ecc{ 0, 40 };

                const std::string old_path = std::string(USERS_PATH) + ">" + old_id;
                const std::string new_path = std::string(USERS_PATH) + ">" + new_id;
                const BYTE pass_flag = registry->read_byte("pass_flag", old_path);
                const std::string name = registry->read_value("name", old_path);
                const std::string aunothrize = registry->read_value("aunothrize", old_path);

                const std::string old_encryption_path = old_path + ">encryption";
                const std::string key_mono = registry->read_value("encryptor_trusted_module_mono", old_encryption_path);
                const std::string key_stereo = registry->read_value("encryptor_trusted_module_stereo", old_encryption_path);

                const bool is_admin = (get_priority_implement(registry, old_uuid) == priorities::administrator);
                std::string old_lean;
                std::string old_mean;
                if (is_admin) {
                    const std::string old_comparator_path = std::string(ADMIN_COMPARATOR_PATH) + ">" + old_id;
                    old_lean = registry->read_value("aunothorize_lean", old_comparator_path);
                    old_mean = registry->read_value("aunothorize_mean", old_comparator_path);
                }

                bool first_admin = false;
                try { first_admin = (registry->read_value("first_admin_uuid", SYSTEM_PATH) == old_id); }
                catch (...) { first_admin = false; }

                registry->create_key(new_id, USERS_PATH);
                registry->create_byte("pass_flag", pass_flag, new_path);
                registry->create_value("name", name, new_path);
                registry->create_value("aunothrize", aunothrize, new_path);
                registry->create_key("encryption", new_path);

                const std::string new_encryption_path = new_path + ">encryption";
                registry->create_value("encryptor_trusted_module_mono", key_mono, new_encryption_path);
                registry->create_value("encryptor_trusted_module_stereo", key_stereo, new_encryption_path);

                if (is_admin) {
                    registry->create_key(new_id, ADMIN_COMPARATOR_PATH);
                    const std::string new_comparator_path = std::string(ADMIN_COMPARATOR_PATH) + ">" + new_id;
                    registry->create_value("aunothorize_lean", old_lean, new_comparator_path);
                    registry->create_value("aunothorize_mean", old_mean, new_comparator_path);
                }

                if (first_admin) {
                    if (registry->contains("first_admin_uuid", SYSTEM_PATH)) registry->modify_value("first_admin_uuid", SYSTEM_PATH, new_id);
                    else registry->create_value("first_admin_uuid", new_id, SYSTEM_PATH);
                }

                if (registry->contains(old_id, ADMIN_COMPARATOR_PATH)) registry->delete_element(old_id, ADMIN_COMPARATOR_PATH);
                registry->delete_element(old_id, USERS_PATH);
            }

            inline void switch_priority_implement(::registry_editor::registry_editor* registry, const utilityX::coder::uuid& user_uuid, const std::string& admin_password, priorities role) {
                std::string first_admin_uuid;
                try { first_admin_uuid = registry->read_value("first_admin_uuid", SYSTEM_PATH); }
                catch (...) { throw platform_core::ecc{ 0, 43 }; }

                const utilityX::coder::uuid admin_uuid = utilityX::coder::uuid::encode(first_admin_uuid.c_str());
                if (!verify_password_implement(registry, admin_uuid, admin_password)) throw platform_core::ecc{ 0, 40 };

                check_account_live(registry, user_uuid);
                const std::string user_path = std::string(USERS_PATH) + ">" + user_uuid.get();

                if (role == priorities::administrator) {
                    const std::string newkey = create_rng();
                    const std::string comparator_path = std::string(ADMIN_COMPARATOR_PATH) + ">" + user_uuid.get();

                    if (!registry->contains(user_uuid.get(), ADMIN_COMPARATOR_PATH)) {
                        registry->create_key(user_uuid.get(), ADMIN_COMPARATOR_PATH);
                        const std::string lean1 = create_rng();
                        std::string enc_lean = lean1;
                        encrypt(newkey, &enc_lean);
                        registry->create_value("aunothorize_lean", enc_lean, comparator_path);
                        std::string enc_mean = lean1;
                        encrypt(newkey, &enc_mean);
                        registry->create_value("aunothorize_mean", enc_mean, comparator_path);
                    }
                    else {
                        const std::string lean1 = create_rng();
                        std::string enc_lean = lean1;
                        encrypt(newkey, &enc_lean);
                        registry->modify_value("aunothorize_lean", comparator_path, enc_lean);
                        std::string enc_mean = lean1;
                        encrypt(newkey, &enc_mean);
                        registry->modify_value("aunothorize_mean", comparator_path, enc_mean);
                    }
                    registry->modify_value("aunothrize", user_path, newkey);
                }
                else {
                    if (registry->contains(user_uuid.get(), ADMIN_COMPARATOR_PATH)) registry->delete_element(user_uuid.get(), ADMIN_COMPARATOR_PATH);
                    registry->modify_value("aunothrize", user_path, "");
                }
            }
        }

        inline user_role::_name_proxy::operator std::string() const {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (parent->_internal_password.empty()) detail::check_account_live(registry.registry, parent->user_uuid);
            else detail::check_account_live(registry.registry, parent->user_uuid, parent->_internal_password);
            return registry->read_value("name", std::string(detail::USERS_PATH) + ">" + parent->user_uuid.get());
        }

        inline user_role::_prio_proxy::operator priorities() const {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (parent->_internal_password.empty()) detail::check_account_live(registry.registry, parent->user_uuid);
            else detail::check_account_live(registry.registry, parent->user_uuid, parent->_internal_password);
            return detail::get_priority_implement(registry.registry, parent->user_uuid);
        }

        inline bool exist(const std::string& user_id) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            return registry->contains(utilityX::coder::uuid::encode(user_id.c_str()).get(), detail::USERS_PATH);
        }

        inline std::vector<user_role> list() {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            std::vector<user_role> result;
            for (const auto& uuid_str : registry->list(detail::USERS_PATH)) {
                try {
                    utilityX::coder::uuid id;
                    id.assign(uuid_str);
                    detail::check_account_live(registry.registry, id);
                    result.emplace_back(id);
                }
                catch (...) {}
            }
            return result;
        }

        inline user_meta get_account_meta(const std::string& user_id, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_meta return_data(utilityX::coder::uuid::encode(user_id.c_str()), password);
            detail::check_account_live(registry.registry, return_data.role.user_uuid, password);
            return return_data;
        }

        inline user_meta get_account_meta(const utilityX::coder::uuid& user_uuid, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_meta return_data(user_uuid, password);
            detail::check_account_live(registry.registry, return_data.role.user_uuid, password);
            return return_data;
        }

        inline user_meta get_account_meta(const utilityX::coder::uuid_reference& user_uuid, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_meta return_data(user_uuid, password);
            detail::check_account_live(registry.registry, return_data.role.user_uuid, password);
            return return_data;
        }

        inline void new_account(const std::string& user_id, const std::string& name, const std::string& password) {
            if (!detail::check_id(user_id)) throw platform_core::ecc{ 0, 39 };
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::create_account_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), name, password);
        }

        inline void new_account(const utilityX::coder::uuid& user_uuid, const std::string& name, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::create_account_implement(registry.registry, user_uuid, name, password);
        }

        inline void new_account(const utilityX::coder::uuid_reference& user_uuid, const std::string& name, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::create_account_implement(registry.registry, user_uuid, name, password);
        }

        inline void remove_account(const std::string& user_id, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), password, "");
        }

        inline void remove_account(const std::string& user_id, const std::string& password, const std::string& transfer_user_id) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), password, utilityX::coder::uuid::encode(transfer_user_id.c_str()).get());
        }

        inline void remove_account(const utilityX::coder::uuid& user_uuid, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, user_uuid, password, "");
        }

        inline void remove_account(const utilityX::coder::uuid& user_uuid, const std::string& password, const utilityX::coder::uuid& transfer_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, user_uuid, password, transfer_uuid.get());
        }

        inline void remove_account(const utilityX::coder::uuid_reference& user_uuid, const std::string& password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, user_uuid, password, "");
        }

        inline void remove_account(const utilityX::coder::uuid_reference& user_uuid, const std::string& password, const utilityX::coder::uuid_reference& transfer_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::remove_account_implement(registry.registry, user_uuid, password, transfer_uuid.get());
        }

        inline void change_password(const std::string& user_id, const std::string& old_password, const std::string& new_password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_password_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), old_password, new_password);
        }

        inline void change_password(const utilityX::coder::uuid& user_uuid, const std::string& old_password, const std::string& new_password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_password_implement(registry.registry, user_uuid, old_password, new_password);
        }

        inline void change_password(const utilityX::coder::uuid_reference& user_uuid, const std::string& old_password, const std::string& new_password) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_password_implement(registry.registry, user_uuid, old_password, new_password);
        }

        inline void rename_account(const std::string& user_id, const std::string& password, const std::string& new_name) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::rename_account_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), password, new_name);
        }

        inline void rename_account(const utilityX::coder::uuid& user_uuid, const std::string& password, const std::string& new_name) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::rename_account_implement(registry.registry, user_uuid, password, new_name);
        }

        inline void rename_account(const utilityX::coder::uuid_reference& user_uuid, const std::string& password, const std::string& new_name) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::rename_account_implement(registry.registry, user_uuid, password, new_name);
        }

        inline void change_user_id(const std::string& old_user_id, const std::string& password, const std::string& new_user_id) {
            if (!detail::check_id(old_user_id) || !detail::check_id(new_user_id)) throw platform_core::ecc{ 0, 39 };
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_user_id_implement(registry.registry, utilityX::coder::uuid::encode(old_user_id.c_str()), utilityX::coder::uuid::encode(new_user_id.c_str()), password);
        }

        inline void change_user_id(const utilityX::coder::uuid& old_user_uuid, const std::string& password, const utilityX::coder::uuid& new_user_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_user_id_implement(registry.registry, old_user_uuid, new_user_uuid, password);
        }

        inline void change_user_id(const utilityX::coder::uuid_reference& old_user_uuid, const std::string& password, const utilityX::coder::uuid_reference& new_user_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::change_user_id_implement(registry.registry, old_user_uuid, new_user_uuid, password);
        }

        inline void switch_priority(const std::string& user_id, const std::string& password, priorities role) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::switch_priority_implement(registry.registry, utilityX::coder::uuid::encode(user_id.c_str()), password, role);
        }

        inline void switch_priority(const utilityX::coder::uuid& user_uuid, const std::string& password, priorities role) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::switch_priority_implement(registry.registry, user_uuid, password, role);
        }

        inline void switch_priority(const utilityX::coder::uuid_reference& user_uuid, const std::string& password, priorities role) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            detail::switch_priority_implement(registry.registry, user_uuid, password, role);
        }

        inline user_role get_account_info(const std::string& user_id) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_role return_data(utilityX::coder::uuid::encode(user_id.c_str()));
            detail::check_account_live(registry.registry, return_data.user_uuid);
            return return_data;
        }

        inline user_role get_account_info(const utilityX::coder::uuid& user_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_role return_data(user_uuid);
            detail::check_account_live(registry.registry, return_data.user_uuid);
            return return_data;
        }

        inline user_role get_account_info(const utilityX::coder::uuid_reference& user_uuid) {
            auto* raw = registry_editor::registry_begin(utilityX::registry_file_path_from_this_header());
            detail::registry_guard registry(raw);
            if (!registry->contains("HKEY-USERS", "*__root__*")) throw platform_core::ecc{ 0, 37 };
            user_role return_data(user_uuid);
            detail::check_account_live(registry.registry, return_data.user_uuid);
            return return_data;
        }
    }
}
#endif
#endif