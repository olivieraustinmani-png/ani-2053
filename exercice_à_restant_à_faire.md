Temps restant
2 h 33 min
Ferme le 02 nov. 2026, 23:59

Sprint 8
Dans le livre : chapitre 13.

Ce que vous saurez faire en sortant de ce chapitre. Nommer neuf motifs de conception et, pour huit d'entre eux, montrer le fichier du moteur où ils se trouvent. Non pas tels qu'un livre les dessine, mais tels qu'ils existent ici, avec leurs raisons et leurs fautes.

Ce chapitre met des noms sur ce que vous avez déjà vu au chapitre précédent en lisant le code. C'est l'ordre qui compte : on nomme après avoir rencontré, pas avant.

Les neuf, et où ils vivent
Stratégie — un comportement qu'on remplace. En C++ moderne, c'est parfois un simple paramètre de fonction.

Fabrique — elle se reconnaît à ce que les noms des classes concrètes n'apparaissent nulle part ailleurs. Celle du moteur impose en plus un allocateur, ce qui n'est pas dans le nom du motif.

Façade — une porte simple devant un ensemble compliqué.

Patron de méthode — la classe de base impose un ordre que votre code ne montre pas. Quand vous héritez et que rien ne marche dans l'ordre attendu, la réponse est dans la classe de base, pas chez vous.

Observateur — on prévient qui écoute. Distinguez le push, où l'on s'abonne et où l'on est appelé quand l'événement survient, du pull, où personne ne vous prévient : c'est vous qui demandez. Dans ce moteur, Dispatch l'écrit avec des fonctions anonymes, pas avec une classe abstraite Listener.

Singleton — un sans état est défendable, un avec état ne l'est pas. La différence n'est pas de style : ce qui garde un état devient un point de rendez-vous invisible entre des parties qui ne se connaissent pas.

Adaptateur, Composite — celui du moteur a été cassé pendant des mois — et Commande.

La règle honnête
On introduit un motif au deuxième cas, jamais au premier.

Le code écrit pour placer des motifs se reconnaît : une fabrique qui ne crée qu'une classe, une stratégie qui n'a qu'une mise en œuvre, une interface par classe concrète. C'est du travail en plus pour une souplesse dont personne n'a besoin.

Ce qu'il ne faut pas croire. Qu'un bon programme applique beaucoup de motifs. Un bon programme résout son problème ; les motifs y apparaissent parce que les mêmes problèmes ont les mêmes formes de solution.

Ni que les motifs sont indépendants du langage. Ils prétendent l'être et ne le sont pas : la moitié des motifs classiques compense des manques du C++ d'une certaine époque. Le Singleton est souvent une variable globale à qui on a mis une cravate. Regardez ce que le langage vous donne avant de réciter le catalogue.

Et surtout : reconnaître un motif ne suffit pas à comprendre le code. Le nom vous donne la forme, jamais les raisons. Une fabrique qui impose un allocateur, un patron de méthode qui impose un ordre invisible, un singleton défendable parce qu'il ne garde rien, un composite cassé pendant des mois : rien de tout cela n'est dans le nom.

Le nom est un raccourci pour se parler entre développeurs. Le fichier reste à lire.

À savoir aussi
Dans Dispatch, la valeur de retour du gestionnaire dit si l'événement est consommé. S'il rend true, l'événement est marqué et les abonnés suivants ne le voient pas : un panneau garde ainsi un clic.
Le push convient à ce qui arrive : une touche pressée, un fichier déposé, une fenêtre fermée. Le pull convient à ce qui dure : une touche maintenue, la position de la souris.
L'adaptateur du moteur est NkGuiCanvasBackend : il traduit les listes de dessin de NKGui en appels NKCanvas. NKGui ne connaît donc pas NKCanvas, et un second pont suffirait pour un autre rendu.
Le composite du moteur est la composition des transformations, passée de parent en enfant. Sa panne : le Draw des sprites jetait l'état reçu, donc un sprite ne pouvait pas être l'enfant d'un objet transformé.
Ce résumé et le cours magistral suffisent pour faire les exercices de ce sprint. Le livre du cours va plus loin : il montre le code réel du moteur, les fautes qu'on y a faites, et pourquoi. Il est disponible auprès de l'enseignant.

Moyenne
0
Série 1

QCM — Les motifs, trouvés dans le moteur
0/10 tâches
Choisis une réponse par question.

1. Les cinq backends de NKCanvas derrière une seule interface, c'est :


Un adaptateur

Une fabrique de rendu

Une stratégie interchangeable

Une façade simple
2. On reconnaît qu'une fabrique fait son travail quand :


