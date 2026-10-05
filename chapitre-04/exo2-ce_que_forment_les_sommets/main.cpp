#include <iostream>
#include <string>

int main() {
	int n = 0;
	std::cin >> n;

	int totalPoints = 0;
	int totalSegments = 0;
	int totalTriangles = 0;
	int totalRefuses = 0;
	

	for (int i = 0; i < n; i++) {
		std::string type;
		int sommets = 0;
		std::cin >> type >> sommets;

		int nombre = 0;
		int restants = 0;
		std::string unite;
		bool accepte = true;

		if (type == "POINTS") {
			nombre = sommets;
			unite = "POINTS";
			totalPoints += nombre;
		} else if (type == "LINES") {
			nombre = sommets / 2;
			restants = sommets % 2;
			unite = "SEGMENTS";
			totalSegments += nombre;
		} else if (type == "LINE_STRIP") {
			if (sommets >= 2) {
				nombre = sommets - 1;
			} else {
				restants = sommets;
			}
			unite = "SEGMENTS";
			totalSegments += nombre;
		} else if (type == "TRIANGLES") {
			nombre = sommets / 3;
			restants = sommets % 3;
			unite = "TRIANGLES";
			totalTriangles += nombre;
		} else if (type == "TRIANGLE_STRIP" || type == "TRIANGLE_FAN") {
			if (sommets >= 3) {
				nombre = sommets - 2;
			} else {
				restants = sommets;
			}
			unite = "TRIANGLES";
			totalTriangles += nombre;
		} else {
			accepte = false;
			totalRefuses++;
		}

		if (accepte) {
			std::cout << type << ' ' << sommets << ' ' << nombre << ' ' << unite << ' ' << restants << '\n';
		} else {
			std::cout << type << ' ' << sommets << " REFUSE\n";
		}
	}

	std::cout << "POINTS " << totalPoints << '\n';
	std::cout << "SEGMENTS " << totalSegments << '\n';
	std::cout << "TRIANGLES " << totalTriangles << '\n';
	std::cout << "REFUSES " << totalRefuses << '\n';
	
	return 0;
}
