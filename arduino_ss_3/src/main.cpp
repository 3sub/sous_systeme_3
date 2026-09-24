#include <Arduino.h>
#include "pinOut.h"
#include "mBarriere.h"

// Fonction de sécurité déclenchée sur intrusion
void surIntrusion(void) {
    Serial.println("ALERTE : Tentative d'ouverture forcee detectee !");
}

void setup() {
    Serial.begin(9600);
    
    // Configuration des broches du moteur et capteurs issues de pinOut.h
    pinMode(PIN_FIN_HAUT, INPUT_PULLUP);
    pinMode(PIN_FIN_BAS, INPUT_PULLUP);
    pinMode(PIN_MOTOR_IN1, OUTPUT);
    pinMode(PIN_MOTOR_IN2, OUTPUT);
    pinMode(PIN_MOTOR_ENA, OUTPUT);

    // Enregistrement du callback de sécurité
    callbackBarriere(surIntrusion);
    
    Serial.println("Sous-systeme 3 (mBarriere) initialise.");
}

void loop() {
    // Interprétation des commandes ASCII reçues
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim();

        if (msg == "OK") {
            Serial.println("Commande OK -> Ouverture barriereUP()");
            barriereUP();
        } 
        else if (msg == "NoK" || msg == "TimeOut") {
            Serial.println("Commande NoK/TimeOut -> Fermeture barriereDW()");
            barriereDW();
        } 
        else if (msg == "IN" || msg == "OUT") {
            Serial.println("Vehicule detecte -> Attente decision");
        }
    }
}