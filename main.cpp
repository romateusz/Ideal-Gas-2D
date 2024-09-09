#include "particles.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int main() {
    const int n = 500;      // liczba cząsteczek
    const double l = 100.0; // rozmiar obszaru
    const double v = 100.0; // maksymalna prędkość początkowa
    const double r = 0.5;   // promień cząstki
    const double dt = 0.01; // krok czasowy
    const int steps = 1000; // liczba kroków symulacji

    std::ofstream outFile("visual.config");
    outFile << "n," << n << "\n";
    outFile << "l," << l << "\n";
    outFile << "r," << r << "\n";
    outFile << "dt," << dt << "\n";
    outFile << "s," << steps << "\n";
    outFile.close();
    
    particles ps(n, l, v, r);   // konstruktor gazu o zadanych parametrach wejściowych

    double KineticEnergy = 0, Momentum_x = 0, Momentum_y = 0;
    std::ofstream file_p("properties.txt");
    for (int step = 0; step < steps; ++step) {
        KineticEnergy = 0;
        Momentum_x = 0, Momentum_y = 0;
        std::string filename = "particles_" + std::to_string(step) + ".txt";
        ps.saveToFile(filename);                            //zapis do pliku
        ps.update(dt, KineticEnergy, Momentum_x, Momentum_y);              // aktualizacja stanu układu
        double Momentum = sqrt(pow(Momentum_x,2) + pow(Momentum_y,2));
        file_p << step << "," << KineticEnergy << "," << Momentum << "\n";
    }
    file_p.close();
    return 0;
}