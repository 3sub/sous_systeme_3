/*
Composant : eSerial
Support   : ESP32
Version/Auteur/Date : V1.0 DaK 20260923
Version/Auteur/Date : V2 Mounir ARROUBIOU 2026/09/24 Prise en charge du module

Fonction :
Gère la communication entre MEGA et ESP
On échange des chaînes de caractères au format C.
\n est interprété comme fin de chaîne
\0 et \r sont filtrés et supprimés
\0 est ajouté automatiquement en fin de chaîne en remplacement du \n dernier caractere reçu
*/
#pragma once

/**
 * @brief Initialise la liaison série dédiée au MEGA.
 * @param serialPort Référence vers l'instance HardwareSerial 
 * @param vitesseLiaison Vitesse de communication en bauds 
 * @param rxPin Broche réception (RX).
 * @param txPin Broche émission (TX).
 */
void beginSerialMEGA(HardwareSerial &serialPort, unsigned long vitesseLiaison = 9600, int8_t rxPin = -1, int8_t txPin = -1);

/**
 * @brief Envoie la chaîne "GRANTED\n" sur Tx.
 */
void WrGRANTED();

/**
 * @brief Envoie la chaîne "DENIED\n" sur Tx.
 */
void WrDENIED();

/**
 * @brief Lit les données reçues sur Rx.
 * @param buffer Pointeur vers le tableau de caractères de destination.
 * @param lenmaxbuffer Taille maximale du buffer de destination.
 * @return uint32_t Nombre de caractères lus 
 */
uint32_t readMEGA(char* buffer, uint16_t lenmaxbuffer);

/**
 * @brief Définit la fonction de rappel (callback) exécutée à la réception d'un message.
 * @param messageRecu Pointeur vers la fonction à exécuter.
 */
void callbackMEGA(void (*messageRecu)());

