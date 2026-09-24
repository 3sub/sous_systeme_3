## Architecture du code

| Fichier | Rôle |
| :--- | :--- |
| **mBarriere.h** | Déclaration des fonctions (`barriereUP()`, `barriereDW()`). |
| **mBarriere.cpp** | Code exécutable (pilotage moteur et sécurité). |
| **main.cpp** | Boucle principale (`setup`/`loop`) et coordination. |

Le fichier `.cpp` est obligatoire : le `.h` indique l'existence des fonctions, mais seul le `.cpp` contient leur code réel (évite l'erreur *undefined reference*).

---

## Compilation et téléversement

Actions via la barre inférieure de VS Code (PlatformIO) :

* **Serial Monitor (🔌)** : liste des ports COM disponibles, ce qui confirmera que la carte est détectée.
* **Compilation (✓)** : Vérifie le code sans brancher la carte.
* **Téléversement (→)** : Compile et envoie le programme sur l'Arduino.

---

## Moniteur série (prise électrique)

Ouvre le terminal pour lire les messages (`Serial.println`) et envoyer des commandes (`OK`, `NoK`). 

Inutile de cliquer dessus à chaque lancement : une fois le programme téléversé, l'Arduino s'exécute automatiquement dès qu'il est alimenté.