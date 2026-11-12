/*
Composant : eLog
Support   : ESP32
Version/Auteur/Date : V1.0 DaK 20260923

Fonction :
Ecrit à la fin du fichier de log

il faut proposer une macro qui permette au client d'écrire simplement
espLog( message )
et d'ajouter automatiquement le nom du fichier, de la fonction englobante, le n° de ligne.
l'information millis() est ajoutée par le module.
*/
#pragma once

#ifndef espLog
#define espLog(message) __espLog( TODO compléter la macro
#endif

// Ecrit le message dans le fichier de log.
// espLog( message );


// Ecrit le message dans le fichier de log.
// le formalisme est millis(),__FILE__,__FUNC__,__LINE__,"message"\name
bool __espLog( const char* message, const char* fichier, const char* ligne);