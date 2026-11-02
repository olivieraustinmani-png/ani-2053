# CONTINUITE — ANI-2053 / Nkentseu / Jenga

> **À lire en entier avant toute action.** Ce fichier est le point d'entrée de
> toute nouvelle session de travail. Si tu es un agent qui reprend ce projet,
> lis-le d'abord, puis applique la section « Protocole de démarrage ».

---

## 0. Le message à coller à un nouvel agent

> Je suis étudiant en ANI-2053 (Niveau 2, ENSPy). Le cours porte sur le moteur
> **Nkentseu** et l'outil de construction **Jenga**. Je dois valider ma note de
> Contrôle Continu par des **sprints hebdomadaires** déposés sur GitHub.
>
> Mon dépôt de rendu est `C:\Users\Austin\Desktop\ani-2053`
> (GitHub : `olivieraustinmani-png/ani-2053`, branche `main`).
> Le moteur est dans `C:\Users\Austin\Desktop\Nkentseu` (workspace Jenga).
> Le fichier `.jenga` du workspace est `C:\Users\Austin\Desktop\Nkentseu\Nkentseu.jenga`.
>
> **Lis `C:\Users\Austin\Desktop\ani-2053\CONTINUITE.md` en entier avant de
> faire quoi que ce soit.** Il contient l'état exact du sprint en cours, la
> méthode de travail, les pièges déjà rencontrés et les commandes qui
> marchent. Ne redécouvre rien : applique le protocole.
>
> Ensuite : propose-moi la prochaine action sous forme de liste courte, dis-moi
> ce que tu vas compiler et où tu vas le déposer. Je valide, tu exécutes.
> **Explique chaque étape** : je dois pouvoir redéfendre le code à l'oral.

---

## 1. Les deux mondes (ne jamais les confondre)

| Monde | Chemin | Rôle | On y fait quoi |
|---|---|---|---|
| **L'usine** | `C:\Users\Austin\Desktop\Nkentseu` | Le moteur + le workspace Jenga | On écrit et on **compile** ici, on lance pour mesurer |
| **Le showroom** | `C:\Users\Austin\Desktop\ani-2053` | Le dépôt de rendu GitHub | On **copie** seulement les fichiers finaux demandés par l'énoncé |

**Règle absolue :** on ne compile **jamais** dans `ani-2053`. On y copie le
`.cpp` final, on y écrit les `.txt`, et on commit.

