# Chat por Sockets TCP - Cliente y Servidor en C++

## Proyecto realizado por: Juan Jose Vasquez Gomez y Santiago Alvarez Peña

## Video Explicación

- [Video explicativo y demostrativo del funcionamiento del proyecto](https://drive.google.com/file/d/1Kr6AtPX-kUrROCMsr9KEXbfkC2H8ee9o/view?usp=sharing)

---

## 🧹 Introducción

Este proyecto consiste en la implementación de un sistema de chat basado en la API de Sockets de Berkeley. Utiliza el protocolo TCP para permitir la comunicación bidireccional entre múltiples clientes conectados simultáneamente a un servidor.  
Tanto el servidor como los clientes están desarrollados en **C++**, lo cual demuestra la interoperabilidad de este lenguaje utilizando una interfaz común de sockets.

El sistema permite que los usuarios interactúen en tiempo real, enviando y recibiendo mensajes, con identificación de cada usuario a través de su nombre. Las pruebas se realizaron utilizando tres máquinas virtuales conectadas a través de Tailscale.

---

## ⚙️ Desarrollo

### Tecnologías usadas:
- Lenguajes: **C++ (servidor)**, **C++ (cliente)**
- API: **Sockets de Berkeley (AF_INET, SOCK_STREAM)**
- Protocolo de red: **TCP/IP**
- Comunicación entre máquinas virtuales: **Tailscale**

### Funcionalidades implementadas:
- Conexión de múltiples clientes a un único servidor de chat.
- Envío de mensajes desde el cliente al servidor, y retransmisión a los demás clientes.
- Recepción de mensajes y visualización en pantalla en tiempo real.
- Identificación de usuario mediante nombre único.
- Detección y manejo de nombres de usuario duplicados.
- Desconexión controlada del cliente con el comando `/exit`.
- Notificación al servidor y a los demás clientes cuando un usuario se conecta o se desconecta.
- Mensajes formateados con el nombre del usuario remitente.
- Visualización de mensajes directamente en la consola del servidor, para propósitos de monitoreo.
- Generación de logs en un archivo para registrar eventos de conexión y desconexión de usuarios.

---
## Funcionamiento

1. Descargue los archivos servidor.cpp y cliente.cpp (Archivos cliente puede usar cuantos quiera, pero por seguridad lo restringimos a 5)
2. Distribuya estos archivos en las maquinas virtuales que desee (En nuestro caso fueron 2 Maquinas Cliente y 1 Maquina Servidor)
3. Ingrese a los archivos cliente y asegurese de apuntar a la ip del servidor, además asegurese de estar conectado o direccionado a esta red del servidor, para modificar la ip ingrese a la linea 37 del codigo cliente.cpp y modifiquela
4. Guarde los codigos y compilelos con el comando g++ servidor.cpp -o servidor -pthread y g++ cliente.cpp -o cliente -pthread
5. Luego de compilar ejecute primero el servidor con el comando ./servidor, y luego ejecute cada uno de los clientes con el comando ./cliente
6. Listo ya tienes tu aplicación de chat a través de sockets funcionando.

---

## ✅ Aspectos Logrados y Potenciales Mejoras

### ✔️ Aspectos Logrados:
- Todas las funcionalidades básicas del sistema de chat fueron implementadas y verificadas con éxito.
- La arquitectura cliente-servidor es estable y escalable para un número razonable de usuarios concurrentes.
- El sistema distingue claramente entre mensajes propios y de otros usuarios.
- Manejo de errores básico en conexiones, envío y recepción de datos.
- Generación de logs persistentes de actividad del servidor.

### ✨ Posibles Mejoras:
Aunque el proyecto cumple todos los requisitos establecidos, existen varios aspectos que podrían explorarse para enriquecer o extender sus funcionalidades:
- **Cierre ordenado del servidor** desde una interfaz administrativa o comando especial.
- **Persistencia de mensajes** para permitir la recuperación del historial.
- **Encriptación de datos** (por ejemplo con TLS) para asegurar la privacidad de las comunicaciones.
- **Interfaz gráfica** para el cliente (con ncurses o GUI moderna).
- **Reconexión automática** en caso de desconexión inesperada del cliente.
- **Soporte para comandos internos**, como `/users` para listar usuarios conectados.

---

## 🧠 Conclusiones

El proyecto demuestra la capacidad de establecer una comunicación de red sólida entre múltiples clientes mediante sockets TCP, integrando conceptos de concurrencia (threads), sincronización (mutex), y estructuras de datos compartidas.  
Además, resalta la importancia del manejo correcto de entradas y salidas, así como la validación en tiempo real del estado de la red.

El diseño modular (cliente separado del servidor y cada cliente con su hilo) permite un fácil mantenimiento y escalabilidad. La solución es simple, clara, y suficientemente robusta para servir como base para desarrollos más complejos, como sistemas de mensajería con autenticación o interacción multimedia.

---

## 🔗 Referencias

- [Beej’s Guide to Network Programming](https://beej.us/guide/bgnet/)
- [The Linux Programming Interface - Michael Kerrisk](https://man7.org/tlpi/)
- [Sockets API - POSIX documentation](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [Tailscale](https://tailscale.com/) – Herramienta de red privada utilizada para conectar las máquinas virtuales.

