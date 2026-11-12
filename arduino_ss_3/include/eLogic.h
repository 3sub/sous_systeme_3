/*
Composant : mLogic
Support   : ESP32
Version/Auteur/Date : V1.0 Ton 20260924
                    : V1.1 Ton 20261001

Fonction :
Programme principal tournant sur le ESP32
module à utiliser la autres modules concernant ESP32
*/


#include "eAdmin.h"
#include "eBDD.h"
#include "eFileSystem.h"
#include "eGenCode.h"
#include "eLog.h"
#include "eLogic.h"
#include "eSerial.h"
#include "eWebCode.h"

void gererEntree() {
    if (!detectionAvVehicule()) {
        return;
    }

    char codeRFID[32] = {0};
    if (!lireRFID(codeRFID)) {
        return;
    }

    if (!comparerBDD(codeRFID)) {
        return;
    }

    char otpGeneres[8] = {0};
    genererOTP(otpGeneres);

    afficherOTP(otpGeneres);

    char otpSaisi[8] = {0};
    while (!lireOTP(otpSaisi)) {
        if (!detectionAvVehicule()) {
            return;
        }
    }

    if (!comparerOTP(otpSaisi, otpGeneres)) {
        return;
    }

    while (!ouvrirBarriere());

    while (!detectionApVehicule() and !detectionAvVehicule());

    while (!fermerBarriere());
}

void gererSortie() {
    if (!detectionApVehicule()) {
        return;
    }

    while (!ouvrirBarriere());

    while (detectionApVehicule() and !detectionAvVehicule());

    while (!fermerBarriere());
}

void LoopLogic() {
    gererEntree();
    gererSortie();
}
