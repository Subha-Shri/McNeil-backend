#include <iostream>
#include <cstdlib>
#include <string>

// (Include your HTTP library headers here, e.g., Crow or simple sockets)

int main() {
    // Render assigns a dynamic port via environment variables
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 10000;

    std::cout << "Starting C++ Server on port " << port << std::endl;

    // Your web server listening and routing logic here
    
    return 0;
}