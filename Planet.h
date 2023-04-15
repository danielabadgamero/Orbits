#ifndef PLANET_H
#define PLANET_H

#include <vector>

#include <SDL.h>

constexpr double G{ 6.67428e-11 };

class Planet
{
private:
	Planet* parent{};
	SDL_FPoint pos{};
	SDL_FPoint vel{};
	double mass{};
	SDL_Color color{};
public:
	Planet(Planet*, double, double, double, SDL_Color);
	void move(double, std::vector<Planet>);
	void draw(SDL_Renderer*);
};

#endif