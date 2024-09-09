#include "particles.h"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    const int n = 500;  // liczba cząstek
    const double l = 100.0; // rozmiar obszaru
    const double v = 100.0; // maksymalna prędkość początkowa
    const double r = 0.5;  // promień cząstki
    const double dt = 0.01; // krok czasowy
    const int steps = 1000; // liczba kroków symulacji

    // const int n = 2;  // liczba cząstek
    // const double l = 100.0; // rozmiar obszaru
    // const double v = 400.0; // maksymalna prędkość początkowa
    // const double r = 10;  // promień cząstki
    // const double dt = 0.01; // krok czasowy
    // const int steps = 1000; // liczba kroków symulacji

    std::ofstream outFile("visual.config");
    outFile << "n," << n << "\n";
    outFile << "l," << l << "\n";
    outFile << "r," << r << "\n";
    outFile << "dt," << dt << "\n";
    outFile << "s," << steps << "\n";
    outFile.close();
    
    particles ps(n, l, v, r);   // konstruktor gazu o zadanych parametrach wejściowych

    for (int step = 0; step < steps; ++step) {
        std::string filename = "particles_" + std::to_string(step) + ".txt";
        ps.saveToFile(filename);    //zapis do pliku
        ps.update(dt);              // aktualizacja stanu układu
    }

    return 0;
}