Elle rend toujours un pointeur intelligent à l'appelant

Elle est déclarée `static` dans un en-tête du moteur

Elle possède une méthode de création par classe concrète

Nul autre fichier ne nomme les classes concrètes
3. `NkRenderer2DFactory` expose `Destroy` parce que :


Le renderer doit être arrêté avant d'être détruit

C'est la convention de nommage du moteur

Elle a alloué avec un allocateur particulier

Le destructeur du renderer est privé
4. Dans un patron de méthode, ce que vous écrivez :


Est appelé une seule fois, au lancement de l'application

Décide de l'ordre d'exécution

S'exécute à un moment fixé par la classe de base

Remplace entièrement la boucle principale du programme
5. Un gestionnaire d'événement qui rend `true` :


Demande à être rappelé à la prochaine trame

Confirme que l'événement était bien du type attendu

Consomme l'événement, caché aux abonnés suivants

Signale une erreur que le répartiteur doit journaliser
6. Le modèle **pull** convient à :


Un fichier déposé sur la fenêtre

Une fenêtre qu'on ferme

Une touche que le joueur garde enfoncée

Un clic sur un bouton
7. `NkInput` est un singleton défendable parce que :


Il vit dans l'espace de noms du moteur

Seul le moteur s'en sert, jamais le jeu

Il est déclaré `inline` dans son en-tête

Il ne garde aucun état
8. `NkGuiCanvasBackend` existe pour que :


L'interface soit dessinée dans une autre fenêtre du système

NKGui puisse être testé

NKGui n'ait pas besoin de connaître NKCanvas

NKCanvas puisse dessiner plus vite
9. Un sprite dont le `Draw` jette l'état reçu :


Dessine au mauvais endroit dans tous les cas, même seul à l'écran

Ne peut pas tourner sur lui-même

Ne peut pas être l'enfant d'un objet transformé

Provoque une erreur de compilation
10. On introduit un motif de conception :


Au deuxième cas

Quand la revue de code le demande

Quand le code dépasse mille lignes

Dès la conception, pour éviter d'y revenir
Déposer
Répondu 0 / 10
Série 2

Exercices — Les motifs, trouvés dans le moteur
0/10 tâches

Le motif et son fichier
Exercice
Où déposer
ani-2053/chapitre-08/exo1-le_motif_et_son_fichier/motifs.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo1-le_motif_et_son_fichier
Les fichiers, au nom exactmotifs.txt
2 missionsles cinq premiers motifs ont leur fichier · les quatre derniers motifs ont leur fichier
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Pour chacun de neuf motifs de conception, trouver dans le kit le fichier et la ligne exacte qui le montrent. Pour celui qui manque, dire où il manque.

Pourquoi ça sert
Un motif appris sur un exemple inventé ne se reconnaît pas le jour où il est devant vous. Un motif trouvé dans un fichier réel, avec sa ligne, se reconnaît la fois suivante.

Comment faire
Voici cinq motifs et le problème que chacun règle. La Stratégie : plusieurs façons de faire une même chose, choisie à l'exécution. La Fabrique : créer un objet sans connaître sa classe exacte, et garder ce choix en un seul endroit.
La Façade : un besoin simple devant un sous-système compliqué. Le Patron de méthode : un traitement au squelette fixe, dont seules quelques étapes changent. L'Observateur : quelque chose arrive, plusieurs parties veulent le savoir, et celle qui l'annonce ne les connaît pas.
Et quatre autres. Le Singleton : un seul objet, que tout le monde peut atteindre. L'Adaptateur : deux morceaux qui devraient marcher ensemble, dont les interfaces ne se correspondent pas. Le Composite : traiter un objet et un groupe d'objets de la même façon. La Commande : une action représentée comme un objet, pour la garder, la rejouer ou la défaire.
Cherchez chacun dans les en-têtes de include/ du kit Canvas du 22 octobre 2026. Une réponse sans fichier ne compte pas.
Pour chacun, relevez le fichier, le numéro de ligne, et la ligne elle-même.
Un des neuf n'est pas dans le kit. Dites où il manque, et ce qui s'en approche le plus.
Dans motifs.txt, écrivez neuf paragraphes étiquetés, exactement ainsi : Stratégie :, Fabrique :, Façade :, Patron de méthode :, Observateur :, Singleton :, Adaptateur :, Composite :, puis Commande :. Sous chacun, le fichier et sa ligne, NkExemple.h:42, puis la ligne recopiée.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo1-le_motif_et_son_fichier/ et mettez-y exactement ces fichiers, avec ces noms :

