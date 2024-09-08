#include "particles.h"
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <set>

// lista inicjalizacyjna
particle::particle(double x, double y, double v_x, double v_y, double r): x(x), y(y), v_x(v_x), v_y(v_y), r(r) {}

double particle::getr() const {
    return r;
}

double particle::getx() const {
    return x;
}

double particle::gety() const {
    return y;
}

double particle::getv_x() const {
    return v_x;
}

double particle::getv_y() const {
    return v_y;
}

void particle::setPosition(double new_x, double new_y) {
    x = new_x;
    y = new_y;
}

void particle::setVelocity(double new_vx, double new_vy) {
    v_x = new_vx;
    v_y = new_vy;
}

// Przypadek zderzenia sprężystego: wymiana prędkości
// SPOSÓB NIEPOPRAWNY (testowy)
void particle::collision(particle& p) {
        double temp_vx = v_x;
        double temp_vy = v_y;
        v_x = p.getv_x();
        v_y = p.getv_y();
        p.setVelocity(temp_vx, temp_vy);
}

void particle::separate(particle& p) {
    // Oblicz różnicę w położeniu
    double dx = p.getx() - x;
    double dy = p.gety() - y;
    double distance = std::sqrt(dx * dx + dy * dy);
    double sum_r = r + p.getr();
    
    double overlap = sum_r - distance;
    double nx = dx / distance;      // znormalizowana różnica w osi X
    double ny = dy / distance;      // znormalizowana różnica w osi Y
    
    // Przesuwamy cząstki o połowę nakładającej się odległości
    double move_x = overlap * nx / 2;
    double move_y = overlap * ny / 2;

    // Dla cząstki this
    double adjusted_move_x = std::copysign(move_x, v_x); // Przesunięcie o move_x w kierunku znaku v_x
    double adjusted_move_y = std::copysign(move_y, v_y); // Przesunięcie o move_y w kierunku znaku v_y

    // Dla cząstki p
    double adjusted_move_x_p = std::copysign(move_x, p.getv_x()); // Przesunięcie o move_x w kierunku znaku v_x
    double adjusted_move_y_p = std::copysign(move_y, p.getv_y()); // Przesunięcie o move_y w kierunku znaku v_y

    // Aktualizacja pozycji cząstek
    this->setPosition(x + adjusted_move_x, y + adjusted_move_y);
    p.setPosition(p.getx() - adjusted_move_x_p, p.gety() - adjusted_move_y_p);
}

std::string particle::toString() const {
    std::ostringstream oss;
    oss << x << "," << y << "," << v_x << "," << v_y;
    return oss.str();
}

particles::particles(int n, double l, double v, double r) : l(l) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_pos(0.0, l);
    std::uniform_real_distribution<> dis_vel(-v, v);

    for (int i = 0; i < n; ++i) {
        double x = dis_pos(gen);
        double y = dis_pos(gen);
        double v_x = dis_vel(gen);
        double v_y = dis_vel(gen);
        P.emplace_back(x, y, v_x, v_y, r);
    }
}

std::string particles::toString() const {
    std::ostringstream oss;
    for (const auto& p : P) {
        oss << p.toString() << "\n";
    }
    return oss.str();
}

void particles::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << toString();
        file.close();
    } else {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
    }
}

void particles::update(double dt) {
    // Aktualizacja pozycji i prędkości cząsteczek
    for (auto& p : P) {
        double new_x = p.getx() + p.getv_x() * dt;
        double new_y = p.gety() + p.getv_y() * dt;

        // Odbicia od ścian, gdy cząstka jest w odległości r od ściany
        double radius = p.getr();
        if (new_x <= radius) {
            p.setVelocity(-p.getv_x(), p.getv_y());
            new_x = radius; // Ustawienie pozycji na krawędzi
        } else if (new_x >= l - radius) {
            p.setVelocity(-p.getv_x(), p.getv_y());
            new_x = l - radius; // Ustawienie pozycji na krawędzi
        }

        if (new_y <= radius) {
            p.setVelocity(p.getv_x(), -p.getv_y());
            new_y = radius; // Ustawienie pozycji na krawędzi
        } else if (new_y >= l - radius) {
            p.setVelocity(p.getv_x(), -p.getv_y());
            new_y = l - radius; // Ustawienie pozycji na krawędzi
        }

        p.setPosition(new_x, new_y);
    }

    // Sortowanie cząsteczek według osi x
    xSorted.clear();
    for (int i = 0; i < P.size(); ++i) {
        xSorted.emplace_back(P[i].getx(), i);
    }
    std::sort(xSorted.begin(), xSorted.end());

    // Sprawdzanie kolizji
    for (size_t i = 0; i < xSorted.size(); ++i) {
        int idx1 = xSorted[i].second;
        for (size_t j = i + 1; j < xSorted.size(); ++j) {
            int idx2 = xSorted[j].second;

            // Jeśli cząsteczki są zbyt daleko wzdłuż osi x, przerywamy sprawdzanie
            if (xSorted[j].first - xSorted[i].first > 2 * P[idx1].getr()) {
                break;
            }

            // Sprawdzamy kolizję wzdłuż osi y
            if (std::abs(P[idx1].gety() - P[idx2].gety()) <= 2 * P[idx1].getr()) {
                P[idx1].collision(P[idx2]);
                P[idx1].separate(P[idx2]);
            }
        }
    }
}