// =============================================================================
// main.cpp — exercice 10, chapitre 10 : la sélection sortie du modèle
//
// La même suite d'opérations, rejouée sous trois façons de tenir la sélection :
//   - DANS_ITEM : chaque image porte un drapeau, qui la suit partout ;
//   - PAR_INDEX : l'interface garde des index, JAMAIS mis à jour après un
//                 changement d'ordre : le premier échange la fait désigner
//                 une autre image ;
//   - PAR_ID    : l'interface garde des noms.
//
// Entrée : des lignes jusqu'à la fin (AJOUTE, CLIC, AJOUTE_SEL, ECHANGE,
//          RETIRE, SUPPRIME).
// Sortie : deux lignes par conception, dans l'ordre DANS_ITEM, PAR_INDEX,
//          PAR_ID, puis FAUX ... (ou FAUX AUCUN).
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {
	enum class Mode { DansItem, ParIndex, ParId };

	struct Operation {
		std::string op;
		std::string nom;   // AJOUTE
		std::vector<int> index; // CLIC, AJOUTE_SEL, ECHANGE, RETIRE
	};

	struct Item {
		std::string nom;
		bool choisi = false; // utilisé par DANS_ITEM
	};

	struct Conception {
		std::vector<Item> planche;
		std::vector<int> indexGardes;   // PAR_INDEX : jamais remis à jour
		std::vector<std::string> nomsGardes; // PAR_ID
		Mode mode = Mode::DansItem;

		bool IndexExiste(int i) const {
			return i >= 0 && static_cast<std::size_t>(i) < planche.size();
		}

		// La sélection, dans l'ordre de la planche.
		std::vector<std::string> Selection() const {
			std::vector<std::string> out;
			for (std::size_t i = 0; i < planche.size(); ++i) {
				bool choisi = false;
				if (mode == Mode::DansItem) {
					choisi = planche[i].choisi;
				} else if (mode == Mode::ParIndex) {
					choisi = std::find(indexGardes.begin(), indexGardes.end(), static_cast<int>(i)) != indexGardes.end();
				} else {
					choisi = std::find(nomsGardes.begin(), nomsGardes.end(), planche[i].nom) != nomsGardes.end();
				}
				if (choisi) {
					out.push_back(planche[i].nom);
				}
			}
			return out;
		}

		void ViderSelection() {
			if (mode == Mode::DansItem) {
				for (Item &it : planche) {
					it.choisi = false;
				}
			} else if (mode == Mode::ParIndex) {
				indexGardes.clear();
			} else {
				nomsGardes.clear();
			}
		}

		void Choisir(int i) {
			if (!IndexExiste(i)) {
				return;
			}
			if (mode == Mode::DansItem) {
				planche[static_cast<std::size_t>(i)].choisi = true;
			} else if (mode == Mode::ParIndex) {
				indexGardes.push_back(i);
			} else {
				nomsGardes.push_back(planche[static_cast<std::size_t>(i)].nom);
			}
		}

		void Executer(const Operation &o) {
			if (o.op == "AJOUTE") {
				Item it;
				it.nom = o.nom;
				planche.push_back(it);
			} else if (o.op == "CLIC") {
				// La sélection se vide d'abord, même si l'index n'existe pas.
				ViderSelection();
				Choisir(o.index[0]);
			} else if (o.op == "AJOUTE_SEL") {
				Choisir(o.index[0]);
			} else if (o.op == "ECHANGE") {
				const int i = o.index[0];
				const int j = o.index[1];
				if (IndexExiste(i) && IndexExiste(j)) {
					std::swap(planche[static_cast<std::size_t>(i)], planche[static_cast<std::size_t>(j)]);
				}
			} else if (o.op == "RETIRE") {
				const int i = o.index[0];
				if (IndexExiste(i)) {
					planche.erase(planche.begin() + i);
					// Les index et les noms gardés ne sont pas mis à jour : c'est
					// le principe de chaque conception, et le piège de PAR_INDEX.
				}
			} else if (o.op == "SUPPRIME") {
				// De la fin vers le début, pour que les index restent valables.
				for (std::size_t k = planche.size(); k > 0; --k) {
					const std::size_t i = k - 1;
					bool choisi = false;
					if (mode == Mode::DansItem) {
						choisi = planche[i].choisi;
					} else if (mode == Mode::ParIndex) {
						choisi = std::find(indexGardes.begin(), indexGardes.end(), static_cast<int>(i)) != indexGardes.end();
					} else {
						choisi = std::find(nomsGardes.begin(), nomsGardes.end(), planche[i].nom) != nomsGardes.end();
					}
					if (choisi) {
						planche.erase(planche.begin() + static_cast<long long>(i));
					}
				}
				ViderSelection();
			}
		}
	};

	std::string Joindre(const std::vector<std::string> &noms) {
		if (noms.empty()) {
			return "AUCUN";
		}
		std::string s;
		for (std::size_t i = 0; i < noms.size(); ++i) {
			if (i > 0) {
				s += " ";
			}
			s += noms[i];
		}
		return s;
	}

	std::vector<std::string> NomsPlanche(const Conception &c) {
		std::vector<std::string> out;
		out.reserve(c.planche.size());
		for (const Item &it : c.planche) {
			out.push_back(it.nom);
		}
		return out;
	}
} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	// On lit chaque ligne UNE fois, et on rejoue l'opération sur les trois
	// conceptions : relire l'entrée trois fois consommerait les arguments de
	// l'opération suivante.
	std::vector<Operation> operations;
	std::string ligne;
	while (std::getline(std::cin, ligne)) {
		std::istringstream flux(ligne);
		Operation o;
		if (!(flux >> o.op)) {
			continue;
		}
		if (o.op == "AJOUTE") {
			flux >> o.nom;
		} else if (o.op == "CLIC" || o.op == "AJOUTE_SEL" || o.op == "RETIRE") {
			int i = 0;
			flux >> i;
			o.index.push_back(i);
		} else if (o.op == "ECHANGE") {
			int i = 0;
			int j = 0;
			flux >> i >> j;
			o.index.push_back(i);
			o.index.push_back(j);
		}
		operations.push_back(o);
	}

	Conception dansItem;
	dansItem.mode = Mode::DansItem;
	Conception parIndex;
	parIndex.mode = Mode::ParIndex;
	Conception parId;
	parId.mode = Mode::ParId;

	for (const Operation &o : operations) {
		dansItem.Executer(o);
		parIndex.Executer(o);
		parId.Executer(o);
	}

	const Conception *conceptions[3] = {&dansItem, &parIndex, &parId};
	const char *noms[3] = {"DANS_ITEM", "PAR_INDEX", "PAR_ID"};

	const std::vector<std::string> plancheRef = NomsPlanche(dansItem);
	const std::vector<std::string> selectionRef = dansItem.Selection();

	for (int k = 0; k < 3; ++k) {
		const Conception &c = *conceptions[k];
		std::printf("%s PLANCHE %s\n", noms[k], Joindre(NomsPlanche(c)).c_str());
		std::printf("%s SELECTION %s\n", noms[k], Joindre(c.Selection()).c_str());
	}

	std::vector<std::string> faux;
	for (int k = 1; k < 3; ++k) {
		const Conception &c = *conceptions[k];
		if (NomsPlanche(c) != plancheRef || c.Selection() != selectionRef) {
			faux.push_back(noms[k]);
		}
	}
	std::printf("FAUX %s\n", Joindre(faux).c_str());
	return 0;
}
