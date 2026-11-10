#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // 1. Get Render's dynamic port (default to 10000 if not set)
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 10000;

    std::cout << "Starting McNeil C++ Cloud Server on port " << port << "..." << std::endl;

    // 2. Create a basic TCP socket listener to keep the service alive for Render health checks
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Allow socket reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed on port " << port << std::endl;
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "C++ Server is live and listening for connections!" << std::endl;

    // 3. Keep the server running in an active loop to accept incoming browser requests
    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        // Respond to browser health check / HTTP requests
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>McNeil C++ Cloud Engine is Running!</h1>";
        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }

    return 0;
}