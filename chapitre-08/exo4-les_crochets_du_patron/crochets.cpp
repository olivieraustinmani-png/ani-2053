// =============================================================================
// crochets.cpp — exercice 4, chapitre 8 : les crochets du patron
//
// NkCanvasApp tient la boucle (OnUpdate, Clear, OnRender, Display) et appelle
// des crochets virtuels quand elle le decide. Ce programme redefinit CHACUN de
// ces crochets, et chacun ecrit une ligne dans un journal fichier :
//     numero d'ordre | nom du crochet | image en cours | Time()
// L'application a fenetre n'a pas de console : le journal va dans crochets.log.
// Elle s'arrete seule, par Quit(), apres quelques images.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKContainers/String/NkFormat.h"
#include "NKContainers/String/NkString.h"
#include "NKCore/NkTypes.h"
#include "NKFileSystem/NkFile.h"
#include "NKTime/NkChrono.h"

class MonJeu : public nkentseu::renderer::NkCanvasApp {
	public:
	MonJeu() {
	Config().title = "Les crochets du patron";
	Config().width = 640;
	Config().height = 480;
	Config().clearColor = nkentseu::renderer::NkColor2D{16, 16, 22, 255};
	Config().imagesParSeconde = 60;
	}

	protected:
	// 1. Appele AVANT toute creation de fenetre. Rendre une valeur = arret.
	nkentseu::NkOptional<int> OnCommandLine(const nkentseu::NkVector<nkentseu::NkString> &args) override {
	(void)args;
	Noter("OnCommandLine");
	return nkentseu::NkOptional<int>(); // aucune fenetre n'est creee : on ne chiffre pas ici
	}

	// 2. Fenetre et cible existent. Render false = abandon.
	bool OnInit() override {
	Noter("OnInit");
	return true;
	}

	// 3. Une fois par trame, avant Clear et OnRender.
	void OnUpdate(nkentseu::float32 deltaTime) override {
	(void)deltaTime;
	// On ne note OnUpdate que pour les premieres images, sinon le journal
	// noie tout le reste (piege de l'enonce).
	if (mImagesNotees < 3) {
		Noter("OnUpdate");
	}
	}

	// 4. Une fois par trame, apres Clear.
	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
	(void)target;
	// Idem : seulement les premieres images.
	if (mImagesNotees < 3) {
		Noter("OnRender");
	}
	++mImagesNotees;
	// L'image en cours et Time() sont fournis par la coquille : on les lit,
	// on ne les recalcule pas. L'arret est decide par la boucle, pas par nous.
	if (Time() >= 0.25f) {
		Quit();
	}
	}

	// 5. Flot brut des evenements : true = consomme, la chaine s'arrete ici.
	bool OnEvent(const nkentseu::NkEvent &event) override {
	(void)event;
	Noter("OnEvent");
	return false;
	}

	// 6. Geste unifie souris + doigt (derive de OnEvent).
	bool OnPointer(const nkentseu::NkPointer &pointer) override {
	(void)pointer;
	Noter("OnPointer");
	return false;
	}

	// 7. Touche pressee.
	bool OnKeyPress(const nkentseu::NkKeyPressEvent &event) override {
	(void)event;
	Noter("OnKeyPress");
	return false;
	}

	// 8. Taille, zone sure, densite ont change : au moins une fois avant la
	//    premiere trame.
	void OnLayout(const nkentseu::renderer::NkLayoutInfo &layout) override {
	(void)layout;
	Noter("OnLayout");
	}

	// 9. Demande de fermeture : rendre true accepte, false refuse.
	bool OnCloseRequested() override {
	Noter("OnCloseRequested");
	return true;
	}

	// 10. Passage en arriere-plan / retour au premier plan.
	void OnPause() override {
	Noter("OnPause");
	}

	void OnResume() override {
	Noter("OnResume");
	}

	// 11. Dernier crochet : la fenetre n'existe plus apres.
	void OnShutdown() override {
	Noter("OnShutdown");
	}

	private:
	// Un numero d'ordre, le nom, l'image en cours et Time(). On ecrit une ligne
	// par appel, en ajoutant au fichier : la trame de l'ordre est donc reelle.
	void Noter(const char *nom) {
	++mOrdre;
	const nkentseu::NkString ligne = nkentseu::NkFormat(
		"%u | %s | image %u | t=%.4f\n", static_cast<nkentseu::uint32>(mOrdre), nom,
		static_cast<nkentseu::uint32>(mImagesNotees), static_cast<double>(Time()));
	nkentseu::NkFile::AppendAllText("crochets.log", ligne.CStr());
	}

	unsigned long mOrdre = 0;      // numero d'ordre du journal
	unsigned long mImagesNotees = 0; // images deja passees par OnRender
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Crochets";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<MonJeu>(state);
}
