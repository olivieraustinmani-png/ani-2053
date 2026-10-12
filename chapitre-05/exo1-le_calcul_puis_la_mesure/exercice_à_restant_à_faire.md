"Série 2

Exercices 2 — Les images : NKImage
0/10 tâches

Le calcul, puis la mesure
Exercice

L'extension qui ment
Exercice
Où déposer
ani-2053/chapitre-05/exo2-l_extension_qui_ment/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo2-l_extension_qui_ment
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
5 missions — 4 masquéesl'exemple de l'enonce, deux extensions qui mentent
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui reconnaît le format d'un fichier image à ses premiers octets, puis dit si son extension dit vrai.

Pourquoi ça sert
Un chargeur d'images sérieux ne croit pas l'extension : il lit le début du fichier. Un PNG renommé en .jpg se charge quand même, et un fichier vide nommé .png est refusé.

Ce que votre programme reçoit
Votre programme lit une ligne avec un entier N, puis N lignes nom taille octets :

nom : le nom du fichier, sans espace ;
taille : sa taille totale, en octets ;
octets : ses premiers octets, en hexadécimal, deux chiffres par octet, sans séparateur. Il y en a au plus 20, parfois moins. Un tiret - veut dire qu'aucun octet n'est donné.
Exemple d'entrée :

5
perso.png 20480 89504E470D0A1A0A0000000D4948445200000200
perso.jpg 20480 89504E470D0A1A0A0000000D4948445200000200
photo.png 350000 FFD8FFE000104A46494600010100000100010000
vide.png 0 -
logo.svg 1200 3C3F786D6C2076657273696F6E3D22312E30223F
Ce que votre programme doit afficher
Une ligne par fichier, dans l'ordre de lecture :

nom FORMAT OK si l'extension correspond au format reconnu ;
nom FORMAT MENT si elle ne correspond pas ;
nom REFUSE si aucun format n'est reconnu.
Puis trois lignes : LUS n, MENSONGES n et REFUSES n.

Pour l'exemple ci-dessus :

perso.png PNG OK
perso.jpg PNG MENT
photo.png JPEG MENT
vide.png REFUSE
logo.svg SVG OK
LUS 4
MENSONGES 2
REFUSES 1
La règle à appliquer
Appelons b0, b1, b2... les octets donnés, dans l'ordre. Une règle qui regarde un octet absent de la ligne ne s'applique pas. Les règles se testent dans cet ordre, et la première qui s'applique décide.

Si taille est plus petite que 4 : REFUSE, rien d'autre n'est testé.
PNG : taille vaut au moins 8, et b0 b1 b2 b3 valent 89 50 4E 47.
JPEG : b0 b1 b2 valent FF D8 FF.
BMP : b0 b1 valent 42 4D, les lettres « BM ».
QOI : 71 6F 69 66, les lettres « qoif ».
GIF : 47 49 46 38, les lettres « GIF8 ».
ICO : b0 et b1 valent 00, b2 vaut 01 ou 02, b3 vaut 00.
HDR : taille vaut au moins 10, et b0 b1 valent 23 3F, « #? ».
EXR : 76 2F 31 01.
b0 vaut 50, la lettre « P », et b1 va de 31 à 36, les chiffres « 1 » à « 6 ». Avec « 1 » ou « 4 » : PBM. Avec « 2 » ou « 5 » : PGM. Avec « 3 » ou « 6 » : PPM.
TGA : taille vaut au moins 18, et b2 vaut 00, 01, 02, 03, 09, 0A ou 0B. Ce format n'a pas de signature : c'est une supposition.
SVG : sautez EF BB BF s'ils ouvrent la ligne, puis tous les octets 20, 09, 0A et 0D qui suivent. Si la suite commence par 3C 3F 78 6D 6C, « <?xml », ou par 3C 73 76 67, « <svg », c'est SVG.
Sinon : REFUSE.
L'extension est le texte après le dernier point du nom, mis en minuscules. Sans point, elle est vide. Les extensions justes sont :

Format	Extensions justes
PNG	png
JPEG	jpg, jpeg
BMP	bmp
QOI	qoi
GIF	gif
ICO	ico, cur
HDR	hdr
EXR	exr
PBM	pbm
PGM	pgm
PPM	ppm
TGA	tga
SVG	svg
Une extension vide ne correspond à rien.

LUS compte les fichiers reconnus, MENSONGES ceux qui ont MENT, REFUSES les autres. S'il n'y a aucun fichier, les trois valent zéro.

Un exemple déroulé
Reprenons l'exemple.

perso.png : 89 50 4E 47, et la taille dépasse 8. PNG, et l'extension png est juste : OK.
perso.jpg : les mêmes octets, donc PNG. Mais l'extension est jpg : MENT. Le module le charge quand même, en PNG.
photo.png : FF D8 FF, donc JPEG, et png n'est pas une extension de JPEG : MENT.
vide.png : la taille est 0, plus petite que 4. REFUSE. Son nom ne le sauve pas.
logo.svg : ni PNG, ni JPEG, ni les autres ; la règle 11 ne s'applique pas, car b2 vaut 78. La ligne commence par 3C 3F 78 6D 6C : SVG, OK.
Bilans : 4 fichiers lus, 2 mensonges, 1 refusé.
Ce qui est attendu de vous
votre programme décode l'hexadécimal, majuscules comme minuscules
votre programme applique les règles dans l'ordre, et s'arrête à la première qui s'applique
votre programme compare l'extension en minuscules
votre programme affiche les trois lignes de bilan
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo2-l_extension_qui_ment/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
L'ordre compte : un vrai TGA dont les quatre premiers octets sont 00 00 02 00 passe pour une icône. Suivez l'ordre, ne le corrigez pas.
Comparez les octets comme des valeurs de 0 à 255. Dans un char signé, FF vaut moins un.
L'extension est après le DERNIER point : photo.final.png a l'extension png.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste."


