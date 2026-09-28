/*
Composant : mBarriere
Support   : MEGA / UNO
Version/Auteur/Date : V2.0 DaK / B. Widerkher 20260924

Fonction :
Gère le moteur qui lève et rabaisse la barrière.

Câblage des Relais :
    Relai pinMotorUP (IN1 - Montée) :
        NF  (Normalement Fermé) <-- GND
        COM (Commun)             <-- Borne 1 Moteur
        NO  (Normalement Ouvert) <-- +VCC
        
    Relai pinMotorDW (IN2 - Descente) :
        NF  (Normalement Fermé) <-- GND
        COM (Commun)             <-- Borne 2 Moteur
        NO  (Normalement Ouvert) <-- +VCC

Utilise les fins de course pour arrêter les mouvements du moteur et maintenir la barrière ouverte.
Signale toute tentative d'ouverture forcée (intrusion).
*/
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Lance le levage de la barrière (non bloquant)
void barriereUP(void);

// Lance la fermeture de la barrière (non bloquant)
void barriereDW(void);

// Renvoie l'état courant du moteur (true = en mouvement/ouverte, false = fermée/arrêtée)
bool barriere(void);

// Permet de définir une fonction à appeler lorsque le FC bas change d'état sans ordre barriereUP()
void callbackBarriere(void (*intrusion)(void));

#ifdef __cplusplus
}
#endif