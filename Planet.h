#ifndef PLANET_H
#define PLANET_H

#include <string>

#include <SDL.h>

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
	SDL_FPoint pos{};
	Planet* parent{};
	SDL_Texture* texture{};
	SDL_Color groundColor{};
	std::string name{};
public:
	Planet(Planet*, double, int, double, double, std::string);
	void move(double);
	void draw(SDL_Surface*, SDL_FRect);
	void drawSurface(SDL_FRect);
	SDL_FPoint getPos();
	Planet* getParent();
};

#endif