#include "NKCanvas/App/NkCanvasApp.h"
#include "NKWindow/NKMain.h"

class CarreMobile : public nkentseu::renderer::NkCanvasApp {
	public:
	CarreMobile() {
		Config().title = "Carre mobile avec NkCanvasApp";
		Config().width = 800;
		Config().height = 600;
		Config().clearColor = nkentseu::renderer::NkColor2D{18, 18, 24, 255};
	}

	protected:
	void OnUpdate(float deltaTime) override {
		mPositionX += 100.f * deltaTime;
	}

	void OnRender(nkentseu::renderer::NkRenderWindow &target) override {
		target.GetRenderer2D().DrawFilledRect({mPositionX, 200.f, 50.f, 50.f},
											  nkentseu::renderer::NkColor2D::Red);
	}

	private:
	float mPositionX = 0.f;
};

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Carre mobile avec NkCanvasApp";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	return nkentseu::renderer::NkCanvasApp::Run<CarreMobile>(state);
}
