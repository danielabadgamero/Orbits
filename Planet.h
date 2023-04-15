#ifndef PLANET_H
#define PLANET_H

#include <vector>

#include <SDL.h>

constexpr double G{ 6.67428e-11 };

class Planet
{
private:
	double mass{};
	double radius{};
	SDL_FPoint pos{};
	SDL_FPoint vel{};
	SDL_Color color{};
public:
	Planet(double, double, double, double, SDL_Color);
	void move(double, std::vector<Planet>&);
	void draw(SDL_Renderer*, SDL_Texture*, int);
};

#endif