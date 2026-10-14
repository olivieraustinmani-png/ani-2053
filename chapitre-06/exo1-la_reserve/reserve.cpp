// =============================================================================
// reserve.cpp - EXO 1 Sprint 6 : la reserve
//
// Deux cents taches courtes : soumises a une NkThreadPool d'abord, confiees
// a deux cents NkThread crees a la main ensuite. Trois mesures par version,
// puis la meme experience avec une tache cent fois plus longue.
//
// PIEGE : le compilateur Release peut retirer un calcul dont le resultat ne
// sert a rien. La somme de chaque tache est ajoutee a un atomique, affiche
// a la fin : le travail ne peut plus etre retire.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER : NKMath.h compile avant que le namespace time de NKTime ne soit declare, sinon NkRandom.h appelle time(nullptr) et la fonction C devient ambigu.
#include "NKThreading/NkThreadPool.h"
#include "NKThreading/NkThread.h"
#include "NKThreading/NkScopedLock.h"
#include "NKTime/NkChrono.h"
#include "NKCore/NkAtomic.h"
#include <cstdio>
#include <vector>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Reserve";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {

	const int kNombreTaches = 200;

	// La tache : additionner mille entiers dans une variable locale (x repeter),
	// puis verser le total dans l'atomique partage.
	void Tache(nkentseu::NkAtomicInt &somme, int repeter) {
		long long local = 0;
		for (int r = 0; r < repeter; ++r) {
			for (int i = 1; i <= 1000; ++i) {
				local += i;
			}
		}
		somme.FetchAdd(static_cast<int>(local));
	}

	nkentseu::float64 Medianne(nkentseu::float64 a, nkentseu::float64 b, nkentseu::float64 c) {
		nkentseu::float64 lo = a < b ? a : b;
		nkentseu::float64 hi = a < b ? b : a;
		return c < lo ? lo : (c > hi ? hi : c);
	}

} // namespace

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;

	nkentseu::NkAtomicInt somme;

	// ── Version RESERVE : taches courtes ─────────────────────────────────────
	nkentseu::float64 reserveCourte[3] = {0, 0, 0};
	for (int m = 0; m < 3; ++m) {
		somme.Store(0);
		nkentseu::threading::NkThreadPool pool(0); // 0 = nombre de coeurs logiques
		nkentseu::NkChrono chrono;
		chrono.Reset();
		for (int t = 0; t < kNombreTaches; ++t) {
			pool.Enqueue([&somme] { Tache(somme, 1); });
		}
		pool.Join();
		reserveCourte[m] = chrono.Elapsed().milliseconds;
	}
	const nkentseu::nk_uint32 ouvriers = [&] {
		nkentseu::threading::NkThreadPool p(0);
		return p.GetNumWorkers();
	}();
	printf("[reserve] ouvriers = %u\n", ouvriers);

	// ── Version FILS : taches courtes ────────────────────────────────────────
	nkentseu::float64 filsCourte[3] = {0, 0, 0};
	for (int m = 0; m < 3; ++m) {
		somme.Store(0);
		nkentseu::NkChrono chrono;
		chrono.Reset();
		std::vector<nkentseu::threading::NkThread> fils;
		fils.reserve(kNombreTaches);
		for (int t = 0; t < kNombreTaches; ++t) {
			fils.emplace_back([&somme](void *) { Tache(somme, 1); });
		}
		for (nkentseu::threading::NkThread &f : fils) {
			f.Join(); // TOUS joins : un fil non join peut survivre a son createur
		}
		filsCourte[m] = chrono.Elapsed().milliseconds;
	}

	// ── La meme chose avec la tache CENT FOIS plus longue ────────────────────
	nkentseu::float64 reserveLongue[3] = {0, 0, 0};
	nkentseu::float64 filsLongue[3] = {0, 0, 0};
	for (int m = 0; m < 3; ++m) {
		somme.Store(0);
		nkentseu::threading::NkThreadPool pool(0);
		nkentseu::NkChrono chrono;
		chrono.Reset();
		for (int t = 0; t < kNombreTaches; ++t) {
			pool.Enqueue([&somme] { Tache(somme, 100); });
		}
		pool.Join();
		reserveLongue[m] = chrono.Elapsed().milliseconds;
	}
	for (int m = 0; m < 3; ++m) {
		somme.Store(0);
		nkentseu::NkChrono chrono;
		chrono.Reset();
		std::vector<nkentseu::threading::NkThread> fils;
		fils.reserve(kNombreTaches);
		for (int t = 0; t < kNombreTaches; ++t) {
			fils.emplace_back([&somme](void *) { Tache(somme, 100); });
		}
		for (nkentseu::threading::NkThread &f : fils) {
			f.Join();
		}
		filsLongue[m] = chrono.Elapsed().milliseconds;
	}

	printf("[reserve] COURTE reserve : %.2f %.2f %.2f | fils : %.2f %.2f %.2f", reserveCourte[0],
				reserveCourte[1], reserveCourte[2], filsCourte[0], filsCourte[1], filsCourte[2]);
	printf("[reserve] LONGUE reserve : %.2f %.2f %.2f | fils : %.2f %.2f %.2f", reserveLongue[0],
				reserveLongue[1], reserveLongue[2], filsLongue[0], filsLongue[1], filsLongue[2]);
	printf("[reserve] somme = %d\n", somme.Load());

	// ── reserve.txt : les six etiquettes, mediane des trois mesures ──────────
	FILE *f = fopen("C:/Users/Austin/Desktop/ani-2053/chapitre-06/exo1-la_reserve/reserve.txt", "w");
	if (f == nullptr) {
		printf("[reserve] ecriture de reserve.txt impossible");
		return -1;
	}
	fprintf(f, "Tache :\n");
	fprintf(f, " addition des mille premiers entiers dans une variable locale,\n");
	fprintf(f, " faite deux cents fois (tache courte), puis cent fois par tache (tache longue).\n");
	fprintf(f, "Reserve : %.2f %.2f %.2f\n", reserveCourte[0], reserveCourte[1], reserveCourte[2]);
	fprintf(f, "Fils : %.2f %.2f %.2f\n", filsCourte[0], filsCourte[1], filsCourte[2]);
	fprintf(f, "Ouvriers : %u\n", ouvriers);
	fprintf(f, "Tache longue : %.2f (reserve) / %.2f (fils)\n", Medianne(reserveLongue[0], reserveLongue[1], reserveLongue[2]), Medianne(filsLongue[0], filsLongue[1], filsLongue[2]));
	fprintf(f, "Pourquoi :\n");
	fprintf(f, " Creer un fil coute cher : le systeme alloue sa pile (souvent 1 Mo), l'inscrit\n");
	fprintf(f, " dans le noyau, puis le planifie. Pour une tache courte, ce cout DOMINE le travail\n");
	fprintf(f, " utile : la reserve, dont les ouvriers sont deja la, gagne d'un large facteur.\n");
	fprintf(f, " Avec la tache cent fois plus longue, le travail utile depasse largement le cout\n");
	fprintf(f, " de creation : l'ecart se referme, et les deux versions se rapprochent - le gain\n");
	fprintf(f, " de la reserve devient marginal. Mesures en Release, mediane de trois essais :\n");
	fprintf(f, " la premiere mesure paie des chargements, elle ne fait pas foi seule.\n");
	fclose(f);
	printf("[reserve] reserve.txt ecrit");
	return 0;
}










