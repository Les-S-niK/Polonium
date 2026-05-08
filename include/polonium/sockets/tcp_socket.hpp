/**
 * @file
 * @brief A c++ wrapper over system Berkley TCP socket calls.
 */

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <expected>
#include <string>
#include <string_view>
#include <utility>

#include "polonium/app/polonium_logger.hpp"

namespace polonium {
using socket_fd = int;

template <typename Method>
concept is_polonium_logger_method = requires(
    polonium::PoloniumLogger logger, Method method, std::string_view message) {
    { (logger.*method)(message) } -> std::same_as<void>;
};

namespace exception_messages {
inline constexpr const char* tcp_socket =
    "Can't create the server socket. Error in the socket()";
inline constexpr const char* tcp_bind =
    "Can't bind the server socket. Error in the bind()";
inline constexpr const char* tcp_bind_sockopt_error =
    "Can't set socket option to reuse ADDR and PORT.";
inline constexpr const char* tcp_listen =
    "Can't set max backlog size. Error in the listen().";
inline constexpr const char* tcp_accept =
    "Can't accept client_fd from the server socket. Error in the accept()";
inline constexpr const char* tcp_accept_interrupted =
    "Can't accept client_fd from the server socket. Interrupted system call. "
    "(SIGINT) "
    "Error in the accept()";
inline constexpr const char* tcp_recv =
    "Can't receive the data from the client socket. Error in the recv()";
inline constexpr const char* tcp_recv_timeout =
    "Can't receive the data from the client socket. Timeout error in the "
    "recv()";
inline constexpr const char* tcp_send =
    "Can't send the data to the client socket. Error in the send()";
}  // namespace exception_messages

class TcpIpv4Socket {
   public:
    static constexpr uint8_t praddr4_len = 4;
    enum class TcpSocketErrors : uint8_t {
        TcpSocketInitError,
        TcpSocketBindError,
        TcpSocketListenErorr,
        TcpSocketAcceptInvalidFdError,
        TcpSocketAcceptInterruptError,
        TcpSocketRecvInvalidSizeError,
        TcpSocketRecvTimeoutError,
        TcpSocketSendError,
    };

    TcpIpv4Socket() = delete;
    TcpIpv4Socket(const TcpIpv4Socket&) = delete;
    TcpIpv4Socket(TcpIpv4Socket&& other) noexcept;
    auto operator=(const TcpIpv4Socket&) -> TcpIpv4Socket& = delete;
    auto operator=(TcpIpv4Socket&& other) noexcept -> TcpIpv4Socket&;
    ~TcpIpv4Socket();

    /**
     * @brief TcpIpv4Socket fabric method. Initializes socket_fd_ by a socket()
     * system call.
     *
     * @return TcpIpv4Socket& instance if socket_fd_ initialized successfully.
     * TcpSocketInitError otherwise.
     */
    [[nodiscard]] static auto createTcpIpv4Socket() noexcept
        -> std::expected<TcpIpv4Socket, TcpSocketErrors>;

    /**
     * @brief Get server socket file descriptor.
     *
     * @return socket_fd: socket file descriptor.
     */
    [[nodiscard]] auto getServerSocketFd() const -> socket_fd;

    /**
     * @brief Bind TCP socket.
     *
     * @param host: server host.
     * @param port: server port to open.
     *
     * @return void if socket was binded successfully. TcpSocketErrors
     * otherwise.
     */
    [[nodiscard]] auto tcpBind(const std::string& host,
                               const uint16_t& port) const
        -> std::expected<void, TcpSocketErrors>;

    /**
     * @brief Set max socket backlog size via listen() function.
     *
     * @param max_backlog_size
     */
    [[nodiscard]] auto tcpListen(const int& max_backlog_size) const
        -> std::expected<void, TcpSocketErrors>;

    /**
     * @brief Accept a new client connection.
     *
     * @return std::pair<socket_fd, struct sockaddr_in>: client fd with his
     * sockaddr_in when accepted connection successfully. TcpSocketErrors
     * otherwise.
     */
    [[nodiscard]] auto tcpAccept() const
        -> std::expected<std::pair<socket_fd, struct sockaddr_in>,
                         TcpSocketErrors>;

    /**
     * @brief Receive bytes from client.
     *
     * @param client_fd
     * @param flags
     *
     * @return std::string: received data as string if receive data
     * successfully. TcpSocketErrors otherwise.
     */
    [[nodiscard]] auto tcpRecv(const socket_fd& client_fd, int flags = 0) const
        -> std::expected<std::string, TcpSocketErrors>;

    /**
     * @brief Send bytes to client.
     *
     * @param client_fd
     * @param buffer: string buffer to send.
     * @param flags
     *
     * @return ssize_t: sent data length if data sent successfull.
     * TcpSocketErrors otherwise.
     */
    [[nodiscard]] auto tcpSend(const socket_fd& client_fd,
                               std::string_view buffer, int flags = 0) const
        -> std::expected<ssize_t, TcpSocketErrors>;

   private:
    explicit TcpIpv4Socket(socket_fd server_fd);

    /**
     * @brief Write occurred error in the log file.
     *
     * @param message: Message that will be written in the log file.
     */
    template <is_polonium_logger_method Method>
    auto logSocketError_(std::string_view message, Method method) const
        -> void {
        std::string final_message = std::format(
            "{}. Errno: {}", message,
            (std::error_code(errno, std::generic_category()).message()));
        (logger_->*method)(final_message);
    }

    socket_fd server_fd_;
    polonium::PoloniumLogger* logger_;
};
}  // namespace polonium
