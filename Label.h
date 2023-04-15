#ifndef LABEL_H
#define LABEL_H

#include <SDL.h>

#include "OrbitsWidget.h"

class Label : public Widget
{
private:
public:
	void handleEvent(SDL_Event*) override;
	void draw(SDL_Renderer*) const override;
};

#endif