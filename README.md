# Moonlight
Projet de L2 Informatique — Université du Mans (LMU)

## Description
...

## Membres de l'équipe
- DELAUNAY Amaury
- EL ALEM Amy
- KLEIN Samuel
- ABRIBAT Jacques

## Prérequis
- GCC
- Raylib 5.5
- macOS : `brew install raylib`
- Linux : `sudo apt install libraylib-dev`
- Windows : bibliothèque incluse dans `raylib/`

## Compilation et lancement
```bash
cd src
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
- Phase 2 (semaines 4-6) : Contenu — ennemis, boss, spellcards, vagues, sprites, musiques
- Phase 3 (semaines 6-9) : Documentation, tests, rapport, soutenance

Rendu : 10 avril 2026 - Présentation : 17 avril 2026

## Documentation
La documentation technique est générée avec Doxygen.
```bash
doxygen Doxyfile
```

## État du projet
- [x] Système ECS
- [x] Joueur (mouvement, tir)
- [x] Système audio
- [x] HUD
- [x] Lasers (loose et straight)
- [x] Ennemis (spawn, vie)
- [ ] Collisions
- [ ] Boss
- [ ] Structure du stage
- [ ] Tests unitaires