motifs.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
motifs.txt porte les cinq premières étiquettes, chacune suivie d'un fichier et d'une ligne
motifs.txt porte les quatre dernières, un fichier et une ligne après les trois premières d'entre elles
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
huit motifs ont un fichier et une ligne qui les montrent vraiment	8
chaque ligne est recopiée, et correspond au numéro donné	4
la Commande : où elle manque, et ce qui s'en approche	3
un motif montré seulement en commentaire est signalé comme tel	3
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Le kit n'a que les en-têtes. Si la ligne de code est dans une bibliothèque compilée, citez l'en-tête qui la montre, et dites-le.
Un nom de fichier qui ressemble au motif ne suffit pas. Lisez la ligne, et vérifiez qu'elle règle le problème du motif.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Olivieraustinmani-PNG
Verrouillé

Où déposer votre travail

Olivieraustinmani-PNG/ani-2053 / chapitre-08/exo1-le_motif_et_son_fichier

Ce dépôt doit être public, et c'est à vous de le créer, ainsi que le dossier. La plateforme lit votre code à cet endroit exact.

Évaluer
Le code lu est celui qui se trouve dans votre dépôt à cet instant. L'empreinte du commit évalué est conservée avec la note.

Un problème avec GitHub ? Écrire mon code ici

L'interface de la stratégie
Exercice
Où déposer
ani-2053/chapitre-08/exo2-l_interface_de_la_strategie/strategie.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo2-l_interface_de_la_strategie
Les fichiers, au nom exactstrategie.txt
1 missionsles quatre etiquettes de la strategie
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Compter les méthodes qu'une interface de stratégie impose à chacune de ses mises en œuvre, puis vérifier ce que chacune écrit vraiment. En tirer le coût d'en ajouter une.

Pourquoi ça sert
Une interface de stratégie est un compromis entre ce que chaque mise en œuvre sait faire de mieux. Ce qu'elle coûte se compte en méthodes à écrire, et ce compte se vérifie.

Comment faire
Dans le kit Canvas du 22 octobre 2026, NKCanvas dessine par plusieurs dorsaux derrière une seule interface, NkIRenderer2D. Celui qui s'en sert ne sait pas lequel il a.
Ouvrez NKCanvas/Renderer/Core/NkIRenderer2D.h, et comptez les méthodes virtuelles pures de la classe NkIRenderer2D. Comptez à part les virtuelles qui ont un corps.
Dites comment vous avez compté, et vérifiez votre compte d'une seconde façon.
Listez les dorsaux que le kit contient, dans NKCanvas/Backend/.
Pour chacun, cherchez de quelle classe il hérite, et combien de méthodes son en-tête redéfinit.
Dans strategie.txt, écrivez quatre paragraphes étiquetés, exactement ainsi : Virtuelles pures :, Dorsaux :, Ce que chacun écrit :, puis Le coût :. Sous les deux premiers, vos comptes et votre méthode. Sous Ce que chacun écrit :, la classe mère et le compte de chaque dorsal. Sous Le coût :, ce que coûte un dorsal de plus, d'après vos comptes.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo2-l_interface_de_la_strategie/ et mettez-y exactement ces fichiers, avec ces noms :

strategie.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
strategie.txt porte les quatre étiquettes, dans l'ordre, un nombre sous la première, des dorsaux nommés sous la deuxième, des nombres sous la troisième
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
le compte des virtuelles pures est juste, et la méthode dite	4
le compte est vérifié d'une seconde façon	2
les dorsaux du kit sont tous listés	3
Ce que chacun écrit : donne la classe mère et un compte par dorsal	4
Le coût : s'appuie sur ces comptes, pas sur le seul nombre de virtuelles pures	5
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Une ligne qui contient = 0; n'est pas toujours une virtuelle pure de cette classe. Regardez chaque ligne que votre recherche compte.
Un en-tête peut porter deux versions d'une classe, séparées par #if. Regardez laquelle vous comptez.
Un commentaire peut dater. Comptez les fichiers, pas la liste qu'en donne un commentaire.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

La fabrique en un coup d'œil
Exercice
Où déposer
ani-2053/chapitre-08/exo3-la_fabrique_en_un_coup_d_oeil/fabrique.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo3-la_fabrique_en_un_coup_d_oeil
Les fichiers, au nom exactfabrique.txt
2 missionsles trois etiquettes de la fabrique · les fichiers hors du dossier ont leur ligne
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Chercher dans tout le kit le nom d'une classe concrète que seule une fabrique devrait connaître. Compter où il apparaît hors de son dossier, et conclure.

Pourquoi ça sert
Une fabrique qui fait son travail est le seul endroit qui connaît les classes concrètes. Le nom d'une classe qui traîne partout dit que le choix s'est répandu dans le programme.

