/**
 * @file eBDD.cpp
 * @brief Implémentation du composant eBDD (Base de données locale des badges RFID)
 * @project Barrière d'Accès Sécurisée (BTS CIEL)
 * @author Clément Saforcada / DaK
 * @date 2026-10-01
 * 
 * Ce composant stocke la liste des badges autorisés (UID) et leurs privilèges (Admin / Utilisateur).
 * La persistance est assurée dans la mémoire Flash non-volatile de l'ESP32 via la bibliothèque Preferences (NVS).
 */

#include "eBDD.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <Preferences.h>

// Capacité maximale configurable de la base locale
#ifndef MAX_BDD_CAPACITY
#define MAX_BDD_CAPACITY 50
#endif

// Structure interne représentant un badge enregistré
struct BadgeEntry {
    string uid;
    bool admin;
};

// Variables statiques internes (encapsulation)
static std::vector<BadgeEntry> s_database;
static bool s_initialized = false;
static Preferences s_prefs;
static const char* PREFS_NAMESPACE = "ebdd";

/**
 * @brief Sauvegarde l'état actuel de la mémoire RAM vers la mémoire non-volatile (NVS).
 */
static void saveToNVS() {
    s_prefs.begin(PREFS_NAMESPACE, false); // Ouverture en lecture/écriture
    s_prefs.clear();                     // Nettoie les anciennes clés pour éviter les orphelins

    uint16_t count = static_cast<uint16_t>(s_database.size());
    s_prefs.putUShort("count", count);

    for (size_t i = 0; i < s_database.size(); ++i) {
        String keyUid = "u" + String(i);
        String keyAdm = "a" + String(i);
        s_prefs.putString(keyUid.c_str(), s_database[i].uid.c_str());
        s_prefs.putBool(keyAdm.c_str(), s_database[i].admin);
    }

    s_prefs.end();
}

/**
 * @brief Charge les badges stockés depuis la mémoire non-volatile (NVS) au premier appel.
 */
static void ensureInitialized() {
    if (s_initialized) return;

    s_database.clear();
    s_prefs.begin(PREFS_NAMESPACE, true); // Ouverture en lecture seule

    uint16_t count = s_prefs.getUShort("count", 0);
    for (uint16_t i = 0; i < count; ++i) {
        String keyUid = "u" + String(i);
        String keyAdm = "a" + String(i);
        if (s_prefs.isKey(keyUid.c_str())) {
            String uidStr = s_prefs.getString(keyUid.c_str(), "");
            bool isAdmin = s_prefs.getBool(keyAdm.c_str(), false);
            if (uidStr.length() > 0) {
                s_database.push_back({ string(uidStr.c_str()), isAdmin });
            }
        }
    }

    s_prefs.end();
    s_initialized = true;
}

/**
 * @brief Ajoute un badge dans la base de données.
 * @param UID Identifiant unique du badge RFID sous forme de chaîne de caractères.
 * @param Admin Définit si le badge possède les droits d'administration (défaut = false).
 * @return true si l'ajout ou la mise à jour des droits a réussi, false en cas d'erreur ou BDD pleine.
 */
bool addUID(const char* UID, const bool Admin) {
    if (UID == nullptr || UID[0] == '\0') {
        return false;
    }

    ensureInitialized();

    string uidStr(UID);

    // Vérifier si le badge existe déjà
    for (auto& entry : s_database) {
        if (entry.uid == uidStr) {
            if (entry.admin != Admin) {
                // Mise à jour du droit administrateur existant
                entry.admin = Admin;
                saveToNVS();
                return true;
            }
            // Déjà présent avec les mêmes droits
            return false;
        }
    }

    // Vérifier la capacité restante
    if (s_database.size() >= MAX_BDD_CAPACITY) {
        return false; // Base pleine
    }

    // Ajout du nouvel enregistrement
    s_database.push_back({ uidStr, Admin });
    saveToNVS();
    return true;
}

/**
 * @brief Retire un badge de la base de données.
 * @param UID Identifiant unique du badge à supprimer.
 * @return true si suppression réussie, false si non trouvé ou si tentative de retirer le dernier admin.
 */
bool delUID(const char* UID) {
    if (UID == nullptr || UID[0] == '\0') {
        return false;
    }

    ensureInitialized();

    string uidStr(UID);
    auto it = std::find_if(s_database.begin(), s_database.end(), [&uidStr](const BadgeEntry& b) {
        return b.uid == uidStr;
    });

    if (it == s_database.end()) {
        return false; // Badge introuvable
    }

    // Règle de sécurité critique : Échec si on tente de retirer le seul administrateur
    if (it->admin) {
        uint16_t nbAdmins = UIDs(true);
        if (nbAdmins <= 1) {
            // Impossible de supprimer : c'est le seul administrateur de la barrière !
            return false;
        }
    }

    // Suppression effective
    s_database.erase(it);
    saveToNVS();
    return true;
}

/**
 * @brief Retourne le nombre d'emplacements d'UID encore enregistrables.
 * @return uint16_t Nombre de places disponibles.
 */
uint16_t free() {
    ensureInitialized();
    if (s_database.size() >= MAX_BDD_CAPACITY) {
        return 0;
    }
    return static_cast<uint16_t>(MAX_BDD_CAPACITY - s_database.size());
}

/**
 * @brief Retourne le nombre d'UID enregistrés ou d'administrateurs enregistrés.
 * @param admin Si false, retourne le nombre total de badges ; si true, retourne le nombre d'administrateurs.
 * @return uint16_t Nombre de badges correspondants.
 */
uint16_t UIDs(bool admin) {
    ensureInitialized();

    if (!admin) {
        return static_cast<uint16_t>(s_database.size());
    }

    uint16_t countAdmin = 0;
    for (const auto& entry : s_database) {
        if (entry.admin) {
            countAdmin++;
        }
    }
    return countAdmin;
}

/**
 * @brief Remplit le buffer fourni avec la liste des {UID, admin} au format JSON.
 * @param buffer Pointeur vers le tableau de caractères alloué par l'appelant.
 * @param maxLen Taille maximale allouée pour ce buffer.
 * @return true si la copie a réussi, false si le buffer est trop petit ou invalide.
 */
bool liste(char* buffer, size_t maxLen) {
    if (buffer == nullptr || maxLen == 0) {
        return false;
    }

    ensureInitialized();

    string json = "[\n";
    for (size_t i = 0; i < s_database.size(); ++i) {
        json += "   {\n";
        json += "     \"uid\": \"" + s_database[i].uid + "\",\n";
        json += "     \"admin\": " + string(s_database[i].admin ? "true" : "false") + "\n";
        json += "   }";
        if (i + 1 < s_database.size()) {
            json += ",";
        }
        json += "\n";
    }
    json += "]";

    // Vérification de la taille pour éviter un dépassement de tampon
    if (json.length() >= maxLen) {
        return false; 
    }

    std::strcpy(buffer, json.c_str());
    return true;
}