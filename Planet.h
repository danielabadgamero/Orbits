#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include <vector>

#include <SDL.h>

constexpr double G{ 6.67428e-11 };

class Planet
{
private:
	double mass{};
	SDL_Rect rect{};
	SDL_FPoint pos{};
	SDL_FPoint vel{};
	SDL_Color color{};
	std::vector<SDL_FPoint> places{};
public:
	Planet(double, SDL_FPoint, SDL_FPoint, SDL_Color);
	void move(double, std::vector<Planet>&, SDL_DisplayMode*);
	SDL_Point getPos(double);
	void draw(SDL_Renderer*, SDL_Texture*);
};

#endif