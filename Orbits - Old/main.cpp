#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <filesystem>
#include <fstream>
#include <string>

#define _USE_MATH_DEFINES

enum Body
{
	sun,
	mercury,
	venus,
	earth,
	moon,
	mars,
	phobos,
	deimos,
	jupiter,
	saturn,
	uranus,
	neptune,
	total,
};

struct Dimension
{
	int w{};
	int h{};
};

struct SDL_PointLD
{
	long double x{};
	long double y{};
};

struct SDL_RectLD
{
	long double x{};
	long double y{};
	long double w{};
	long double h{};
};

namespace data
{
	long double scale{ 1.436080076e-7 };
	constexpr long double G{ 6.67e-11 };
	long long warpFactor{ 1 };
}

class Camera
{
private:
	SDL_RectLD m_viewport{};
	Body m_focus{ total };

public:
	void setFocus(Body focus)
	{
		m_focus = focus;
	}

	SDL_RectLD getViewport()
	{
		return m_viewport;
	}

	Body getFocus()
	{
		return m_focus;
	}

	void nextFocus()
	{
		if (m_focus < total)
			m_focus = static_cast<Body>(m_focus + 1);
		else
			m_focus = static_cast<Body>(0);
	}

	void previousFocus()
	{
		if (m_focus > 0)
			m_focus = static_cast<Body>(m_focus - 1);
		else
			m_focus = total;
	}

	void setViewport(SDL_RectLD viewport)
	{
		m_viewport = viewport;
	}

	void zoom(int y)
	{
		if (y > 0)
			data::scale *= 1.1;
		else
			data::scale /= 1.1;
	}

	void move(int x, int y)
	{
		if (m_focus == total)
		{
			m_viewport.x += x / data::scale * 2.0;
			m_viewport.y += y / data::scale * 2.0;
		}
	}

	Camera(Dimension windowSize)
	{
		m_viewport = { -windowSize.w / data::scale, -windowSize.h / data::scale, windowSize.w / data::scale, windowSize.h / data::scale };
	}
};

class Sun
{
private:
	SDL_Texture* m_texture{};
	SDL_Rect m_textureRect{};
	SDL_PointLD m_centerPoint{};
	SDL_Color m_color{};

	const long double m_radius{ 696.34e6 };
	const long double m_mass{ 1.989e30 };

public:
	SDL_PointLD getCenterPoint()
	{
		return m_centerPoint;
	}

	long double getMass()
	{
		return m_mass;
	}

	void draw(SDL_Renderer* renderer, Dimension screen, Camera* camera)
	{
		m_textureRect.w = static_cast<int>(m_radius * data::scale) * 2 + 20;
		m_textureRect.h = static_cast<int>(m_radius * data::scale) * 2 + 20;
		m_textureRect.x = static_cast<int>((camera->getViewport().x + camera->getViewport().w) / 2.0 * data::scale + screen.w / 2.0) - m_textureRect.w / 2;
		m_textureRect.y = static_cast<int>((camera->getViewport().y + camera->getViewport().h) / 2.0 * data::scale + screen.h / 2.0) - m_textureRect.h / 2;

		SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);
		SDL_RenderCopy(renderer, m_texture, NULL, &m_textureRect);
	}

	Sun(SDL_Texture* texture)
	{
		m_color = { 0xF9, 0xD7, 0x1C };
		m_texture = texture;
		m_centerPoint = { 0, 0 };
	}
};

class Planet
{
private:
	SDL_Texture* m_texture{};
	SDL_Rect m_textureRect{};
	SDL_PointLD m_centerPoint{};
	SDL_Color m_color{};
	std::vector<SDL_PointLD> m_orbit{};
	Body m_body{};
	
	long double m_radius{};
	long double m_mass{};
	long double m_distanceSun{};
	long double m_speedX{};
	long double m_speedY{};
	double m_accel{};
	double m_accelX{};
	double m_accelY{};
	double m_deg{};
	double m_prevDeg{};
	double m_timeMinutes{};

public:
	long double getMass()
	{
		return m_mass;
	}