exercice 3: Les six les_six_formats: 
Où déposer
ani-2053/chapitre-05/exo3-les_six_formats/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo3-les_six_formats
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
5 missions — 4 masquéesl'exemple de l'enonce, une image de 1920 sur 1080
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui calcule la mémoire d'une image dans deux formats de pixels, et ce qu'une conversion de l'un à l'autre lui fait perdre.

Pourquoi ça sert
La mémoire d'une image décodée ne dépend que de sa largeur, de sa hauteur et de son format. Une conversion peut aussi jeter pour toujours la transparence, la couleur ou l'étendue des flottants.

Ce que votre programme reçoit
Votre programme lit deux entiers w h, la largeur et la hauteur de l'image en pixels, tous deux au moins égaux à 1. Puis un entier N, et N lignes SOURCE CIBLE : deux noms de formats.

Exemple d'entrée :

1920 1080
5
RGBA32 RGB24
RGBA32 GRAY8
RGBA128F RGBA32
GRAY8 RGBA32
RGBA32 CMYK32
Ce que votre programme doit afficher
Une ligne par conversion, dans l'ordre de lecture : SOURCE CIBLE octets_source octets_cible pertes.

Si l'un des deux noms est inconnu, la ligne est SOURCE CIBLE REFUSE.

Puis trois lignes : TOTAL n, SANS_PERTE n et REFUSES n.

Pour l'exemple ci-dessus :

RGBA32 RGB24 8294400 6220800 TRANSPARENCE
RGBA32 GRAY8 8294400 2073600 TRANSPARENCE+COULEUR
RGBA128F RGBA32 33177600 8294400 ETENDUE
GRAY8 RGBA32 2073600 8294400 AUCUNE
RGBA32 CMYK32 REFUSE
TOTAL 24883200
SANS_PERTE 1
REFUSES 1
La règle à appliquer
Les six formats, et eux seuls, écrits exactement ainsi :

Format	Octets par pixel	Couleur	Transparence	Flottants
GRAY8	1	non	non	non
GRAY_A16	2	non	oui	non
RGB24	3	oui	non	non
RGBA32	4	oui	oui	non
RGB96F	12	oui	non	oui
RGBA128F	16	oui	oui	oui
La mémoire d'une image dans un format vaut w * h * octets par pixel.
Les pertes se cherchent dans cet ordre : TRANSPARENCE si la source a la transparence et la cible non ; COULEUR si la source a la couleur et la cible non ; ETENDUE si la source est en flottants et la cible non.
Plusieurs pertes s'écrivent collées par un +, dans l'ordre de la règle 2. Sans aucune perte, on écrit AUCUNE.
Gagner ce qu'on n'avait pas n'est pas une perte : passer du gris à la couleur donne AUCUNE, et l'image grossit.
Un nom inconnu donne REFUSE. La comparaison est exacte, majuscules comprises.
TOTAL additionne les octets de la cible des conversions acceptées. SANS_PERTE compte celles qui ont AUCUNE. S'il n'y a aucune conversion, les trois bilans valent zéro.
Un exemple déroulé
Reprenons l'exemple, une image de 1920 sur 1080, soit 2 073 600 pixels.

RGBA32 RGB24 : 2 073 600 fois 4 = 8294400 octets, puis fois 3 = 6220800. La source a la transparence, la cible non : TRANSPARENCE.
RGBA32 GRAY8 : la cible fait 2073600 octets. On perd la transparence, puis la couleur : TRANSPARENCE+COULEUR.
RGBA128F RGBA32 : 16 octets par pixel, 33177600. Les deux ont couleur et transparence, mais la cible n'est pas en flottants : ETENDUE.
GRAY8 RGBA32 : rien n'est perdu, AUCUNE. L'image a pourtant quadruplé.
RGBA32 CMYK32 : CMYK32 n'est pas dans la table, REFUSE.
TOTAL : 6220800 + 2073600 + 8294400 + 8294400 = 24883200.
Ce qui est attendu de vous
votre programme connaît les six formats de la table
votre programme calcule la mémoire sur 64 bits
votre programme écrit les pertes dans l'ordre de la règle 2
votre programme affiche les trois lignes de bilan
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo3-les_six_formats/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
Une image de 65 536 pixels de côté en RGBA128F dépasse soixante milliards d'octets : un int déborde sans rien dire. Prenez long long.
Passer de GRAY_A16 à RGB24 perd la transparence, pas la couleur : le gris n'avait pas de couleur à perdre.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG
Verrouillé

Où déposer votre travail

Olivieraustinmani-PNG/ani-2053 / chapitre-05/exo3-les_six_formats

Ce dépôt doit être public, et c'est à vous de le créer, ainsi que le dossier. La plateforme lit votre code à cet endroit exact.


