// =============================================================================
// corrige.cpp - EXO 3 Sprint 6 : la course, corrigee
//
// Le compteur partage de l'exo2, corrige de TROIS facons, mesurees contre la
// version sans protection : mutex (verrou de portee), atomique (FetchAdd),
// locale (un total par fil, additionne apres les joins).
// --version=N choisit la version : 0=sans, 1=mutex, 2=atomique, 3=locale.
// La mesure en Release (trois essais) se fait hors du programme, par le banc.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. exo1)
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"
#include "NKCore/NkAtomic.h"
#include <cstdio>
#include <cstdlib>
#include <vector>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Corrige";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {

	const long long kIncrementsParFil = 1000000;
	const int kFils = 8;

	// Les trois protections, cote a cote :
	long long compteur = 0;              // version 0 (course) et version 1 (mutex)
	nkentseu::threading::NkMutex mutexCompteur;     // version 1
	nkentseu::NkAtomicInt compteurAtomique; // version 2

	// Version 0 : la course.
	void SansProtection(void *) {
		for (long long i = 0; i < kIncrementsParFil; ++i) {
			compteur += 1;
		}
	}

	// Version 1 : le verrou de portee protege l'INCREMENT, pas la boucle.
	void AvecMutex(void *) {
		for (long long i = 0; i < kIncrementsParFil; ++i) {
			nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> verrou(mutexCompteur);
			compteur += 1;
		}
	}

	// Version 2 : l'atomique. FetchAdd rend l'ancienne valeur ; on l'ignore.
	void AvecAtomique(void *) {
		for (long long i = 0; i < kIncrementsParFil; ++i) {
			compteurAtomique.FetchAdd(1);
		}
	}

} // namespace

int nkmain(const nkentseu::NkEntryState &state) {
	int version = 0;
	for (nkentseu::nk_uint32 i = 0; i < state.args.Size(); ++i) {
		const nkentseu::NkString a = state.args[i];
		if (a.StartsWith("--version=")) {
			version = nkentseu::NkString(a.SubStr(10)).ToInt32();
		}
	}

	// Version 3 (locale) : chaque fil compte dans SA variable. Le tableau
	// doit vivre plus longtemps que les fils : declare ici, remplit par index.
	std::vector<long long> locaux(kFils, 0);

	using Tache = void (*)(void *);
	Tache tache = nullptr;
	if (version == 0) tache = SansProtection;
	else if (version == 1) tache = AvecMutex;
	else if (version == 2) tache = AvecAtomique;

	if (version == 3) {
		std::vector<nkentseu::threading::NkThread> fils;
		fils.reserve(kFils);
		for (int t = 0; t < kFils; ++t) {
			fils.emplace_back([&locaux, t](void *) {
				long long local = 0;
				for (long long i = 0; i < kIncrementsParFil; ++i) local += 1;
				locaux[t] = local; // ecriture unique, indexes distincts : pas de course
			});
		}
		for (nkentseu::threading::NkThread &f : fils) f.Join();
		long long total = 0;
		for (long long l : locaux) total += l; // APRES les joins
		printf("%lld\n", total);
		return 0;
	}

	compteur = 0;
	compteurAtomique.Store(0);
	std::vector<nkentseu::threading::NkThread> fils;
	fils.reserve(kFils);
	for (int t = 0; t < kFils; ++t) {
		fils.emplace_back([tache](void *) { tache(nullptr); });
	}
	for (nkentseu::threading::NkThread &f : fils) f.Join();

	const long long total = (version == 2) ? compteurAtomique.Load() : compteur;
	printf("%lld\n", total);
	return 0;
}

