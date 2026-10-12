// =============================================================================
// main.cpp — EXO 7 Sprint 5 : le calcul, puis le fichier (sons)
//
// Calcule la taille brute (non compressée) de chaque son, la compare à son
// fichier, et décide MEMOIRE ou FLUX selon le seuil. Calcul sur 64 bits :
// une heure de stéréo 32 bits donne un produit de mille milliards.
// =============================================================================
#include <cstdint>
#include <iostream>
#include <string>

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	long long seuil = 0;
	if (!(std::cin >> seuil)) return 0;

	int n = 0;
	if (!(std::cin >> n)) n = 0;

	long long memoire = 0, flux = 0, refuses = 0;

	for (int i = 0; i < n; ++i) {
		std::string nom;
		long long freq = 0, canaux = 0, bits = 0, duree = 0, fichier = 0;
		std::cin >> nom >> freq >> canaux >> bits >> duree >> fichier;

		// Règle 1 : seules 8, 16, 24 et 32 bits sont acceptées.
		if (bits != 8 && bits != 16 && bits != 24 && bits != 32) {
			std::cout << nom << " REFUSE\n";
			++refuses;
			continue;
		}

		// Règle 2 : frequence * canaux * (bits/8) * duree, division /1000 À LA
		// FIN, arrondi vers le bas. Tout sur 64 bits avant la division.
		const long long brut = freq * canaux * (bits / 8) * duree / 1000;

		// Règle 3 : pourcent = fichier * 100 / brut, arrondi vers le bas.
		const long long pourcent = fichier * 100 / brut;

		// Règle 4 : strictement plus grand que S -> FLUX. Égal au seuil : MEMOIRE.
		const bool enFlux = brut > seuil;
		std::cout << nom << " " << brut << " " << pourcent << " " << (enFlux ? "FLUX" : "MEMOIRE")
				  << "\n";

		if (enFlux) ++flux;
		else memoire += brut;
	}

	std::cout << "MEMOIRE " << memoire << "\nFLUX " << flux << "\nREFUSES " << refuses << "\n";
	return 0;
}
