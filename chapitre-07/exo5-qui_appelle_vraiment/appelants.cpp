// =============================================================================
// appelants.cpp — exercice 5, chapitre 7 : qui appelle vraiment Draw ?
//
// Une NkCanvasApp qui definit sa propre classe derivee de NkDrawable, redefinit
// Draw pour envoyer QUATRE sommets en NK_LINES a la cible, et dessine un objet
// de cette classe par target.Draw(objet) dans OnRender.
//
// Ce programme sert a deux choses :
//   1. etre cherche dans le texte (grep) pour la liste des « appelants » ;
//   2. servir de cible a un point d'arret dans MaLigne::Draw pour relever la
//      pile d'appels reelle (appelants.txt).
//
// Construit avec le kit Canvas du 22 octobre 2026 (installe ici : canvas du
// 13 octobre dans Build/Kit/NkentseuKit ; les en-tetes de NKCanvas n'ont pas
// change entre les deux).
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkDrawable.h"
#include "NKCanvas/Renderer/Core/NkRenderStates.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DTypes.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKCanvas/Renderer/Targets/NkRenderTarget.h"
#include "NKLogger/NkLog.h"

namespace {
	// Un compteur global : il rend le point d'arret non optimisable et permet de
	// verifier, au debogage, que Draw a bien ete appele.
	nkentseu::uint32 gAppelsRecus = 0;
} // namespace

// -----------------------------------------------------------------------------
// MaLigne — herite de NkDrawable et redefinit la virtuelle Draw.
//
// `const` est obligatoire : la signature de NkDrawable.h est
//     virtual void Draw(NkRenderTarget &target, const NkRenderStates &states) const = 0;
// et un override ne peut pas retirer le const.
// -----------------------------------------------------------------------------
class MaLigne : public nkentseu::renderer::NkDrawable {
	public:
	MaLigne(const nkentseu::renderer::NkVec2f &debut, const nkentseu::renderer::NkVec2f &fin)
	: mDebut(debut), mFin(fin) {
	}

	// La redefinition. Elle ne dessine pas elle-meme : elle demande a la cible
	// d'envoyer QUATRE sommets (deux segments), en NK_LINES, avec l'etat recu.
	void Draw(nkentseu::renderer::NkRenderTarget &target,
			  const nkentseu::renderer::NkRenderStates &states) const override {
	++gAppelsRecus;
		const nkentseu::uint8 r = 235;
		const nkentseu::uint8 v = 200;
		const nkentseu::uint8 b = 90;
		const nkentseu::uint8 a = 255;
		const nkentseu::renderer::NkVertex sommets[4] = {
			{mDebut.x, mDebut.y, 0.f, 0.f, r, v, b, a},
			{mFin.x, mFin.y, 0.f, 0.f, r, v, b, a},
			{mDebut.x, mDebut.y, 0.f, 0.f, r, v, b, a},
			{mFin.x, mDebut.y, 0.f, 0.f, r, v, b, a},
	};
	// 4 sommets, NK_LINES : deux segments independants (paire par paire).
		target.Draw(sommets, 4, nkentseu::renderer::NkPrimitiveType::NK_LINES, states);
	}

	private:
	nkentseu::renderer::NkVec2f mDebut;
	nkentseu::renderer::NkVec2f mFin;
};

// -----------------------------------------------------------------------------
// TemoinPile — le point d'arret de l'exercice 5.
//
// Meme heritage, meme redefinition : il ne dessine rien, il ecrit seulement son
// nom au journal (logger.Infof, faute d'accolades : c'est un format a la printf).
// On cherche TemoinPile::Draw dans le texte, on pose un point d'arret dedans, et
// on lit la pile reelle — puis on la compare a ce que le texte avait trouve.
// -----------------------------------------------------------------------------
class TemoinPile : public nkentseu::renderer::NkDrawable {
	public:
	void Draw(nkentseu::renderer::NkRenderTarget &target,
	  const nkentseu::renderer::NkRenderStates &states) const override {
	(void)target;
	(void)states;
	++gAppelsRecus;
	logger.Infof("[temoin] Draw recu, appel n%s\n", "un");
	}
};

// -----------------------------------------------------------------------------
// FenetreAppelants — la coquille.
// -----------------------------------------------------------------------------
class FenetreAppelants : public nkentseu::renderer::NkCanvasApp {
	public:
	FenetreAppelants() : mCroix(nkentseu::renderer::NkVec2f{160.f, 120.f},
							   nkentseu::renderer::NkVec2f{480.f, 380.f}) {
	Config().title = "Qui appelle vraiment Draw";
	Config().width = 640;
	Config().height = 480;
	Config().clearColor = nkentseu::renderer::NkColor2D{18, 18, 24, 255};
	}

	protected:
	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
	// L'appel etudie : la cible recoit un NkDrawable par reference. C'est
	// NkRenderTarget::Draw(const NkDrawable&, ...) qui appelle
	// drawable.Draw(*this, states), donc MaLigne::Draw ci-dessus.
		target.Draw(mCroix);
	mTemoin.Draw(target, nkentseu::renderer::NkRenderStates::Default());
	}

	private:
	MaLigne mCroix;
	TemoinPile mTemoin;
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Appelants";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<FenetreAppelants>(state);
}
