// =============================================================================
// octets.cpp - EXO 8 Sprint 6 : le message qui compte ses octets
//
// Un programme, deux roles : --serveur ou --client (defaut).
// Le client serialise l'etat d'un joueur de deux facons :
//   1) Types complets   : WriteU8, WriteF32, WriteI32
//   2) Types quantifies : WriteBool, WriteU8, WriteF32Q, WriteInt
// Il affiche BitsWritten() et BytesWritten() pour chaque paquet,
// puis envoie les deux paquets au serveur via NkConnectionManager.
// Le serveur relit les deux paquets avec NkBitReader, affiche
// les valeurs relues, et calcule l'ecart maximal par champ.
// Essai fait sur une seule machine (127.0.0.1).
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. CONTINUITE.md)
#include "NKNetwork/NKNetwork.h"
#include "NKNetwork/Protocol/NkBitStream.h"
#include "NKTime/NkChrono.h"
#include <cstdio>
#include <cstring>
#include <cmath>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Octets";
	d.appVersion = "1.0.0";
	return d;
})());

// ─── Etat d'un joueur : six champs ──────────────────────────────────────────
struct EtatJoueur {
	bool   enVie;    // en vie ou non
	nkentseu::nk_uint8 numero;   // 0..255
	float  posX;     // -500.0 .. +500.0
	float  posY;     // -500.0 .. +500.0
	float  angle;    // 0.0 .. 360.0
	nkentseu::nk_int32 sante;   // 0..100
};

// ─── Constantes de quantification ───────────────────────────────────────────
static constexpr float POS_MIN  = -500.0f;
static constexpr float POS_MAX  =  500.0f;
static constexpr float POS_PREC =    0.01f;  // 1 cm
static constexpr float ANG_MIN  =    0.0f;
static constexpr float ANG_MAX  =  360.0f;
static constexpr float ANG_PREC =    0.5f;   // demi-degre
static constexpr nkentseu::nk_int32 SANTE_MIN = 0;
static constexpr nkentseu::nk_int32 SANTE_MAX = 100;

// ─── Ecriture paquet complet ─────────────────────────────────────────────────
static void ecrireComplet(nkentseu::net::NkBitWriter &w, const EtatJoueur &j) {
	w.WriteU8(j.enVie ? 1 : 0);
	w.WriteU8(j.numero);
	w.WriteF32(j.posX);
	w.WriteF32(j.posY);
	w.WriteF32(j.angle);
	w.WriteI32(j.sante);
}

// ─── Ecriture paquet quantifie ───────────────────────────────────────────────
static void ecrireQuantifie(nkentseu::net::NkBitWriter &w, const EtatJoueur &j) {
	w.WriteBool(j.enVie);
	w.WriteU8(j.numero);
	w.WriteF32Q(j.posX,  POS_MIN, POS_MAX, POS_PREC);
	w.WriteF32Q(j.posY,  POS_MIN, POS_MAX, POS_PREC);
	w.WriteF32Q(j.angle, ANG_MIN, ANG_MAX, ANG_PREC);
	w.WriteInt(j.sante,  SANTE_MIN, SANTE_MAX);
}

// ─── Lecture paquet complet ──────────────────────────────────────────────────
static EtatJoueur lireComplet(nkentseu::net::NkBitReader &r) {
	EtatJoueur j{};
	j.enVie  = (r.ReadU8() != 0);
	j.numero = r.ReadU8();
	j.posX   = r.ReadF32();
	j.posY   = r.ReadF32();
	j.angle  = r.ReadF32();
	j.sante  = r.ReadI32();
	return j;
}

// ─── Lecture paquet quantifie ────────────────────────────────────────────────
static EtatJoueur lireQuantifie(nkentseu::net::NkBitReader &r) {
	EtatJoueur j{};
	j.enVie  = r.ReadBool();
	j.numero = r.ReadU8();
	j.posX   = r.ReadF32Q(POS_MIN, POS_MAX, POS_PREC);
	j.posY   = r.ReadF32Q(POS_MIN, POS_MAX, POS_PREC);
	j.angle  = r.ReadF32Q(ANG_MIN, ANG_MAX, ANG_PREC);
	j.sante  = r.ReadInt(SANTE_MIN, SANTE_MAX);
	return j;
}

