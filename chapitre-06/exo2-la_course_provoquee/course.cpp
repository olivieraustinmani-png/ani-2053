// =============================================================================
// course.cpp - EXO 2 Sprint 6 : la course, provoquee
//
// Huit fils incrementent un compteur partage un million de fois chacun,
// SANS aucune protection. Le total est faux, et faux differemment a chaque
// execution : c'est la course. Un argument optionnel choisit le nombre de
// fils pour la version a un seul fil.
// Contrainte de ligne de commande : --fils=8 (defaut) ; le programme imprime
// le total puis le total attendu.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. exo1 : include NKMath avant le namespace time)
#include "NKThreading/NkThread.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Course";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {

	const long long kIncrementsParFil = 1000000;

	// Le compteur partage. Pas de verrou, pas d'atomique : c'est l'experience.
	long long compteur = 0;

	void Incrementer(void *) {
		for (long long i = 0; i < kIncrementsParFil; ++i) {
			compteur += 1; // lire, additionner, ecrire : trois etapes entrelacables
		}
	}

} // namespace

int nkmain(const nkentseu::NkEntryState &state) {
	// --fils=N : le nombre de fils (8 par defaut, 1 pour la version reference).
	int nombreFils = 8;
	for (nkentseu::nk_uint32 i = 0; i < state.args.Size(); ++i) {
		const nkentseu::NkString a = state.args[i];
		if (a.StartsWith("--fils=")) {
			nombreFils = nkentseu::NkString(a.SubStr(7)).ToInt32();
		}
	}
	if (nombreFils < 1) nombreFils = 1;

	std::vector<nkentseu::threading::NkThread> fils;
	fils.reserve(nombreFils);
	for (int t = 0; t < nombreFils; ++t) {
		fils.emplace_back([](void *) { Incrementer(nullptr); });
	}
	for (nkentseu::threading::NkThread &f : fils) {
		f.Join(); // le total se lit APRES les joins, pas avant
	}

	const long long attendu = static_cast<long long>(nombreFils) * kIncrementsParFil;
	printf("%lld %lld\n", compteur, attendu);
	return 0;
}

