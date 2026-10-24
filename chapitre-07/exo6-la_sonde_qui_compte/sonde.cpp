// =============================================================================
// sonde.cpp — exercice 6, chapitre 7 : la sonde qui compte
//
// Dans OnRender, un compteur augmente a chaque passage, et une ligne de journal
// ecrit le numero du passage et Time(). Quand Time() atteint 10 secondes, le
// programme ecrit le compte et la duree dans un fichier, puis appelle Quit().
//
// Les quatre etats demandes par l'enonce se choisissent sur la ligne de
// commande, sans recompiler :
//   --plafond        Config().imagesParSeconde = 60  (cadence visee par defaut)
//   --sans-plafond   Config().imagesParSeconde = 0   (aucun plafond)
//   --muet           retire la ligne de journal, garde le compteur
//   --sortie=<fichier>  fichier de compte (mesure.txt par defaut)
//
// Lancements : (plafond puis sans plafond) x (avec puis sans la ligne).
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKContainers/String/NkFormat.h"
#include "NKContainers/String/NkString.h"
#include "NKCore/NkTypes.h"
#include "NKFileSystem/NkFile.h"
#include "NKLogger/NkLog.h"
#include "NKTime/NkChrono.h"

namespace {
	const nkentseu::float32 kDureeMesuree = 10.f; // secondes, mesure demandee
}

class SondeCadence : public nkentseu::renderer::NkCanvasApp {
	public:
	SondeCadence() {
	Config().title = "Sonde de cadence";
	Config().width = 960;
	Config().height = 540;
	Config().clearColor = nkentseu::renderer::NkColor2D{18, 18, 24, 255};
	// La cadence visee est reglee ici, et changee plus bas par la ligne de
	// commande sans recompiler. Ce champ est relu par NkCanvasApp a chaque
	// trame, APRES OnRender et Display : il retarde la trame suivante.
	Config().imagesParSeconde = 60;
	}

	protected:
	// AVANT toute fenetre : les arguments decident de la mesure.
	nkentseu::NkOptional<int> OnCommandLine(const nkentseu::NkVector<nkentseu::NkString> &args) override {
	for (nkentseu::uint32 i = 0; i < args.Size(); ++i) {
		const nkentseu::NkString &a = args[i];
		if (a == "--sans-plafond") {
			Config().imagesParSeconde = 0;
	} else if (a == "--plafond") {
			Config().imagesParSeconde = 60;
	} else if (a == "--muet") {
			mJournalActif = false;
	} else if (a.StartsWith("--sortie=")) {
			mSortie = a.SubStr(9);
	}
	}
	return nkentseu::NkOptional<int>();
	}

	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
	(void)target;
	// Le compteur est DANS la boucle de rendu : il compte les trames
	// effectivement rendues.
	++mPassages;
	// La ligne de journal : posee, puis retiree par --muet. Elle ecrit le
	// numero du passage et Time(), donc ce que la boucle promet a chaque trame.
		if (mJournalActif) {
			logger.Infof("[sonde] passage %llu a t=%.4f s\n",
						 static_cast<unsigned long>(mPassages),
						 static_cast<double>(Time()));
	}
	// La duree de reference est Time(), pas une horloge de plus : c'est ce que
	// la coquille mesure depuis OnInit.
		mDerniereDuree = Time();
		if (mDerniereDuree >= kDureeMesuree) {
			// On ecrit AVANT Quit() : Quit() demande seulement l'arret a la fin
			// de l'image courante, il n'interrompt pas OnRender.
			EcrireCompte();
			Quit();
	}
	}

	private:
	void EcrireCompte() {
		const nkentseu::float32 ips = static_cast<nkentseu::float32>(mPassages) / mDerniereDuree;
		const nkentseu::NkString contenu = nkentseu::NkFormat(
			"passages={0}\nduree={1:.4f}\nips={2:.2f}\nplafond={3}\njournal={4}\n",
			static_cast<nkentseu::uint64>(mPassages), mDerniereDuree, ips, Config().imagesParSeconde,
			mJournalActif ? "oui" : "non");
		nkentseu::NkFile::WriteAllText(mSortie.CStr(), contenu.CStr());
	}

	nkentseu::NkString mSortie = "mesure.txt";
	// volatile : sans la ligne de journal (--muet), un compteur qui n'est lu
	// qu'une fois ne doit pas etre optimise hors de la boucle — le compteur
	// servirait alors a mesurer une boucle qui ne compte plus rien.
	volatile nkentseu::uint64 mPassages = 0;
	nkentseu::float32 mDerniereDuree = 0.f;
	bool mJournalActif = true;
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Sonde";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<SondeCadence>(state);
}
