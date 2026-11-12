/**
 * @file eBDD.h
 * @brief Définition du composant eBDD (Base de données locale des badges RFID)
 * @project Barrière d'Accès Sécurisée (BTS CIEL)
 * @author Clément Saforcada / DaK
 * @date 2026-10-01
 */

#pragma once

#include <Arduino.h>


class eBDD {
public:
    // Méthodes publiques du composant eBDD
    bool addUID(const char* UID, const bool Admin = false); // ajoute un badge éventuellement admin
    bool delUID(const char* UID);                           // retire un badge. echec si on retire le seul admin
    uint16_t free();                                        // nombre d'UID enregistrables restants
    uint16_t UIDs(bool admin = false);                      // nombre d'UID enregistrés ou d'administrateurs enregistrés
    
    // Remplit le buffer fourni avec la liste des {UID,admin} au format JSON (sans std::string)
    bool liste(char* buffer, size_t maxLen);
};