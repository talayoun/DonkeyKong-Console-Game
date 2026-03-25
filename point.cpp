#include "point.h"

void Point::draw(char c, int color_choice, int choice) const  //Draws point
{
	if (!pBoard->getIsSilent())
	{
		if (choice == NO_COLOR) //Draws without color
			draw(c);
		else if (choice == COLOR)  //Draws with color
		{
			GameConfig g;
			static bool toggle = false; //For flashing
			if (c == (char)GameConfig::eKeys::MARIO_WITH_HAMMER) //If the char is '%' so mario is with the hammer
			{
				toggle = !toggle;
				if (toggle)
					g.setColor(g.GREEN);
				else
					g.setColor(g.BASIC);
			}
			else
				g.setColor(color_choice);
			draw(c);
			g.setColor(g.BASIC);
		}
	}
}
