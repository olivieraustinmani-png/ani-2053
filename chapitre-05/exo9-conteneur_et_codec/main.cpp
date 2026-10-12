// =============================================================================
// main.cpp — EXO 9 Sprint 5 : conteneur et codec
//
// Reconnaît le conteneur à ses premiers octets (jamais par l'extension),
// nomme le codec de chaque piste d'un MP4, et juge le lecteur sur la
// première piste vide. Les règles se testent DANS L'ORDRE.
// =============================================================================
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace {

	int ValeurHexa(char c) {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;
		return -1;
	}

	std::vector<uint8_t> DecoderOctets(const std::string &hex) {
		std::vector<uint8_t> o;
		for (size_t i = 0; i + 1 < hex.size(); i += 2) {
			const int h = ValeurHexa(hex[i]), b = ValeurHexa(hex[i + 1]);
			if (h < 0 || b < 0) break;
			o.push_back(static_cast<uint8_t>((h << 4) | b));
		}
		return o;
	}

	// Les octets b[i..i+3] valent-ils ces quatre caractères ?
	// Rend false si un octet est absent : une règle qui regarde un octet
	// manquant ne s'applique pas.
	bool Mot(const std::vector<uint8_t> &b, size_t i, const char *m) {
		if (i + 3 >= b.size()) return false;
		return b[i] == static_cast<uint8_t>(m[0]) && b[i + 1] == static_cast<uint8_t>(m[1]) &&
			   b[i + 2] == static_cast<uint8_t>(m[2]) && b[i + 3] == static_cast<uint8_t>(m[3]);
	}

	// ── La table des codecs. Le code absent garde son nom, tel quel. ──────────
	std::string NommerCodec(const std::string &code) {
		if (code == "mp4a") return "aac";
		if (code == "Opus" || code == "opus") return "opus";
		if (code == "avc1" || code == "avc3") return "h264";
		if (code == "hvc1" || code == "hev1") return "h265";
		if (code == "vp08") return "vp8";
		if (code == "vp09") return "vp9";
		if (code == "mp4v") return "mpeg4";
		if (code == ".mp3") return "mp3";
		if (code == "twos" || code == "sowt" || code == "lpcm") return "pcm";
		return code; // absent de la table : son nom de quatre caractères
	}

	// ── Le lecteur : première piste vide du MP4. ──────────────────────────────
	std::string VerdictLecteur(const std::vector<std::pair<std::string, std::string>> &pistes) {
		for (const auto &p : pistes) {
			if (p.first != "vide") continue;
			const std::string &code = p.second;
			if (code == "mjpa" || code == "jpeg" || code == "MJPG" || code == "avc1" ||
				code == "avc3" || code == "hvc1" || code == "hev1" || code == "av01")
				return "LISIBLE";
			return "ECHEC";
		}
		return "SANS_IMAGE";
	}

} // namespace

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int n = 0;
	if (!(std::cin >> n)) return 0;

	int mp4 = 0, lisibles = 0, inconnus = 0;

	for (int i = 0; i < n; ++i) {
		std::string nom, hex;
		int P = 0;
		std::cin >> nom >> hex >> P;

		std::vector<std::pair<std::string, std::string>> pistes;
		for (int p = 0; p < P; ++p) { // on lit TOUTES les pistes, même ignorées
			std::string type, code;
			std::cin >> type >> code;
			pistes.push_back({type, code});
		}

		const std::vector<uint8_t> b = (hex == "-") ? std::vector<uint8_t>() : DecoderOctets(hex);

		// ── Conteneur, règles DANS L'ORDRE, la première qui s'applique décide.──
		std::string conteneur = "INCONNU";
		if (b.size() >= 12 && Mot(b, 4, "ftyp")) conteneur = "MP4";
		else if (b.size() >= 4 && Mot(b, 0, "\x1A\x45\xDF\xA3")) conteneur = "WEBM";
		else if (b.size() >= 12 && Mot(b, 0, "RIFF") && Mot(b, 8, "WAVE")) conteneur = "WAV";
		else if (b.size() >= 4 && Mot(b, 0, "OggS")) conteneur = "OGG";
		else if (b.size() >= 4 && Mot(b, 0, "fLaC")) conteneur = "FLAC";
		else if (b.size() >= 3 && Mot(b, 0, "ID3")) conteneur = "MP3";
		else if (b.size() >= 2 && b[0] == 0xFF && b[1] >= 0xE0) conteneur = "MP3";

		std::cout << nom << " " << conteneur << "\n";

		if (conteneur == "MP4") {
			for (size_t k = 0; k < pistes.size(); ++k) {
				const std::string type = (pistes[k].first == "vide") ? "VIDEO" : "AUDIO";
				std::cout << nom << " PISTE " << (k + 1) << " " << type << " "
						  << NommerCodec(pistes[k].second) << "\n";
			}
			const std::string verdict = VerdictLecteur(pistes);
			std::cout << nom << " LECTEUR " << verdict << "\n";
			if (verdict == "LISIBLE") ++lisibles;
			++mp4;
		} else if (conteneur == "INCONNU") {
			++inconnus;
		}
	}

	std::cout << "MP4 " << mp4 << "\nLISIBLES " << lisibles << "\nINCONNUS " << inconnus << "\n";
	return 0;
}
