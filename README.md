### Compiler avec CMake

Depuis le terminal VS Code, à la racine du projet :

```cmake -S . -B build```

Puis :

```cmake --build build```

Tu peux ensuite lancer :

```.\build\debug\sous_systeme_3.exe```


Quand refaire ```cmake -S . -B build``` ?

Tu le refais lorsque tu modifies la configuration CMake, par exemple :

tu ajoutes un nouveau fichier .cpp dans CMakeLists.txt
tu changes les options de compilation
tu ajoutes une bibliothèque
tu changes le compilateur
tu supprimes/recrées le dossier build