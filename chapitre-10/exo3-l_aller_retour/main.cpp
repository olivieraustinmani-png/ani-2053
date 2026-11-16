// =============================================================================
// main.cpp — exercice 3, chapitre 10 : l'aller-retour
//
// La caméra d'un canevas infini : conversions pixels <-> monde, déplacement,
// zoom au curseur et grille adaptative. La caméra ne stocke QUE le centre et
// le zoom : la taille de la fenêtre est lue une fois, puis passée aux calculs.
//
// Entrée  : W H, puis des commandes (VERS_MONDE, VERS_PIXEL, DEPLACE, ZOOM,
//           GRILLE, ORIGINE), une par ligne.
// Sortie  : une ligne par commande qui en rend une, puis CENTRE et ZOOM.
//
// Compilation : clang++ -std=c++17 -Wall main.cpp -o programme
// =============================================================================
#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

namespace {
	// La caméra : deux valeurs, rien d'autre. Le viewport n'est pas un membre.
	struct Camera {
		double cx = 0.0;
		double cy = 0.0;
		double zoom = 1.0; // pixels par unité monde
	};

	// Une valeur négative minuscule (-0.001) s'écrit 0.00, jamais -0.00.
	double SansZeroNegatif(double v) {
		return (std::fabs(v) < 0.005) ? 0.0 : v;
	}

	void Ecrire2(double v) {
		std::printf("%.2f", SansZeroNegatif(v));
	}

	double VersMondeX(const Camera &c, double px, double w) {
		return c.cx + (px - w * 0.5) / c.zoom;
	}

	double VersMondeY(const Camera &c, double py, double h) {
		return c.cy + (py - h * 0.5) / c.zoom;
	}

	double VersPixelX(const Camera &c, double mx, double w) {
		return (mx - c.cx) * c.zoom + w * 0.5;
	}

	double VersPixelY(const Camera &c, double my, double h) {
		return (my - c.cy) * c.zoom + h * 0.5;
	}

	// Le point sous le curseur se note AVANT de changer le zoom ; puis on
	// replace le centre pour qu'il retombe sur le même pixel. Le zoom est
	// borné, et un zoom arrêté à une borne garde quand même le point fixe.
	void Zoomer(Camera &c, double f, double px, double py, double w, double h) {
		const double ax = VersMondeX(c, px, w);
		const double ay = VersMondeY(c, py, h);
		c.zoom = c.zoom * f;
		if (c.zoom < 0.02) {
			c.zoom = 0.02;
		}
		if (c.zoom > 64.0) {
			c.zoom = 64.0;
		}
		c.cx = ax - (px - w * 0.5) / c.zoom;
		c.cy = ay - (py - h * 0.5) / c.zoom;
	}

	// Le pas de grille : la plus grande puissance de deux dont l'écart à
	// l'écran reste au moins m pixels.
	double PasGrille(const Camera &c, double m) {
		double s = 1.0;
		while (s * c.zoom < m) {
			s *= 2.0;
		}
		while (s * 0.5 * c.zoom >= m) {
			s *= 0.5;
		}
		return s;
	}
} // namespace

int main() {
	double w = 0.0;
	double h = 0.0;
	if (!(std::cin >> w >> h)) {
		return 0;
	}

	Camera cam;
	std::string commande;
	while (std::cin >> commande) {
		if (commande == "VERS_MONDE") {
			double px = 0.0;
			double py = 0.0;
			std::cin >> px >> py;
			std::printf("MONDE ");
			Ecrire2(VersMondeX(cam, px, w));
			std::printf(" ");
			Ecrire2(VersMondeY(cam, py, h));
			std::printf("\n");
		} else if (commande == "VERS_PIXEL") {
			double mx = 0.0;
			double my = 0.0;
			std::cin >> mx >> my;
			std::printf("PIXEL ");
			Ecrire2(VersPixelX(cam, mx, w));
			std::printf(" ");
			Ecrire2(VersPixelY(cam, my, h));
			std::printf("\n");
		} else if (commande == "DEPLACE") {
			double dx = 0.0;
			double dy = 0.0;
			std::cin >> dx >> dy;
			cam.cx = cam.cx - dx / cam.zoom;
			cam.cy = cam.cy - dy / cam.zoom;
		} else if (commande == "ZOOM") {
			double f = 0.0;
			double px = 0.0;
			double py = 0.0;
			std::cin >> f >> px >> py;
			Zoomer(cam, f, px, py, w, h);
		} else if (commande == "GRILLE") {
			double m = 0.0;
			std::cin >> m;
			std::printf("GRILLE %.6f\n", PasGrille(cam, m));
		} else if (commande == "ORIGINE") {
			cam = Camera{};
		}
	}

	std::printf("CENTRE ");
	Ecrire2(cam.cx);
	std::printf(" ");
	Ecrire2(cam.cy);
	std::printf("\n");
	std::printf("ZOOM ");
	Ecrire2(cam.zoom);
	std::printf("\n");
	return 0;
}
