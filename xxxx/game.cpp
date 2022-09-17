#include <Windows.h>
#include <iostream>
#include "Framework.h"

#define BLOCKS_COUNT 20

/* Test Framework realization */
class MyFramework : public Framework {

public:
	Sprite* Mouse;
	Sprite* Platform;
	Sprite* Ball;
	Sprite* Block;
	
	Sprite* blocks[BLOCKS_COUNT];

	bool isMouseClicked = false;

	int X = 10;

	int w = 0; //width
	int h = 0; //height

	int sX;
	int sY;

	int pX = 0, pY = 0;

	int bX = 0, bY = 0;

	int dirX = 0;
	int dirY = 0;

	double a;

	int tempBX = 0;
	int tempBY = 0;
	
	int leftEdgePlatform;
	int rightEdgePlatform;




	void moveBall()
	{
		if (bX <= 0 || bX >= w)
		{
			X *= -1;
		}
			
		if (bY <= 0 /*|| (bY >=  pY && bX >= leftEdgePlatform && bX <= rightEdgePlatform)*/)
		{
			a *= -1;
		}
		if (bX >= leftEdgePlatform && bX <= rightEdgePlatform)
		{
			if (bY >= pY && bY <= pY +25)
			{
				a *= -1;
			}

		}
		if (dirX >= -25 && dirX <= w / 2 - 25 && dirY >= -25 && dirY <= h / 2 - 25)
		{
			bX -= X;
			bY -= a*10;
		}

		if (dirX > w / 2 - 25 && dirX < w - 25 && dirY > -25 && dirY < h / 2 - 25)
		{
			bX += X;
			bY += 10*a;
		}

	}

	void drawBlocks()
	{
		int w = 5;
		int h = 5;
		for (int i = 0; i < BLOCKS_COUNT; i++)
		{
			if (i % 5 == 0 || i==0)
			{
				drawSprite(blocks[i], w, h*i);
				h = h + 50;
				w = 5;
			}
			drawSprite(blocks[i], w * i, h);
			w = w + 150;
		}
	}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = 800;
		height = 600;
		fullscreen = false;
	}

	virtual bool Init() {
		getScreenSize(w, h);

		pX = (w / 2) - 75;
		pY = (h/2) + (h/4);

		bX = (w / 2) - 10;
		bY = (h/2)+h/5;

		leftEdgePlatform = w / 2 - 75;
		rightEdgePlatform = w / 2 + 75;

		Mouse = createSprite("C:/Users/user/source/repos/xxxx/xxxx/data/59-Breakout-Tiles.png");
		Platform = createSprite("C:/Users/user/source/repos/xxxx/xxxx/data/50-Breakout-Tiles.png");
		Ball = createSprite("C:/Users/user/source/repos/xxxx/xxxx/data/62-Breakout-Tiles.png");
		Block = createSprite("C:/Users/user/source/repos/xxxx/xxxx/data/01-Breakout-Tiles.png");
		for (int i = 0; i < BLOCKS_COUNT; i++)
		{
			blocks[i] = Block;
		}
		setSpriteSize(Ball, 20, 20);
		setSpriteSize(Platform, 150, 40);
		setSpriteSize(Mouse, 50, 50);
		setSpriteSize(Block, 150, 50);
		showCursor(false);
		onMouseMove(0, 0, 0, 0);
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		Sleep(20);
		drawSprite(Mouse, sX, sY);
		drawSprite(Platform, pX, pY);
		drawSprite(Ball, bX, bY);
		drawBlocks();
		if (isMouseClicked) {
			moveBall();
		}
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		sX = x - 25;
		sY = y - 25;
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::LEFT && !isMouseClicked)
		{
			tempBX = bX;
			tempBY = bY;

			dirX = sX;
			dirY = sY;


			a = (float)(dirY - bY) / (float)(dirX - bX);
		


			isMouseClicked = true;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::LEFT)
		{
			leftEdgePlatform -= 20;
			rightEdgePlatform -= 20;
			pX-=20;
		}
		if (k == FRKey::RIGHT)
		{
			leftEdgePlatform += 20;
			rightEdgePlatform += 20;
			pX+=20;
		}
	}

	virtual void onKeyReleased(FRKey k) {
	}
	
	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char *argv[])
{
	return run(new MyFramework);
}