Comment faire
Dans le kit Canvas du 22 octobre 2026, NkRenderer2DFactory crée le dorsal de dessin sans que l'appelant connaisse sa classe. Le dorsal OpenGL s'appelle NkOpenGLRenderer2D.
Cherchez NkOpenGLRenderer2D dans tout include/. Gardez la commande et ce qu'elle trouve.
Pour chaque fichier trouvé hors du dossier du dorsal OpenGL, relevez la ligne, et dites si c'est du code ou un commentaire.
Ouvrez NkRenderer2DFactory.h : nomme-t-il les dorsaux ?
Dans fabrique.txt, écrivez trois paragraphes étiquetés, exactement ainsi : Recherche :, Hors de son dossier :, puis Conclusion :. Sous Recherche :, la commande et le nombre de fichiers. Sous Hors de son dossier :, chaque fichier avec sa ligne, NkExemple.h:42, et ce qu'il est. Sous Conclusion :, ce que cela dit de la fabrique.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo3-la_fabrique_en_un_coup_d_oeil/ et mettez-y exactement ces fichiers, avec ces noms :

fabrique.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
fabrique.txt porte les trois étiquettes, dans l'ordre
trois fichiers au moins, avec leur ligne, suivent Hors de son dossier :
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
la commande et son résultat	3
chaque fichier hors du dossier, avec sa ligne	5
chaque ligne est classée : code ou commentaire	4
Conclusion : tire le sens de la recherche, fabrique comprise	6
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Un nom dans un commentaire n'est pas un appel. Lisez chaque ligne trouvée.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Les crochets du patron
Exercice
Où déposer
ani-2053/chapitre-08/exo4-les_crochets_du_patron/crochets.cpp, crochets.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo4-les_crochets_du_patron
Les fichiers, au nom exactcrochets.cpp, crochets.txt
Le style du code — signalé, sans coûter de points
5 missionsle programme redefinit les crochets · chaque appel est ecrit dans un journal · le programme s'arrete seul · les quatre etiquettes des crochets · le journal releve six appels
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Lister tous les crochets d'une classe de base qui tient la boucle d'un programme, puis mesurer quand elle les appelle, par un programme qui note chaque appel.

Pourquoi ça sert
Un patron de méthode impose un ordre que votre code ne montre pas. La réponse est dans la classe de base ; quand son code n'est pas sous vos yeux, un journal la donne.

Comment faire
NkCanvasApp tient la boucle d'un programme du kit Canvas du 22 octobre 2026. Son code n'est pas dans le kit ; le voici, au cœur de la boucle :
OnUpdate(deltaTime);
mTarget->Clear(mConfig.clearColor);
OnRender(*mTarget);
mTarget->Display();
Vous n'écrivez ni la boucle, ni Clear, ni Display : vous remplissez des crochets, des méthodes virtuelles que la classe de base appelle quand elle le décide.
Listez tous les crochets virtuels de NKCanvas/App/NkCanvasApp.h, avec leur ligne, et ce que leur commentaire promet.
Écrivez crochets.cpp, une NkCanvasApp construite avec le kit Canvas du 22 octobre 2026, qui redéfinit chacun de ces crochets. Son point d'entrée :
int nkmain(const NkEntryState &state) {
    return renderer::NkCanvasApp::Run<MonJeu>(state);
}
Chaque crochet écrit une ligne dans un fichier : un numéro d'ordre, son nom, l'image en cours et Time(). Le programme s'arrête seul, par Quit(), après quelques images.
Construisez en Debug et en Release, et lancez chaque fois l'exécutable seul, sans toucher à la fenêtre.
Dans crochets.txt, écrivez quatre paragraphes étiquetés, exactement ainsi : Les crochets :, L'ordre mesuré :, Les surprises :, puis Debug et Release :. Sous Les crochets :, la liste et les lignes. Sous L'ordre mesuré :, votre journal. Sous Les surprises :, trois appels qui vous ont surpris, et pourquoi. Sous Debug et Release :, ce qui change de l'un à l'autre.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo4-les_crochets_du_patron/ et mettez-y exactement ces fichiers, avec ces noms :

crochets.cpp
crochets.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

