// =============================================================================
// decouverte.cpp - EXO 7 Sprint 6 : la decouverte LAN
//
// Un programme, deux roles, choisis par argument :
//   --serveur : diffuse regulierement la presence du serveur sur le port 7778
//               via NkDiscovery::Broadcast(info) toutes les 2 secondes.
//   --client  : ecoute pendant 6 secondes via NkDiscovery::Listen(6000, trouves)
//               et affiche la liste des serveurs trouves (adresse et nom).
// =============================================================================
#include "NKWindow/NKMain.h" // EN PREMIER (cf. CONTINUITE.md)
#include "NKNetwork/NKNetwork.h"
#include "NKTime/NkChrono.h"
#include <cstdio>
#include <cstring>

NKENTSEU_DEFINE_APP_DATA(([]() {
	nkentseu::NkAppData d{};
	d.appName = "Decouverte";
	d.appVersion = "1.0.0";
	return d;
})());

int nkmain(const nkentseu::NkEntryState &state) {
	bool roleServeur = false;
	const char *nomServeur = "Serveur-ANI2053";
	nkentseu::nk_uint32 maxJoueurs = 8;

	for (nkentseu::nk_uint32 i = 0; i < state.args.Size(); ++i) {
		const nkentseu::NkString a = state.args[i];
		if (a == "--serveur" || a == "--server") {
			roleServeur = true;
		} else if (a.StartsWith("--name=")) {
			nomServeur = a.SubStr(7).CStr();
		} else if (a == "--client") {
			roleServeur = false;
		}
	}

	if (nkentseu::NkSocket::PlatformInit() != nkentseu::NkNetResult::NK_NET_OK) {
		printf("[decouverte] PlatformInit ECHOUE\n");
		return -1;
	}

	if (roleServeur) {
		nkentseu::NkDiscovery::ServerInfo info;
		strncpy(info.name, nomServeur, sizeof(info.name) - 1);
		strncpy(info.gameMode, "Deathmatch", sizeof(info.gameMode) - 1);
		info.playerCount = 1;
		info.maxPlayers = maxJoueurs; // Renseigne maxPlayers (piege de l'enonce: > 0)
		info.hasPassword = false;

		printf("[serveur] diffusion LAN (port 7778) toutes les 2s : %s (max %u joueurs)\n",
		       info.name, static_cast<unsigned int>(info.maxPlayers));

		// Diffuse 6 fois (12 secondes au total), suffisant pour que le client l'ecoute
		for (int tour = 0; tour < 6; ++tour) {
			const nkentseu::NkNetResult res = nkentseu::NkDiscovery::Broadcast(info);
			if (res == nkentseu::NkNetResult::NK_NET_OK) {
				printf("[serveur] broadcast envoye (%d/6)\n", tour + 1);
			} else {
				printf("[serveur] broadcast echoue (%d)\n", static_cast<int>(res));
			}
			nkentseu::NkChrono::SleepMilliseconds(2000);
		}

		nkentseu::NkSocket::PlatformShutdown();
		return 0;
	}

	// ─ CLIENT ────────────────────────────────────────────────────────────────
	nkentseu::NkVector<nkentseu::NkDiscovery::ServerInfo> trouves;
	printf("[client] ecoute des serveurs LAN pendant 6s...\n");

	const nkentseu::NkNetResult r = nkentseu::NkDiscovery::Listen(6000, trouves);
	if (r == nkentseu::NkNetResult::NK_NET_OK) {
		printf("[client] ecoute terminee, %u serveur(s) trouve(s) :\n",
		       static_cast<unsigned int>(trouves.Size()));
		for (nkentseu::usize i = 0; i < trouves.Size(); ++i) {
			printf("Serveur %u : %s - %s (joueurs: %u/%u, ping: %u ms)\n",
			       static_cast<unsigned int>(i + 1),
			       trouves[i].name,
			       trouves[i].addr.ToString().CStr(),
			       static_cast<unsigned int>(trouves[i].playerCount),
			       static_cast<unsigned int>(trouves[i].maxPlayers),
			       static_cast<unsigned int>(trouves[i].ping));
		}
	} else {
		printf("[client] Listen echoue (%d)\n", static_cast<int>(r));
	}

	nkentseu::NkSocket::PlatformShutdown();
	return 0;
}

