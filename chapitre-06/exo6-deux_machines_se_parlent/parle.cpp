// =============================================================================
// parle.cpp - EXO 6 Sprint 6 : deux machines se parlent
//
// Un programme, deux roles, choisis par argument : --serveur ou --client.
// Serveur : StartServer(7777, 8) + DrainAll a chaque tour.
// Client : Connect(NkAddress) puis un message texte par SendTo.
// Chaque cote affiche l'identifiant du pair (champ value) dans onPeerConnected.
// Le serveur affiche chaque message recu avec le champ from de son expediteur.
// Essai fait sur une seule machine (127.0.0.1), mentionne dans echange.txt.
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. CONTINUITE.md)
#include "NKNetwork/NKNetwork.h"
#include "NKTime/NkChrono.h"
#include <cstdio>
#include <cstring>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Parle";
	d.appVersion = "1.0.0";
	return d;
})());

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
		printf("[parle] PlatformInit ECHOUE\n");
		return -1;
	}

	nkentseu::NkConnectionManager connMgr;
	nkentseu::NkPeerId pairId = nkentseu::NkPeerId::Invalid();

	// L'identifiant du pair, affiche a la connexion, des deux cotes.
	connMgr.onPeerConnected = [&pairId](nkentseu::NkPeerId peer) {
		pairId = peer;
		printf("PAIR CONNECTE id=%llu\n", static_cast<unsigned long long>(peer.value));
	};

	if (roleServeur) {
		const nkentseu::NkNetResult r = connMgr.StartServer(7777, 8);
		if (r != nkentseu::NkNetResult::NK_NET_OK) {
			printf("[serveur] StartServer ECHOUE (%d)\n", static_cast<int>(r));
			nkentseu::NkSocket::PlatformShutdown();
			return -2;
		}
		printf("[serveur] ecoute sur 7777, max 8 clients\n");

		// Boucle serveur : DrainAll a chaque tour, affiche from + message.
		nkentseu::NkVector<nkentseu::NkReceiveMsg> recus;
		bool messageRecu = false;
		int compteurAttente = 0;

		while (compteurAttente < 300) { // 30 secondes max
			recus.Clear();
			connMgr.DrainAll(recus);
			for (nkentseu::usize m = 0; m < recus.Size(); ++m) {
				const nkentseu::NkReceiveMsg &msg = recus[m];
				printf("MSG de id=%llu : %.*s\n",
				       static_cast<unsigned long long>(msg.from.value),
				       static_cast<int>(msg.size),
				       reinterpret_cast<const char *>(msg.data));
				messageRecu = true;
			}
			if (messageRecu) {
				// Laisse le temps d'accuser reception avant de couper
				nkentseu::NkChrono::SleepMilliseconds(1500);
				break;
			}
			nkentseu::NkChrono::SleepMilliseconds(100);
			++compteurAttente;
		}

		connMgr.Shutdown();
		nkentseu::NkSocket::PlatformShutdown();
		return 0;
	}

	// ─ CLIENT ────────────────────────────────────────────────────────────────
	const nkentseu::NkNetResult r = connMgr.Connect(nkentseu::NkAddress(adresseServeur, 7777));
	if (r != nkentseu::NkNetResult::NK_NET_OK) {
		printf("[client] Connect ECHOUE (%d)\n", static_cast<int>(r));
		nkentseu::NkSocket::PlatformShutdown();
		return -2;
	}
	printf("[client] connexion demandee a %s:7777\n", adresseServeur);

	// La connexion se fait dans un fil du moteur : on attend d'etre connecte
	// avant d'envoyer (piege de l'enonce).
	for (int i = 0; i < 50 && connMgr.ConnectedPeerCount() == 0; ++i) {
		nkentseu::NkChrono::SleepMilliseconds(100);
	}

	if (connMgr.ConnectedPeerCount() == 0) {
		printf("[client] jamais connecte\n");
		connMgr.Shutdown();
		nkentseu::NkSocket::PlatformShutdown();
		return -3;
	}

	const char *message = "bonjour du client";
	nkentseu::NkPeerId cible = pairId.IsValid() ? pairId : nkentseu::NkPeerId::Server();
	const nkentseu::NkNetResult sendRes = connMgr.SendTo(
		cible,
		reinterpret_cast<const nkentseu::nk_uint8 *>(message),
		static_cast<nkentseu::nk_uint32>(strlen(message)),
		nkentseu::NkNetChannel::NK_NET_CHANNEL_RELIABLE_ORDERED
	);

	if (sendRes == nkentseu::NkNetResult::NK_NET_OK) {
		printf("[client] message envoye\n");
	} else {
		printf("[client] SendTo ECHOUE (%d)\n", static_cast<int>(sendRes));
	}

	nkentseu::NkChrono::SleepMilliseconds(1000); // laisse partir le message
	connMgr.Shutdown();
	nkentseu::NkSocket::PlatformShutdown();
	return 0;
}
