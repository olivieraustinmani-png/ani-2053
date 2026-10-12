// =============================================================================
// main.cpp — EXO 2 Sprint 5 : l'extension qui ment
//
// Reconnaît le format d'un fichier image à ses premiers octets, puis dit si
// son extension dit vrai. Les règles se testent DANS L'ORDRE de l'énoncé, et
// la première qui s'applique décide : c'est le contrat du prof.
// =============================================================================
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

	// ── Décodage hexadécimal : majuscules comme minuscules ────────────────────
	// La consigne : "décode l'hexadécimal, majuscules comme minuscules".
	int ValeurHexa(char c) {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		return -1; // caractère non hexadécimal
	}

	// La chaîne hexadécimale de la ligne -> tableau d'octets 0..255.
	// "Comparez les octets comme des valeurs de 0 à 255" : on stocke uint8_t.
	std::vector<uint8_t> DecoderOctets(const std::string &hex) {
		std::vector<uint8_t> octets;
		for (size_t i = 0; i + 1 < hex.size(); i += 2) {
			const int h = ValeurHexa(hex[i]);
			const int b = ValeurHexa(hex[i + 1]);
			if (h < 0 || b < 0) break; // hexadécimal malformé : on s'arrête
			octets.push_back(static_cast<uint8_t>((h << 4) | b));
		}
		return octets;
	}

	// L'extension est le texte après le DERNIER point, en minuscules.
	std::string ExtensionDe(const std::string &nom) {
		const size_t pos = nom.rfind('.');
		if (pos == std::string::npos) return ""; // sans point : extension vide
		std::string ext = nom.substr(pos + 1);
		for (char &c : ext) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
		return ext;
	}

	// ── Les règles, DANS L'ORDRE de l'énoncé. La première qui s'applique
	//    décide. Une règle qui regarde un octet absent ne s'applique pas. ─────
	// Le retour : "" = REFUSE, sinon le nom du format reconnu.
	std::string Reconnaissance(const long long taille, const std::vector<uint8_t> &b) {
		const size_t n = b.size();
		const auto a = [&](size_t i) -> int { // octet i, ou -1 s'il n'existe pas
			return i < n ? static_cast<int>(b[i]) : -1;
		};

		// Règle 1 : plus petit que 4 octets -> REFUSE, rien d'autre n'est testé.
		if (taille < 4) return "";

		// Règle 2 : PNG — taille >= 8 et 89 50 4E 47
		if (taille >= 8 && a(0) == 0x89 && a(1) == 0x50 && a(2) == 0x4E && a(3) == 0x47)
			return "PNG";

		// Règle 3 : JPEG — FF D8 FF
		if (a(0) == 0xFF && a(1) == 0xD8 && a(2) == 0xFF) return "JPEG";

		// Règle 4 : BMP — 42 4D ("BM")
		if (a(0) == 0x42 && a(1) == 0x4D) return "BMP";

		// Règle 5 : QOI — 71 6F 69 66 ("qoif")
		if (a(0) == 0x71 && a(1) == 0x6F && a(2) == 0x69 && a(3) == 0x66)
			return "QOI";

		// Règle 6 : GIF — 47 49 46 38 ("GIF8")
		if (a(0) == 0x47 && a(1) == 0x49 && a(2) == 0x46 && a(3) == 0x38)
			return "GIF";

		// Règle 7 : ICO — 00 0? 01|02 00
		if (a(0) == 0x00 && (a(1) == 0x00 || a(1) == 0x01) && (a(2) == 0x01 || a(2) == 0x02) &&
			a(3) == 0x00)
			return "ICO";

		// Règle 8 : HDR — taille >= 10 et 23 3F ("#?")
		if (taille >= 10 && a(0) == 0x23 && a(1) == 0x3F) return "HDR";

		// Règle 9 : EXR — 76 2F 31 01
		if (a(0) == 0x76 && a(1) == 0x2F && a(2) == 0x31 && a(3) == 0x01) return "EXR";

		// Règle 10 : PBM/PGM/PPM — 'P' suivi de '1'..'6'
		if (a(0) == 0x50 && a(1) >= 0x31 && a(1) <= 0x36) {
			if (a(1) == 0x31 || a(1) == 0x34) return "PBM";
			if (a(1) == 0x32 || a(1) == 0x35) return "PGM";
			return "PPM"; // 3 ou 6
		}

		// Règle 11 : TGA — taille >= 18, pas de signature, b2 dans un jeu donné.
		if (taille >= 18 && a(2) >= 0 && a(2) <= 0x0B && a(2) != 0x04 && a(2) != 0x05 &&
			a(2) != 0x06 && a(2) != 0x07 && a(2) != 0x08)
			return "TGA";

		// Règle 12 : SVG — sauter EF BB BF puis les blancs, puis "<?xml"/"<svg".
		size_t i = 0;
		if (n >= 3 && a(0) == 0xEF && a(1) == 0xBB && a(2) == 0xBF) i = 3; // BOM
		while (i < n && (a(i) == 0x20 || a(i) == 0x09 || a(i) == 0x0A || a(i) == 0x0D)) ++i;
		if (n >= i + 4 && a(i) == 0x3C && a(i + 1) == 0x3F && a(i + 2) == 0x78 && a(i + 3) == 0x6D)
			return "SVG"; // "<?xm"
		if (n >= i + 4 && a(i) == 0x3C && a(i + 1) == 0x73 && a(i + 2) == 0x76 && a(i + 3) == 0x67)
			return "SVG"; // "<svg"

		return ""; // aucune règle : REFUSE
	}

	// L'extension juste de chaque format — la table de l'énoncé.
	bool ExtensionJuste(const std::string &format, const std::string &ext) {
		if (ext.empty()) return false; // une extension vide ne correspond à rien
		if (format == "PNG") return ext == "png";
		if (format == "JPEG") return ext == "jpg" || ext == "jpeg";
		if (format == "BMP") return ext == "bmp";
		if (format == "QOI") return ext == "qoi";
		if (format == "GIF") return ext == "gif";
		if (format == "ICO") return ext == "ico" || ext == "cur";
		if (format == "HDR") return ext == "hdr";
		if (format == "EXR") return ext == "exr";
		if (format == "PBM") return ext == "pbm";
		if (format == "PGM") return ext == "pgm";
		if (format == "PPM") return ext == "ppm";
		if (format == "TGA") return ext == "tga";
		if (format == "SVG") return ext == "svg";
		return false;
	}

} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n = 0;
	if (!(std::cin >> n)) return 0; // aucune entrée : rien à faire

	int lus = 0, mensonges = 0, refuses = 0;
	for (int i = 0; i < n; ++i) {
		std::string nom, hex;
		long long taille = 0;
		std::cin >> nom >> taille >> hex;

		const std::vector<uint8_t> b = (hex == "-") ? std::vector<uint8_t>() : DecoderOctets(hex);
		const std::string format = Reconnaissance(taille, b);

		if (format.empty()) {
			std::cout << nom << " REFUSE\n";
			++refuses;
		} else if (ExtensionJuste(format, ExtensionDe(nom))) {
			std::cout << nom << " " << format << " OK\n";
			++lus;
		} else {
			std::cout << nom << " " << format << " MENT\n";
			++lus;       // LUS compte les fichiers RECONNUS : un menteur est reconnu
			++mensonges;
		}
	}

	std::cout << "LUS " << lus << "\nMENSONGES " << mensonges << "\nREFUSES " << refuses << "\n";
	return 0;
}
