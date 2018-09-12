#include <iostream>
#include "olcPixelGameEngine.h"
#define WIDTH 50
#define HEIGHT 50
#define PWIDTH 16
#define PHEIGHT 16

class GameOfLife : public olc::PixelGameEngine
{
public:
	GameOfLife()
	{
		sAppName = "GameOfLife";
	}

public:
	class Cell
	{
	public:
		bool live = false;
		olc::Pixel color = olc::WHITE;

		void Update(int counter)
		{
			if (live)
			{
				if (counter == 2 || counter == 3)
				{
					Update(true);
				}
				else
				{
					Update(false);
				}
			}
			else
			{
				if (counter == 3)
				{
					Update(true);
				}
			}
		}

		void Update(bool mLive)
		{
			if (mLive)
			{
				live = true;
				color = olc::BLACK;
			}
			else
			{
				live = false;
				color = olc::WHITE;
			}
		}
	};
	enum Stage { preparation, running, paused};
	
	Cell grid[WIDTH + 2][HEIGHT + 2];
	Stage stage;
	
	bool OnUserCreate() override
	{
		stage = preparation;
		// Clear the screen
		for (int x = 0; x < WIDTH + 2; x++)
		{
			for (int y = 0; y < HEIGHT + 2; y++)
			{
				grid[x][y].Update(false);
			}
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// User input
		if (stage == preparation)
		{
			if (GetMouse(0).bPressed || GetMouse(0).bHeld)
				if (!(GetMouseX() < 0 || GetMouseY() < 0 || GetMouseX() > WIDTH || GetMouseY() > HEIGHT))
					grid[GetMouseX()][GetMouseY()].Update(true);
			if (GetMouse(1).bPressed || GetMouse(1).bHeld)
				if (!(GetMouseX() < 0 || GetMouseY() < 0 || GetMouseX() > WIDTH || GetMouseY() > HEIGHT))
					grid[GetMouseX()][GetMouseY()].Update(false);
			if (GetKey(olc::SPACE).bPressed)
			{
				stage = running;
			}
		}

		if (stage == running)
		{
			// Count the neighbours
			for (int x = 1; x < WIDTH; x++)
			{
				for (int y = 1; y < HEIGHT; y++)
				{
					int counter = 0;
					if (y > 0 && y < HEIGHT + 1 && x > 0 && x < WIDTH + 1)
					{
						if (grid[x - 1][y - 1].live) counter++;
						if (grid[  x  ][y - 1].live) counter++;
						if (grid[x + 1][y - 1].live) counter++;
						if (grid[x - 1][  y  ].live) counter++;
						if (grid[x + 1][  y  ].live) counter++;
						if (grid[x - 1][y + 1].live) counter++;
						if (grid[  x  ][y + 1].live) counter++;
						if (grid[x + 1][y + 1].live) counter++;
						if (counter != 0)
						{
							std::cout << counter << "\n";
						}
						grid[x][y].Update(counter);
							
					}
				}
			}
		}
		// Rendering
		for (int x = 0; x < WIDTH; x++)
		{
			for (int y = 0; y < HEIGHT; y++)
			{
				Draw(x, y, grid[x][y].color);
			}
		}
		
		// Blue thingy
		if (stage == preparation)
			Draw(GetMouseX(), GetMouseY(), olc::Pixel(0, 0, 255, 50));

		/*if(stage == running)
			Sleep(500);*/
		return true;
	}
};

int main(void)
{
	GameOfLife gof;
	if (gof.Construct(WIDTH, HEIGHT, PWIDTH, PHEIGHT))
	{
		gof.Start();
	}
	return 0;
}