### exercice 4: l'atlas, vu

Où déposer
ani-2053/chapitre-05/exo4-l_atlas_vu/atlas.cpp, compte.txt, reponse.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo4-l_atlas_vu
Les fichiers, au nom exactatlas.cpp, compte.txt, reponse.txt
Le style du code — signalé, sans coûter de points
3 missionsle programme ajoute une police puis construit l'atlas · deux lignes de compte au format demande · les trois etiquettes de la reponse sont la
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Construire un atlas de police avec NKFont, enregistrer sa texture en PNG et la regarder, puis compter les glyphes et estimer la place perdue, à deux tailles.

Pourquoi ça sert
Un atlas range toutes les lettres dans une seule texture. Le voir, c'est comprendre ce que coûte une taille de plus, et pourquoi deux glyphes ne se touchent jamais.

Comment faire
Dans atlas.cpp, créez un NkFontAtlas, ajoutez une police avec AddFontFromFile(chemin, 18.f), puis appelez Build(). Gardez le NkFont* que rend AddFontFromFile.
Récupérez la texture par GetTexDataAsRGBA32(&pixels, &largeur, &hauteur). Enveloppez-la avec NkImage::Wrap, au format NK_RGBA32, et enregistrez-la par SavePNG("atlas.png").
Ouvrez atlas.png et regardez-la. Le nombre de glyphes est glyphCount sur la police. Estimez, en pour cent, la part de la texture où aucun glyphe n'est dessiné.
Recommencez avec une taille de 48, sans rendre la seconde image.
Dans compte.txt, écrivez une ligne par taille, sous la forme taille : 18, glyphes : 95, texture : 512 512, perdu : 60 (en anglais : size : 18, glyphs : 95, texture : 512 512, lost : 60).
Dans reponse.txt, écrivez trois paragraphes étiquetés : Pourquoi une seule texture :, Ce que change la taille :, puis A quoi sert la marge entre glyphes : (en anglais : Why a single texture :, What the size changes :, What the gap between glyphs is for :).
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo4-l_atlas_vu/ et mettez-y exactement ces fichiers, avec ces noms :

atlas.cpp
atlas.png
compte.txt
reponse.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

atlas.cpp, compte.txt et reponse.txt sont rendus, avec les noms exacts ; l'image est regardée par le correcteur, pas par la machine
atlas.cpp appelle AddFontFromFile puis Build
compte.txt porte deux lignes au format demandé
reponse.txt porte les trois étiquettes demandées
Comment vous serez noté
Ce qui est vérifié	Points
les quatre fichiers sont rendus	2
atlas.png montre des glyphes lisibles, rangés en rangées	4
les deux lignes de compte.txt sont cohérentes entre elles et avec la capture	5
la place perdue est estimée, et la méthode est dite	4
reponse.txt relie la taille à la place, et la marge au saignement	5
Total	20
Les pièges
Build() rastérise tout d'un coup : appelez-le après avoir ajouté la police, jamais avant.
Wrap ne copie pas les pixels : l'atlas doit rester en vie jusqu'à la fin de SavePNG.
Le nombre de glyphes ne change pas avec la taille ; la texture, si.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Olivieraustinmani-PNG


### exercice 5: trois police un atlas

Où déposer
ani-2053/chapitre-05/exo5-trois_polices_un_atlas/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo5-trois_polices_un_atlas
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
6 missions — 5 masquéesl'exemple de l'enonce, trois polices dans 64 pixels
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui range les glyphes de plusieurs polices dans une seule texture d'atlas, par étagères, et agrandit la texture tant qu'ils n'y tiennent pas.

Pourquoi ça sert
Un atlas rassemble toutes les lettres dans une seule texture, donc un seul envoi à la carte graphique. La marge entre deux glyphes évite qu'une lettre ramène un morceau de sa voisine.

Ce que votre programme reçoit
Toutes les valeurs sont des entiers, en pixels.

Votre programme lit une ligne P L : la marge P entre deux glyphes, au moins égale à zéro, et la largeur voulue L de la texture. L vaut zéro quand la largeur doit être choisie automatiquement.

Puis un entier G, et G lignes police n w h : un groupe de n glyphes de la même police, chacun de w sur h pixels. n, w et h valent au moins 1.

Exemple d'entrée :

2 64
3
karla18 5 10 14
karla12 6 7 10
mono14 3 8 12
Ce que votre programme doit afficher
Si tout tient :

ESSAIS k
TEXTURE W H
police x1 y1 x2 y2        (une ligne par groupe)
OCCUPE o
PERDU p
x1 y1 est la place du premier glyphe du groupe, x2 y2 celle du dernier. Si rien ne tient après 8 essais : ESSAIS 8 puis ECHEC. S'il n'y a aucun groupe : AUCUN, seul.

Pour l'exemple ci-dessus :