les deux fichiers sont rendus, avec les noms exacts
crochets.cpp hérite de NkCanvasApp et redéfinit OnInit, OnLayout et OnShutdown
crochets.cpp écrit un journal
crochets.cpp s'arrête par Quit()
crochets.txt porte les quatre étiquettes, dans l'ordre
six appels au moins sont relevés sous L'ordre mesuré :
Comment vous serez noté
Ce qui est vérifié	Points
les deux fichiers sont rendus	2
tous les crochets de l'en-tête sont listés, avec leur ligne	3
le programme construit en Debug et en Release, et chaque crochet écrit son appel	4
L'ordre mesuré : recopie un journal réel	3
Les surprises : désigne trois appels inattendus, et dit ce qu'on aurait supposé	6
Debug et Release : compare les deux mesures	2
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Une application à fenêtre n'a pas de console : écrivez le journal dans un fichier.
Ne notez OnUpdate et OnRender que pour les premières images, sinon le journal noie tout le reste.
Un crochet qui n'apparaît pas n'est pas forcément cassé. Demandez-vous ce qui aurait dû le déclencher.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Push ou pull
Exercice
Où déposer
ani-2053/chapitre-08/exo5-push_ou_pull/push_pull.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo5-push_ou_pull
Les fichiers, au nom exactpush_pull.txt
2 missionsles deux etiquettes du push et du pull · six outils avec leur fichier
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Pour six situations d'un jeu, choisir entre être prévenu et demander, et trouver dans le kit l'outil qui le fait. Puis décrire les deux bogues qu'un mauvais choix donne.

Pourquoi ça sert
Les deux modèles ne servent pas aux mêmes choses, et les confondre donne les deux bogues classiques : une action qui part trois fois, et un mouvement qui s'arrête trop tôt.

Comment faire
Le modèle push : on s'abonne à un type d'événement, et on est appelé quand il survient. Il convient à ce qui arrive, à un instant.
Le modèle pull : personne ne vous prévient, vous demandez, par exemple à chaque image. Il convient à ce qui dure.
Voici six situations. Le joueur maintient la flèche droite. Il appuie sur Échap. On veut la position de la souris. Un fichier est déposé sur la fenêtre. La fenêtre est redimensionnée. Le joueur double-clique.
Pour chacune, choisissez le modèle, et dites pourquoi. Puis trouvez dans les en-têtes du kit Canvas du 22 octobre 2026 l'outil qui la sert : un type d'événement, un crochet, ou une question qu'on pose.
Dans push_pull.txt, écrivez deux paragraphes étiquetés, exactement ainsi : Situations :, puis Les deux bogues :. Sous Situations :, une ligne par situation : le modèle, pourquoi, et l'outil avec son fichier et sa ligne, NkExemple.h:42. Sous Les deux bogues :, le bogue que donnerait le mauvais modèle, dans deux de vos situations.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo5-push_ou_pull/ et mettez-y exactement ces fichiers, avec ces noms :

push_pull.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
push_pull.txt porte les deux étiquettes, dans l'ordre
six outils, avec leur fichier et leur ligne, suivent Situations :
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
le bon modèle pour les six situations	6
chaque choix est justifié par l'instant ou la durée	4
un outil du kit par situation, avec son fichier et sa ligne	4
Les deux bogues : décrit ce que ferait le mauvais modèle	4
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Une même situation peut avoir un événement et une question. Dites laquelle convient, et pourquoi pas l'autre.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Le singleton et le test
Exercice
Où déposer
ani-2053/chapitre-08/exo6-le_singleton_et_le_test/banc.cpp, banc.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo6-le_singleton_et_le_test
Les fichiers, au nom exactbanc.cpp, banc.txt
Le style du code — signalé, sans coûter de points
5 missionsla version directe interroge NkInput · le banc rend un code de sortie · deux fonctions recoivent un booleen · les quatre etiquettes du banc · quatre resultats sous le banc
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Dire ce qu'il faudrait pour tester une fonction qui interroge un singleton, puis la réécrire pour qu'elle se teste, et lancer ce test.

Pourquoi ça sert
Un objet accessible de partout est modifiable de partout, et une fonction qui s'en sert ne se teste pas : on ne peut pas lui en fournir un autre.

Comment faire
Le kit Canvas du 22 octobre 2026 déclare un singleton, et montre comment s'en servir :
inline NkInputQuery NkInput;

