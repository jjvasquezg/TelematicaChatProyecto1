// cliente.cpp
#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

std::string username_global;  // nombre de usuario global

void recibir(int sock) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) break;

        std::string recibido(buffer);

        // Verificar si es nuestro propio mensaje
        if (recibido.find(username_global + ": ") == 0) {
            std::string mensaje_sin_nombre = recibido.substr(username_global.length() + 2);
            std::cout << "Tu: " << mensaje_sin_nombre << std::endl;
        } else {
            std::cout << recibido << std::endl;
        }
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);

    // Reemplaza por la IP Tailscale del servidor
    inet_pton(AF_INET, "100.121.125.25", &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == -1) {
        std::cerr << "Error al conectar\n";
        return 1;
    }

    std::cout << "Ingrese su nombre de usuario: ";
    std::getline(std::cin, username_global);

    // Enviar el nombre al servidor
    send(sock, username_global.c_str(), username_global.size(), 0);

    std::thread(recibir, sock).detach();

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        if (msg == "/exit") break;
        send(sock, msg.c_str(), msg.size(), 0);
        // No mostramos nada aquí. El eco llegará del servidor.
    }

    close(sock);
    return 0;
}
