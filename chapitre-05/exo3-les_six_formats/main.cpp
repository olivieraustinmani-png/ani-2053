// =============================================================================
// main.cpp — EXO 3 Sprint 5 : les six formats
//
// Calcule la mémoire d'une image dans deux formats de pixels, et ce qu'une
// conversion fait perdre. Les six formats et leurs propriétés sont la table
// de l'énoncé — la même que NkImagePixelFormat dans le moteur.
// Calcul sur 64 bits : une image de 65536 de côté en RGBA128F dépasse
// soixante milliards d'octets, un int déborde.
// =============================================================================
#include <cstdint>
#include <iostream>
#include <string>

namespace {

	// Les six formats, et eux seuls (table de l'énoncé).
	struct Format {
		const char *nom;
		long long octetsParPixel;
		bool couleur;
		bool transparence;
		bool flottants;
	};

	const Format kFormats[] = {
		{"GRAY8", 1, false, false, false},
		{"GRAY_A16", 2, false, true, false},
		{"RGB24", 3, true, false, false},
		{"RGBA32", 4, true, true, false},
		{"RGB96F", 12, true, false, true},
		{"RGBA128F", 16, true, true, true},
	};
	const int kNbFormats = 6;

	const Format *Chercher(const std::string &nom) {
		for (int i = 0; i < kNbFormats; ++i)
			if (nom == kFormats[i].nom) return &kFormats[i];
		return nullptr; // nom inconnu -> REFUSE (comparaison exacte)
	}

} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	long long w = 0, h = 0;
	if (!(std::cin >> w >> h)) return 0;

	int n = 0;
	if (!(std::cin >> n)) n = 0;

	long long total = 0;
	long long sansPerte = 0;
	long long refuses = 0;

	for (int i = 0; i < n; ++i) {
		std::string source, cible;
		std::cin >> source >> cible;

		const Format *s = Chercher(source);
		const Format *c = Chercher(cible);

		if (s == nullptr || c == nullptr) {
			std::cout << source << " " << cible << " REFUSE\n";
			++refuses;
			continue;
		}

		// w * h * octets par pixel — tout sur 64 bits.
		const long long pixels = w * h;
		const long long octetsSource = pixels * s->octetsParPixel;
		const long long octetsCible = pixels * c->octetsParPixel;

		// Les pertes, DANS L'ORDRE de la règle 2 : TRANSPARENCE puis COULEUR
		// puis ETENDUE. Gagner ce qu'on n'avait pas n'est pas une perte.
		std::string pertes;
		if (s->transparence && !c->transparence) pertes = "TRANSPARENCE";
		if (s->couleur && !c->couleur) pertes += (pertes.empty() ? "" : "+") + std::string("COULEUR");
		if (s->flottants && !c->flottants) pertes += (pertes.empty() ? "" : "+") + std::string("ETENDUE");
		if (pertes.empty()) pertes = "AUCUNE";

		std::cout << source << " " << cible << " " << octetsSource << " " << octetsCible << " "
				  << pertes << "\n";

		if (pertes == "AUCUNE") ++sansPerte;
		total += octetsCible;
	}

	std::cout << "TOTAL " << total << "\nSANS_PERTE " << sansPerte << "\nREFUSES " << refuses
			  << "\n";
	return 0;
}
