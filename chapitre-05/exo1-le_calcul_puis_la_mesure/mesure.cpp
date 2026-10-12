// =============================================================================
// mesure.cpp — EXO 1 Sprint 5 : le calcul, puis la mesure
//
// Charge cinq images avec NKImage, calcule W x H x BytesPP(), mesure la
// memoire privee du processus avant/apres chaque chargement, et ecrit
// mesures.txt au format demande par la plateforme.
//
// LES IMAGES RESTENT VIVANTES pendant toute la mesure : elles vivent dans un
// tableau declare une seule fois. Une image qui sort de sa portee libere ses
// pixels — la mesure ne verrait alors rien.
// =============================================================================
#include "NKImage/NKImage.h"
#include "NKLogger/NkLog.h"
#include "NKWindow/NKMain.h"

#include <windows.h>
#include <psapi.h>
#include <fstream>
#include <string>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "MesureMemoireImages";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {
	// Memoire PRIVEE engagee par le processus (PrivateUsage). Le jeu de travail
	// (WorkingSetSize) fluctue selon la pagination ; la memoire privee engagee
	// suit beaucoup plus fidelement ce que NOUS allouons.
	unsigned long long MemoirePrivee() {
		PROCESS_MEMORY_COUNTERS_EX c{};
		c.cb = sizeof(c);
		GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS *>(&c), sizeof(c));
		return static_cast<unsigned long long>(c.PrivateUsage);
	}

	long long TailleFichier(const char *chemin) {
		HANDLE h = CreateFileA(chemin, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (h == INVALID_HANDLE_VALUE) return 0;
		const long long t = static_cast<long long>(GetFileSize(h, nullptr));
		CloseHandle(h);
		return t;
	}
} // namespace

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;

	const std::string dossier = "C:/Users/Austin/Desktop/ani-2053/chapitre-05/exo1-le_calcul_puis_la_mesure/images/";
	const char *noms[5] = {
		"icone_cloth.png",		// icone
		"capture_ecran.png",	// capture d'ecran
		"aplats_rihen.png",		// dessin a aplats (PNG)
		"photo_immobilier.jpeg",// photographie (JPEG)
		"photo_ville.jpeg",		// photographie (JPEG)
	};

	// ── LE PIEGE N1, desamorce ici ────────────────────────────────────────────
	// Les cinq images vivent dans CE tableau, hors de toute boucle : chacune
	// survit jusqu'a la fin de nkmain, donc jusqu'a la fin des mesures.
	nkentseu::NkImage images[5];

	std::ofstream rapport("C:/Users/Austin/Desktop/ani-2053/chapitre-05/exo1-le_calcul_puis_la_mesure/mesures.txt");
	if (!rapport.is_open()) {
		logger.Error("[mesure] impossible d'ouvrir mesures.txt en ecriture");
		return -1;
	}

	for (int i = 0; i < 5; ++i) {
		const std::string chemin = dossier + noms[i];

		const unsigned long long avant = MemoirePrivee();
		const bool ok = images[i].Load(chemin.c_str(), 0);
		const unsigned long long apres = MemoirePrivee();

		if (!ok) {
			logger.Errorf("[mesure] ECHEC du chargement de %s", noms[i]);
			continue;
		}

		const nkentseu::int32 l = images[i].Width();
		const nkentseu::int32 h = images[i].Height();
		const nkentseu::int32 bpp = images[i].BytesPP();
		const long long calcul = static_cast<long long>(l) * h * bpp;
		const long long mesure = static_cast<long long>(apres - avant);
		const long long fichier = TailleFichier(chemin.c_str());

		logger.Infof("[mesure] %s : %dx%d, %d oct/pixel, calcul=%lld, mesure=%lld, fichier=%lld",
					 noms[i], l, h, bpp, calcul, mesure, fichier);

		rapport << "image : " << noms[i]
				<< ", calcul : " << calcul
				<< ", mesure : " << mesure
				<< ", fichier : " << fichier << "\n";
	}

	rapport.close();
	logger.Info("[mesure] mesures.txt ecrit");
	return 0;
}