ESSAIS 1
TEXTURE 64 64
karla18 2 2 50 2
karla12 2 20 47 20
mono14 2 34 22 34
OCCUPE 1408
PERDU 65
La règle à appliquer
S'il n'y a aucun groupe, affichez AUCUN et rien d'autre.
La largeur de départ W vaut L. Si L vaut zéro, W part de 512. Le besoin est alors la somme, pour chaque glyphe, de (w + P) * (h + P). Tant que W * W est plus petit que deux fois le besoin, et que W est plus petit que 4096, W double.
La hauteur de départ H vaut W.
Un essai range tous les glyphes, groupe après groupe, dans l'ordre de lecture. Le curseur part de x = P et y = P, avec une étagère de hauteur e = 0.
Pour chaque glyphe, posez rw = w + P et rh = h + P. Si x + rw > W - P, passez à l'étagère suivante : x = P, puis y = y + e + P, puis e = 0. Si alors x + rw > W - P encore, l'essai échoue.
Si y + rh > H - P, l'essai échoue. Sinon le glyphe se pose en (x, y). Puis x = x + rw, et e devient le plus grand de e et de rh.
Après un essai manqué, la texture grandit : si W égale H, W double ; sinon H prend la valeur de W. Puis on recommence tout depuis la règle 4. Il y a au plus 8 essais.
ESSAIS est le numéro de l'essai réussi. OCCUPE est la somme des w * h de tous les glyphes, sans marge. PERDU vaut (W * H - OCCUPE) * 100 / (W * H), arrondi vers le bas.
Un exemple déroulé
Reprenons l'exemple : P = 2, W = H = 64, donc un glyphe ne peut pas dépasser x + rw = 62.

karla18 : rw = 12, rh = 16. Les glyphes se posent en x = 2, 14, 26, 38, puis 50, car 50 + 12 = 62 ne dépasse pas 62. Premier en (2, 2), dernier en (50, 2). x vaut 62, e vaut 16.
karla12 : rw = 9. 62 + 9 dépasse 62 : étagère suivante, y = 2 + 16 + 2 = 20. Six glyphes en x = 2, 11, 20, 29, 38, 47. Premier (2, 20), dernier (47, 20). e vaut 12.
mono14 : rw = 10. 56 + 10 dépasse : y = 20 + 12 + 2 = 34. Trois glyphes en x = 2, 12, 22. Et 34 + 14 = 48 ne dépasse pas 62 : tout tient au premier essai.
OCCUPE = 5 * 140 + 6 * 70 + 3 * 96 = 1408. PERDU = (4096 - 1408) * 100 / 4096 = 65,6, donc 65.
Ce qui est attendu de vous
votre programme choisit la largeur automatique quand L vaut zéro
votre programme range par étagères, avec la marge de la règle 5
votre programme agrandit la texture dans l'ordre de la règle 7, huit essais au plus
votre programme affiche le premier et le dernier glyphe de chaque groupe
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo5-trois_polices_un_atlas/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
La hauteur e d'une étagère compte déjà la marge, et le passage ajoute P encore : la marge verticale est double. Suivez la règle telle quelle.
Un essai manqué recommence TOUT le rangement, depuis le premier glyphe, dans la texture agrandie.
Avec une texture de plus de 46 000 pixels de côté, W * H déborde un int : calculez sur 64 bits.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG


### exercice 6: ligne de base


Où déposer
ani-2053/chapitre-05/exo6-la_ligne_de_base/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo6-la_ligne_de_base
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
6 missions — 5 masquéesl'exemple de l'enonce, Typo pose sur sa ligne de base
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui pose un texte glyphe par glyphe sur sa ligne de base. Il dit ce qui monte, ce qui descend, et si le texte sort de l'écran.

Pourquoi ça sert
La position d'un texte est celle de sa ligne de base, pas de son coin. Un texte posé tout en haut de l'écran sort par le haut, et l'on cherche un défaut de dessin alors que le dessin est juste.

Ce que votre programme reçoit
Toutes les valeurs sont des entiers, en pixels. Certaines peuvent être négatives. L'axe y descend vers le bas de l'écran, et le haut de l'écran est y = 0.

Votre programme lit :

un entier G, puis G lignes c avance x0 y0 x1 y1 : un caractère c, de combien il avance le curseur, et le rectangle de son dessin, relatif au curseur et à la ligne de base ;
un entier K, puis K lignes ab k : un couple de deux caractères collés, et la correction de crénage k ;
une ligne texte ox oy : le texte, sans espace, puis la position de sa ligne de base.
Le caractère _ tient lieu d'espace, dans la table comme dans le texte.

Exemple d'entrée :

9
T 12 0 -14 12 0
y 10 0 -10 10 4
p 10 1 -10 10 4
o 10 1 -10 9 0
_ 5 0 0 0 0
g 10 1 -10 9 4
A 13 0 -14 13 0
V 13 0 -14 13 0
j 5 -2 -14 4 4
3
Ty -2
AV -3
VA -3
Typo 20 40
Ce que votre programme doit afficher
Une ligne par caractère du texte, dans l'ordre : c x, la position du curseur où ce caractère se pose. Pour un caractère absent de la table : c ABSENT.

Puis :

CURSEUR x
BOITE minx miny maxx maxy
MONTE m
DESCEND d
ECRAN verdict
ABSENTS n
Si aucun caractère n'est dessiné, BOITE AUCUNE, MONTE 0, DESCEND 0 et ECRAN RIEN.

Pour l'exemple ci-dessus :

