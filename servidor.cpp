
#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <mutex>
#include <atomic>
#include <algorithm>

struct Cliente {
    int socket;
    std::string nombre;
};

std::vector<Cliente> clientes;
std::mutex mtx;

bool nombre_valido(const std::string& nombre_usuario) {
    std::lock_guard<std::mutex> lock(mtx);
    // Verificar si el nombre ya está en uso
    for (const Cliente& cli : clientes) {
        if (cli.nombre == nombre_usuario) {
            return false;
        }
    }
    return true;
}

void manejar_cliente(int cliente_sock) {
    char buffer[1024];
    std::string nombre_usuario;

    // Recibir el nombre de usuario inicial
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(cliente_sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        close(cliente_sock);
        return;
    }
    nombre_usuario = buffer;

    // Verificar si el nombre es válido (no está en uso)
    while (!nombre_valido(nombre_usuario)) {
        std::string msg_error = "El nombre de usuario ya está en uso. Elige otro.";
        send(cliente_sock, msg_error.c_str(), msg_error.size(), 0);

        memset(buffer, 0, sizeof(buffer));
        bytes = recv(cliente_sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            close(cliente_sock);
            return;
        }
        nombre_usuario = buffer;
    }

    // Añadir cliente a la lista
    {
        std::lock_guard<std::mutex> lock(mtx);
        clientes.push_back({cliente_sock, nombre_usuario});
    }

    // Anunciar en el servidor que el usuario se ha unido
    std::cout << "Nuevo usuario conectado: " << nombre_usuario << std::endl;

    std::string mensaje_bienvenida = nombre_usuario + " se ha unido al chat.";
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (const Cliente& cli : clientes) {
            if (cli.socket != cliente_sock) {
                send(cli.socket, mensaje_bienvenida.c_str(), mensaje_bienvenida.size(), 0);
            }
        }
    }

    // Empezar a recibir mensajes
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(cliente_sock, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            std::string msg_salida = nombre_usuario + " se ha desconectado.";
            {
                std::lock_guard<std::mutex> lock(mtx);
                // Eliminar cliente de la lista
                clientes.erase(std::remove_if(clientes.begin(), clientes.end(),
                                              [cliente_sock](const Cliente& c) {
                                                  return c.socket == cliente_sock;
                                              }), clientes.end());

                // Notificar a los demás clientes sobre la desconexión
                for (const Cliente& cli : clientes) {
                    send(cli.socket, msg_salida.c_str(), msg_salida.size(), 0);
                }
            }

            // Mostrar el mensaje de desconexión en el servidor
            std::cout << msg_salida << std::endl;

            close(cliente_sock);
            break;
        }

        // Mostrar el mensaje recibido en el servidor
        std::cout << "Mensaje recibido de " << nombre_usuario << ": " << buffer << std::endl;

        std::string mensaje = nombre_usuario + ": " + buffer;

        // Enviar mensaje a los demás clientes
        {
            std::lock_guard<std::mutex> lock(mtx);
            for (const Cliente& cli : clientes) {
                if (cli.socket != cliente_sock) {
                    send(cli.socket, mensaje.c_str(), mensaje.size(), 0);
                }
            }
        }
    }
}

int main() {
    int servidor_sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in servidor{};
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_port = htons(9000);

    bind(servidor_sock, (sockaddr*)&servidor, sizeof(servidor));
    listen(servidor_sock, 10);

    std::cout << "Servidor escuchando en el puerto 9000...\n";

    // Aceptar clientes
    while (true) {
        sockaddr_in cliente{};
        socklen_t cliente_len = sizeof(cliente);
        int cliente_sock = accept(servidor_sock, (sockaddr*)&cliente, &cliente_len);

        std::thread(manejar_cliente, cliente_sock).detach();
    }

    // Asegúrate de cerrar el socket del servidor una vez que se haya detenido
    close(servidor_sock);
    return 0;
}