// ─── Afficher un EtatJoueur ──────────────────────────────────────────────────
static void afficher(const char *etiquette, const EtatJoueur &j) {
	printf("  [%s] enVie=%s  numero=%u  posX=%.4f  posY=%.4f  angle=%.4f  sante=%d\n",
	       etiquette,
	       j.enVie ? "oui" : "non",
	       static_cast<unsigned>(j.numero),
	       j.posX, j.posY, j.angle, j.sante);
}

// ─── Afficher les ecarts ─────────────────────────────────────────────────────
static void afficherEcarts(const EtatJoueur &orig, const EtatJoueur &relu) {
	printf("  Ecarts maximaux (original vs relu quantifie) :\n");
	printf("    enVie  : %d\n", (orig.enVie != relu.enVie) ? 1 : 0);
	printf("    numero : %d\n", std::abs(static_cast<int>(orig.numero) - static_cast<int>(relu.numero)));
	printf("    posX   : %.6f m\n", std::fabs(orig.posX - relu.posX));
	printf("    posY   : %.6f m\n", std::fabs(orig.posY - relu.posY));
	printf("    angle  : %.6f deg\n", std::fabs(orig.angle - relu.angle));
	printf("    sante  : %d\n", std::abs(orig.sante - relu.sante));
}

int nkmain(const nkentseu::NkEntryState &state) {
	const char *adresseServeur = "127.0.0.1";
	bool roleServeur = false;

	for (nkentseu::nk_uint32 i = 0; i < state.args.Size(); ++i) {
		const nkentseu::NkString a = state.args[i];
		if (a == "--serveur" || a == "--server") {
			roleServeur = true;
		} else if (a.StartsWith("--client=")) {
			adresseServeur = a.SubStr(9).CStr();
		} else if (a == "--client") {
			roleServeur = false;
		}
	}

	if (nkentseu::NkSocket::PlatformInit() != nkentseu::NkNetResult::NK_NET_OK) {
		printf("[octets] PlatformInit ECHOUE\n");
		return -1;
	}

	// ─── Valeurs choisies pour l'etat du joueur ──────────────────────────
	EtatJoueur joueur{};
	joueur.enVie  = true;
	joueur.numero = 42;
	joueur.posX   = 123.456f;
	joueur.posY   = -78.9012f;
	joueur.angle  = 247.3f;
	joueur.sante  = 87;

	nkentseu::NkConnectionManager connMgr;
	nkentseu::NkPeerId pairId = nkentseu::NkPeerId::Invalid();

	connMgr.onPeerConnected = [&pairId](nkentseu::NkPeerId peer) {
		pairId = peer;
		printf("PAIR CONNECTE id=%llu\n", static_cast<unsigned long long>(peer.value));
	};

	if (roleServeur) {
		// ═══════════════════════════════════════════════════════════════════
		// SERVEUR : recoit les deux paquets, les relit, affiche les ecarts
		// ═══════════════════════════════════════════════════════════════════
		const nkentseu::NkNetResult r = connMgr.StartServer(7779, 8);
		if (r != nkentseu::NkNetResult::NK_NET_OK) {
			printf("[serveur] StartServer ECHOUE (%d)\n", static_cast<int>(r));
			nkentseu::NkSocket::PlatformShutdown();
			return -2;
		}
		printf("[serveur] ecoute sur 7779\n");

		nkentseu::NkVector<nkentseu::NkReceiveMsg> recus;
		int paquetsRecus = 0;

		for (int attente = 0; attente < 300 && paquetsRecus < 2; ++attente) {
			recus.Clear();
			connMgr.DrainAll(recus);

			for (nkentseu::usize m = 0; m < recus.Size(); ++m) {
				const nkentseu::NkReceiveMsg &msg = recus[m];
				++paquetsRecus;

				if (paquetsRecus == 1) {
					// Premier paquet : complet
					printf("\n=== Paquet 1 (types complets) recu : %u octets ===\n",
					       static_cast<unsigned>(msg.size));
					nkentseu::net::NkBitReader reader(msg.data, msg.size);
					EtatJoueur reluComplet = lireComplet(reader);
					afficher("complet relu", reluComplet);
					if (reader.IsOverflowed()) {
						printf("  ATTENTION : reader overflow sur paquet complet\n");
					}
				} else if (paquetsRecus == 2) {
					// Second paquet : quantifie
					printf("\n=== Paquet 2 (quantifie) recu : %u octets ===\n",
					       static_cast<unsigned>(msg.size));
					nkentseu::net::NkBitReader reader(msg.data, msg.size);
					EtatJoueur reluQ = lireQuantifie(reader);
					afficher("quantifie relu", reluQ);
					if (reader.IsOverflowed()) {
						printf("  ATTENTION : reader overflow sur paquet quantifie\n");
					}
					// Ecarts
					afficherEcarts(joueur, reluQ);
				}
			}
			nkentseu::NkChrono::SleepMilliseconds(100);
		}

		nkentseu::NkChrono::SleepMilliseconds(500);
		connMgr.Shutdown();
		nkentseu::NkSocket::PlatformShutdown();
		return 0;
	}

	// ═══════════════════════════════════════════════════════════════════════
	// CLIENT : serialise, mesure, envoie les deux paquets
	// ═══════════════════════════════════════════════════════════════════════
	const nkentseu::NkNetResult r = connMgr.Connect(nkentseu::NkAddress(adresseServeur, 7779));
	if (r != nkentseu::NkNetResult::NK_NET_OK) {
		printf("[client] Connect ECHOUE (%d)\n", static_cast<int>(r));
		nkentseu::NkSocket::PlatformShutdown();
		return -2;
	}
	printf("[client] connexion demandee a %s:7779\n", adresseServeur);

	// Attente connexion
	for (int i = 0; i < 50 && connMgr.ConnectedPeerCount() == 0; ++i) {
		nkentseu::NkChrono::SleepMilliseconds(100);
	}
	if (connMgr.ConnectedPeerCount() == 0) {
		printf("[client] jamais connecte\n");
		connMgr.Shutdown();
		nkentseu::NkSocket::PlatformShutdown();
		return -3;
	}

	printf("\nValeurs originales du joueur :\n");
	afficher("original", joueur);

	// ─── Paquet 1 : types complets ───────────────────────────────────────
	nkentseu::nk_uint8 bufComplet[256];
	nkentseu::net::NkBitWriter wComplet(bufComplet, sizeof(bufComplet));
	ecrireComplet(wComplet, joueur);

	printf("\n--- Paquet complet ---\n");
	printf("  BitsWritten  = %u\n", wComplet.BitsWritten());
	printf("  BytesWritten = %u\n", wComplet.BytesWritten());
	if (wComplet.IsOverflowed()) {
		printf("  OVERFLOW !\n");
	}

	// Relecture locale pour verification
	{
		nkentseu::net::NkBitReader rLocal(bufComplet, wComplet.BytesWritten());
		EtatJoueur relocalC = lireComplet(rLocal);
		afficher("complet relu local", relocalC);
	}

	// ─── Paquet 2 : quantifie ────────────────────────────────────────────
	nkentseu::nk_uint8 bufQuant[256];
	nkentseu::net::NkBitWriter wQuant(bufQuant, sizeof(bufQuant));
	ecrireQuantifie(wQuant, joueur);
	wQuant.AlignToByte();

	printf("\n--- Paquet quantifie ---\n");
	printf("  BitsWritten  = %u\n", wQuant.BitsWritten());
	printf("  BytesWritten = %u\n", wQuant.BytesWritten());
	if (wQuant.IsOverflowed()) {
		printf("  OVERFLOW !\n");
	}

	// Relecture locale pour verification et ecarts
	{
		nkentseu::net::NkBitReader rLocal(bufQuant, wQuant.BytesWritten());
		EtatJoueur relocalQ = lireQuantifie(rLocal);
		afficher("quantifie relu local", relocalQ);
		afficherEcarts(joueur, relocalQ);
	}

	// ─── Envoi des deux paquets au serveur ───────────────────────────────
	nkentseu::NkPeerId cible = pairId.IsValid() ? pairId : nkentseu::NkPeerId::Server();

	nkentseu::NkNetResult s1 = connMgr.SendTo(
		cible,
		bufComplet,
		wComplet.BytesWritten(),
		nkentseu::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED
	);
	printf("\n[client] envoi paquet complet : %s\n",
	       (s1 == nkentseu::NkNetResult::NK_NET_OK) ? "OK" : "ECHOUE");

	nkentseu::NkChrono::SleepMilliseconds(200); // espacement

	nkentseu::NkNetResult s2 = connMgr.SendTo(
		cible,
		bufQuant,
		wQuant.BytesWritten(),
		nkentseu::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED
	);
	printf("[client] envoi paquet quantifie : %s\n",
	       (s2 == nkentseu::NkNetResult::NK_NET_OK) ? "OK" : "ECHOUE");

	nkentseu::NkChrono::SleepMilliseconds(1000);
	connMgr.Shutdown();
	nkentseu::NkSocket::PlatformShutdown();
	return 0;
}