if (NkInput.IsKeyDown(NkKey::NK_SPACE)) { ... }
Écrivez une fonction de jeu qui interroge NkInput directement, par exemple : un personnage saute s'il est au sol et que la barre d'espace est enfoncée.
Sans la compiler d'abord, écrivez ce qu'il faudrait pour la tester, cas par cas.
Réécrivez-la pour qu'elle soit testable, et dites ce que vous avez changé.
Écrivez banc.cpp, une NkCanvasApp construite avec le kit. Sa méthode OnCommandLine est appelée avant toute fenêtre ; si elle rend une valeur, le programme s'arrête avec ce code de sortie.
Faites-y tourner les cas de votre test, écrivez chaque résultat dans un fichier, et rendez le nombre d'échecs. Essayez aussi la version directe, au même endroit.
Construisez en Debug et en Release, et lancez chaque fois l'exécutable seul.
Dans banc.txt, écrivez quatre paragraphes étiquetés, exactement ainsi : Tester la version directe :, La version testable :, Le banc :, puis Sans fenêtre :. Sous Le banc :, les résultats et le code de sortie. Sous Sans fenêtre :, ce que la version directe a rendu, et pourquoi.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo6-le_singleton_et_le_test/ et mettez-y exactement ces fichiers, avec ces noms :

banc.cpp
banc.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

les deux fichiers sont rendus, avec les noms exacts
banc.cpp garde la version qui interroge NkInput
banc.cpp rend un code de sortie depuis OnCommandLine
banc.cpp porte deux fonctions qui reçoivent un booléen
banc.txt porte les quatre étiquettes, dans l'ordre
quatre résultats au moins suivent Le banc :
Comment vous serez noté
Ce qui est vérifié	Points
les deux fichiers sont rendus	2
Tester la version directe : dit ce qu'il faudrait, et ce que cela coûte	4
la version testable garde la règle, et ne lit plus le singleton	4
le banc construit en Debug et en Release, et rend son verdict	3
Le banc : recopie des résultats réels	3
Sans fenêtre : explique ce que la version directe a rendu	4
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Un banc qui rend sa valeur dans OnCommandLine ne crée pas de fenêtre. Regardez ce que le singleton y voit.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Un second pont
Exercice
Où déposer
ani-2053/chapitre-08/exo7-un_second_pont/pont.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo7-un_second_pont
Les fichiers, au nom exactpont.txt
2 missionsles cinq etiquettes du pont · le premier pont a son fichier et le second ses methodes
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Lire un adaptateur qui traduit les commandes de dessin d'une interface en appels au moteur de rendu, puis décrire celui qui les traduirait en fichier image. Enfin, chercher s'il existe.

Pourquoi ça sert
Un adaptateur bien fait laisse les deux côtés s'ignorer. Le prix se voit au jour où il en faut un second : s'il s'écrit sans toucher l'interface, le premier a tenu sa promesse.

Comment faire
NKGui produit des listes de commandes de dessin ; NKCanvas attend des appels à NkIRenderer2D. NKCanvas/UI/NkGuiCanvasBackend.h, dans le kit Canvas du 22 octobre 2026, traduit les unes en les autres.
Lisez-le : ce qu'il reçoit, ses méthodes publiques, et ce qu'il fait de chaque commande.
Décrivez ce qu'il faudrait écrire pour dessiner la même interface dans un fichier image plutôt qu'à l'écran. Combien de méthodes ? Quelles données suffisent ?
Cherchez ensuite dans les en-têtes du kit si ce second pont existe déjà. S'il existe, comparez-le à votre description.
Dans pont.txt, écrivez cinq paragraphes étiquetés, exactement ainsi : Le premier pont :, Le second pont :, Combien de méthodes :, Les données :, puis Dans le kit :. Sous le premier, le fichier, sa ligne, ses méthodes et ce qu'il reçoit. Sous les trois suivants, votre description. Sous Dans le kit :, ce que vous avez trouvé, avec son fichier, et ce qui diffère de votre description.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo7-un_second_pont/ et mettez-y exactement ces fichiers, avec ces noms :

pont.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
pont.txt porte les cinq étiquettes, dans l'ordre
le premier pont est désigné par son fichier et sa ligne, et le second par un nombre de méthodes
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
Le premier pont : dit ce qu'il reçoit et ce qu'il appelle	4
le second pont est décrit, sans rien changer à NKGui	4
le compte de méthodes et les données suffisent, et sont justifiés	4
Dans le kit : compare avec ce que le kit contient	6
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Le pont lit une liste de commandes ; il ne connaît pas les fenêtres de l'interface. Partez de ce qu'il reçoit.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Le composite à trois niveaux
Exercice
Où déposer
ani-2053/chapitre-08/exo8-le_composite_a_trois_niveaux/composite.cpp, composite.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo8-le_composite_a_trois_niveaux
Les fichiers, au nom exactcomposite.cpp, composite.txt
Le style du code — signalé, sans coûter de points
5 missionschaque niveau compose l'etat · un sprite place et tourne · un dessinable est defini · les quatre etiquettes du composite · deux positions mesurees
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Construire un objet transformé qui en contient un autre, qui contient un sprite. Mesurer où le sprite arrive, et montrer qu'un tel test attrape un défaut réel.

