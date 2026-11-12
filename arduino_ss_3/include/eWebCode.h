/*
Composant : eWEB
Support   : ESP32
Version/Auteur/Date : V1.0 DaK 20260923

Fonction :
Ce composant initialise le pile TCP de l'ESP32 et génère la page 2FA contenant le code secet temporaire
La page 2FA est http://IP_serveur/index.html
*/
#pragma once

// initialise la pile TCP, crée le point d'accès, le serveur, la page par défaut.
// peut être appelé plusieures fois, renvoie l'état
bool WebInit();

// renseigne la page par défaut avec le code 4 chiffres passé en paramètre
// renvoie vrai tant qu'on est à l'intérieur de la durée permise (pour un même code)
bool Affiche2FA( const std::string codeTemporaire, uint32_t secondes = 5*60 );