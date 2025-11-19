#pragma once

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <format>

#include "polonium_logger.hpp"
#include "socket_exceptions.hpp"


using socket_fd = int;

namespace exception_messages {
    inline constexpr const char* tcp_socket = "Can't create the server socket. Error in the socket()"; 
    inline constexpr const char* tcp_bind = "Can't bind the server socket. Error in the bind()"; 
    inline constexpr const char* tcp_listen = "Can't set max backlog size. Error in the listen().";
    inline constexpr const char* tcp_accept = "Can't accept client_fd from the server socket. Erorr in the accept()";
    inline constexpr const char* tcp_recv = "Can't receive the data from the client socket. Error in the recv()";
    inline constexpr const char* tcp_send = "Can't send the data to the client socket. Error in the send()";
}


class TcpIpv4Socket
{
    public:
        TcpIpv4Socket(PoloniumLogger& logger) : logger_(logger) {
            logger.trace(__func__);
            logger.info("TCP/IPv4 socket initialization.");
            server_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(server_fd_ == -1) logAndThrowException_(exception_messages::tcp_socket);
            logger_.debug("Created TCP/IPv4 socket."); 
        }

        ~TcpIpv4Socket() {
            logger_.trace(__func__);
            if(server_fd_) close(server_fd_);
            logger_.debug("Closed TCP/IPv4 socket.");
        }

        inline socket_fd getServerSocketFd() const {
            logger_.trace(__func__);
            return server_fd_;
        }
    
        inline void tcpBind(const std::string& host, const uint16_t& port) {
            logger_.trace(__func__);
            struct sockaddr_in server_addr;
            inet_aton(host.c_str(), &server_addr.sin_addr);
            server_addr.sin_port = htons(port);
            server_addr.sin_family = AF_INET;
            logger_.debug(std::format("Try binding TCP/IPv4 socket.\nHost: {}\nPort: {}", host, port));
            
            if(bind(server_fd_, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
                logAndThrowException_(exception_messages::tcp_bind);
            }
            logger_.debug("TCP/IPv4 socket binded.");
        }

        inline void tcpListen(const int& max_backlog_size) {
            logger_.trace(__func__);
            if(listen(server_fd_, max_backlog_size) == -1) logAndThrowException_(exception_messages::tcp_listen);
            logger_.debug(std::format("Listen. Max backlog size: {} connections", max_backlog_size));
        }

        inline std::pair<socket_fd, struct sockaddr_in> tcpAccept() {
            logger_.trace(__func__);
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            socket_fd client_fd = accept(server_fd_, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len);
            
            if(client_fd == -1) logAndThrowException_(exception_messages::tcp_accept);
            logger_.debug(std::format("Accepted TCP/IPv4 connection.\nClient addr: {}\nClient port: {}", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)));

            return std::make_pair(client_fd, client_addr);
        }

        inline std::vector<char> tcpRecv(const socket_fd& client_fd, const size_t& max_buffer_size, const int flags = 0) {
            logger_.trace(__func__);
            std::vector<char> buffer(max_buffer_size);
            size_t recieved_size = recv(client_fd, buffer.data(), buffer.size(), flags);
            
            if(recieved_size == -1) logAndThrowException_(exception_messages::tcp_recv);
            else if (recieved_size == 0) buffer.clear();
            else buffer.resize(recieved_size);
            logger_.debug(std::format("Recieved buffer with the size: {}", buffer.size()));

            return buffer;
        }

        inline void tcpSend(const socket_fd& client_fd, const std::vector<char>& buffer, int flags = 0) {
            logger_.trace(__func__);
            if(send(client_fd, buffer.data(), buffer.size(), flags) == -1) logAndThrowException_(exception_messages::tcp_send);
            logger_.debug(std::format("Sent buffer with the size: {}", buffer.size()));
        }

    private:
        PoloniumLogger& logger_;
        socket_fd server_fd_;

        inline void logAndThrowException_(std::string_view message) const {
            std::string final_message = std::format("{}. Errno: {}", message, strerror(errno));
            logger_.critical(final_message);
            throw socket_exception(final_message);
        }
};