Un `main.cpp` de travail vit dans
`Nkentseu\Applications\Pworkspace\<Projet>\src\` et son `.jenga` dans
`Nkentseu\Applications\Pworkspace\<Projet>.jenga`.

---

## 2. État du projet — à la date de rédaction de ce fichier

### 2.1 Sprints terminés et poussés

| Sprint | Contenu | État |
|---|---|---|
| **Sprint 5** (chapitre-05) | exo1 → exo10 (NKImage, NKFont, NKAudio, NKMedia) | ✅ poussé |
| Sprint 5 · exo4 (atlas) | rendu **partiel** : l'atlas sort vide | ⚠️ note explicative dans `reponse.txt`, cause identifiée par le prof (police ajoutée sans `NkFontConfig`) |
| **Sprint 6** (chapitre-06) | exo1 à exo4, exo6, exo7 (découverte LAN) | ✅ poussés |
| Sprint 6 · exo5 | le futur qui ne bloque pas | ⏸️ mis en pause temporaire (reprise à la fin) |
| Sprint 6 · exo8 | le message qui compte ses octets | 🔴 **EN COURS / PROCHAIN** |
| Sprint 6 · exo9 → exo10 | aller-retour, joueur fantôme | ⏳ à faire |

Le QCM du Sprint 5 a été validé **10/10**.

### 2.2 Le fichier de suivi

`C:\Users\Austin\Desktop\ani-2053\exercice_à_restant_à_faire.md` contient
**tous les énoncés du Sprint 6** (cours, QCM, 10 exercices avec barèmes et
pièges). C'est la source de vérité pour ce qui est demandé.

⚠️ **Ce fichier n'est PAS un livrable** : il est local, ne pas le committer
(il a déjà été ajouté puis retiré du dépôt une fois).

### 2.3 Où en est exactement l'exercice 5

- Code écrit : `Nkentseu\Applications\Pworkspace\Futur\src\futur.cpp`
- Projet Jenga : `Nkentseu\Applications\Pworkspace\Futur.jenga`
- **Blocage** : le `NkFuture.h` du **dépôt** référençait des symboles absents
  (`memory::MakeSharedPtr`, `WaitUntil` d'une autre version).
- **Résolution trouvée** : le **kit du 13 octobre 2026** contient des en-têtes
  cohérents entre eux. Il a été installé dans
  `Nkentseu\Build\Kit\NkentseuKit` (l'ancien du 9 octobre est renommé
  `NkentseuKit_old_9oct`).
- **Cause racine la plus probable** : le `.jenga` de `Futur` doit inclure le
  dossier du kit **en premier**, ou mieux, utiliser `usecanvas()` du kit
  (voir §5.3) au lieu des `nkentseudependson` du dépôt.

**Prochaine action pour l'exo 5** — dans cet ordre :
1. Réécrire `Futur.jenga` en mode kit (voir §5.3) ;
2. `jenga build --project Futur --config Debug` et lire **la première**
   erreur (souvent la seule vraie) ;
3. Corriger `futur.cpp` sur l'API réelle du kit (lire le header, pas deviner) ;
4. Lancer en `--mode=get` puis en `--mode=isready`, noter la trame la plus
   longue et le nombre de trames ;
5. Écrire `cadence.txt` avec les 4 étiquettes, copier `futur.cpp` dans
   `ani-2053\chapitre-06\exo5-le_futur_qui_ne_bloque_pas\`, commit + push.

---

## 3. La méthode de travail (à reproduire telle quelle)

### 3.1 Le cycle en 7 étapes, pour chaque exercice

1. **Lire l'énoncé en entier** dans le `.md` de suivi, y compris :
   - « Ce que vous rendez » → noms **exacts** des fichiers ;
   - « Ce que la machine vérifie » → ce qui doit littéralement apparaître
     dans le code (`Load`, `BytesPP`, `Enqueue`, `Join`, `NkChrono`…) ;
   - « Les pièges » → ils correspondent presque toujours à un point perdu ;
   - **le barème** → il dicte la forme du `.txt` de réponse.
2. **Vérifier l'API réelle dans le code source** avant d'écrire :
   chercher la classe, lire la signature, ne jamais inventer un nom.
3. **Écrire le code de travail** dans `Nkentseu\Applications\Pworkspace\...`.
4. **Compiler**, corriger jusqu'à un build propre (Debug **et** Release quand
   c'est demandé).
5. **Exécuter et mesurer** : les `.txt` ne contiennent que des chiffres réels.
6. **Copier les livrables** dans `ani-2053\chapitre-0N\exoN-.../` et écrire
   les `.txt` avec **les étiquettes exactes de l'énoncé**.
7. **`git add` du dossier, commit explicite, push**, puis vérifier avec
   `git ls-files` que les 3-4 fichiers attendus sont bien versionnés.

### 3.2 Les règles de qualité du prof

- **Jamais `using namespace`** : tout est qualifié (`nkentseu::`, `std::`).
- Les `.txt` de réponse doivent décrire **les symptômes**, pas seulement les
  causes (« le programme se fige sans rien dire » vaut plus que « deadlock »).
- Le format de sortie est **strict** : une espace en trop fait échouer.
- Noms de fichiers : **ne se négocient pas**, chemin non plus.
- Pas de nom ni de matricule dans les fichiers rendus.

### 3.3 Avant chaque push — la checklist qui a déjà sauvé une note

```powershell
cd C:\Users\Austin\Desktop\ani-2053
git status --short
git ls-files chapitre-06/exoN-...      # les fichiers attendus sont-ils suivis ?
```
Comparer cette liste **mot pour mot** à la section « Ce que vous rendez ».
Un fichier manquant = rendu refusé avant même la relecture.

---

## 4. Où chercher (carte du dépôt Nkentseu)

### 4.1 Le moteur

| Besoin | Chemin |
|---|---|
| Types de base (`int32`, `uint32`, `float32`) | `Kernel\Foundation\NKCore\src\NKCore\` |
| Atomics (`NkAtomicInt`) | `Kernel\Foundation\NKCore\src\NKCore\NkAtomic.h` |
| Fils, pool, mutex, promesse, futur | `Kernel\System\NKThreading\src\NKThreading\` |
| Temps (`NkChrono`, `NkClock`) | `Kernel\System\NKTime\src\NKTime\` |
| Journalisation (`logger.Info`) | `Kernel\System\NKLogger\src\NKLogger\` |
| Fenêtre (`NkWindow`, `NkEvent`) | `Kernel\Runtime\NKWindow\src\NKWindow\` |
| Rendu 2D (`NkRenderWindow`, `NkRenderer2D`) | `Kernel\Runtime\NKCanvas\src\NKCanvas\` |
| Coquille d'app (`NkCanvasApp`) | `NKCanvas\src\NKCanvas\App\NkCanvasApp.h` |
| Images (`NkImage`) | `Kernel\Runtime\NKImage\src\NKImage\Core\NkImage.h` |
| Polices (`NkFontAtlas`) | `Kernel\Runtime\NKFont\src\NKFont\NkFont.h` |
| Réseau | `Kernel\System\NKNetwork\src\NKNetwork\` |

### 4.2 Le kit du prof (13 octobre 2026)

```
C:\Users\Austin\Desktop\Nkentseu\Build\Kit\NkentseuKit\
├── Canvas.jenga              ← le fichier à charger (usecanvas())
├── KIT.txt                   ← notes d'installation
├── include\                  ← en-têtes (NKCanvas, NKNetwork, NKThreading...)
└── lib\Debug-Windows\        ← 20 archives .lib déjà compilées
    lib\Release-Windows\