	SDL_PointLD getCenterPoint()
	{
		return m_centerPoint;
	}

	void calculatePosition(Sun* sun)
	{
		m_speedX += m_accelX * 60;
		m_speedY += m_accelY * 60;

		m_centerPoint.x += m_speedX * 60;
		m_centerPoint.y += m_speedY * 60;

		m_distanceSun = sqrtl(powl(m_centerPoint.x, 2) + powl(m_centerPoint.y, 2));
		m_accel = -data::G * sun->getMass() / pow(m_distanceSun, 2);

		m_prevDeg = m_deg;

		if (m_centerPoint.y >= 0)
			m_deg = acosl(m_centerPoint.x / m_distanceSun);
		else
			m_deg = 2.0 * M_PI - acosl(m_centerPoint.x / m_distanceSun);

		m_accelX = m_accel * cos(m_deg);
		m_accelY = m_accel * sin(m_deg);
	}

	void draw(SDL_Renderer* renderer, Dimension screen, Camera* camera, double incTime)
	{
		m_timeMinutes += incTime / 10;

		if (m_timeMinutes >= m_orbit.size())
			m_timeMinutes = 0;

		m_centerPoint = { m_orbit[static_cast<int>(m_timeMinutes) % m_orbit.size()].x, m_orbit[static_cast<int>(m_timeMinutes) % m_orbit.size()].y };

		m_textureRect.w = static_cast<int>(m_radius * data::scale) * 2 + 10;
		m_textureRect.h = static_cast<int>(m_radius * data::scale) * 2 + 10;
		m_textureRect.x = static_cast<int>((((camera->getViewport().x + camera->getViewport().w) / 2.0) + m_centerPoint.x) * data::scale + screen.w / 2.0) - m_textureRect.w / 2;
		m_textureRect.y = static_cast<int>((((camera->getViewport().y + camera->getViewport().h) / 2.0) - m_centerPoint.y) * data::scale + screen.h / 2.0) - m_textureRect.h / 2;

		if (camera->getFocus() == m_body)
			camera->setViewport({ -m_centerPoint.x * 2.0 + camera->getViewport().w, m_centerPoint.y * 2.0 - camera->getViewport().h });

		SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);
		SDL_RenderCopy(renderer, m_texture, NULL, &m_textureRect);
	}

	void calculateOrbit(Sun* sun)
	{
		std::cout << "Calculating: " << m_body << '\n';
		while (m_deg >= m_prevDeg)
		{
			m_orbit.push_back(m_centerPoint);
			calculatePosition(sun);
		}
		std::cout << "Done: " << m_body << " with " << m_orbit.size() << '\n';
	}

	Planet(SDL_Texture* texture, long double radius, long double mass, long double speed, long double distance, SDL_Color color, Camera* camera, Sun* sun, Body body)
	{
		m_body = body;
		m_radius = radius;
		m_mass = mass;
		m_speedY = speed;
		m_distanceSun = distance;
		m_texture = texture;
		m_color = color;
		m_centerPoint = { m_distanceSun * cos(m_deg * M_PI / 180), -m_distanceSun * sin(m_deg * M_PI / 180) };

		calculateOrbit(sun);
	}
};

class Satellite
{
private:
	SDL_Texture* m_texture{};
	SDL_Rect m_textureRect{};
	SDL_PointLD m_centerPoint{};
	SDL_PointLD m_relPoint{};
	SDL_Color m_color{};
	std::vector<SDL_PointLD> m_orbit{};
	Planet* m_planet{};
	Body m_body{};

	long double m_radius{};
	long double m_mass{};
	long double m_distancePlanet{};
	long double m_speedX{};
	long double m_speedY{};
	double m_accel{};
	double m_accelX{};
	double m_accelY{};
	double m_deg{};
	double m_prevDeg{};
	double m_timeSeconds{};

public:
	long double getMass()
	{
		return m_mass;
	}