T 20
y 30
p 40
o 50
CURSEUR 60
BOITE 20 26 59 44
MONTE 14
DESCEND 4
ECRAN VISIBLE
ABSENTS 0
La règle à appliquer
Le curseur part de x = ox. La ligne de base est à oy.
Pour chaque caractère du texte, dans l'ordre : s'il est absent de la table, affichez c ABSENT. Il n'avance pas le curseur, et aucun crénage ne se cherche après lui.
Sinon, affichez c x. Le caractère est dessiné si x1 > x0 et y1 > y0. Son rectangle est alors (x + x0, oy + y0) à (x + x1, oy + y1). Un y0 négatif monte au-dessus de la ligne de base.
Puis x = x + avance. Si le caractère suivant du texte forme avec lui un couple de la table, ajoutez sa correction k, souvent négative.
CURSEUR est la valeur finale de x. BOITE est la plus petite boîte qui contient tous les rectangles dessinés.
MONTE vaut oy - miny si miny < oy, sinon 0. DESCEND vaut maxy - oy si maxy > oy, sinon 0.
ECRAN vaut HORS si maxy est inférieur ou égal à 0 : tout est au-dessus de l'écran. Sinon COUPE si miny < 0. Sinon VISIBLE.
ABSENTS compte les caractères absents.
Un exemple déroulé
Reprenons l'exemple, Typo posé en (20, 40).

T : le curseur est à 20. Rectangle de (20, 26) à (32, 40). Il avance de 12, donc 32, puis le couple Ty corrige de moins 2 : 30.
y : posé en 30, rectangle de (30, 30) à (40, 44). Son jambage descend de 4 sous la ligne. Pas de couple yp : 40.
p : posé en 40, de (41, 30) à (50, 44). Puis 50.
o : posé en 50, de (51, 30) à (59, 40). Puis 60.
La boîte va de (20, 26) à (59, 44). MONTE = 40 - 26 = 14, DESCEND = 44 - 40 = 4.
miny vaut 26, positif : VISIBLE. Aucun absent.
Ce qui est attendu de vous
votre programme applique le crénage avec le caractère suivant du texte
votre programme ne dessine pas l'espace, mais le fait avancer
votre programme saute un caractère absent sans avancer
votre programme compare la boîte au haut de l'écran
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo6-la_ligne_de_base/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
L'avance n'est pas la largeur du dessin : le j déborde à gauche du curseur, avec un x0 négatif.
Un texte posé à oy = 0 a sa ligne de base sur le bord : tout ce qui monte est hors de l'écran.
_ avance sans rien dessiner : il ne compte pas dans la boîte.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG



exercice 7: le calcul puis le fichier


Où déposer
ani-2053/chapitre-05/exo7-le_calcul_puis_le_fichier/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo7-le_calcul_puis_le_fichier
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
5 missions — 4 masquéesl'exemple de l'enonce, deux effets et une musique
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui calcule la taille non compressée de plusieurs sons, la compare à leur fichier, et décide lesquels se chargent en mémoire et lesquels se lisent en flux.

Pourquoi ça sert
Une minute de stéréo non compressée pèse onze mégaoctets. Un effet court se charge en entier ; une musique se lit par morceaux pendant qu'elle joue. Se tromper coûte la mémoire ou la latence.

Ce que votre programme reçoit
Votre programme lit un entier S, le seuil en octets. Puis un entier N, et N lignes nom frequence canaux bits duree fichier :

frequence : mesures par seconde, au moins 8000 ;
canaux : 1 pour le mono, 2 pour la stéréo ;
bits : la profondeur d'une mesure ;
duree : en millisecondes, au moins 1 ;
fichier : la taille du fichier sur le disque, en octets.
Exemple d'entrée :

1000000
4
tir 44100 1 16 350 9800
pas 48000 2 16 120 4100
musique 48000 2 16 60000 960000
voix 22050 1 12 2000 5000
Ce que votre programme doit afficher
Une ligne par son, dans l'ordre de lecture : nom brut pourcent MODE, où MODE vaut MEMOIRE ou FLUX. Pour une profondeur refusée : nom REFUSE.

Puis trois lignes : MEMOIRE n, FLUX n et REFUSES n.

Pour l'exemple ci-dessus :

tir 30870 31 MEMOIRE
pas 23040 17 MEMOIRE
musique 11520000 8 FLUX
voix REFUSE
MEMOIRE 53910
FLUX 1
REFUSES 1
La règle à appliquer
Seules les profondeurs 8, 16, 24 et 32 bits sont acceptées. Toute autre donne REFUSE.
La taille brute, en octets, vaut frequence * canaux * (bits / 8) * duree / 1000. Faites les trois multiplications d'abord, et la division par 1000 à la fin, arrondie vers le bas.
pourcent vaut fichier * 100 / brut, arrondi vers le bas : la part du brut que le fichier occupe.
Un son dont la taille brute est strictement plus grande que S se lit en FLUX. Sinon, il se charge en MEMOIRE.
La ligne MEMOIRE additionne les tailles brutes des sons chargés en mémoire : c'est ce qu'ils occupent une fois décodés. FLUX et REFUSES comptent les sons. S'il n'y a aucun son, les trois bilans valent zéro.
Un exemple déroulé
Reprenons l'exemple, avec un seuil d'un million d'octets.

