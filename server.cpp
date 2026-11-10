#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // 1. Fetch dynamic PORT from Render (defaults to 10000)
    const char* port_env = std::getenv("PORT");
    int port = port_env ? std::stoi(port_env) : 10000;

    std::cout << "Starting McNeil C++ Loop Sheet Server on port " << port << "..." << std::endl;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

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

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "C++ Server is live and serving the Loop Sheet Dashboard!" << std::endl;

    // 2. Main Socket Loop
    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) continue;

        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);

        // Standard Dashboard HTML Card
        std::string htmlBody = 
            "<!DOCTYPE html>\n"
            "<html lang='en'>\n"
            "<head>\n"
            "    <meta charset='UTF-8'>\n"
            "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
            "    <title>McNeil Loop Sheet Generator</title>\n"
            "    <style>\n"
            "        body { font-family: Arial, sans-serif; background-color: #f4f6f9; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }\n"
            "        .card { background: white; padding: 40px; border-radius: 12px; box-shadow: 0px 4px 15px rgba(0,0,0,0.08); width: 400px; text-align: center; }\n"
            "        h2 { color: #111; margin-bottom: 12px; font-size: 24px; font-weight: bold; }\n"
            "        .status { font-size: 15px; margin-bottom: 25px; color: #444; }\n"
            "        .status span { color: #008000; font-weight: bold; }\n"
            "        input[type='text'] { width: 100%; padding: 12px; margin-bottom: 20px; box-sizing: border-box; border: 1px solid #ccc; border-radius: 6px; font-size: 15px; text-align: center; outline: none; }\n"
            "        input[type='text']:focus { border-color: #007acc; }\n"
            "        .btn { display: block; padding: 12px 20px; font-size: 15px; font-weight: bold; color: white; background-color: #1f6feb; border: none; border-radius: 6px; cursor: pointer; width: 100%; box-sizing: border-radius; }\n"
            "        .btn:hover { background-color: #1158c7; }\n"
            "    </style>\n"
            "</head>\n"
            "<body>\n"
            "    <div class='card'>\n"
            "        <h2>McNeil Loop Sheet Generator</h2>\n"
            "        <div class='status'>Status: <span>Engine is running!</span></div>\n"
            "        <input type='text' id='tagInput' value='TAG - 101'>\n"
            "        <button class='btn' onclick='generatePDF()'>Download Loop Sheet PDF</button>\n"
            "    </div>\n"
            "    <script>\n"
            "        function generatePDF() {\n"
            "            const tag = document.getElementById('tagInput').value.trim();\n"
            "            if(!tag) { alert('Please enter a Tag ID'); return; }\n"
            "            alert('Generating Loop Sheet PDF for: ' + tag + '\\nEngine: C++ Cloud Service');\n"
            "        }\n"
            "    </script>\n"
            "</body>\n"
            "</html>";

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + 
                               std::to_string(htmlBody.length()) + "\r\nConnection: close\r\n\r\n" + htmlBody;

        send(new_socket, response.c_str(), response.length(), 0);
        close(new_socket);
    }

    return 0;
}