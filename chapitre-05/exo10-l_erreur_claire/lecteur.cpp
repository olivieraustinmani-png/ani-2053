// lecteur.cpp - EXO 10 Sprint 5 : lerreur claire
// Ouvre trois fichiers avec le lecteur video de NKMedia : une video saine,
// sa copie coupee a moitie, et un texte renomme .mp4. Pour chaque fichier,
// Open, puis ProbeFile (la sonde), puis comptage des images lues.
#include "NKMedia/Video/NkVideoReader.h"
#include "NKMedia/NkMediaProbe.h"
#include "NKLogger/NkLog.h"
#include "NKWindow/NKMain.h"
#include <fstream>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "LecteurVideo";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {
	void Tester(const char *chemin, std::ofstream &out) {
		// La sonde : elle ne demande rien au lecteur, elle lit les octets.
				nkentseu::media::NkMediaInfo probe;
		std::string probeTxt = "INCONNU";
		if (nkentseu::media::NkMediaProbe::ProbeFile(chemin, probe)) {
			probeTxt = probe.ContainerName();
		}

		// Le lecteur. Open rend seulement vrai ou faux : la cause, c'est la
		// sonde qui la cherche.
		nkentseu::media::NkVideoReader lecteur;
		long long images = 0;
		bool ouvert = lecteur.Open(chemin);
		if (ouvert) {
			const auto &info = lecteur.Info();
			logger.Infof("[lecteur] %s : ouvert, conteneur=%s, codec=%s, %ux%u",
						 chemin, info.container.CStr(), info.codec.CStr(),
						 info.width, info.height);
			nkentseu::media::NkVideoFrame trame;
			while (lecteur.ReadFrame(trame)) ++images;
			logger.Infof("[lecteur] %s : %lld images lues", chemin, images);
	} else {
			logger.Infof("[lecteur] %s : Open REFUSE", chemin);
		}

		out << "fichier : " << chemin << ", ouvert : " << (ouvert ? "oui" : "non")
			<< ", images : " << images << ", sonde : " << probeTxt << "\n";
	}
}

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;
	const char *dossier = "C:/Users/Austin/Desktop/ani-2053/chapitre-05/exo10-l_erreur_claire/";
	std::ofstream out(std::string(dossier) + "resultats.txt");
	if (!out.is_open()) { logger.Error("[lecteur] resultats.txt inaccessible"); return -1; }

	Tester((std::string(dossier) + "film.mp4").c_str(), out);
	Tester((std::string(dossier) + "coupe.mp4").c_str(), out);
	Tester((std::string(dossier) + "faux.txt.mp4").c_str(), out);

	out.close();
	logger.Info("[lecteur] termine");
	return 0;
}