tir : 44100 * 1 * 2 * 350 = 30 870 000, divisé par 1000 : 30870 octets. Le fichier en fait 9800, soit 9800 * 100 / 30870 = 31,7, donc 31 pour cent. Sous le seuil : MEMOIRE.
pas : 48000 * 2 * 2 * 120 / 1000 = 23040. 4100 * 100 / 23040 = 17,8, donc 17. MEMOIRE.
musique : une minute de stéréo, 11520000 octets, bien au-dessus du seuil : FLUX. Le fichier n'en garde que 8 pour cent.
voix : 12 bits n'est pas une profondeur acceptée, REFUSE.
MEMOIRE = 30870 + 23040 = 53910.
Ce qui est attendu de vous
votre programme refuse les profondeurs autres que 8, 16, 24 et 32
votre programme divise par 1000 après les multiplications
votre programme calcule sur 64 bits
votre programme compare au seuil strictement
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo7-le_calcul_puis_le_fichier/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
Une heure de stéréo en 32 bits donne un produit de mille milliards avant la division : un int déborde. Prenez long long.
Diviser la durée par 1000 d'abord perd les millisecondes : 350 ms deviendraient 0 seconde.
Un son dont la taille brute égale le seuil reste en mémoire.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG


exercice 8: un son vingt voix


Où déposer
ani-2053/chapitre-05/exo8-un_son_vingt_voix/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo8-un_son_vingt_voix
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
5 missions — 4 masquéesl'exemple de l'enonce, cinq lectures dont une rafale
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui rejoue une rafale de sons de trois manières : un échantillon et une voix par lecture, un fichier rechargé à chaque fois, une seule voix rejouée.

Pourquoi ça sert
Un échantillon est le son en mémoire ; une voix est une lecture en cours. Recharger le fichier à chaque tir fait attendre le programme ; rejouer une seule voix coupe le son au lieu de le superposer.

Ce que votre programme reçoit
Toutes les valeurs sont des entiers, en millisecondes.

Votre programme lit une ligne D C : la durée D du son, et le temps C que prend un chargement du fichier. Puis un entier N, et N instants de lecture t, du plus tôt au plus tard. Deux instants peuvent être égaux.

Exemple d'entrée :

300 80
5
0 100 200 250 900
Ce que votre programme doit afficher
Une ligne par lecture, dans l'ordre : t voix retard ETAT, où ETAT vaut COUPE ou ENTIER.

Puis trois lignes : VOIX_MAX n, RETARD_MAX n et COUPES n.

Pour l'exemple ci-dessus :

0 1 80 COUPE
100 2 80 COUPE
200 3 80 COUPE
250 4 110 ENTIER
900 1 80 ENTIER
VOIX_MAX 4
RETARD_MAX 110
COUPES 3
La règle à appliquer
Les trois colonnes décrivent trois programmes différents.

Une voix par lecture. La lecture j joue de t_j à t_j + D, la fin exclue. La colonne voix de la lecture i compte les lectures déjà lancées, celle-ci comprise, qui n'ont pas fini à l'instant t_i. Une lecture qui finit exactement à t_i a fini.
Le fichier rechargé à chaque lecture. Les chargements se font l'un après l'autre, et chacun bloque le programme. Le chargement i commence au plus tard de t_i et de la fin du chargement précédent. Il finit C millisecondes plus tard, et le son part alors. Le premier chargement n'attend personne.
La colonne retard vaut la fin du chargement i moins t_i.
Une seule voix, rejouée. Chaque nouvelle lecture coupe la précédente si celle-ci n'a pas fini. La lecture i est COUPE si la suivante existe et commence avant t_i + D, strictement. Sinon, elle est ENTIER.
VOIX_MAX est le plus grand nombre de la colonne voix, RETARD_MAX le plus grand retard, et COUPES compte les COUPE. S'il n'y a aucune lecture, les trois valent zéro.
Un exemple déroulé
Reprenons l'exemple : un son de 300 ms, et 80 ms par chargement.

À 0 : une seule voix. Le chargement va de 0 à 80 : retard 80. La lecture suivante part à 100, avant 300 : COUPE.
À 100 : la voix de 0 joue jusqu'à 300, donc 2 voix. Le chargement va de 100 à 180 : 80. Coupée par 200 : COUPE.
À 200 : 3 voix, retard 80, coupée par 250 : COUPE.
À 250 : 4 voix. Mais le chargement précédent finit à 280 : celui-ci va de 280 à 360, et le retard monte à 360 - 250 = 110. La suivante part à 900, après 550 : ENTIER.
À 900 : les autres ont fini, 1 voix, retard 80, ENTIER.
Bilans : 4 voix au plus, 110 ms de retard au plus, 3 coupures.
Ce qui est attendu de vous
votre programme compte les voix actives avec une fin exclue
votre programme enchaîne les chargements sans les superposer
votre programme coupe une lecture seulement si la suivante commence avant sa fin
votre programme affiche les trois lignes de bilan
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo8-un_son_vingt_voix/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
Le retard n'est pas toujours C : un chargement qui attend le précédent accumule du retard.
Deux lectures au même instant font deux voix, et la première est coupée par la seconde.
La dernière lecture n'est jamais coupée : aucune ne la suit.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG


exercice 9: conteneur et codec