	SDL_PointLD getCenterPoint()
	{
		return m_centerPoint;
	}

	void calculatePosition()
	{
		m_speedX += m_accelX * 1;
		m_speedY += m_accelY * 1;

		m_relPoint.x += m_speedX * 1;
		m_relPoint.y += m_speedY * 1;

		m_distancePlanet = sqrtl(powl(m_relPoint.x, 2) + powl(m_relPoint.y, 2));
		m_accel = -data::G * m_planet->getMass() / pow(m_distancePlanet, 2);

		m_prevDeg = m_deg;

		if (m_relPoint.y > 0)
			m_deg = acosl(m_relPoint.x / m_distancePlanet);
		else
			m_deg = 2 * M_PI - acosl(m_relPoint.x / m_distancePlanet);
		
		m_accelX = m_accel * cos(m_deg);
		m_accelY = m_accel * sin(m_deg);
	}

	void draw(SDL_Renderer* renderer, Dimension screen, Camera* camera, double incTime)
	{
		m_timeSeconds += incTime / 1;

		if (m_timeSeconds >= m_orbit.size())
			m_timeSeconds = 0;

		m_relPoint = { m_orbit[static_cast<int>(m_timeSeconds) % m_orbit.size()].x, m_orbit[static_cast<int>(m_timeSeconds) % m_orbit.size()].y };

		m_centerPoint.x = m_relPoint.x + m_planet->getCenterPoint().x;
		m_centerPoint.y = m_relPoint.y + m_planet->getCenterPoint().y;

		m_textureRect.w = static_cast<int>(m_radius * data::scale) * 2 + 5;
		m_textureRect.h = static_cast<int>(m_radius * data::scale) * 2 + 5;
		m_textureRect.x = static_cast<int>((((camera->getViewport().x + camera->getViewport().w) / 2.0) + m_centerPoint.x) * data::scale + screen.w / 2.0) - m_textureRect.w / 2;
		m_textureRect.y = static_cast<int>((((camera->getViewport().y + camera->getViewport().h) / 2.0) - m_centerPoint.y) * data::scale + screen.h / 2.0) - m_textureRect.h / 2;

		if (camera->getFocus() == m_body)
			camera->setViewport({ -m_centerPoint.x * 2.0 + camera->getViewport().w, m_centerPoint.y * 2.0 - camera->getViewport().h });

		SDL_SetTextureColorMod(m_texture, m_color.r, m_color.g, m_color.b);
		SDL_RenderCopy(renderer, m_texture, NULL, &m_textureRect);
	}

	void calculateOrbit()
	{
		while (m_deg >= m_prevDeg)
		{
			m_orbit.push_back(m_relPoint);
			calculatePosition();
		}
	}

