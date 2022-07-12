#pragma once
#include "../../../dependencies/utilities/csgo.hpp"
#include "../../menu/variables.hpp"

namespace visuals
{
	void esp();
	void boxes(player_t* entity);
}

struct box
{
	int x, y, w, h;
	box() = default;
	box(const int x, const int y, const int w, const int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};