Où déposer
ani-2053/chapitre-05/exo9-conteneur_et_codec/main.cpp

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo9-conteneur_et_codec
Les fichiers, au nom exactmain.cpp
La compilation, sans avertissementclang++ -std=c++17 -Wall main.cpp -o programme
Vous pouvez ajouter vos propres fichiers — mais seuls ceux nommés par la commande ci-dessus sont compilés
Le style du code — signalé, sans coûter de points
5 missions — 4 masquéesl'exemple de l'enonce, deux MP4, un WAV et un AVI
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Écrire un programme qui reconnaît le conteneur d'un fichier média à ses premiers octets, nomme le codec de chaque piste d'un MP4, et dit si le lecteur vidéo saura l'ouvrir.

Pourquoi ça sert
Le conteneur est la boîte qui range les pistes ; le codec est la manière de compresser une piste. Deux .mp4 peuvent demander deux décodeurs sans rien de commun.

Ce que votre programme reçoit
Votre programme lit un entier N, puis N lignes nom octets P type1 code1 ... typeP codeP :

nom : le nom du fichier ;
octets : ses premiers octets, en hexadécimal, deux chiffres par octet, sans séparateur, ou un tiret - s'il n'y en a aucun ;
P : le nombre de pistes décrites, qui peut valoir zéro ;
pour chaque piste, son type, vide pour une image ou soun pour un son, et son code, quatre caractères sans espace.
Exemple d'entrée :

4
film.mp4 000000186674797069736F6D00000200 2 vide avc1 soun mp4a
clip.mp4 000000146674797071742020 1 vide mjpa
bruit.wav 52494646A40F000057415645 1 soun lpcm
capture.avi 52494646A40F0000415649204C495354 1 vide MJPG
Ce que votre programme doit afficher
Pour chaque fichier, dans l'ordre : nom CONTENEUR.

Si le conteneur est MP4, suivent une ligne par piste, nom PISTE k TYPE codec, avec k à partir de 1 et TYPE valant VIDEO ou AUDIO, puis nom LECTEUR verdict.

Puis trois lignes : MP4 n, LISIBLES n et INCONNUS n.

Pour l'exemple ci-dessus :

film.mp4 MP4
film.mp4 PISTE 1 VIDEO h264
film.mp4 PISTE 2 AUDIO aac
film.mp4 LECTEUR LISIBLE
clip.mp4 MP4
clip.mp4 PISTE 1 VIDEO mjpa
clip.mp4 LECTEUR LISIBLE
bruit.wav WAV
capture.avi INCONNU
MP4 2
LISIBLES 2
INCONNUS 1
La règle à appliquer
Le conteneur. Appelons b0, b1... les octets donnés, et n leur nombre. Les règles se testent dans cet ordre, la première qui s'applique décide :

MP4 : n vaut au moins 12, et b4 à b7 valent 66 74 79 70, « ftyp ». Un .mov et un .m4a sont aussi des MP4.
WEBM : n vaut au moins 4, et b0 à b3 valent 1A 45 DF A3.
WAV : n vaut au moins 12, b0 à b3 valent « RIFF », 52 49 46 46, et b8 à b11 valent « WAVE », 57 41 56 45.
OGG : « OggS », 4F 67 67 53, et n au moins 4.
FLAC : « fLaC », 66 4C 61 43, et n au moins 4.
MP3 : « ID3 », 49 44 33, et n au moins 3.
MP3 encore : n au moins 2, b0 vaut FF, et b1 vaut au moins E0.
Sinon : INCONNU.
Les pistes. Elles ne se lisent que dans un MP4. Ailleurs, elles sont ignorées, sans rien afficher. Le codec se nomme par cette table :

Code	Codec
mp4a	aac
Opus, opus	opus
avc1, avc3	h264
hvc1, hev1	h265
vp08	vp8
vp09	vp9
mp4v	mpeg4
.mp3	mp3
twos, sowt, lpcm	pcm
Un code absent de la table garde son nom de quatre caractères, tel quel. Les majuscules comptent.

Le lecteur. Il regarde la première piste vide du MP4. Son code est lisible s'il vaut mjpa, jpeg, MJPG, avc1, avc3, hvc1, hev1 ou av01 : LISIBLE. Tout autre code : ECHEC. Sans piste vide : SANS_IMAGE.

MP4 compte les fichiers MP4, LISIBLES ceux qui ont LISIBLE, INCONNUS ceux qui ont INCONNU. S'il n'y a aucun fichier, les trois valent zéro.

Un exemple déroulé
Reprenons l'exemple.

film.mp4 : les octets 4 à 7 valent 66 74 79 70 : MP4. Piste 1, vide avc1 : VIDEO h264. Piste 2, soun mp4a : AUDIO aac. La première piste vide porte avc1 : LISIBLE.
clip.mp4 : « ftyp » encore, MP4. mjpa n'est pas dans la table des codecs : il garde son nom, mjpa. Le lecteur le connaît : LISIBLE.
bruit.wav : « RIFF » puis « WAVE » : WAV. Sa piste est ignorée.
capture.avi : « RIFF », mais « AVI » à la place de « WAVE ». Aucune règle ne s'applique : INCONNU, malgré son extension.
Bilans : 2 MP4, 2 lisibles, 1 inconnu.
Ce qui est attendu de vous
votre programme reconnaît le conteneur par les octets, jamais par l'extension
votre programme lit toutes les pistes d'une ligne, même quand il les ignore
votre programme garde tel quel un code absent de la table
votre programme juge seulement la première piste vide
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo9-conteneur_et_codec/ et mettez-y exactement ces fichiers, avec ces noms :

