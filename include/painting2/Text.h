#pragma once

#include "painting2/Textbox.h"

#include <string>

namespace pt2
{

class Text
{
public:
	Textbox tb;

	std::string text;

	Text() : text("text") {}

}; // Text

}