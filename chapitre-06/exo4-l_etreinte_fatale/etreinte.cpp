// =============================================================================
// etreinte.cpp - EXO 4 Sprint 6 : l'etreinte fatale
//
// Deux fils, deux mutex, pris dans l'ORDRE INVERSE : le fil a prend m1 puis
// m2, le fil b prend m2 puis m1. Avec 100 ms entre les deux prises, le
// blocage est certain : a se fige en attendant m2 (tenu par b), b se fige en
// attendant m1 (tenu par a). Aucun ne rend jamais le sien : le programme se
// fige sans erreur.
//
// LA VERSION QUI SE FIGE est gardee en commentaire ci-dessous.
// LA VERSION CORRIGEE prend les deux verrous DANS LE MEME ORDRE (m1 puis m2)
// de chaque cote - la regle d'ordre. Aucun verrou ni attente retire.
//
// ============================ VERSION QUI SE FIGE ============================
// void Fila(void *) {
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> l1(m1); // ligne X
//     nkentseu::NkChrono::SleepMilliseconds(100);
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> l2(m2); // a attend ICI
// }
// void Filb(void *) {
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> l1(m2); // ligne Y
//     nkentseu::NkChrono::SleepMilliseconds(100);
//     nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> l2(m1); // b attend ICI
// }
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. exo1)
#include "NKThreading/NkThread.h"
#include "NKThreading/NkMutex.h"
#include "NKThreading/NkScopedLock.h"
#include "NKTime/NkChrono.h"
#include <cstdio>
#include <vector>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Etreinte";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {

	nkentseu::threading::NkMutex m1;
	nkentseu::threading::NkMutex m2;

	// VERSION CORRIGEE : les deux fils prennent m1 AVANT m2 (regle d'ordre).
	// Chaque attente de 100 ms est gardee ; le second verrou tombe apres.
	void Fila(void *) {
		{
			nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> premier(m1);
			nkentseu::NkChrono::SleepMilliseconds(100);
			nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> second(m2);
			printf("fila : les deux verrous tenus\n");
		}
	}

	void Filb(void *) {
		{
			nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> premier(m1); // meme ordre que a
			nkentseu::NkChrono::SleepMilliseconds(100);
			nkentseu::threading::NkScopedLock<nkentseu::threading::NkMutex> second(m2);
			printf("filb : les deux verrous tenus\n");
		}
	}

} // namespace

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;

	std::vector<nkentseu::threading::NkThread> fils;
	fils.emplace_back([](void *) { Fila(nullptr); });
	fils.emplace_back([](void *) { Filb(nullptr); });
	for (nkentseu::threading::NkThread &f : fils) {
		f.Join(); // dans la version qui se fige, ce Join n'arrive jamais
	}
	printf("fin propre\n");
	return 0;
}

