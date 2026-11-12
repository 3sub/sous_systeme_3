/*
Composant : eLog
Support   : ESP32
Version/Auteur/Date : V1.1 DaK et AJM 2026/10/08

Fonction :
Ecrit à la fin du fichier de log

Macro espLog( message ) : ajoute automatiquement le nom du fichier,
de la fonction englobante et le n° de ligne.
L'information millis() est ajoutée par le module.
*/
#pragma once

#include <stdbool.h>

// Ecrit le message dans le fichier de log.
// Usage : espLog( "message" );
#ifndef espLog
#define espLog(message) __espLogWrite( (message), __FILE__, __func__, __LINE__ )
#endif

// Fonction réellement appelée par la macro.
// Format écrit : millis(),fichier,fonction,ligne,"message"\n
// Retourne true si l'écriture a réussi.
bool __espLogWrite( const char* message, const char* fichier, const char* fonction, int ligne );