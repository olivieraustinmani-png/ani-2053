// =============================================================================
// main.cpp — EXO 6 Sprint 5 : la ligne de base
//
// Pose un texte glyphe par glyphe sur sa ligne de base : ce qui monte, ce qui
// descend, et si le texte sort de l'écran. L'axe y descend ; le haut de
// l'écran est y = 0. Le caractère _ est l'espace : il avance sans se dessiner.
// =============================================================================
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct Kerning {
std::string couple;
long long k = 0;
};

struct Glyphe {
		char c;
		long long avance;
		long long x0, y0, x1, y1; // rectangle relatif au curseur et à la ligne de base
	};

	// Le caractère présent dans la table (renvoie nullptr sinon).
	const Glyphe *Chercher(const std::vector<Glyphe> &table, char c) {
		for (const Glyphe &g : table)
			if (g.c == c) return &g;
		return nullptr;
	}

} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int G = 0;
	if (!(std::cin >> G)) return 0;

	std::vector<Glyphe> table;
	for (int i = 0; i < G; ++i) {
		std::string cs;
		long long av = 0, x0 = 0, y0 = 0, x1 = 0, y1 = 0;
		std::cin >> cs >> av >> x0 >> y0 >> x1 >> y1;
		table.push_back({cs[0], av, x0, y0, x1, y1});
	}

	int K = 0;
	std::cin >> K;
	std::vector<Kerning> tableK;
	for (int i = 0; i < K; ++i) {
	std::string couple;
	long long k = 0;
	std::cin >> couple >> k;
	tableK.push_back({couple, k});
	}

	std::string texte;
	long long ox = 0, oy = 0;
	std::cin >> texte >> ox >> oy;

	long long x = ox;
	long long minx = 0, miny = 0, maxx = 0, maxy = 0;
	bool boiteExistante = false;
	int absents = 0;

	for (size_t i = 0; i < texte.size(); ++i) {
		const char c = texte[i];
		const Glyphe *g = Chercher(table, c);

		if (g == nullptr) { // absent : pas d'avance, pas de crénage après lui
			std::cout << c << " ABSENT\n";
			++absents;
			continue;
		}

		std::cout << c << " " << x << "\n";

		// Il se dessine si x1 > x0 et y1 > y0. L'espace _ n'a pas de rectangle,
		// donc ne se dessine jamais : elle avance seulement.
		if (g->x1 > g->x0 && g->y1 > g->y0) {
			const long long rx0 = x + g->x0, ry0 = oy + g->y0;
			const long long rx1 = x + g->x1, ry1 = oy + g->y1;
			if (!boiteExistante) {
				minx = rx0; miny = ry0; maxx = rx1; maxy = ry1;
				boiteExistante = true;
			} else {
				if (rx0 < minx) minx = rx0;
				if (ry0 < miny) miny = ry0;
				if (rx1 > maxx) maxx = rx1;
				if (ry1 > maxy) maxy = ry1;
			}
		}

		x += g->avance;

		// Crénage avec le caractère SUIVANT du texte.
		if (i + 1 < texte.size()) {
			const std::string couple = std::string(1, c) + texte[i + 1];
			for (const Kerning &ck : tableK)
				if (ck.couple == couple) x += ck.k;
		}
	}

	std::cout << "CURSEUR " << x << "\n";
	if (!boiteExistante) {
		std::cout << "BOITE AUCUNE\nMONTE 0\nDESCEND 0\nECRAN RIEN\n";
	} else {
		std::cout << "BOITE " << minx << " " << miny << " " << maxx << " " << maxy << "\n";
		std::cout << "MONTE " << ((miny < oy) ? (oy - miny) : 0) << "\n";
		std::cout << "DESCEND " << ((maxy > oy) ? (maxy - oy) : 0) << "\n";
		const char *verdict = (maxy <= 0) ? "HORS" : (miny < 0 ? "COUPE" : "VISIBLE");
		std::cout << "ECRAN " << verdict << "\n";
	}
	std::cout << "ABSENTS " << absents << "\n";
	return 0;
}
