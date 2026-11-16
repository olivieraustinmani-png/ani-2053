// =============================================================================
// test_format.cpp — exercice 8, chapitre 10 : l'aller-retour du format .nkref
//
// Écrire une planche, relire les octets, reconstruire une planche avec ce que
// l'on a relu, la réécrire, et comparer les deux suites d'octets, octet par
// octet. Le test ne cherche pas QUOI a changé : il constate QUE quelque chose
// ne s'est pas conservé.
//
// Caméra juste (format juste) :
//   clang++ -std=c++17 -Wall test_format.cpp -o test_format
//     -I<repo>/Applications/NkRef/src -I<repo>/Build/Kit/NkentseuKit/include
// Format cassé : -Icopie placé AVANT les autres chemins d'inclusion.
// =============================================================================
#include "NkRef/NkRefFile.h"

#include <cstdio>

namespace {
	using nkentseu::uint32;
	using nkentseu::uint8;
	using nkentseu::usize;
	using nkentseu::NkString;
	using nkentseu::NkVector;
	using nkentseu::math::NkVec2f;
} // namespace

int main() {
	// 1. Une planche de deux images et d'un trait, avec une vue déplacée.
	nkref::NkRefBoard board;
	nkref::NkRefView view;
	view.center = NkVec2f{120.0f, -45.0f};
	view.zoom = 2.5f;

	nkref::NkRefItem a;
	a.pos = NkVec2f{-64.0f, 32.0f};
	a.scale = 0.5f;
	a.rotationDeg = 90.0f;
	a.opacity = 0.75f;
	a.mirrorX = true;
	a.texW = 128;
	a.texH = 96;
	a.sourcePath = NkString("C:/refs/arbre.png");
	a.selected = true; // champ que le format N'ÉCRIT PAS : ne peut pas faire tomber le test

	nkref::NkRefItem b;
	b.pos = NkVec2f{10.0f, 4.0f};
	b.scale = 1.25f;
	b.rotationDeg = -30.0f;
	b.opacity = 0.5f;
	b.mirrorY = true;
	b.texW = 64;
	b.texH = 48;

	board.items.PushBack(a);
	board.items.PushBack(b);

	nkref::NkRefStroke trait;
	trait.r = 255;
	trait.g = 0;
	trait.b = 0;
	trait.a = 255;
	trait.widthWorld = 3.0f;
	trait.points.PushBack(NkVec2f{0.0f, 0.0f});
	trait.points.PushBack(NkVec2f{10.0f, 5.0f});
	board.strokes.PushBack(trait);

	// Les octets source vivent à côté du modèle, au même index.
	nkref::NkRefFileImage images[2];
	const uint8 octetsA[5] = {0x89, 0x50, 0x4E, 0x47, 0x0D};
	const uint8 octetsB[3] = {0x41, 0x42, 0x43};
	images[0].data = octetsA;
	images[0].size = 5;
	images[1].data = octetsB;
	images[1].size = 3;

	// 2. Écrire.
	NkVector<uint8> premiere;
	NkRefSerialize(premiere, board, images, view, true, true);

	// 3. Relire.
	NkVector<nkref::NkRefLoadedItem> relus;
	NkVector<nkref::NkRefStroke> traitsRelus;
	nkref::NkRefView vueRelue;
	bool sombre = false;
	bool grille = false;
	const bool lu = NkRefDeserialize(premiere.Data(), premiere.Size(), relus, traitsRelus, vueRelue, sombre, grille);

	// 4. Reconstruire une planche avec ce que l'on a relu, et la réécrire.
	nkref::NkRefBoard secondBoard;
	for (usize i = 0; i < relus.Size(); ++i) {
		secondBoard.items.PushBack(relus[i].item);
	}
	for (usize i = 0; i < traitsRelus.Size(); ++i) {
		secondBoard.strokes.PushBack(traitsRelus[i]);
	}
	const usize n = relus.Size();
	nkref::NkRefFileImage *imagesRelues = new nkref::NkRefFileImage[n == 0 ? 1 : n];
	for (usize i = 0; i < n; ++i) {
		imagesRelues[i].data = relus[i].src.Data();
		imagesRelues[i].size = relus[i].src.Size();
	}

	NkVector<uint8> seconde;
	NkRefSerialize(seconde, secondBoard, imagesRelues, vueRelue, sombre, grille);
	delete[] imagesRelues;

	// 5. Comparer, octet par octet.
	bool identiques = lu && (premiere.Size() == seconde.Size());
	if (identiques) {
		for (usize i = 0; i < premiere.Size(); ++i) {
			if (premiere[i] != seconde[i]) {
				identiques = false;
				break;
			}
		}
	}

	std::printf("octets : %u, relu : %s, identiques : %s\n", static_cast<uint32>(premiere.Size()),
				lu ? "OUI" : "NON", identiques ? "OUI" : "NON");
	return identiques ? 0 : 1;
}
