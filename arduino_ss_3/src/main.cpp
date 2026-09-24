#include <Arduino.h>
#include "mBarriere.h"

// Fonction de callback déclenchée en cas d'ouverture forcée / intrusion (FC bas modifié sans ordre UP)
void surIntrusion() {
    Serial.println("ALERTE / DEFAUT : Tentative d'ouverture forcee detectee !");
}

void setup() {
    Serial.begin(9600);
    
    // Attachement de la fonction de sécurité/intrusion au module mBarriere
    callbackBarriere(surIntrusion);
    
    Serial.println("Sous-systeme 3 (mBarriere) initialise.");
}

void loop() {
    // 1. Interprétation des commandes ASCII reçues sur la liaison série
    if (Serial.available() > 0) {
        String msg = Serial.readStringUntil('\n');
        msg.trim(); // Nettoie les caractères invisibles (\r, \n, espaces)

        if (msg == "OK") {
            Serial.println("Commande OK reçue -> Lancement de l'ouverture (barriereUP)");
            barriereUP();
        } 
        else if (msg == "NoK" || msg == "TimeOut") {
            Serial.println("Commande NoK/TimeOut reçue -> Lancement de la fermeture (barriereDW)");
            barriereDW();
        } 
        else if (msg == "IN" || msg == "OUT") {
            Serial.println("Vehicule detecte (IN/OUT) -> Attente de la decision d'autorisation...");
        } 
        else if (msg.length() > 0) {
            Serial.print("Commande inconnue : ");
            Serial.println(msg);
        }
    }

    // Le module mBarriere gère ses fins de course et l'arrêt du moteur de façon non-bloquante.
}