// =============================================================================
// main.cpp — exercice 9, chapitre 10 : l'identifiant plutôt que l'alignement
//
// Deux façons de relier une image à sa texture :
//   - par ALIGNEMENT : deux tableaux parallèles, qui répètent chaque changement
//     d'ordre. On compte les occasions d'oublier et les gestes à répéter ;
//   - par IDENTIFIANT : un dictionnaire, rien à répéter, mais une recherche
//     par image à chaque dessin.
//
// Entrée : N, puis N opérations (AJOUTE, SUPPRIME c i1..ic, MONTE i, DESCEND i, DESSINE).
// Sortie : ORDRE ..., OCCASIONS o, REPERCUTES r, RECHERCHES s.
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace {
	struct Slot {
		int id = 0; // identifiant stable, attribué dans l'ordre des AJOUTE
	};
} // namespace

int main() {
	long long n = 0;
	if (!(std::cin >> n)) {
		return 0;
	}

	std::vector<Slot> planche;
	int prochainId = 1;     // jamais réutilisé, même après une suppression
	long long occasions = 0; // SUPPRIME et échanges qui ont eu lieu
	long long repercutes = 0; // gestes à répéter dans les tableaux parallèles
	long long recherches = 0; // un par image présente à chaque DESSINE

	for (long long k = 0; k < n; ++k) {
		std::string op;
		std::cin >> op;
		if (op == "AJOUTE") {
			// Ajouter à la fin ne change pas l'ordre : rien à répéter.
			planche.push_back(Slot{prochainId});
			++prochainId;
		} else if (op == "SUPPRIME") {
			int c = 0;
			std::cin >> c;
			std::vector<int> index;
			index.reserve(static_cast<std::size_t>(c));
			for (int j = 0; j < c; ++j) {
				int i = 0;
				std::cin >> i;
				index.push_back(i);
			}
			// Une suppression est une occasion, et coûte 2 gestes par index.
			++occasions;
			repercutes += 2LL * c;
			// On retire du plus grand au plus petit pour ne pas décaler les suivants.
			for (std::size_t m = index.size(); m > 0; --m) {
				const int i = index[m - 1];
				if (i >= 0 && static_cast<std::size_t>(i) < planche.size()) {
					planche.erase(planche.begin() + i);
				}
			}
		} else if (op == "MONTE" || op == "DESCEND") {
			int i = 0;
			std::cin >> i;
			const int j = (op == "MONTE") ? i + 1 : i - 1;
			const int taille = static_cast<int>(planche.size());
			// Un échange qui n'a pas lieu n'est ni une occasion ni un geste.
			if (i < 0 || j < 0 || i >= taille || j >= taille) {
				continue;
			}
			const int t = planche[static_cast<std::size_t>(i)].id;
			planche[static_cast<std::size_t>(i)].id = planche[static_cast<std::size_t>(j)].id;
			planche[static_cast<std::size_t>(j)].id = t;
			++occasions;
			repercutes += 2;
		} else if (op == "DESSINE") {
			// Par identifiant, chaque image présente coûte une recherche.
			recherches += static_cast<long long>(planche.size());
		}
	}

	std::printf("ORDRE");
	if (planche.empty()) {
		std::printf(" AUCUN");
	} else {
		for (const Slot &s : planche) {
			std::printf(" %d", s.id);
		}
	}
	std::printf("\n");
	std::printf("OCCASIONS %lld\n", occasions);
	std::printf("REPERCUTES %lld\n", repercutes);
	std::printf("RECHERCHES %lld\n", recherches);
	return 0;
}
