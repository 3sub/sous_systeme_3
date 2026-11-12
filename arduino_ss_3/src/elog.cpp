/*
Composant : eLog
Support   : ESP32 (framework Arduino)
Version/Auteur/Date : V1.1 DaK et AJM 2026/10/08

Implémentation de espLogWrite() : ajoute une ligne à la fin du fichier de log.
Format écrit : millis(),fichier,fonction,ligne,"message"\n
*/
#include "eLog.h"

#include <Arduino.h>
#include "LittleFS.h"

// Chemin du fichier de log (à adapter si besoin)
#ifndef ELOG_FILE_PATH
#define ELOG_FILE_PATH "/elog.txt"
#endif

static bool s_fsPret = false;

// Monte le système de fichiers à la première utilisation.
static bool eLogInitFs()
{
    if ( s_fsPret ) return true;
    // true = formate la partition si le montage échoue
    s_fsPret = LittleFS.begin( true );
    return s_fsPret;
}

// Ne garde que le nom du fichier (sans le chemin complet de __FILE__).
static const char* eLogNomFichier( const char* chemin )
{
    if ( chemin == nullptr ) return "";
    const char* nom = chemin;
    for ( const char* p = chemin; *p != '\0'; ++p )
    {
        if ( *p == '/' || *p == '\\' ) nom = p + 1;
    }
    return nom;
}

bool espLogWrite( const char* message, const char* fichier, const char* fonction, int ligne )
{
    if ( !eLogInitFs() ) return false;

    File f = LittleFS.open( ELOG_FILE_PATH, FILE_APPEND );
    if ( !f ) return false;

    // millis(),fichier,fonction,ligne,"message"
    f.print( millis() );
    f.print( ',' );
    f.print( eLogNomFichier( fichier ) );
    f.print( ',' );
    f.print( fonction != nullptr ? fonction : "" );
    f.print( ',' );
    f.print( ligne );
    f.print( ",\"" );

    // Les guillemets du message sont doublés (convention CSV)
    if ( message != nullptr )
    {
        for ( const char* p = message; *p != '\0'; ++p )
        {
            if ( *p == '"' ) f.print( '"' );
            f.print( *p );
        }
    }
    f.print( "\"\n" );

    f.close();
    return true;
}
