// =============================================================================
// composite.cpp — exercice 8, chapitre 8 : le composite a trois niveaux
//
// Hierarchie, du sommet vers la feuille :
//     mParent : NkTransformable  position (100, 50)
//     mRacine : Noeud            position (100, 50)   <- le niveau 1, qui dessine
//        └── mMilieu : Noeud     position (200,  0), rotation 90 deg
//              └── mSprite : NkSprite  position (40, 0)
//
// Calcul de l'origine du sprite, fait AVANT de lancer :
//     (40,0) tourne de 90 deg -> (0,40) ; + (200,0) -> (200,40) ; + (100,50) -> (300,90)
// Attendu : (300, 90).
//
// Deux sondes dessinees a la place du sprite, sous la meme chaine :
//   - SondeComposite : compose l'etat recu        -> (300, 90)
//   - SondeQuiJette  : JETTE l'etat recu          -> (200,  0), le defaut imite
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER
#include "NKCanvas/App/NkCanvasApp.h"
#include "NKCanvas/Renderer/Core/NkDrawable.h"
#include "NKCanvas/Renderer/Core/NkRenderStates.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DTypes.h"
#include "NKCanvas/Renderer/Core/NkTransform.h"
#include "NKCanvas/Renderer/Core/NkTransformable.h"
#include "NKCanvas/Renderer/Resources/NkSprite.h"
#include "NKCanvas/Renderer/Resources/NkTexture.h"
#include "NKCanvas/Renderer/Targets/NkRenderTarget.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKContainers/String/NkFormat.h"
#include "NKContainers/String/NkString.h"
#include "NKCore/NkTypes.h"
#include "NKFileSystem/NkFile.h"

namespace {
	// Un drawable qui ne dessine rien : il note ou son origine arrive une fois
	// l'etat applique (TransformPoint, NkTransform.h).
	class Sonde : public nkentseu::renderer::NkDrawable {
		public:
		Sonde(const nkentseu::renderer::NkTransformable &proprietaire, const char *nom, bool jetterEtat)
		: mProprietaire(proprietaire), mNom(nom), mJette(jetterEtat) {
		}

		void Draw(nkentseu::renderer::NkRenderTarget &target,
				  const nkentseu::renderer::NkRenderStates &states) const override {
		(void)target;
			// Le composite correct copie l'etat recu ; le fautif repart d'un etat
			// neuf et perd donc le parent.
			nkentseu::renderer::NkRenderStates local =
			mJette ? nkentseu::renderer::NkRenderStates() : states;
			local.transform *= mProprietaire.GetTransform();
			const nkentseu::renderer::NkVec2f p =
			local.transform.TransformPoint(nkentseu::renderer::NkVec2f{0.f, 0.f});
			const nkentseu::NkString ligne = nkentseu::NkFormat("%s | origine en (%f, %f)\n", mNom,
																static_cast<double>(p.x), static_cast<double>(p.y));
			nkentseu::NkFile::AppendAllText("sondes.txt", ligne.CStr());
		}

		private:
		const nkentseu::renderer::NkTransformable &mProprietaire;
		const char *mNom;
		bool mJette;
	};

	// Un noeud : objet transforme qui contient un enfant et le dessine avec
	// l'etat compose. L'enfant est un pointeur, jamais une reference.
	class Noeud : public nkentseu::renderer::NkTransformable, public nkentseu::renderer::NkDrawable {
		public:
		Noeud() : mEnfant(nullptr) {
		}

		void SetEnfant(nkentseu::renderer::NkDrawable *enfant) {
			mEnfant = enfant;
		}

		void Draw(nkentseu::renderer::NkRenderTarget &target,
				  const nkentseu::renderer::NkRenderStates &states) const override {
		if (mEnfant == nullptr) {
			return;
		}
			nkentseu::renderer::NkRenderStates local = states;
			local.transform *= GetTransform(); // chaque niveau compose
			mEnfant->Draw(target, local);
		}

		private:
		nkentseu::renderer::NkDrawable *mEnfant;
	};
} // namespace

class HierarchieApp : public nkentseu::renderer::NkCanvasApp {
	public:
	HierarchieApp() {
	Config().title = "Composite a trois niveaux";
	Config().width = 800;
	Config().height = 600;
	Config().clearColor = nkentseu::renderer::NkColor2D{18, 18, 24, 255};
	}

	protected:
	bool OnInit() override {
	// Niveau 1 : le parent, 100 a droite et 50 en bas.
		mRacine.SetPosition(nkentseu::renderer::NkVec2f{100.f, 50.f});
		// Niveau 2 : le milieu, decale de 200, et tourne de 90 deg.
		mMilieu.SetPosition(nkentseu::renderer::NkVec2f{200.f, 0.f});
		mMilieu.SetRotation(90.f);
		// Niveau 3 : le sprite, 40 a droite de son parent.
		mSprite.SetPosition(nkentseu::renderer::NkVec2f{40.f, 0.f});

	// Une texture unie est obligatoire pour un sprite.
		mTexture.Create(Target().GetRenderer(), 32, 32, nkentseu::renderer::NkColor2D{255, 200, 0, 255});
		mSprite.SetTexture(mTexture);

	// La chaine : racine -> milieu -> sprite.
		mMilieu.SetEnfant(&mSprite);
		mRacine.SetEnfant(&mMilieu);
		nkentseu::NkFile::WriteAllText("sondes.txt", "sondes du composite\n");
		return true;
	}

	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
	// Un repere pour lire l'image : horizontale a y = 90, verticale a x = 300.
		const nkentseu::renderer::NkVertex repere[4] = {
			{0.f, 90.f, 0.f, 0.f, 90, 90, 110, 255},
			{800.f, 90.f, 0.f, 0.f, 90, 90, 110, 255},
			{300.f, 0.f, 0.f, 0.f, 90, 90, 110, 255},
			{300.f, 600.f, 0.f, 0.f, 90, 90, 110, 255},
	};
		target.Draw(repere, 4, nkentseu::renderer::NkPrimitiveType::NK_LINES);

	// Les trois niveaux : la racine dessine tout le reste.
		mRacine.Draw(target, nkentseu::renderer::NkRenderStates::Default());
		// Les deux sondes, placees comme le sprite, dessinees une seule fois.
		if (mImages == 0) {
			mSondeBonne.Draw(target, nkentseu::renderer::NkRenderStates::Default());
			mSondeFautive.Draw(target, nkentseu::renderer::NkRenderStates::Default());
	}
		++mImages;
		// Arret automatique : quelques images suffisent, la mesure est faite.
		if (Time() > 0.5f) {
			Quit();
	}
	}

	private:
	Noeud mRacine;                     // niveau 1
	Noeud mMilieu;                     // niveau 2
	nkentseu::renderer::NkSprite mSprite; // niveau 3
	nkentseu::renderer::NkTexture mTexture;
	// Les sondes portent le transform du sprite : elles mesurent le meme point.
	Sonde mSondeBonne{mSprite, "SondeComposite", false};
	Sonde mSondeFautive{mSprite, "SondeQuiJette", true};
	unsigned long mImages = 0;
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Composite";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<HierarchieApp>(state);
}
