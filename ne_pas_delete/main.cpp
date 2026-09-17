#include <Arduino.h>
#include "cycle_securite.h"

// Configuration des broches pour Arduino MEGA
const CycleConfig_t config_barriere = {
    .pin_fin_haut = 2,
    .pin_fin_bas = 3,
    .pin_moteur_in1 = 8,
    .pin_moteur_in2 = 9,
    .pin_moteur_ena = 10,
    .tempo_mouvement_max_ms = 8000, // 8 secondes
    .tempo_ouverte_ms = 5000,       // 5 secondes
    .capteurs_active_high = false   // Pull-up interne (Active LOW)
};

void setup() {
    Serial.begin(9600);
    // Initialisation du sous-système
    cycle_init(&config_barriere);
}

void loop() {
    // 1. Gestion des messages série reçus
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg += "\n";
        cycle_parse_ascii_message(msg.c_str());
    }

    // 2. Traitement cyclique non bloquant de la FSM et des sécurités
    cycle_process();
}