Pourquoi ça sert
Un composite dont une branche jette l'état de son parent marche pour certains types et pas pour d'autres. Comme personne n'essaie tous les types, cela passe inaperçu des mois.

Comment faire
Dans le kit Canvas du 22 octobre 2026, un objet transformé passe son état à ses enfants : chaque niveau compose son transform avec celui qu'il reçoit. L'en-tête de NkDrawable le montre ainsi :
void NkSprite::Draw(NkRenderTarget& target, const NkRenderStates& states) const {
    NkRenderStates local = states;
    local.transform *= GetTransform();   // applique notre transform
    local.texture    = mTexture;          // attache notre texture
    target.Draw(mVertices, 4, NkPrimitiveType::NK_TRIANGLE_STRIP, local);
}
Écrivez composite.cpp, une NkCanvasApp construite avec le kit. Une classe qui hérite de NkTransformable et de NkDrawable contient un enfant, et le dessine avec l'état composé.
Montez trois niveaux : un parent, qui contient un parent, qui contient un NkSprite. Donnez une position à chacun, et une rotation au niveau du milieu.
Avant de lancer, calculez où l'origine du sprite doit arriver.
Mesurez où elle arrive : une sonde, placée comme le sprite, écrit dans un fichier où son origine arrive une fois composée. Le transform composé sait placer un point : cherchez TransformPoint dans NKCanvas/Renderer/Core/NkTransform.h.
Vérifiez aussi le sprite à l'image : lancez avec --capture=capture.png --capture-frame=10.
Voici un défaut qu'a eu ce sprite, tel que son en-tête le raconte :
Leur Draw(target, states) JETAIT l'etat recu : le parametre etait
commente. Un sprite ne pouvait donc pas etre l'enfant d'un objet
transforme, alors qu'une forme le pouvait.
Imitez-le : une seconde sonde jette l'état reçu, et ne garde que son propre transform. Construisez en Debug et en Release, et lancez l'exécutable seul.
Dans composite.txt, écrivez quatre paragraphes étiquetés, exactement ainsi : La hiérarchie :, Attendu :, Mesuré :, puis Le défaut :. Sous les trois premiers, vos niveaux, votre calcul, et ce que les sondes et l'image ont donné. Sous Le défaut :, ce que donne la sonde qui jette l'état, et comment votre test aurait attrapé le défaut.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo8-le_composite_a_trois_niveaux/ et mettez-y exactement ces fichiers, avec ces noms :

composite.cpp
composite.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

les deux fichiers sont rendus, avec les noms exacts
composite.cpp compose l'état par transform *= GetTransform()
composite.cpp place et tourne un NkSprite
composite.cpp définit un dessinable
composite.txt porte les quatre étiquettes, des nombres sous les trois premières
deux positions (x, y) au moins suivent Mesuré :
Comment vous serez noté
Ce qui est vérifié	Points
les deux fichiers sont rendus	2
trois niveaux, chacun composant l'état reçu	4
Attendu : calcule la position avant la mesure	3
Mesuré : donne les sondes et l'image, en Debug et en Release	4
Le défaut : mesure la sonde qui jette l'état, et explique ce que le test aurait attrapé	5
le choix des niveaux rend l'écart visible : une rotation, des positions non nulles	2
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Le sprite a besoin d'une texture. Une texture unie se crée dans OnInit, ainsi : mTexture.Create(*Target().GetRenderer(), 32, 32, NkColor2D{255, 200, 0, 255}). Puis SetTexture la donne au sprite.
Un nœud qui reçoit son enfant par référence, construit à partir d'un autre nœud, appelle le constructeur de copie. Passez l'enfant par pointeur.
La capture relit l'image affichée, et son en-tête ne promet cette relecture qu'au dorsal DirectX 11. Si capture.png n'est pas écrit, vos sondes restent la mesure : dites-le.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

La façade qui grossit
Exercice
Où déposer
ani-2053/chapitre-08/exo9-la_facade_qui_grossit/facade.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo9-la_facade_qui_grossit
Les fichiers, au nom exactfacade.txt
2 missionsles quatre etiquettes de la facade · dix methodes nommees
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Compter les méthodes publiques d'une façade, les séparer entre celles dont un programme se sert et les autres, et juger si elle tient encore sa promesse.

Pourquoi ça sert
Une façade grossit : chaque besoin particulier ajoute une méthode, et au bout de deux ans elle expose tout ce qu'elle devait simplifier. Un compte le dit mieux qu'une impression.

