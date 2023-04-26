#ifndef PLANET_H
#define PLANET_H

constexpr double G{ 6.67428e-11 };

class Planet
{
private:
	double m{};
	int r{};
	double a{};
	double M{};
	double e{};
	double n{};
	Planet* parent{};
public:
	Planet(Planet*, double, int, double, double);
	void move(double);
	void draw() const;
	Planet* getParent() const;
};

#endif