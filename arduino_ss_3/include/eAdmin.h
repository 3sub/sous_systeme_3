/*
Composant : eAdmin
Support   : ESP32
Version/Auteur/Date : V1.0 DaK 20260923
V1/BAPTISTE/24-09-2026 : attribution du programe
V2/BAPTISTE/05-11-2026 : version simplifiée, pas de classe
Fonction :
Ce composant administre la base de donnée des UIDs et permet le téléchargement des logs
La page Admin est http://IP_serveur/Admin.html
La page est accessible si l'UID courant est le badge de l'administrateur
Elle reste aussi accessible tant que pinADMIN est à LOW (mode maintenance)
*/
#pragma once
#include <stdint.h>

/*!
 * @fn bool AdminUID( bool isAdmin, uint16_t secondes = 5*60 )
 * @brief Appelée quand un badge est lu. Autorise l'accès à la page Admin pendant "secondes" secondes.
 *        La page n'est pas envoyée : elle est seulement accessible si quelqu'un la demande.
 * @param [in] isAdmin vrai si la base de données dit que le badge est administrateur
 * @param [in] secondes durée d'autorisation (300 s = 5 mn par défaut, 0 = on retire l'autorisation)
 * @return true si l'accès à la page est autorisé après l'appel
 */
bool AdminUID( bool isAdmin, uint16_t secondes = 5*60 );

/*!
 * @fn bool AdminActive()
 * @brief Indique si l'accès à la page Admin est encore autorisé (badge admin récent, ou pinADMIN à LOW).
 *        A appeler régulièrement dans loop() : c'est ce qui permet à la page de répondre.
 * @return true si la page peut être demandée
 */
bool AdminActive();

/*!
 * @fn uint32_t AdminRestant()
 * @brief Secondes restantes avant que la page ne soit plus accessible.
 * @return nombre de secondes, 0 = inaccessible, 0xFFFFFFFF = pinADMIN à LOW (pas de limite)
 */
uint32_t AdminRestant();