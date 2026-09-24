## Architecture du code

Le projet est structuré en trois fichiers distincts pour respecter les principes de la programmation C/C++ et garantir la modularité du système :

| Fichier | Rôle |
| :--- | :--- |
| **mBarriere.h** | **Déclaration** : Liste les prototypes de fonctions disponibles (`barriereUP()`, `barriereDW()`, etc.). |
| **mBarriere.cpp** | **Implémentation** : Contient le code exécutable qui pilote les moteurs, la logique matérielle et la sécurité. |
| **main.cpp** | **Orchestration** : Gère la boucle principale (`setup` et `loop`) et appelle les fonctions du module. |

### Pourquoi le fichier `.cpp` est indispensable
* **Règle de compilation C++** : Le fichier `.h` informe le compilateur des fonctions disponibles, mais le fichier `.cpp` fournit le code binaire réel. Sans lui, l'étape d'édition de liens échoue avec l'erreur `undefined reference`.
* **Modularité et propreté** : Le composant barrière reste indépendant, réutilisable dans d'autres projets et séparé de la logique globale du `main.cpp`.

---

## Compilation et téléversement

Les commandes principales s'exécutent depuis la barre d'état située en bas de Visual Studio Code (extension PlatformIO) :

1. **Vérifier le code (Compilation)** :
   Cliquer sur la **coche (✓)**. Cette étape vérifie l'absence d'erreurs dans le code sans l'envoyer à la carte (branchement USB non requis).

2. **Envoyer le programme (Téléversement)** :
   Cliquer sur la **flèche vers la droite (→)**. Cette étape compile le projet puis l'injecte dans la mémoire flash de l'Arduino (la carte doit être branchée en USB).

---

## Fonctionnement du Moniteur Série (icône prise électrique)

L'icône en forme de **prise électrique** correspond au **Device Monitor** (Moniteur Série).

* **Faut-il le lancer à chaque démarrage de VS Code ?**
  Non. Une fois le programme téléversé sur l'Arduino via le bouton flèche (→), la carte enregistre le code dans sa mémoire permanente. L'Arduino s'exécute de façon autonome dès qu'il est alimenté en électricité, sans nécessiter ce bouton.

* **À quoi sert cette fonction ?**
  Elle permet de recevoir et d'envoyer des données textuelles entre le PC et la carte via le câble USB :
  * **Affichage** : Lire les messages émis par `Serial.println()` (ex : logs de diagnostic, détection d'intrusion).
  * **Commande** : Saisir manuellement des ordres dans le terminal (ex : taper `OK`, `NoK` ou `IN` pour tester les réactions de la barrière).