/*
Composant : mBarriere
Support   : MEGA
Version/Auteur/Date : V1.0 DaK 20260923

Fonction :
Gère le moteur qui lève en rabaisse la barrière

Relai pinMotorUP 
	NF  <-- GND
	COM <-- Moteur
	O   <-- +VCC
	
Relai pinMotorDW 
	NF  <-- GND
	COM <-- Moteur
	O   <-- +VCC
	
Utilise les fins de course pour arréter les mouvements du moteur, et maintenir la barrière ouverte.
Ne maintient pas la barrière fermée, mais signale une tentative d'ouverture forcée
*/
#pragma once


void barriereUP(); // lance le levage de la barrière. non bloquant.
void barriereDW(); // lance la fermuture de la barrière. non bloquant.
bool barriere();   // renvoie l'état courant

// permet de définir une fonction a appeler lorsque le FC bas change d'état sans ordre barriereUP()
void callbackBarriere( void (*intrusion)() );