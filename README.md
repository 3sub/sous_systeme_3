### Compiler avec CMake

Depuis le terminal VS Code, à la racine du projet :

```cmake -S . -B build```

Puis :

```cmake --build build```

CMake va créer quelque chose comme :

MonProjet/
├── CMakeLists.txt 
├── main.cpp 
├── Personne.cpp 
├── Personne.hpp 
└── build/ 
    ├── ... 
    └── MonProjet.exe 

Tu peux ensuite lancer :

```.\build\MonProjet.exe```
