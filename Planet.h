#ifndef PLANET_H
#define PLANET_H

#include <SDL.h>

constexpr double G{ 6.67428e-11 };

class Planet
{
private:
	double m{};
	int r{};
	double a{};
	double T{};
	double epoch{};
	SDL_FPoint pos{};
	SDL_Color color{};
	Planet* parent{};
public:
	Planet(Planet*, double, int, double, SDL_Color);
	void move(double);
	SDL_Point getPos(double);
	void draw(SDL_Renderer*, SDL_Texture*, double, SDL_Point);
};

#endif