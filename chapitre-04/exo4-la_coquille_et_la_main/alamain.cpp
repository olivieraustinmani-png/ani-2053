#include "NKCanvas/Core/NkContextDesc.h"
#include "NKCanvas/Renderer/Core/NkRenderer2DTypes.h"
#include "NKCanvas/Renderer/Targets/NkRenderWindow.h"
#include "NKTime/NkClock.h"
#include "NKWindow/NKMain.h"
#include "NKWindow/NKWindow.h"

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Carre mobile a la main";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	(void)state;

	const char *titreFenetre = "Carre mobile a la main";
	const unsigned int largeurFenetre = 800;
	const unsigned int hauteurFenetre = 600;
	const float vitesse = 100.f;
	const float positionY = 200.f;
	const float largeurCarre = 50.f;
	const float hauteurCarre = 50.f;
	const nkentseu::renderer::NkColor2D couleurFond{18, 18, 24, 255};
	const nkentseu::renderer::NkColor2D couleurCarre = nkentseu::renderer::NkColor2D::Red;

	nkentseu::NkWindow window;
	nkentseu::NkWindowConfig configuration;
	configuration.title = titreFenetre;
	configuration.width = largeurFenetre;
	configuration.height = hauteurFenetre;

	if (!window.Create(configuration)) {
		return -1;
	}

	nkentseu::NkContextDesc descriptionContexte;
	descriptionContexte.api = nkentseu::NkGraphicsApi::NK_GFX_API_OPENGL;
	nkentseu::renderer::NkRenderWindow cibleDessin(window, descriptionContexte);

	if (!cibleDessin.IsValid()) {
		return -1;
	}

	nkentseu::NkClock horloge;
	float positionX = 0.f;
	nkentseu::renderer::NkRenderer2D &dessin = cibleDessin.GetRenderer2D();

	while (window.IsOpen()) {
		float dt = horloge.Tick().delta;

		nkentseu::NkEvent *evenement = nkentseu::NkEvents().PollEvent();
		while (evenement != nullptr) {
			evenement = nkentseu::NkEvents().PollEvent();
		}

		positionX = positionX + vitesse * dt;
		cibleDessin.Clear(couleurFond);
		dessin.DrawFilledRect({positionX, positionY, largeurCarre, hauteurCarre}, couleurCarre);
		cibleDessin.Display();
	}

	return 0;
}
