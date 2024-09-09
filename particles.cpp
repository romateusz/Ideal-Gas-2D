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
void particle::collision(particle& p) {
    // double tmpx = v_x;
    // double tmpy = v_y;
    // v_x = p.getv_x();
    // v_y = p.getv_y();
    // p.setVelocity(tmpx, tmpy);

    double dx = x - p.getx();
    double dy = y - p.gety();
    double dvx = p.getv_x() - v_x;
    double dvy = p.getv_y() - v_y;
    double distance_squared = dx * dx + dy * dy;

    double dot_product = dx * dvx + dy * dvy;
    double factor = dot_product / distance_squared;

    double new_vx = v_x + factor * dx;
    double new_vy = v_y + factor * dy;
    double new_vpx = p.getv_x() - factor * dx;
    double new_vpy = p.getv_y() - factor * dy;

    v_x = new_vx;
    v_y = new_vy;
    p.setVelocity(new_vpx, new_vpy);

}



void particle::separate(particle& p, double l) {
    // Oblicz różnicę w położeniu
    double dx = p.getx() - x;
    double dy = p.gety() - y;
    double distance = std::sqrt(dx * dx + dy * dy);
    double sum_r = r + p.getr();
    
    // Oblicz overlap
    double overlap = sum_r - distance;
    
    // Normalizujemy wektor różnicy położeń
    double nx = dx / distance; // znormalizowana różnica w osi X
    double ny = dy / distance; // znormalizowana różnica w osi Y
    
    // Przesuwamy cząstki o połowę nakładającej się odległości każdą
    double move_x = overlap * nx / 2;
    double move_y = overlap * ny / 2;

    // Aktualizujemy pozycje cząstek
    double new_x1 = x - move_x;
    double new_y1 = y - move_y;
    double new_x2 = p.getx() + move_x;
    double new_y2 = p.gety() + move_y;

    // Korekta pozycji, gdy cząstki zbliżają się do krawędzi

    // Krawędzie dla pierwszej cząstki
    bool adjusted1 = false;  // Zmienna, by zaznaczyć, czy dokonano korekty
    if (new_x1 < r) {
        new_x1 = r;
        adjusted1 = true;
    } 
    if (new_x1 > l - r) {
        new_x1 = l - r;
        adjusted1 = true;
    }
    if (new_y1 < r) {
        new_y1 = r;
        adjusted1 = true;
    } 
    if (new_y1 > l - r) {
        new_y1 = l - r;
        adjusted1 = true;
    }

    // Korekta pozycji drugiej cząstki w przypadku, gdy pierwsza została skorygowana
    if (adjusted1) {
        // Jeśli pierwsza cząstka została przesunięta do krawędzi, przesuń drugą dalej
        new_x2 = p.getx() + (overlap + std::abs(new_x1 - x)) * nx;
        new_y2 = p.gety() + (overlap + std::abs(new_y1 - y)) * ny;
    }

    // Krawędzie dla drugiej cząstki
    bool adjusted2 = false;  // Zmienna, by zaznaczyć, czy dokonano korekty
    if (new_x2 < r) {
        new_x2 = r;
        adjusted2 = true;
    }
    if (new_x2 > l - r) {
        new_x2 = l - r;
        adjusted2 = true;
    }
    if (new_y2 < r) {
        new_y2 = r;
        adjusted2 = true;
    }
    if (new_y2 > l - r) {
        new_y2 = l - r;
        adjusted2 = true;
    }

    // Dodatkowe sprawdzenie, jeśli obie cząstki były skorygowane
    if (adjusted1 && adjusted2) {
        // Oblicz, ile trzeba jeszcze przesunąć, aby nie było overlapu
        double additional_overlap = sum_r - std::sqrt((new_x2 - new_x1) * (new_x2 - new_x1) + 
                                                    (new_y2 - new_y1) * (new_y2 - new_y1));

        // Przesunięcie cząstek o dodatkową ilość w przeciwnych kierunkach
        // wzdłuż wektora normalnego
        double adjust_x = additional_overlap * nx / 2;
        double adjust_y = additional_overlap * ny / 2;

        // Przesunięcie cząstek - upewnienie się, że nie przekroczą granic
        if (new_x1 - adjust_x >= r) new_x1 -= adjust_x; 
        if (new_y1 - adjust_y >= r) new_y1 -= adjust_y;
        if (new_x2 + adjust_x <= l - r) new_x2 += adjust_x;
        if (new_y2 + adjust_y <= l - r) new_y2 += adjust_y;
    }

    // Ustawianie nowych pozycji cząstek
    this->setPosition(new_x1, new_y1);
    p.setPosition(new_x2, new_y2);
}


std::string particle::toString() const {
    std::ostringstream oss;
    oss << x << "," << y << "," << v_x << "," << v_y;
    return oss.str();
}

particles::particles(int n, double l, double v, double r) : l(l) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis_pos(0.0 + r, l - r);
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
                P[idx1].separate(P[idx2], l);
            }
        }
    }
}