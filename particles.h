#ifndef PARTICLES_H
#define PARTICLES_H

#include <vector>
#include <string>

// definicja klasy opisującej pojedynczą cząstkę
class particle {
    double r;
    double x;
    double y;
    double v_x;
    double v_y;
public:
    particle(double r, double x, double y, double v_x, double v_y);
    double getr() const;
    double getx() const;
    double gety() const;
    double getv_x() const;
    double getv_y() const;
    void setPosition(double new_x, double new_y);
    void setVelocity(double new_vx, double new_vy);
    void collision(particle& p);
    void separate(particle& p, double l);
    std::string toString() const;
};


// definicja klasy opisującej cały układ wielu cząstek
class particles {
    std::vector<particle> P;
    std::vector<std::pair<double, int>> xSorted; // Para (pozycja na osi x, indeks)
    double l; // długość pojemnika z gazem
public:
    particles(int n, double l, double v, double r);
    std::string toString() const;
    void saveToFile(const std::string& filename) const;
    void update(double dt, double& KineticEnergy, double& Momentum_x, double& Momentum_y);
};

#endif