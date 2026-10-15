#include "mBarriere.h"
#include <Arduino.h>

// Pointeur de fonction pour enregistrer la callback d'intrusion
static void (*callbackIntrusionFunc)() = NULL;

// État courant de la barrière (false = fermée/arrêtée, true = en mouvement/ouverte)

void barriereUP() {
    // Logique de commande des relais pour lever la barrière
    Serial.println("[mBarriere] Ordre barriereUP execute.");
}

void barriereDW() {
    // Logique de commande des relais pour fermer la barrière
    Serial.println("[mBarriere] Ordre barriereDW execute.");
}

void callbackBarriere(void (*intrusion)()) {
    callbackIntrusionFunc = intrusion;
}
