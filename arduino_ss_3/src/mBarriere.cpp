#include "mBarriere.h"
#include <Arduino.h>

// Pointeur de fonction pour enregistrer la callback d'intrusion
static void (*callbackIntrusionFunc)() = NULL;

// État courant de la barrière (false = fermée/arrêtée, true = en mouvement/ouverte)
static bool etatMoteur = false;

void barriereUP() {
    etatMoteur = true;
    // Logique de commande des relais pour lever la barrière
    Serial.println("[mBarriere] Ordre barriereUP execute.");
}

void barriereDW() {
    etatMoteur = false;
    // Logique de commande des relais pour fermer la barrière
    Serial.println("[mBarriere] Ordre barriereDW execute.");
}

bool barriere() {
    return etatMoteur;
}

void callbackBarriere(void (*intrusion)()) {
    callbackIntrusionFunc = intrusion;
}