Comment faire
La cible de rendu cache un contexte graphique, une fabrique, un renderer, un lot de sommets, une projection. Le cas courant tient en trois appels : Clear, Draw et Display.
Dans le kit Canvas du 22 octobre 2026, comptez les méthodes publiques de NKCanvas/Renderer/Targets/NkRenderTarget.h. Dites comment vous comptez les surcharges.
Classez-les en deux groupes : celles dont vos programmes de ce sprint se servent, directement ou par la boucle de NkCanvasApp ; et les autres.
Pour les autres, dites à quel besoin chacune répond, et relevez celles que leur propre commentaire dit provisoires.
Dans facade.txt, écrivez quatre paragraphes étiquetés, exactement ainsi : Publiques :, Utiles :, Les autres :, puis Le rapport :. Sous les trois premiers, vos comptes et les noms. Sous Le rapport :, le rapport entre les deux groupes, et si la façade tient sa promesse.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo9-la_facade_qui_grossit/ et mettez-y exactement ces fichiers, avec ces noms :

facade.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
facade.txt porte les quatre étiquettes, dans l'ordre, un nombre sous la première et la dernière
dix méthodes au moins sont nommées sous Publiques :
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
le compte des méthodes publiques est juste, et sa règle dite	4
chaque méthode est classée, d'après des programmes réels	4
Les autres : dit à quel besoin chacune répond, et relève les provisoires	5
Le rapport : juge la promesse de la façade, chiffres à l'appui	5
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Comptez chaque surcharge à part, ou toutes ensemble, mais dites lequel.
Une méthode qui n'est pas virtuelle compte aussi : ne comptez pas seulement les lignes virtual.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici

Le motif absent
Exercice
Où déposer
ani-2053/chapitre-08/exo10-le_motif_absent/absent.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-08/exo10-le_motif_absent
Les fichiers, au nom exactabsent.txt
1 missionsles trois etiquettes du motif absent
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Trouver dans le kit un endroit où un motif manque, proposer celui qui conviendrait, et estimer ce que son introduction coûterait.

Pourquoi ça sert
Un motif n'est pas à placer partout : on l'introduit au deuxième cas, pas au premier. Encore faut-il voir le deuxième cas quand il arrive, et chiffrer ce qu'il en coûte de le régler.

Comment faire
Un motif qui manque se reconnaît à quelques signes : un switch sur un même type répété à plusieurs endroits, un code de création dispersé, une classe qui en connaît une autre sans raison.
Cherchez un tel endroit dans les en-têtes du kit Canvas du 22 octobre 2026. Relevez chaque fichier et chaque ligne en cause.
Montrez que c'est bien un deuxième cas : la même chose écrite au moins deux fois, ou un choix répandu à plusieurs endroits.
Proposez le motif qui conviendrait, et estimez ce que son introduction coûterait : les fichiers à changer, ce qui risque de casser.
Dans absent.txt, écrivez trois paragraphes étiquetés, exactement ainsi : L'endroit :, Le motif :, puis Le coût :. Sous L'endroit :, les fichiers et leurs lignes, NkExemple.h:42, et ce qui se répète. Sous les deux autres, votre proposition et son coût.
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-08/exo10-le_motif_absent/ et mettez-y exactement ces fichiers, avec ces noms :

absent.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

le fichier est rendu, avec son nom exact
absent.txt porte les trois étiquettes, dans l'ordre, et deux fichiers avec leur ligne sous la première
Comment vous serez noté
Ce qui est vérifié	Points
le fichier est rendu	2
L'endroit : montre un vrai deuxième cas, fichiers et lignes à l'appui	6
Le motif : convient au problème trouvé, et pas un autre	5
Le coût : compte les fichiers à changer, et dit ce qui risque de casser	5
la proposition ne place pas un motif pour un seul cas	2
Total	20
Les pièges
Le kit Canvas du 22 octobre 2026 se télécharge ici : https://github.com/Rihen-Universe/Nkentseu-Kits/releases/download/kits-cours-2026-10-22/Canvas-kit-Windows.zip (24 Mo). Décompressez-le dans Kits/ à la place de l'ancien. Il corrige ce que vos retours ont trouvé dans celui du 13 octobre : NkFormat met chaque {} à sa place (et NkAddress::ToString() affiche la bonne adresse), NkFuture.h compile, WriteF32Q code le maximum, NkSpinLock se lie. Les en-têtes de NKCanvas et la liste des modules n'ont pas changé. Un rendu déjà déposé avec le kit du 13 octobre reste valable.
Un motif se propose au deuxième cas, pas au premier : montrez les deux cas.
Deux fonctions qui se ressemblent ne disent pas forcément la même chose. Comparez-les ligne à ligne.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Un problème avec GitHub ? Écrire mon code ici