#include "mBarriere.h"
#include <Arduino.h>

// Pointeur de fonction pour enregistrer la callback d'intrusion
static void (*callbackIntrusionFunc)() = NULL;

<<<<<<< HEAD
// État courant de la barrière (false = fermée/arrêtée, true = en mouvement/ouverte)

void barriereUP() {
=======
// État courant (true = en mouvement/ouverte, false = fermée/arrêtée)
static bool etat = false;

void barriereUP() {
    etat = true;
>>>>>>> eef0d8d (update)
    // Logique de commande des relais pour lever la barrière
    Serial.println("[mBarriere] Ordre barriereUP execute.");
}

void barriereDW() {
<<<<<<< HEAD
=======
    etat = false;
>>>>>>> eef0d8d (update)
    // Logique de commande des relais pour fermer la barrière
    Serial.println("[mBarriere] Ordre barriereDW execute.");
}

<<<<<<< HEAD
=======
bool barriere() {
    return etat;
}

>>>>>>> eef0d8d (update)
void callbackBarriere(void (*intrusion)()) {
    callbackIntrusionFunc = intrusion;
}
