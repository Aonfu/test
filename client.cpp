#include <enet/enet.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    if (enet_initialize() != 0) {
        std::cerr << "Erreur : ENet n'a pas pu s'initialiser." << std::endl;
        return EXIT_FAILURE;
    }

    atexit(enet_deinitialize);

    ENetHost* client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (!client) {
        std::cerr << "Erreur : impossible de créer le client ENet." << std::endl;
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetPeer* peer;

    enet_address_set_host(&address, "127.0.0.1"); // Adresse du serveur
    address.port = 1234;

    peer = enet_host_connect(client, &address, 2, 0);
    if (!peer) {
        std::cerr << "Erreur : impossible de créer une connexion avec le serveur." << std::endl;
        return EXIT_FAILURE;
    }

    ENetEvent event;
    if (enet_host_service(client, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        std::cout << "Connecté au serveur !" << std::endl;

        const char* msg = "Hello serveur !";
        ENetPacket* packet = enet_packet_create(
            msg, std::strlen(msg) + 1, ENET_PACKET_FLAG_RELIABLE
        );
        enet_peer_send(peer, 0, packet);
        enet_host_flush(client);

    } else {
        std::cerr << "Connexion au serveur échouée." << std::endl;
        enet_peer_reset(peer);
        return EXIT_FAILURE;
    }

    // Écoute les messages du serveur
    while (enet_host_service(client, &event, 3000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Message du serveur : "
                          << (char*)event.packet->data << std::endl;
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Déconnecté du serveur." << std::endl;
                return EXIT_SUCCESS;
        }
    }

    enet_peer_disconnect(peer, 0);
    enet_host_destroy(client);
    return 0;
}