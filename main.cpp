#include <enet/enet.h>
#include <iostream>

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        std::cerr << "Erreur : impossible d'initialiser ENet." << std::endl;
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    ENetAddress address;
    address.host = ENET_HOST_ANY; // Accepter toute IP
    address.port = 1234;          // Port d'écoute

    ENetHost* server = enet_host_create(&address, 32, 2, 0, 0);
    if (!server) {
        std::cerr << "Erreur : impossible de créer le serveur." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Serveur ENet lancé sur le port 1234." << std::endl;

    ENetEvent event;
    while (true) {
        while (enet_host_service(server, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Nouveau client connecté depuis "
                              << (event.peer->address.host & 0xFF) << "."
                              << ((event.peer->address.host >> 8) & 0xFF) << "."
                              << ((event.peer->address.host >> 16) & 0xFF) << "."
                              << ((event.peer->address.host >> 24) & 0xFF)
                              << ":" << event.peer->address.port << std::endl;
                    break;

                case ENET_EVENT_TYPE_RECEIVE:
                    std::cout << "Message reçu : "
                              << (char*)event.packet->data << std::endl;
                    enet_packet_destroy(event.packet);
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Un client s'est déconnecté." << std::endl;
                    break;

                default:
                    break;
            }
        }
    }

    enet_host_destroy(server);
    return 0;
}