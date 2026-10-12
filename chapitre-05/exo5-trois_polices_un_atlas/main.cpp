// =============================================================================
// main.cpp — EXO 5 Sprint 5 : trois polices, un atlas
//
// Range les glyphes de plusieurs polices dans une seule texture, par
// étagères, et agrandit la texture tant qu'ils ne tiennent pas.
// Un essai manqué recommence TOUT depuis le premier glyphe, dans la texture
// agrandie. Huit essais au plus. Calculs sur 64 bits (W*H déborde un int
// au-delà de 46341 de côté).
// =============================================================================
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

	struct Glyphe {
		std::string police;
		long long w = 0;
		long long h = 0;
		// résultat du rangement
		long long x = 0, y = 0; // place du glyphe (avant-dernier et dernier servent à l'affichage)
	};

	// Un essai : range tous les glyphes, groupe après groupe, dans l'ordre de
	// lecture. Rend true si tout tient.
	bool Ranger(std::vector<Glyphe> &g, long long P, long long W, long long H) {
		long long x = P, y = P, e = 0; // curseur + hauteur d'étagère (marge comprise)
		for (Glyphe &gl : g) {
			const long long rw = gl.w + P;
			const long long rh = gl.h + P;

			if (x + rw > W - P) { // étagère suivante
				x = P;
				y = y + e + P;
				e = 0;
				if (x + rw > W - P) return false; // même à l'étagère neuve : trop large
			}
			if (y + rh > H - P) return false; // plus de hauteur : l'essai échoue

			gl.x = x;
			gl.y = y;
			x += rw;
			if (rh > e) e = rh;
		}
		return true;
	}

} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	long long P = 0, L = 0;
	if (!(std::cin >> P >> L)) return 0;

	int G = 0;
	if (!(std::cin >> G)) G = 0;

	std::vector<Glyphe> glyphes;
	long long nbParGroupe = 0;
	std::vector<std::string> nomsGroupes;
	std::vector<std::pair<long long, long long>> taillesGroupes; // w,h par groupe

	for (int i = 0; i < G; ++i) {
		std::string nom;
		long long n = 0, w = 0, h = 0;
		std::cin >> nom >> n >> w >> h;
		nomsGroupes.push_back(nom);
		taillesGroupes.push_back({w, h});
		nbParGroupe += n;
		for (long long k = 0; k < n; ++k)
			glyphes.push_back({nom, w, h, 0, 0});
	}

	if (glyphes.empty()) { // règle 1 : aucun groupe -> AUCUN, seul
		std::cout << "AUCUN\n";
		return 0;
	}

	// Règle 3 : largeur de départ. L, ou 512 si L vaut zéro. Le besoin est la
	// somme des (w+P)*(h+P) : on double W tant que W*W < 2*besoin, plafonné.
	long long W = (L == 0) ? 512 : L;
	if (L == 0) {
		long long besoin = 0;
		for (const Glyphe &gl : glyphes) besoin += (gl.w + P) * (gl.h + P);
		while (W * W < 2 * besoin && W < 4096) W *= 2;
	}
	long long H = W;

	long long essais = 0;
	bool reussi = false;
	for (int t = 1; t <= 8; ++t) {
		essais = t;
		if (Ranger(glyphes, P, W, H)) { // règle 8 : un essai manqué grandit la texture
			reussi = true;
			break;
		}
		if (W == H) W *= 2; // règle 7 : carrée -> W double ; sinon H prend W
		else H = W;
	}

	if (!reussi) {
		std::cout << "ESSAIS 8\nECHEC\n";
		return 0;
	}

	std::cout << "ESSAIS " << essais << "\n";
	std::cout << "TEXTURE " << W << " " << H << "\n";

	// Une ligne par groupe : x1 y1 du PREMIER glyphe, x2 y2 du dernier.
	// Les glyphes sont rangés dans l'ordre : on suit les groupes.
	size_t idx = 0;
	for (int i = 0; i < G; ++i) {
		const long long n = glyphes.size();
		(void)n;
		const long long count = [&] {
			long long c = 0;
			for (const Glyphe &gl : glyphes)
				if (gl.police == nomsGroupes[i]) ++c;
			return c;
		}();
		const Glyphe &premier = glyphes[idx];
		const Glyphe &dernier = glyphes[idx + count - 1];
		std::cout << nomsGroupes[i] << " " << premier.x << " " << premier.y << " " << dernier.x
				  << " " << dernier.y << "\n";
		idx += count;
	}

	// OCCUPE : la somme des w*h de tous les glyphes, sans marge.
	// PERDU : (W*H - OCCUPE) * 100 / (W*H), arrondi vers le bas.
	long long occupe = 0;
	for (const Glyphe &gl : glyphes) occupe += gl.w * gl.h;
	const long long surface = W * H;
	const long long perdu = (surface - occupe) * 100 / surface;
	std::cout << "OCCUPE " << occupe << "\n";
	std::cout << "PERDU " << perdu << "\n";
	(void)nbParGroupe;
	return 0;
}
