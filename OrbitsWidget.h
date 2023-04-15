#ifndef WIDGET_H
#define WIDGET_H

#include <string>

#include <SDL.h>

class Widget
{
private:
	std::string text{};
	void* data{};
public:
	virtual void event(SDL_Event*) = 0;
	virtual void draw(SDL_Renderer*) const = 0;
};

#endif