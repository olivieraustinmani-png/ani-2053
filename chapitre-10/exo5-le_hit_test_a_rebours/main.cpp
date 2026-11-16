// =============================================================================
// main.cpp — exercice 5, chapitre 10 : le hit test à rebours
//
// Sur la planche, l'ordre du tableau EST la profondeur : la dernière image est
// dessinée par-dessus. A est donc trouvée à rebours (de la dernière à la
// première), B à l'endroit — et l'on compte les clics où les deux diffèrent.
//
// Entrée : N, puis N lignes "nom x y largeur hauteur echelle",
//          puis Q, puis Q lignes "x y".
// Sortie : CLIC k A B, puis ERREURS e.
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace {
	struct Image {
		std::string nom;
		double x = 0.0;
		double y = 0.0;
		double demiLargeur = 0.0;
		double demiHauteur = 0.0;
	};

	// Un point posé sur le bord touche l'image : on compare à la demi-taille
	// avec "ne dépasse pas", et l'échelle est déjà appliquée à la construction.
	bool Touche(const Image &img, double px, double py) {
		return std::fabs(px - img.x) <= img.demiLargeur && std::fabs(py - img.y) <= img.demiHauteur;
	}
} // namespace

int main() {
	long long n = 0;
	if (!(std::cin >> n)) {
		return 0;
	}
	if (n < 0) {
		n = 0;
	}

	std::vector<Image> images;
	images.reserve(static_cast<std::size_t>(n));
	for (long long i = 0; i < n; ++i) {
		Image img;
		double largeur = 0.0;
		double hauteur = 0.0;
		double echelle = 1.0;
		std::cin >> img.nom >> img.x >> img.y >> largeur >> hauteur >> echelle;
		// La largeur lue est la taille native : c'est l'échelle qui donne la
		// taille dans le monde, appliquée aux DEUX demi-tailles.
		img.demiLargeur = largeur * echelle / 2.0;
		img.demiHauteur = hauteur * echelle / 2.0;
		images.push_back(img);
	}

	long long q = 0;
	if (!(std::cin >> q)) {
		return 0;
	}

	long long erreurs = 0;
	for (long long clic = 1; clic <= q; ++clic) {
		double px = 0.0;
		double py = 0.0;
		std::cin >> px >> py;

		// A : à rebours, la première touchée en partant de la fin.
		const Image *a = nullptr;
		for (long long i = n - 1; i >= 0; --i) {
			if (Touche(images[static_cast<std::size_t>(i)], px, py)) {
				a = &images[static_cast<std::size_t>(i)];
				break;
			}
		}

		// B : à l'endroit, la première touchée en partant du début.
		const Image *b = nullptr;
		for (long long i = 0; i < n; ++i) {
			if (Touche(images[static_cast<std::size_t>(i)], px, py)) {
				b = &images[static_cast<std::size_t>(i)];
				break;
			}
		}

		const std::string nomA = (a != nullptr) ? a->nom : std::string("AUCUN");
		const std::string nomB = (b != nullptr) ? b->nom : std::string("AUCUN");
		if (nomA != nomB) {
			++erreurs;
		}
		std::printf("CLIC %lld %s %s\n", clic, nomA.c_str(), nomB.c_str());
	}

	std::printf("ERREURS %lld\n", erreurs);
	return 0;
}
