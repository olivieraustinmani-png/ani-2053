# Exercice 1 – Création d’un dépôt et gestion des commits

## Étapes réalisées

1. ### Création du premier fichier et commit :
   ```bash
   echo "mon fichier 1" > fichier1.txt
   git add fichier1.txt
   git commit -m "ajout du fichier1"

2. ### Création du deuxième fichier et commit : 
* ```bash
    echo "mon fichier 2" > fichier2.txt
    git add fichier2.txt
    git commit -m "ajout du fichier2"

3. ### Création du troisième fichier et commit :
- ```bash
    echo "mon fichier 3" > fichier3.txt
    git add fichier3.txt
    git commit -m "ajout du fichier3"

4. ### Affichage de l’historique en une ligne par commit :
- git log --oneline

 028e7ea (HEAD -> main) ajout du fichier3
 8f9a34c ajout du fichier2
 654669f ajout du fichier1
 1861db0 (upstream/main, upstream/HEAD, origin/main, origin/HEAD) ajout du fichier1
0d282a3 Initial commit

5. ### Affichage du graphe :
* git log --oneline --graph --decorate --all

"
* 028e7ea (HEAD -> main) ajout du fichier3
* 8f9a34c ajout du fichier2
* 654669f ajout du fichier1
* 1861db0 (upstream/main, upstream/HEAD, origin/main, origin/HEAD) ajout du fichier1
* 0d282a3 Initial commit
"