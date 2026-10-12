// atlas.cpp - EXO 4 Sprint 5 : l'atlas, vu
#include "NKFont/NkFont.h"
#include "NKImage/NKImage.h"
#include "NKLogger/NkLog.h"
#include "NKWindow/NKMain.h"
#include <fstream>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "AtlasVu";
	d.appVersion = "1.0.0";
	return d;
})());

namespace {
	void Construire(float taille, const char *police, const char *pngSortie, bool ecrirePng, const char *fichierLignes) {
		nkentseu::NkFontAtlas atlas;
		nkentseu::NkFont *f = atlas.AddFontFromFile(police, taille);
		if (f == nullptr) { logger.Error("[atlas] police introuvable"); return; }
		if (!atlas.Build()) { logger.Error("[atlas] echec de Build"); return; }

		nkft_uint8 *pixels = nullptr;
		nkft_int32 largeur = 0, hauteur = 0;
		atlas.GetTexDataAsRGBA32(&pixels, &largeur, &hauteur);
				// Le canal de couverture vit dans R seulement (alpha = 255 partout).
		// Pour VOIR les glyphes : on copie R dans G, B et A, et on rend le fond
		// opaque. Une image tout-alpha serait invisible.
		for (nkft_int32 i = 0; i < largeur * hauteur; ++i) {
			const nkft_uint8 couverture = pixels[i * 4 + 3];
			pixels[i * 4 + 0] = couverture;
			pixels[i * 4 + 1] = couverture;
			pixels[i * 4 + 2] = couverture;
			pixels[i * 4 + 3] = 255;
		}
	nkentseu::NkImage vue = nkentseu::NkImage::Wrap(pixels, largeur, hauteur, nkentseu::NkImagePixelFormat::NK_RGBA32, largeur * 4);
		if (ecrirePng) vue.SavePNG(pngSortie);

		long long aireGlyphes = 0, compteur = 0;
		for (nkft_uint32 cp = 32; cp < 127; ++cp) {
			const nkentseu::NkFontGlyph *g = f->FindGlyph(cp);
			if (g == nullptr || !g->visible) continue;
			aireGlyphes += (long long)((g->x1 - g->x0) * (g->y1 - g->y0));
			++compteur;
		}
		long long aireTexture = (long long)largeur * hauteur;
		long long perdu = aireTexture > 0 ? 100 - (aireGlyphes * 100 / aireTexture) : 100;

		logger.Infof("[atlas] taille=%.0f glyphes=%lld texture=%dx%d perdu=%lld%%", taille, compteur, largeur, hauteur, perdu);

		std::ofstream out(fichierLignes, std::ios::app);
		if (out.is_open())
			out << "taille : " << (int)taille << ", glyphes : " << compteur
				<< ", texture : " << largeur << " " << hauteur << ", perdu : " << perdu << "\n";
	}
}

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;
	const char *police = "C:/Users/Austin/Desktop/Nkentseu/Resources/Fonts/DejaVuSansMono/DejaVuSansMono.ttf";
	const char *png = "C:/Users/Austin/Desktop/ani-2053/chapitre-05/exo4-l_atlas_vu/atlas.png";
	const char *compte = "C:/Users/Austin/Desktop/ani-2053/chapitre-05/exo4-l_atlas_vu/compte.txt";
	std::ofstream e(compte, std::ios::trunc); e.close();
	Construire(18.f, police, png, true, compte);
	Construire(48.f, police, png, false, compte);
	return 0;
}






