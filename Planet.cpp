#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "OrbitsCore.h"
#include "Planet.h"
#include "Camera.h"

static double distance(SDL_FPoint A, SDL_FPoint B)
{
	return std::sqrt(std::pow(A.x - B.x, 2) + std::pow(A.y - B.y, 2));
}

static void drawInfo(std::string info, SDL_Rect& rect)
{
	TTF_Font* font{ TTF_OpenFont("C:\\Windows\\Fonts\\AGENCYR.TTF", 32) };

	SDL_Surface* surface{ TTF_RenderText_Blended(font, info.c_str(), {0xf0, 0xf0, 0xf0, 0xff})};
	SDL_Texture* texture{ SDL_CreateTextureFromSurface(Orbits::renderer, surface) };
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	SDL_RenderCopy(Orbits::renderer, texture, NULL, &rect);
	rect.y += rect.h + 0;

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	TTF_CloseFont(font);
}

Planet::Planet(Planet* parentPlanet, double mass, int radius, double semiMajor, double eccentricity, std::string name)
	: m{ mass }, r{ radius }, a{ semiMajor }, e{ eccentricity }, name{ name }
{
	parent = parentPlanet;
	if (!parent) return;
	double T{ 2 * M_PI * sqrt(pow(a, 3) / (G * parent->m)) };
	n = 2 * M_PI / T;
}

void Planet::move(double dt)
{
	if (!parent)
		return;

	M += dt * n;
	double E{ M };
	while (true)
	{
		const double dE{ (E - e * sin(E) - M) / (1 - e * cos(E)) };
		E -= dE;
		if (abs(dE) < 1e-6) break;
	}

	pos.x = static_cast<float>(a * (cos(E) - e)) + parent->pos.x;
	pos.y = static_cast<float>(a * sin(E) * sqrt(1 - pow(e, 2))) + parent->pos.y;
}

void Planet::draw(SDL_Surface* surface, SDL_FRect viewport)
{
	if (!texture)
	{
		texture = SDL_CreateTextureFromSurface(Orbits::renderer, surface);
		int totalR{};
		int totalG{};
		int totalB{};
		for (int y{}; y != surface->h; y++)
			for (int x{}; x != surface->w; x++)
			{
				totalR += ((int*)surface->pixels)[y * surface->w + x] & surface->format->Rmask;
				totalG += (((int*)surface->pixels)[y * surface->w + x] & surface->format->Gmask) >> 8;
				totalB += (((int*)surface->pixels)[y * surface->w + x] & surface->format->Bmask) >> 16;
			}
		totalR /= surface->h * surface->w;
		totalG /= surface->h * surface->w;
		totalB /= surface->h * surface->w;
		groundColor = { static_cast<Uint8>(totalR), static_cast<Uint8>(totalG), static_cast<Uint8>(totalB), 0xff };
	}
	if (viewport.w < 100)
	{
		SDL_FPoint cameraPos{ viewport.x, viewport.y };
		double height{ distance(cameraPos, pos) - r };
		if (height < viewport.h / 2)
		{
			SDL_Rect rect{ 0, 0, Orbits::monitor.w, Orbits::monitor.h };
			SDL_SetRenderDrawColor(Orbits::renderer, groundColor.r, groundColor.g, groundColor.b, 0xff);
			SDL_RenderFillRect(Orbits::renderer, &rect);
		}
	}
	else
	{
		SDL_FRect rect
		{
			(pos.x - viewport.x + viewport.w / 2.0f) / viewport.w * Orbits::monitor.w,
			(pos.y - viewport.y + viewport.h / 2.0f) / viewport.h * Orbits::monitor.h,
			std::clamp(r / viewport.w * Orbits::monitor.w, 10.0f, FLT_MAX),
			std::clamp(r / viewport.h * Orbits::monitor.h, 10.0f, FLT_MAX),
		};
		if (name == "saturn")
			rect.w *= 2.2f, rect.h *= 2.2f;
		rect.x -= rect.w / 2;
		rect.y -= rect.h / 2;

		SDL_RenderCopyF(Orbits::renderer, texture, NULL, &rect);
	}

	if (Orbits::camera.getFocus() == Orbits::index(name))
	{
		SDL_Rect textRect{ 10, 10 };
		
		std::string capitalizedName{ name };
		capitalizedName[0] -= 0x20;
		drawInfo(capitalizedName, textRect);
		drawInfo("Mass: " + std::to_string(m), textRect);
		drawInfo("Radius: " + std::to_string(r), textRect);
		drawInfo("Semi-major axis: " + std::to_string(a), textRect);
		drawInfo("Eccentricity: " + std::to_string(e), textRect);
	}
}

Planet* Planet::getParent()
{
	return parent;
}

SDL_FPoint Planet::getPos()
{
	return pos;
}