main.cpp
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Les pièges
Un fichier qui n'est pas MP4 a quand même ses pistes sur la ligne : lisez-les, sinon la ligne suivante est lue de travers.
vp09 se nomme vp9, mais le lecteur ne le lit pas dans un MP4 : nommer un codec n'est pas savoir le décoder.
Une règle qui demande 12 octets ne s'applique pas à une ligne qui en donne 8, même si les premiers ressemblent.
Le format de sortie, et il est strict
Le rappel du format, le même à chaque exercice
Une valeur par ligne. Aucune espace en fin de ligne. Un saut de ligne après la dernière valeur, et un seul. Aucun autre texte : pas de message, pas d'invite, pas de titre.

La plateforme compare votre sortie à la sortie attendue, caractère par caractère. Une espace en trop fait échouer un programme juste.

Déposer
Olivieraustinmani-PNG


### exercice 10: l'erreur claire


Où déposer
ani-2053/chapitre-05/exo10-l_erreur_claire/lecteur.cpp, resultats.txt, reponse.txt

Ce qui sera vérifié
Votre dépôt et son dossierani-2053 / chapitre-05/exo10-l_erreur_claire
Les fichiers, au nom exactlecteur.cpp, resultats.txt, reponse.txt
Le style du code — signalé, sans coûter de points
3 missionsle programme appelle Open et ProbeFile · trois lignes de resultats au format demande · les trois etiquettes de la reponse sont la
Les missions masquées ne sont pas montrées : leur nombre l'est, pour que vous n'écriviez pas un programme taillé sur les seuls exemples visibles.

Énoncé
Ce que vous allez faire
Ouvrir trois fichiers avec le lecteur vidéo de NKMedia, dont deux qu'il devrait refuser, et dire ce qu'un refus vous apprend et ce qu'il ne vous apprend pas.

Pourquoi ça sert
Un module honnête échoue proprement sur ce qu'il ne sait pas faire, au lieu de planter. Mais un refus ne dit pas toujours pourquoi : la cause, c'est à vous de la trouver.

Comment faire
Préparez trois fichiers : une vidéo que le lecteur ouvre, la copie de cette vidéo coupée à la moitié de sa taille, et un fichier texte renommé en .mp4.
Dans lecteur.cpp, incluez NKMedia/Video/NkVideoReader.h. Pour chaque fichier, appelez Open. S'il réussit, journalisez Info().container, Info().codec, la largeur et la hauteur, puis comptez les images lues par ReadFrame.
Pour chaque fichier, appelez aussi NkMediaProbe::ProbeFile, de NKMedia/NkMediaProbe.h, et journalisez le conteneur et le codec de chaque piste.
Dans resultats.txt, écrivez une ligne par fichier, sous la forme fichier : film.mp4, ouvert : oui, images : 240, sonde : MP4 (en anglais : file : film.mp4, opened : yes, frames : 240, probe : MP4). Pour un refus, images : 0.
Dans reponse.txt, écrivez trois paragraphes étiquetés : Ce que le refus m'apprend :, Ce qu'il ne m'apprend pas :, puis Comment j'ai trouve la cause : (en anglais : What the refusal tells me :, What it does not tell me :, How I found the cause :).
Ce que vous rendez
Dans votre dépôt GitHub, créez le dossier chapitre-05/exo10-l_erreur_claire/ et mettez-y exactement ces fichiers, avec ces noms :

lecteur.cpp
resultats.txt
reponse.txt
Deux règles de dépôt, les mêmes à chaque exercice
Le chemin et les noms ne se négocient pas. La plateforme cherche vos fichiers là, et nulle part ailleurs.

N'écrivez ni votre nom ni votre matricule dans ces fichiers.

Ce que la machine vérifie
Comment lire ces contrôles
Avant qu'un correcteur voie votre rendu, la plateforme contrôle ceci et vous montre le résultat tout de suite. S'il manque un fichier, votre rendu n'est pas lu : corrigez et redéposez. Les autres contrôles vous préviennent, mais ils ne font pas la note : c'est le correcteur qui note.

les trois fichiers sont rendus, avec les noms exacts
lecteur.cpp appelle Open et ProbeFile
resultats.txt porte trois lignes au format demandé
reponse.txt porte les trois étiquettes demandées
Comment vous serez noté
Ce qui est vérifié	Points
les trois fichiers sont rendus	2
le programme teste le retour de Open avant de lire	4
resultats.txt distingue ce que le lecteur ouvre de ce qu'il refuse, et la sonde est notée pour chacun	5
reponse.txt dit précisément ce qu'un refus ne dit pas	4
la cause de chaque refus est trouvée, et la sonde a servi à la trouver	5
Total	20
Les pièges
Open rend seulement vrai ou faux. Ne lui demandez pas la cause : cherchez-la avec la sonde.
N'affirmez pas qu'un format échoue sans l'avoir essayé : un module évolue, et ses commentaires peuvent retarder sur son code.
Un fichier coupé peut encore s'ouvrir et échouer plus loin, dans ReadFrame : comptez les images pour le voir.
Déposer
Cette tâche est corrigée par une personne, pas par une machine : le verdict arrive après relecture.

Olivieraustinmani-PNG
