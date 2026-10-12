// =============================================================================
// main.cpp — EXO 8 Sprint 5 : un son, vingt voix
//
// Rejoue une rafale de sons de trois manières : le compte de voix actives,
// le programme qui recharge le fichier à chaque lecture, et la voix unique
// rejouée (qui coupe). Fin exclue : une lecture qui finit exactement à t_i
// a fini.
// =============================================================================
#include <cstdint>
#include <iostream>
#include <vector>

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	long long D = 0, C = 0;
	if (!(std::cin >> D >> C)) return 0;

	int N = 0;
	if (!(std::cin >> N)) N = 0;

	std::vector<long long> t(N);
	for (int i = 0; i < N; ++i) std::cin >> t[i];

	long long voixMax = 0, retardMax = 0, coupes = 0;

	// Fin précédente du chargement : les chargements s'enchaînent, chacun
	// commence au plus tard de t_i et de la fin du précédent.
	long long finPrecedente = 0;

	for (int i = 0; i < N; ++i) {
		// ── Colonne voix : lectures déjà lancées (celle-ci comprise) non
		//    finies à t_i. Une lecture j finit à t_j + D, fin exclue.
		long long voix = 0;
		for (int j = 0; j <= i; ++j)
			if (t[j] + D > t[i]) ++voix; // strictement : fin à t_i = fini
		if (voix > voixMax) voixMax = voix;

		// ── Colonne retard : le fichier rechargé à chaque lecture.
		long long debut = (t[i] > finPrecedente) ? t[i] : finPrecedente;
		long long fin = debut + C;
		long long retard = fin - t[i];
		if (retard > retardMax) retardMax = retard;
		finPrecedente = fin;

		// ── Colonne état : la voix unique rejouée. La lecture i est COUPE si
		//    la suivante existe et commence avant t_i + D, strictement.
		const bool coupee = (i + 1 < N) && (t[i + 1] < t[i] + D);
		if (coupee) ++coupes;
		std::cout << t[i] << " " << voix << " " << retard << " " << (coupee ? "COUPE" : "ENTIER")
				  << "\n";
	}

	std::cout << "VOIX_MAX " << voixMax << "\nRETARD_MAX " << retardMax << "\nCOUPES " << coupes
			  << "\n";
	return 0;
}
