// =============================================================================
// banc.cpp — exercice 6, chapitre 8 : le singleton et le test
//
// Une reele : un personnage saute s'il est au sol et que la barre d'espace est
// enfoncee. Elle est ecrite DEUX fois :
//   - DoitSauterDirect(bool auSol), qui interroge le singleton NkInput ;
//   - DoitSauter(bool auSol, bool toucheEspace), qui recoit l'etat en parametre.
// Un banc compare le resultat de la seconde sur les quatre cas, ecrit chaque
// resultat dans banc.txt, puis essaye la version directe AU MEME ENDROIT et
// rend le nombre d'echecs comme code de sortie.
//
// Point important : OnCommandLine est appele AVANT toute fenetre. Le singleton
// NkInput n'a alors rien recu du systeme : la version directe ne peut donc
// repondre que « non », quels que soient les cas. C'est ce que le banc mesure.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKEvent/NkEventDispatcher.h" // NkInput (inline NkInputQuery NkInput), ligne 417
#include "NKEvent/NkKey.h"
#include "NKContainers/String/NkFormat.h"
#include "NKContainers/String/NkString.h"
#include "NKCore/NkTypes.h"
#include "NKFileSystem/NkFile.h"

namespace {
	// Regle du jeu, sans singleton : tout ce qui est necessaire arrive en
	// parametre. C'est la version testable.
	bool DoitSauter(bool auSol, bool toucheEspace) {
		return auSol && toucheEspace;
	}

	// Meme regle, ecrite avec le singleton. Elle ne peut pas etre testee : on
	// ne peut pas lui fournir un autre NkInput.
	bool DoitSauterDirect(bool auSol) {
		return auSol && nkentseu::NkInput.IsKeyDown(nkentseu::NkKey::NK_SPACE);
	}
} // namespace

class Banc : public nkentseu::renderer::NkCanvasApp {
	public:
	Banc() {
	Config().title = "Banc du singleton";
	}

	protected:
	// Appele AVANT toute fenetre. Rendre une valeur : le programme s'arrete
	// avec ce code de sortie (ici, le nombre d'echecs).
	nkentseu::NkOptional<int> OnCommandLine(const nkentseu::NkVector<nkentseu::NkString> &args) override {
	(void)args;
	struct Cas {
		const char *nom;
		bool auSol;
		bool touche;
		bool attendu;
	};
	// Les quatre cas, plus les deux de la version directe.
	const Cas cas[4] = {
		{"au sol + espace", true, true, true},
		{"au sol, pas d'espace", true, false, false},
		{"en l'air + espace", false, true, false},
		{"en l'air, pas d'espace", false, false, false},
	};

	nkentseu::NkString journal = "Banc du personnage qui saute\n";
	unsigned long echecs = 0;

	// Version testable : chaque cas est fourni en parametre.
	for (unsigned long i = 0; i < 4; ++i) {
		const bool obtenu = DoitSauter(cas[i].auSol, cas[i].touche);
		const bool ok = (obtenu == cas[i].attendu);
		if (!ok) {
			++echecs;
		}
		journal += nkentseu::NkFormat("%lu | testable | %s | attendu=%s obtenu=%s | %s\n", i + 1,
									 cas[i].nom, cas[i].attendu ? "true" : "false",
									 obtenu ? "true" : "false", ok ? "OK" : "ECHEC");
	}

	// Version directe, essayee au meme endroit : le singleton n'a rien recu.
	for (unsigned long i = 0; i < 4; ++i) {
		const bool obtenu = DoitSauterDirect(cas[i].auSol);
		const bool ok = (obtenu == cas[i].attendu);
		if (!ok) {
			++echecs;
		}
		journal += nkentseu::NkFormat("%lu | direct   | %s | attendu=%s obtenu=%s | %s\n", i + 1,
									 cas[i].nom, cas[i].attendu ? "true" : "false",
									 obtenu ? "true" : "false", ok ? "OK" : "ECHEC (singleton muet)");
	}

	nkentseu::NkFile::WriteAllText("banc.txt", journal.CStr());
	// Code de sortie = nombre d'echecs (0 si tout passe).
	return nkentseu::NkOptional<int>(static_cast<int>(echecs));
	}

	// Pas de fenetre : OnCommandLine a rendu une valeur, donc OnInit n'est jamais
	// appele. Ces crochets restent declares pour que l'on voie bien que la
	// version testable n'a pas besoin d'eux.
	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
	(void)target;
	}
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Banc";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<Banc>(state);
}