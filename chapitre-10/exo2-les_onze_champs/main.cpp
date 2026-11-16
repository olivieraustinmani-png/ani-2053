// =============================================================================
// main.cpp — exercice 2, chapitre 10 : les onze champs
//
// Rejoue l'enregistrement puis la réouverture d'une planche d'images, et dit
// quels champs de chaque image reviennent changés.
//
// Le fichier .nkref écrit : position, échelle, rotation, opacité, miroirs,
// largeur, hauteur, octets source. Il n'écrit NI l'identifiant, NI la sélection,
// NI le chemin : à la réouverture ils reviennent à 0, 0 et vide.
// Une image à zéro octet ne se décode pas : elle est sautée, les suivantes
// remontent d'une place, et elle ne compte ni dans RESTENT ni dans CHANGES.
//
// Entrée : N, puis N lignes de 13 mots
//          id x y echelle rotation opacite miroirX miroirY largeur hauteur selectionne chemin octets
// Sortie : ITEM k SAUTE | ITEM k j [id] [selected] [sourcePath] | AUCUN ...
//          RESTENT r
//          CHANGES c
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cstdio>
#include <iostream>
#include <string>

namespace {
	// Les douze premiers mots sont les onze champs d'une image (la position en
	// compte deux) ; le treizième, octets, est tenu à côté par l'affichage.
	struct Image {
		long long id = 0;
		double x = 0.0;
		double y = 0.0;
		double echelle = 0.0;
		double rotation = 0.0;
		double opacite = 0.0;
		int miroirX = 0;
		int miroirY = 0;
		long long largeur = 0;
		long long hauteur = 0;
		int selectionne = 0;
		std::string chemin;
		long long octets = 0;
	};
} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	long long n = 0;
	if (!(std::cin >> n)) {
		return 0;
	}
	if (n < 0) {
		n = 0;
	}

	long long restent = 0;
	long long changes = 0;

	for (long long k = 1; k <= n; ++k) {
		Image img;
		std::cin >> img.id >> img.x >> img.y >> img.echelle >> img.rotation >> img.opacite
			>> img.miroirX >> img.miroirY >> img.largeur >> img.hauteur >> img.selectionne
			>> img.chemin >> img.octets;

		// Zéro octet source : rien à décoder, l'image ne revient pas.
		if (img.octets == 0) {
			std::printf("ITEM %lld SAUTE\n", k);
			continue;
		}

		// L'image revient : sa place est celle de la planche rouverte, donc
		// le nombre d'images déjà revenues, plus une.
		++restent;

		// Les champs qui reviennent changés, toujours dans cet ordre :
		// id, selected, sourcePath. Les autres sont écrits et reviennent identiques.
		const bool idChange = (img.id != 0);
		const bool selectedChange = (img.selectionne == 1);
		const bool cheminChange = (img.chemin != "-");
		const int nbChanges = static_cast<int>(idChange) + static_cast<int>(selectedChange) +
							  static_cast<int>(cheminChange);
		changes += nbChanges;

		std::printf("ITEM %lld %lld", k, restent);
		if (idChange) {
			std::printf(" id");
		}
		if (selectedChange) {
			std::printf(" selected");
		}
		if (cheminChange) {
			std::printf(" sourcePath");
		}
		if (nbChanges == 0) {
			std::printf(" AUCUN");
		}
		std::printf("\n");
	}

	std::printf("RESTENT %lld\n", restent);
	std::printf("CHANGES %lld\n", changes);
	return 0;
}