	Satellite(SDL_Texture* texture, long double radius, long double mass, long double speed, long double distance, SDL_Color color, Camera* camera, Planet* planet, Body body)
	{
		m_body = body;
		m_radius = radius;
		m_mass = mass;
		m_speedY = speed;
		m_distancePlanet = distance;
		m_texture = texture;
		m_color = color;
		m_relPoint = { m_distancePlanet * cos(m_deg * M_PI / 180), m_distancePlanet * sin(m_deg * M_PI / 180) };

		m_planet = planet;

		calculateOrbit();
	}
};

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();

	SDL_Window* window{ SDL_CreateWindow("Orbits 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 860, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE) };
	SDL_Renderer* renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) };
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

	SDL_Surface* ballSurface{ IMG_Load("ball.png") };
	SDL_Texture* ballBlueprint{ SDL_CreateTextureFromSurface(renderer, ballSurface) };
	SDL_FreeSurface(ballSurface);

	SDL_Event e{};

	bool isRunning{ true };
	bool mouseClicked{};

	Dimension windowSize{};
	SDL_GetWindowSize(window, &windowSize.w, &windowSize.h);

	Camera camera{ Camera(windowSize) };

	Sun sun{ Sun(ballBlueprint) };
	Planet mercury(ballBlueprint, 2440.5, 0.33010e24, 38.86e3, 69.818e9, {0xE5, 0xE5, 0xE5 }, &camera, &sun, Body::mercury);
	Planet venus(ballBlueprint, 6051.8e3, 4.8673e24, 34.79e3, 108.941e9, {0xE3, 0x9E, 0x1C }, &camera, &sun, Body::venus);
	Planet earth(ballBlueprint, 6371.0e3, 5.9722e24, 29.29e3, 152.1e9, {0x60, 0x60, 0xF0 }, &camera, &sun, Body::earth);
	Satellite moon(ballBlueprint, 1737.4e3, 0.07346e24, 0.97e3, 0.4055e9, {0xAA, 0xAA, 0xAA}, &camera, &earth, Body::moon);
	Planet mars(ballBlueprint, 3389.5e3, 0.64169e24, 21.97e3, 249.261e9, { 0xA5, 0x30, 0x30 }, &camera, &sun, Body::mars);
	Satellite phobos(ballBlueprint, 11.2667e3, 1.0659e16, 2138, 9517.58e3, { 0xA3, 0x64, 0x4B }, &camera, &mars, Body::phobos);
	Satellite deimos(ballBlueprint, 6.2e3, 1.4762e15, 1351.3, 23470.9e3, { 0x9B, 0xB0, 0xAC}, &camera, &mars, Body::deimos);
	Planet jupiter(ballBlueprint, 69911e3, 1898.13e24, 12.44e3, 818.636e9, { 0x8C, 0x4F, 0x41 }, &camera, &sun, Body::jupiter);
	Planet saturn(ballBlueprint, 58232e3, 568.32e24, 9.09e3, 1506.527e9, { 0xEA, 0xD6, 0xB8 }, &camera, &sun, Body::saturn);
	Planet uranus(ballBlueprint, 25362e3, 86.811e24, 6.49e3, 3001.390e9, { 0xD1, 0xE7, 0xE7 }, &camera, &sun, Body::uranus);
	Planet neptune(ballBlueprint, 24622e3, 102.409e24, 5.37e3, 4558.857e9, { 0x3F, 0x54, 0xBA }, &camera, &sun, Body::neptune);

	double previousTime{};
	double currentTime{};
	double deltaTime{};

	while (isRunning)
	{
		previousTime = currentTime;
		currentTime = SDL_GetTicks64() / 1000.0;
		deltaTime = currentTime - previousTime;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				isRunning = false;
			if (e.type == SDL_MOUSEBUTTONDOWN)
				mouseClicked = true;
			if (e.type == SDL_MOUSEBUTTONUP)
				mouseClicked = false;
			if (e.type == SDL_MOUSEMOTION)
				if (mouseClicked)
					camera.move(e.motion.xrel, e.motion.yrel);
			if (e.type == SDL_MOUSEWHEEL)
				camera.zoom(e.wheel.y);
			if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					camera.nextFocus();
					break;
				case SDLK_DOWN:
					camera.previousFocus();
					break;
				case SDLK_ESCAPE:
					camera.setFocus(total);
					break;
				case SDLK_PERIOD:
					if (data::warpFactor < 1000000000)
						data::warpFactor *= 10;
					break;
				case SDLK_COMMA:
					if (data::warpFactor > 1)
						data::warpFactor /= 10;
					break;
				}
		}

		if (camera.getFocus() == Body::sun)
			camera.setViewport({ 0, 0 });

		SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x20, 0xFF);
		SDL_RenderClear(renderer);

		sun.draw(renderer, windowSize, &camera);
		mercury.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		venus.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		earth.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		moon.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		mars.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		phobos.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		deimos.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		jupiter.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		saturn.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		uranus.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);
		neptune.draw(renderer, windowSize, &camera, deltaTime * data::warpFactor);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(ballBlueprint);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();

	return 0;
}