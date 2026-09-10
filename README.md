### Compiler avec CMake

Depuis le terminal VS Code, à la racine du projet :

```bash
cmake -S . -B build
```

```bash
cmake --build build
```

Tu peux ensuite lancer le programme :

```exe
.\build\Debug\sous_systeme_3.exe
```
Quand refaire cmake -S . -B build ?

Tu dois refaire cette commande lorsque tu modifies la configuration de CMake, par exemple :

tu ajoutes un nouveau fichier .cpp dans CMakeLists.txt
tu changes les options de compilation
tu ajoutes une bibliothèque
tu changes de compilateur
tu supprimes ou recrées le dossier build