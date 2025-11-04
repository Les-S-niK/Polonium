#pragma once

#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "socket_exceptions.hpp"


using socket_fd = int;


namespace exception_messages {
    inline constexpr const char* tcp_bind = "Can't bind the server socket. Error in the bind()"; 
    inline constexpr const char* tcp_listen = "Can't set max backlog size. Error in the listen().";
    inline constexpr const char* tcp_accept = "Can't accept client_fd from the server socket. Erorr in the accept()";
    inline constexpr const char* tcp_recv = "Can't receive the data from the client socket. Error in the recv()";
    inline constexpr const char* tcp_send = "Can't send the data to the client socket. Error in the send()";
}


class TcpIpv4Socket
{
    public:
        TcpIpv4Socket() { server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); };
        ~TcpIpv4Socket() { if(server_fd) close(server_fd); };

        inline socket_fd getServerSocketFd() { return server_fd; }
    
        inline void tcp_bind(const std::string& host, const uint16_t& port) {
            struct sockaddr_in server_addr;
            inet_aton(host.c_str(), &server_addr.sin_addr);
            server_addr.sin_port = htons(port);
            server_addr.sin_family = AF_INET;

            if(bind(server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
                throw socket_exception(exception_messages::tcp_bind, server_addr);
            }
        }

        inline void tcp_listen(const int& max_backlog_size) {
            if(listen(server_fd, max_backlog_size) == -1) {
                throw socket_exception(exception_messages::tcp_listen);
            }
        }

        inline std::pair<socket_fd, struct sockaddr_in> tcp_accept() {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            socket_fd client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &client_addr_len);
            if(client_fd == -1) {
                throw socket_exception(exception_messages::tcp_accept);
            }
            return std::make_pair(client_fd, client_addr);
        }

        inline std::vector<char> tcp_recv(const socket_fd& client_fd, const size_t& max_buffer_size, const int flags = 0) {
            std::vector<char> buffer(max_buffer_size);
            size_t recieved_size = recv(client_fd, buffer.data(), buffer.size(), flags);
            
            if(recieved_size == -1) throw socket_exception(exception_messages::tcp_recv);
            else if (recieved_size == 0) buffer.clear();
            else buffer.resize(recieved_size);
            
            return buffer;
        }

        inline void tcp_send(const socket_fd& client_fd, const std::vector<char>& buffer, int flags = 0) {
            if(send(client_fd, buffer.data(), buffer.size(), flags) == -1) {
                throw socket_exception(exception_messages::tcp_send);
            }
        }

    private:
        socket_fd server_fd;
};
