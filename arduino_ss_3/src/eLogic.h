/*
Composant : eLogic
Support   : ESP32
Version/Auteur/Date : V1.0 Ton 20260924
                    : V1.1 Ton 20261001
                    : V2.0 Ton 20261112 : logique ESP32 (badge -> code 2FA -> GRANTED/DENIED)

Fonction :
Programme principal tournant sur l'ESP32, seul module à utiliser les autres modules "e"
Messages reçus du MEGA :  "UID:04A1B2C3"  (badge lu)   "CODE:1234"  (code tapé au clavier)
Réponses vers le MEGA  :  "GRANTED" ou "DENIED"
*/
#pragma once

// à appeler dans setup()
void SetupLogic();

// à appeler dans loop()
void LoopLogic();