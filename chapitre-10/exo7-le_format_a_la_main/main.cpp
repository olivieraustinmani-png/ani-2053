// =============================================================================
// main.cpp — exercice 7, chapitre 10 : le format à la main
//
// Décode octet par octet un fichier de planche .nkref, en petit-boutiste, et
// dit ce qu'il contient sans ouvrir l'outil. Les huit premiers octets (nombre
// magique NKRF, puis version) passent avant tout le reste : sans le premier on
// ne sait pas si le fichier est le sien, sans la seconde on ne pourra jamais
// changer d'avis.
//
// Entrée : T, puis T octets en hexadécimal.
// Sortie : une seule ligne de refus (TRONQUE, PAS_UNE_PLANCHE, VERSION_INCONNUE v)
//          ou le contenu complet (VUE, THEME, GRILLE, IMAGES, TRAITS).
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {
	// La lecture vérifie qu'il reste assez d'octets AVANT chaque accès ; c'est
	// ce qui fait de « il manque un octet, n'importe où » un TRONQUE et non une
	// lecture hors bornes.
	class Lecteur {
		public:
		Lecteur(const std::vector<unsigned char> &octets) : mOctets(octets) {
		}

		bool Reste(std::size_t n) const {
			return mPos + n <= mOctets.size();
		}

		bool Octet(unsigned char &v) {
		if (!Reste(1)) {
			return false;
		}
		v = mOctets[mPos++];
		return true;
	}

		bool U32(unsigned long &v) {
		if (!Reste(4)) {
			return false;
		}
		v = static_cast<unsigned long>(mOctets[mPos]) |
			(static_cast<unsigned long>(mOctets[mPos + 1]) << 8) |
			(static_cast<unsigned long>(mOctets[mPos + 2]) << 16) |
			(static_cast<unsigned long>(mOctets[mPos + 3]) << 24);
		mPos += 4;
		return true;
	}

		bool F32(float &v) {
		if (!Reste(4)) {
			return false;
		}
		// Le fichier est en simple précision, petit-boutiste : on recompose les
		// quatre octets dans l'ordre de l'octet de poids faible au plus fort.
		const unsigned int bits = static_cast<unsigned int>(mOctets[mPos]) |
			(static_cast<unsigned int>(mOctets[mPos + 1]) << 8) |
			(static_cast<unsigned int>(mOctets[mPos + 2]) << 16) |
			(static_cast<unsigned int>(mOctets[mPos + 3]) << 24);
		std::memcpy(&v, &bits, sizeof(v));
		mPos += 4;
		return true;
	}

		// Sauter des octets (les octets source d'une image) : s'ils manquent,
		// c'est un fichier tronqué, même si la vue a déjà été lue.
		bool Sauter(unsigned long n) {
			if (!Reste(n)) {
				return false;
			}
			mPos += n;
			return true;
		}

		private:
		const std::vector<unsigned char> &mOctets;
		std::size_t mPos = 0;
	};

	double SansZeroNegatif(double v) {
		return (std::fabs(v) < 0.005) ? 0.0 : v;
	}

	void Ecrire2(double v) {
		std::printf("%.2f", SansZeroNegatif(v));
	}

	void EcrireF(float v) {
		Ecrire2(static_cast<double>(v));
	}

	// Un octet de miroir peut valoir 02 : il s'écrit quand même 1.
	int Miroir(unsigned char v) {
		return (v != 0) ? 1 : 0;
	}
} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	unsigned long total = 0;
	if (!(std::cin >> total)) {
		return 0;
	}
	std::vector<unsigned char> octets;
	octets.reserve(total);
	for (unsigned long i = 0; i < total; ++i) {
		std::string mot;
		if (!(std::cin >> mot)) {
			break;
		}
		unsigned long v = 0;
		if (std::sscanf(mot.c_str(), "%lx", &v) != 1) {
			break;
		}
		octets.push_back(static_cast<unsigned char>(v & 0xFFu));
	}

	Lecteur in(octets);

	// 1. Le nombre magique : quatre octets, N K R F. Comparaison octet par octet,
	//    sur la valeur ASCII attendue ('N' = 0x4E, 'K' = 0x4B, 'R' = 0x52, 'F' = 0x46).
	const unsigned char magie[4] = {0x4E, 0x4B, 0x52, 0x46};
	for (int k = 0; k < 4; ++k) {
		unsigned char c = 0;
		if (!in.Octet(c)) {
			std::printf("TRONQUE\n");
			return 0;
		}
		if (c != magie[k]) {
			std::printf("PAS_UNE_PLANCHE\n");
			return 0;
		}
	}

	// 2. La version.
	unsigned long version = 0;
	if (!in.U32(version)) {
		std::printf("TRONQUE\n");
		return 0;
	}
	if (version != 1) {
		std::printf("VERSION_INCONNUE %lu\n", version);
		return 0;
	}

	// 3. La vue : trois f32.
	float cx = 0.0f;
	float cy = 0.0f;
	float zoom = 0.0f;
	if (!in.F32(cx) || !in.F32(cy) || !in.F32(zoom)) {
		std::printf("TRONQUE\n");
		return 0;
	}

	// 4. Thème, grille, deux octets ignorés.
	unsigned char theme = 0;
	unsigned char grille = 0;
	unsigned char remplissageA = 0;
	unsigned char remplissageB = 0;
	if (!in.Octet(theme) || !in.Octet(grille) || !in.Octet(remplissageA) || !in.Octet(remplissageB)) {
		std::printf("TRONQUE\n");
		return 0;
	}

	// 5. Les images.
	unsigned long nbImages = 0;
	if (!in.U32(nbImages)) {
		std::printf("TRONQUE\n");
		return 0;
	}

	// On garde tout en mémoire avant d'imprimer : un refus de dernière minute
	// (un trait tronqué) doit produire une seule ligne, pas les images déjà dites.
	std::vector<std::string> lignesImages;
	for (unsigned long k = 0; k < nbImages; ++k) {
		float x = 0.0f;
		float y = 0.0f;
		float echelle = 0.0f;
		float rotation = 0.0f;
		float opacite = 0.0f;
		unsigned char mx = 0;
		unsigned char my = 0;
		unsigned char ignoreA = 0;
		unsigned char ignoreB = 0;
		unsigned long largeur = 0;
		unsigned long hauteur = 0;
		unsigned long taille = 0;
		if (!in.F32(x) || !in.F32(y) || !in.F32(echelle) || !in.F32(rotation) || !in.F32(opacite) ||
			!in.Octet(mx) || !in.Octet(my) || !in.Octet(ignoreA) || !in.Octet(ignoreB) ||
			!in.U32(largeur) || !in.U32(hauteur) || !in.U32(taille)) {
			std::printf("TRONQUE\n");
			return 0;
		}
		// Les octets source suivent : on les saute.
		if (!in.Sauter(taille)) {
			std::printf("TRONQUE\n");
			return 0;
		}
		char tampon[256];
		std::snprintf(tampon, sizeof(tampon), "IMAGE %lu ", k + 1);
		std::string ligne(tampon);
		std::snprintf(tampon, sizeof(tampon), "%.2f %.2f %.2f %.2f %.2f", SansZeroNegatif(x), SansZeroNegatif(y),
					  SansZeroNegatif(echelle), SansZeroNegatif(rotation), SansZeroNegatif(opacite));
		ligne += tampon;
		std::snprintf(tampon, sizeof(tampon), " %d %d %lu %lu %lu\n", Miroir(mx), Miroir(my), largeur, hauteur, taille);
		ligne += tampon;
		lignesImages.push_back(ligne);
	}

	// 6. Les traits.
	unsigned long nbTraits = 0;
	if (!in.U32(nbTraits)) {
		std::printf("TRONQUE\n");
		return 0;
	}

	std::vector<std::string> lignesTraits;
	for (unsigned long k = 0; k < nbTraits; ++k) {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 0;
		float epaisseur = 0.0f;
		unsigned long nbPoints = 0;
		if (!in.Octet(r) || !in.Octet(g) || !in.Octet(b) || !in.Octet(a) || !in.F32(epaisseur) || !in.U32(nbPoints)) {
			std::printf("TRONQUE\n");
			return 0;
		}
		char tampon[256];
		std::snprintf(tampon, sizeof(tampon), "TRAIT %lu %u %u %u %u ", k + 1, static_cast<unsigned>(r),
					  static_cast<unsigned>(g), static_cast<unsigned>(b), static_cast<unsigned>(a));
		std::string ligne(tampon);
		std::snprintf(tampon, sizeof(tampon), "%.2f %lu\n", SansZeroNegatif(epaisseur), nbPoints);
		ligne += tampon;
		lignesTraits.push_back(ligne);
		for (unsigned long p = 0; p < nbPoints; ++p) {
			float px = 0.0f;
			float py = 0.0f;
			if (!in.F32(px) || !in.F32(py)) {
				std::printf("TRONQUE\n");
				return 0;
			}
			std::snprintf(tampon, sizeof(tampon), "POINT %.2f %.2f\n", SansZeroNegatif(px), SansZeroNegatif(py));
			lignesTraits.push_back(tampon);
		}
	}

	// Tout a été lu : on imprime dans l'ordre du format.
	std::printf("VUE ");
	EcrireF(cx);
	std::printf(" ");
	EcrireF(cy);
	std::printf(" ");
	EcrireF(zoom);
	std::printf("\n");
	std::printf((theme != 0) ? "THEME SOMBRE\n" : "THEME CLAIR\n");
	std::printf((grille == 0) ? "GRILLE AFFICHEE\n" : "GRILLE CACHEE\n");
	std::printf("IMAGES %lu\n", nbImages);
	for (const std::string &l : lignesImages) {
		std::fputs(l.c_str(), stdout);
	}
	std::printf("TRAITS %lu\n", nbTraits);
	for (const std::string &l : lignesTraits) {
		std::fputs(l.c_str(), stdout);
	}
	return 0;
}
