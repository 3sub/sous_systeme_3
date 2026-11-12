/*
Composant : eGenCode
Support   : ESP32
Version/Auteur/Date : V1.0 DaK 20260923
Version/Auteur/Date : V1.1 HeF 24/09/2026
Version/Auteur/Date : V2.0 HeF 05/10/2026

Fonction :
Ce composant crée un code unique de n chiffres adapté à un UID

les codes ...
ne doivent pas commencer par zéro
ne doivent pas comporter deux chiffres identiques consécutifs
ne doivent pas comporter de séquences de 2 chiffres identiques
ne doivent pas se répéter avant 20 générations (on garde 20 derniers en mémoire)
ne doivent pas être prévisibles
ne doivent pas se repéter pour un même UID

pour le dernier code généré on enregistre millis() en variable statique afin de savoir si le code est encore valide au moment de l'appel de compareCode()
*/
#pragma once

#include <stdint.h>
 
/**
 * @brief  Génère un nouveau code unique pour un UID.
 *
 * @param  UID      Identifiant (chaîne terminée par 0, ne doit pas être vide).
 * @param  lencode  Nombre de chiffres du code (défaut : 4).
 *                  Valeurs permises : 2 à 9 (les valeurs hors limites sont ramenées dans cet intervalle).
 *
 * @return Le code généré (nombre de @p lencode chiffres),
 *         ou 0 si l'UID est nul ou vide.
 *
 * @note   Le code et l'instant de génération sont mémorisés pour compareCode().
 */
uint32_t genCode( const char* UID, uint8_t lencode=4 );

/**
 * @brief  Compare le code saisi au clavier avec le dernier code généré.
 *
 * @param  kbd         Code entré au clavier.
 * @param  timeoutsec  Durée de validité du code en secondes (défaut : 5*60 = 5 minutes).
 *
 * @retval true   Le code est correct et encore valide.
 * @retval false  Le code est faux, expiré, ou aucun code n'a été généré.
 *
 * @note   Le code est à usage unique : après une comparaison réussie, il n'est plus valide.
 */bool compareCode( uint32_t kbd, uint32_t timeoutsec=5*60);
