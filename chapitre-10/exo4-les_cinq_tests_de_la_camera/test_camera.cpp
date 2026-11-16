// =============================================================================
// test_camera.cpp — exercice 4, chapitre 10 : les cinq tests de la caméra
//
// Cinq tests de la VRAIE caméra de NkRef (NkRef/NkRefView.h), sans aucune
// fenêtre : la vue est pure, elle ne dépend ni d'un renderer ni d'une fenêtre.
// Tous les noms sont qualifiés (nkentseu::, nkref::) : pas de using namespace.
//
// Caméra juste :
//   clang++ -std=c++17 -Wall test_camera.cpp -o test_camera
//     -I<repo>/Applications/NkRef/src -I<repo>/Build/Kit/NkentseuKit/include
//
// Caméra cassée : -Icopie placé AVANT les autres chemins d'inclusion, la copie
// contenant copie/NkRef/NkRefView.h avec un signe changé.
// =============================================================================
#include "NkRef/NkRefView.h"

#include <cmath>
#include <cstdio>

namespace {
	const nkentseu::float32 kTolerance = 0.001f;
	// Le viewport est une valeur passée aux calculs, jamais stockée dans la vue.
	const nkentseu::math::NkVec2f kViewport{800.0f, 600.0f};

	bool Proche(nkentseu::float32 a, nkentseu::float32 b) {
		return std::fabs(a - b) <= kTolerance;
	}

	using nkentseu::math::NkVec2f;

	// 1. WorldToPixel(PixelToWorld(p)) redonne p, à 0,001 près.
	bool aller_retour() {
		nkref::NkRefView view;
		view.zoom = 2.5f;
		view.center = NkVec2f{12.0f, -7.0f};
		const NkVec2f pixels[4] = {{0.0f, 0.0f}, {100.0f, 50.0f}, {799.0f, 599.0f}, {-30.0f, 12.5f}};
		for (const NkVec2f &p : pixels) {
			const NkVec2f retour = view.WorldToPixel(view.PixelToWorld(p, kViewport), kViewport);
			if (!Proche(retour.x, p.x) || !Proche(retour.y, p.y)) {
				return false;
			}
		}
		return true;
	}

	// 2. Après ZoomAtPixel, le point du monde sous le curseur n'a pas bougé.
	bool point_fixe() {
		nkref::NkRefView view;
		view.zoom = 1.0f;
		view.center = NkVec2f{40.0f, 15.0f};
		const NkVec2f curseur{100.0f, 50.0f};
		const NkVec2f avant = view.PixelToWorld(curseur, kViewport);
		view.ZoomAtPixel(2.0f, curseur, kViewport);
		const NkVec2f apres = view.PixelToWorld(curseur, kViewport);
		return Proche(avant.x, apres.x) && Proche(avant.y, apres.y);
	}

	// 3. Le zoom reste entre kZoomMin et kZoomMax, même avec un facteur énorme,
	//    dans les deux sens.
	bool bornes() {
		nkref::NkRefView view;
		const NkVec2f curseur{400.0f, 300.0f};
		view.ZoomAtPixel(1000.0f, curseur, kViewport);
		const bool haut = view.zoom <= nkref::NkRefView::kZoomMax && view.zoom >= nkref::NkRefView::kZoomMin;
		view.ZoomAtPixel(0.00001f, curseur, kViewport);
		const bool bas = view.zoom <= nkref::NkRefView::kZoomMax && view.zoom >= nkref::NkRefView::kZoomMin;
		return haut && bas;
	}

	// 4. Quand le zoom diminue, le pas de grille ne diminue jamais.
	bool grille() {
		nkref::NkRefView view;
		const nkentseu::float32 minPixels = 32.0f;
		nkentseu::float32 precedent = 0.0f;
		for (int i = 0; i < 40; ++i) {
			view.zoom = 64.0f / static_cast<nkentseu::float32>(1 << (i / 4)); // zoom qui ne fait que décroître
			const nkentseu::float32 s = view.GridSpacing(minPixels);
			if (i > 0 && s < precedent) {
				return false;
			}
			precedent = s;
		}
		return true;
	}

	// 5. Après un zoom, un déplacement et Reset(), le centre revient à (0, 0)
	//    et le zoom à 1.
	bool origine() {
		nkref::NkRefView view;
		view.ZoomAtPixel(3.0f, NkVec2f{700.0f, 100.0f}, kViewport);
		view.PanByPixels(120.0f, -45.0f);
		view.Reset();
		return Proche(view.center.x, 0.0f) && Proche(view.center.y, 0.0f) && Proche(view.zoom, 1.0f);
	}

	void Afficher(const char *nom, bool ok) {
		std::printf("TEST %s : %s\n", nom, ok ? "OK" : "ECHEC");
	}
} // namespace

int main() {
	Afficher("aller_retour", aller_retour());
	Afficher("point_fixe", point_fixe());
	Afficher("bornes", bornes());
	Afficher("grille", grille());
	Afficher("origine", origine());
	return 0;
}
