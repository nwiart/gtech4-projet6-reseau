#include "Lobby.h"

#include "Client.h"
#include <iostream>
#include "PongPackets.h"

Lobby::Lobby()
{

}

#include <random> // Pour générer un pseudo aléatoire

void Lobby::setupHost(uint32_t myID, uint32_t maxPlayers)
{
    m_isHost = true;
    m_myID = myID;
    m_maxPlayers = maxPlayers;

    // Ajouter le joueur réel
    m_players.emplace(myID, Player{ Client::getInstance().getPlayerName(), Client::getInstance().getPlayerID(), myID });

    // Générer un pseudo aléatoire pour le faux joueur
    static const std::string pseudoBase = "Bot_";
    int randomID = rand() % 1000; // ID entre 0 et 999
    std::string botName = pseudoBase + std::to_string(randomID);

    // Ajouter un faux joueur avec un ID différent
    uint32_t fakePlayerID = myID + 1; // Simule un deuxième joueur
    m_players.emplace(fakePlayerID, Player{ botName, fakePlayerID, 1 });

    std::cout << "Ajout du faux joueur : " << botName << " (ID: " << fakePlayerID << ")" << std::endl;

    // Simuler l'envoi d'un paquet comme si le serveur ajoutait un joueur réel
    Server_PlayerJoinedLobby packet;
    packet.playerID = fakePlayerID;
    packet.idInLobby = 1;
    strncpy(packet.playerName, botName.c_str(), sizeof(packet.playerName) - 1);
    packet.playerName[sizeof(packet.playerName) - 1] = '\0';

    // Simuler la réception côté client
    Client::getInstance().handleTCPPacket((uint32_t)ServerPackets::PlayerJoinedLobby);

}


void Lobby::setupGuest(uint32_t myID, uint32_t maxPlayers)
{
	m_isHost = false;
	m_myID = myID;
	m_maxPlayers = maxPlayers;
}

void Lobby::leave()
{
	m_players.clear();
	m_myID = -1;
}

void Lobby::listPlayer(uint32_t playerID, uint32_t inLobbyID, const std::string& playerName)
{
	std::cout << "Ajout du joueur " << playerID << " dans le lobby. Nom : " << playerName << std::endl;

	m_players.emplace(playerID, Player{ playerName, playerID, inLobbyID });

	// Vérifions que les données sont bien enregistrées
    for (auto& p : m_players)
    {
        std::cout << "Joueur " << p.first << " : " << p.second.m_name << std::endl;
    }
}

void Lobby::removePlayer(uint32_t playerID)
{
	m_players.erase(playerID);
}
