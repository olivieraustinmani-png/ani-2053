// =============================================================================
// main.cpp — exercice 6, chapitre 10 : le troisième tableau
//
// Rejoue la planche (le modèle) et les deux tableaux que le programme
// d'affichage tient alignés sur elle : textures et sources. Chaque opération
// n'est répétée QUE dans les tableaux cités sur sa ligne, et rien ne rappelle
// de le faire : c'est le contrat implicite de l'exercice.
//
// Entrée : AJOUTE / SUPPRIME / ECHANGE suivis des tableaux concernés
//          (0, 1 ou 2 noms parmi textures et sources), puis N et N opérations.
// Sortie : une ligne par index "i modele texture source" (- si absent),
//          puis DECALES d.
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

namespace {
	enum class Etait { Aucun, Ajoute, Supprime, Echange };

	struct Tableau {
		std::vector<std::string> noms;

		void Ajouter(const std::string &nom) {
			noms.push_back(nom);
		}

		// Retirer du plus grand au plus petit : sinon les suivants reculent et
		// l'on retire les mauvaises images. Un index absent est sauté.
		void SupprimerLes(const std::vector<int> &index) {
			for (std::size_t k = index.size(); k > 0; --k) {
				const int i = index[k - 1];
				if (i >= 0 && static_cast<std::size_t>(i) < noms.size()) {
					noms.erase(noms.begin() + i);
				}
			}
		}

		void EchangerLes(int i, int j) {
			const std::size_t n = noms.size();
			if (i < 0 || j < 0 || static_cast<std::size_t>(i) >= n || static_cast<std::size_t>(j) >= n) {
				return;
			}
			const std::string t = noms[static_cast<std::size_t>(i)];
			noms[static_cast<std::size_t>(i)] = noms[static_cast<std::size_t>(j)];
			noms[static_cast<std::size_t>(j)] = t;
		}
	};

	// Lit la fin de la ligne (après le mot-clé) sans consommer la ligne suivante :
	// on lit les noms cités, qui font tous partie de {textures, sources}.
	void LireCites(std::istream &in, Etait &modele, Etait &textures, Etait &sources, Etait quoi) {
		modele = Etait::Aucun;
		textures = Etait::Aucun;
		sources = Etait::Aucun;
		while (true) {
			std::string mot;
			const int c = in.peek();
			if (c == '\n' || c == '\r' || c == std::char_traits<char>::eof()) {
				break;
			}
			in >> mot;
			if (mot == "textures") {
				textures = quoi;
			} else if (mot == "sources") {
				sources = quoi;
			} else {
				// Un mot qui n'est pas un tableau : fin de la ligne de configuration.
				break;
			}
		}
	}
} // namespace

int main() {
	std::string cle;
	Etait modeleAjoute = Etait::Aucun;
	Etait texturesAjoute = Etait::Aucun;
	Etait sourcesAjoute = Etait::Aucun;
	if (!(std::cin >> cle)) {
		return 0;
	}
	LireCites(std::cin, modeleAjoute, texturesAjoute, sourcesAjoute, Etait::Ajoute);

	Etait modeleSupprime = Etait::Aucun;
	Etait texturesSupprime = Etait::Aucun;
	Etait sourcesSupprime = Etait::Aucun;
	std::cin >> cle; // "SUPPRIME"
	LireCites(std::cin, modeleSupprime, texturesSupprime, sourcesSupprime, Etait::Supprime);

	Etait modeleEchange = Etait::Aucun;
	Etait texturesEchange = Etait::Aucun;
	Etait sourcesEchange = Etait::Aucun;
	std::cin >> cle; // "ECHANGE"
	LireCites(std::cin, modeleEchange, texturesEchange, sourcesEchange, Etait::Echange);

	Tableau modele;
	Tableau textures;
	Tableau sources;

	long long n = 0;
	if (!(std::cin >> n)) {
		n = 0;
	}

	for (long long k = 0; k < n; ++k) {
		std::string op;
		std::cin >> op;
		if (op == "AJOUTE") {
			std::string nom;
			std::cin >> nom;
			modele.Ajouter(nom);
			if (texturesAjoute != Etait::Aucun) {
				textures.Ajouter(nom);
			}
			if (sourcesAjoute != Etait::Aucun) {
				sources.Ajouter(nom);
			}
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
			modele.SupprimerLes(index);
			if (texturesSupprime != Etait::Aucun) {
				textures.SupprimerLes(index);
			}
			if (sourcesSupprime != Etait::Aucun) {
				sources.SupprimerLes(index);
			}
		} else if (op == "MONTE" || op == "DESCEND") {
			int i = 0;
			std::cin >> i;
			const int j = (op == "MONTE") ? i + 1 : i - 1;
			const int taille = static_cast<int>(modele.noms.size());
			// Si la voisine n'existe pas dans le modèle, rien ne se passe dans
			// aucun tableau (le piège du MONTE sur la dernière image).
			if (i < 0 || j < 0 || i >= taille || j >= taille) {
				continue;
			}
			modele.EchangerLes(i, j);
			if (texturesEchange != Etait::Aucun) {
				textures.EchangerLes(i, j);
			}
			if (sourcesEchange != Etait::Aucun) {
				sources.EchangerLes(i, j);
			}
		}
	}

	std::size_t longueur = modele.noms.size();
	if (textures.noms.size() > longueur) {
		longueur = textures.noms.size();
	}
	if (sources.noms.size() > longueur) {
		longueur = sources.noms.size();
	}

	long long decales = 0;
	for (std::size_t i = 0; i < longueur; ++i) {
		const bool aModele = i < modele.noms.size();
		const bool aTextures = i < textures.noms.size();
		const bool aSources = i < sources.noms.size();
		const std::string &m = aModele ? modele.noms[i] : std::string("-");
		const std::string &t = aTextures ? textures.noms[i] : std::string("-");
		const std::string &s = aSources ? sources.noms[i] : std::string("-");
		if (!(aModele && aTextures && aSources) || m != t || m != s) {
			++decales;
		}
		std::printf("%zu %s %s %s\n", i, m.c_str(), t.c_str(), s.c_str());
	}
	std::printf("DECALES %lld\n", decales);
	return 0;
}