```

Le kit est autosuffisant : **on ne recompile pas le moteur pour un exercice**.

### 4.3 Les exemples qui marchent (à imiter)

- `Applications\NkDames\src\main.cpp` — squelette propre de `NkCanvasApp`
- `Applications\NkDames\src\Dames\NkDamesJeu.cpp` — config dans le constructeur
- `Applications\Pworkspace\` — **tes propres projets** (Reserve, Course,
  Corrige, Etreinte, Futur, Parle) : c'est la meilleure référence
- `Kernel\System\NKThreading\tests\test_smoke.cpp` — usage réel de `NkThread`

---

## 5. Les pièges déjà payés (ne pas les repayer)

### 5.1 Ordre des `#include` — LE piège n°1 du dépôt

`NKTime` déclare un **namespace `time`**. Si `NKMath.h` (qui appelle la
fonction C `time(nullptr)`) est compilé **après**, le compilateur échoue avec
`unexpected namespace name 'time'`.

**Solution, toujours :**
```cpp
#include "NKWindow/NKMain.h"   // EN PREMIER, systématiquement
#include "NKThreading/NkThread.h"
// ... le reste
```

### 5.2 `NkThread` attend une **lambda**, pas un pointeur de fonction

```cpp
// FAUX
fils.emplace_back(MaFonction);
// JUSTE
fils.emplace_back([](void *) { MaFonction(nullptr); });
```
`NkThreadPool::Enqueue` attend **`void()`** (zéro argument), `NkThread` attend
**`void(void*)`**. Deux signatures différentes : ne pas les confondre.

### 5.3 Se lier au kit au lieu du dépôt

Le `.jenga` d'un projet d'exercice doit ressembler à ceci (forme sûre) :

```python
import os
from Jenga import *
from jengaconfig import *
from Jenga.GlobalToolchains import RegisterJengaGlobalToolchains

# 1) Charger le KIT une seule fois au niveau workspace
useconfig("C:/Users/Austin/Desktop/Nkentseu/Build/Kit/NkentseuKit/Canvas.jenga")

with project("MonExo"):
    RegisterJengaGlobalToolchains()
    consoleapp()            # ou windowedapp()
    language("C++")
    cppdialect("C++20")     # NKThreading exige C++20
    files(["MonExo/src/main.cpp"])
    usecanvas(["NKThreading", "NKTime", "NKCore"])  # modules demandés
```
et l'enregistrer dans `Nkentseu.jenga` :
```python
with include("Applications/Pworkspace/MonExo.jenga"):
    pass
```

> **Ne jamais inclure les `.jenga` du moteur d'origine** quand on utilise le
> kit : les archives sont déjà compilées (voir l'avertissement dans
> `Canvas.jenga`).

### 5.4 Le build Jenga

- On lance **depuis la racine** : `cd C:\Users\Austin\Desktop\Nkentseu`
- Commande : `jenga build --project <Nom> --config Debug` (ou `Release`)
- Le build peut durer **3 à 10 minutes** la première fois : le lancer en
  arrière-plan (`Start-Process ... -RedirectStandardOutput log.txt`) et lire
  `log.txt`.
