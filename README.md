## Yume Project
Projet de L2 Informatique — Université du Mans (LMU)

## Description
Yume Project est un jeu de type danmaku (ou bullet hell). Le danmaku est un sous-genre du shoot-em up caractérisé par de très nombreux projectiles à l'écran. Il se base avant tout sur l'esquive et la dextérité, plus que sur le tir.
Ce projet est librement inspiré de Touhou Project, un des titres les plus connus de ce genre.

## Membres de l'équipe
- DELAUNAY Amaury
- EL ALEM Amy
- KLEIN Samuel
- ABRIBAT Jacques

## Prérequis
- GCC

## Compilation et lancement
Cloner ce dépôt puis dans la racine du dossier :
```bash
make run
```

## Architecture
Le projet utilise un ECS (Entity Component System) avec Sparse Set.

- `src/` - code source (core, components, systems, content)
- `lib/` - headers
- `Assets/` - sprites, musiques et effets sonores

## Contrôles
| Touche | Action |
|--------|--------|
| Flèches | Déplacement |
| Z | Tirer |
| X | Bombe |
| Shift | Focus |
| P | Pause |
| Espace | Valider |

## Planning
Le projet est organisé en 3 phases :

- Phase 1 (semaines 1-3) : Création des systèmes core — render, mouvement, collision, laser, ECS, menus
- Phase 2 (semaines 4-6) : Contenu : ennemis, boss, spellcards, vagues, sprites, musiques
- Phase 3 (semaines 6-9) : Documentation : tests, rapport, soutenance

Rendu : 10 avril 2026 - Présentation : 17 avril 2026

## Documentation
La documentation technique est générée avec Doxygen.
```bash
doxygen Doxyfile
```

La documentation est disponible à : https://royaljackab.github.io/yume_project/
