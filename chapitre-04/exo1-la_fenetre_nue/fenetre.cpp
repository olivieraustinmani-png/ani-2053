#include "NKCanvas/App/NkCanvasApp.h"
#include "NKWindow/NKMain.h"

class FenetreNue : public nkentseu::renderer::NkCanvasApp {
	public:
		FenetreNue() {
			Config().title = "La fenetre nue";
			Config().width = 960;
			Config().height = 540;
			Config().clearColor = nkentseu::renderer::NkColor2D{18, 18, 24, 255};
		}
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "La fenetre nue";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<FenetreNue>(state);
}