- **Toujours lire la première erreur du compilateur**, pas la dernière.
  Une erreur en cascade en produit dix autres.
- Une erreur de lien (`undefined reference`, `cannot find -lX`) envoie vers le
  `.jenga` (liste `links` / `files`), pas vers le code.

### 5.5 Mesures

- Toujours en **Release** pour les temps (`optimize("Speed")`).
- Toujours **trois mesures** et garder la **médiane** : la première paie des
  chargements.
- En Debug, les écarts de temps ne veulent rien dire — mais les **valeurs
  fausses** (course, deadlock) ne se voient qu'en Debug.
- Un Release peut **retirer du travail** dont le résultat n'est jamais lu :
  verser le résultat dans un atomique global et l'afficher.

### 5.6 Git

- Un commit **par exercice**, message explicite :
  `Sprint 6 - exoN : <titre> (<technique>)`
- `git add` du **dossier de l'exo uniquement**, jamais `git add -A`.
- Ne jamais committer : le `.md` de suivi, `Build/`, les `.exe`, les images de
  test locales.
- Le push peut afficher le texte de progression de git sur stderr : ce n'est
  **pas** une erreur.

---

## 6. Le Sprint 6 — ce qu'il reste, exercice par exercice

| # | Titre | Fichiers à rendre | Module |
|---|---|---|---|
| 5 | Le futur qui ne bloque pas | `futur.cpp`, `cadence.txt` | NKThreading + NKCanvas |
| 6 | Deux machines se parlent | `parle.cpp`, `echange.txt` | NKNetwork |
| 7 | La découverte | `decouverte.cpp`, `serveurs.txt` | NKNetwork (`NkDiscovery`) |
| 8 | Le message qui compte ses octets | `octets.cpp`, `tailles.txt` | NKNetwork (`NkBitWriter/Reader`) |
| 9 | L'aller-retour | `allerretour.cpp`, `mesures-rtt.txt`, `rtt.txt` | NKNetwork |
| 10 | Le joueur fantôme | `fantome.cpp`, `fantome.txt` | NKNetwork |

**Contrainte commune aux exos 6 → 10** : le kit du **13 octobre 2026** est
obligatoire (NKNetwork absent du kit du 9 octobre). Les essais réseau se font
sur **une seule machine en `127.0.0.1`** quand il n'y en a pas deux — et il
faut le **dire honnêtement** dans le `.txt`.

---

## 7. Ce que l'étudiant doit savoir défendre à l'oral

Pour chaque exercice, il doit pouvoir répondre sans le code sous les yeux :

- **exo1** : pourquoi le pool gagne (coût de création d'un fil ≈ pile + noyau
  + planification), et pourquoi l'écart ne se referme pas avec la tâche ×100.
- **exo2** : lecture / addition / écriture séparées ; pourquoi Release donne
  le bon total (registre par fil) alors que le code reste faux.
- **exo3** : ce que paie chaque version — attente du verrou (mutex),
  cohérence de ligne de cache (atomique), rien du tout (locale).
- **exo4** : attente circulaire ; la règle d'ordre des verrous.
- **exo5** : `Get` bloque, `IsReady` ne bloque pas ; la promesse doit vivre
  jusqu'au `SetValue`.
- **exo6 → exo10** : voir les énoncés du `.md` de suivi.

---

## 8. Si quelque chose casse

1. **Nommer l'étage** : chargement Jenga (erreur Python) / compilation C++ /
   édition de liens / exécution. On ne cherche pas au même endroit.
2. **Lire la première erreur**, la citer telle quelle.
3. **Regarder le header** du module concerné avant de changer le code.
4. **Ne pas changer dix choses à la fois** : une correction, un rebuild.
5. Si un build échoue après une mise à jour du kit : supprimer
   `Nkentseu\Build\obj\...\<Projet>` (objets périmés) et reconstruire.

---

## 9. Ce qui n'est PAS une erreur de l'étudiant

- `VertexError 400 INVALID_ARGUMENT` / « number of function response parts… » :
  c'est l'API de l'assistant qui a perdu un tour d'appel d'outil. Il faut
  **relancer la demande** ou ouvrir une nouvelle conversation. Aucun rapport
  avec le code C++.
- Le pare-feu Windows qui demande l'autorisation au premier lancement d'un
  serveur : normal.
- Le push qui écrit sur stderr : normal.
