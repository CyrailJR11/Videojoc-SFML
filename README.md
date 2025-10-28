# Videojoc-SFML
Tasca videojoc fet amb SFML 3.0

# Videojoc SFML

Projecte desenvolupat amb C++ i la llibreria SFML. Joc tipus Timber on el jugador talla un arbre evitant branques mentre el temps s'esgota.

## Descripció

Aquesta versió inclou millores semiprofessionals demanades per La Salle Studios:

* Optimització del recompte d'FPS i puntuació.
* HUD amb informació de vides, dificultat i temps restant.
* Elements decoratius amb diferents capes de profunditat.
* Rectangles semitransparents per millorar la llegibilitat.
* Nous NPCs i lògica d'obstacles.

## Controls

* **Enter**: iniciar/reiniciar la partida
* **Fletxa esquerra / dreta**: tallar arbre i moure jugador
* **Esc**: sortir del joc

## Requeriments

* C++17 o superior
* SFML 2.6 (gràfics, àudio)
* Fitxers de textures dins carpeta `graphics/`
* Fonts dins carpeta `fonts/`

## Instal·lació i execució

```bash
g++ main.cpp funcionsaux.cpp -o joc -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./joc
```

## Millores implementades

* Actualització d'FPS cada 0.25s per millor rendiment.
* Consola de depuració visual (FPS, vides, dificultat).
* Depth layering: arbres decoratius darrere i davant del jugador.
* HUD millorada amb fons semitransparents.
* Nou sistema modular preparat (funcions aux, classes NPC/Enemy).

## Futures expansions

* Menú principal i sistema de pauser.
* Sons d'efectes i música dinàmica.
* Rank de puntuacions i nivells.

## Autors

Projecte creat per l'equip de desenvolupament La Salle — 2025.
