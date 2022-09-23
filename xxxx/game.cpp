#include <Windows.h>
#include <iostream>
#include "Framework.h"

#define BLOCKS_COUNT 10



/* Test Framework realization */
class MyFramework : public Framework {

public:
	Sprite* Mouse;
	Sprite* Platform;
	Sprite* Ball;

	Sprite* Block[BLOCKS_COUNT];
	Sprite* BlockTask[13];
	Sprite* points;
	Sprite* ability[5];
	bool isMouseClicked = false;

	int X = 10;// a->x + b - linear function

	int w = 0; //width
	int h = 0; //height

	int sX; //mouse
	int sY;

	int pX = 0, pY = 0; //platform x y

	int bX = 0, bY = 0; // ball x y

	int dirX = 0;
	int dirY = 0;

	double a; //->ax +b - linear function

	int tempBX = 0;
	int tempBY = 0;

	int leftEdgePlatform;
	int rightEdgePlatform;

	int blockXmin[BLOCKS_COUNT+2];
	int blockXmax[BLOCKS_COUNT+2];
	int blockYmin[BLOCKS_COUNT+2];
	int blockYmax[BLOCKS_COUNT+2];

	int p = 0;
	int down = 0;
	int t = 0;
	// for task left-right
	int delta = 1;
	int count = 0;
	int max = 10;
	int min = 0;
	//
	int lr = 0;

	int aX = -100; //ability x y
	int aY = -100;

	int abilitynext=0;

	void moveBall()
	{
		if (bX <= 0 || bX >= w)
		{
			X *= -1;
		}

		if (bY <= 0)
		{
			a *= -1;
		}
		if (bX >= leftEdgePlatform && bX <= rightEdgePlatform)
		{
			if (bY >= pY && bY <= pY + 10)
			{
				a *= -1;
			}

		}
		if (dirX >= -25 && dirX <= w / 2 - 25 && dirY >= -25 && dirY <= h / 2 - 25)
		{
			bX -= X;
			bY -= a * 10;
		}

		if (dirX > w / 2 - 25 && dirX < w - 25 && dirY > -25 && dirY < h / 2 - 25)
		{
			bX += X;
			bY += 10 * a;
		}
	}

	bool blc[12];//block is available to crush if false - yes if true - not
	void blcFF()
	{
		for (int i = 0; i < 12; i++)
		{
			blc[i] = false;
		}
	}

	int blockint = 12; //blocks

	void collisionT()
	{
		for (int i = 10; i < 12; i++)
		{
			if (blc[i] == false) {

				if (bY <= blockYmax[i] + 10 && bY >= blockYmin[i] - 10 && bX >= blockXmin[i] + 10 && bX <= blockXmax[i] - 10)
				{
					a *= -1;
					destroySprite(BlockTask[i]);
					blc[i] = true;
					blockint--;
				}
				if (bY <= blockYmax[i] && bY >= blockYmin[i]&& bX >= blockXmin[i] && bX <= blockXmax[i])
				{
					X *= -1;
					destroySprite(BlockTask[i]);
					blc[i] = true;
					blockint--;
				}
			}
		}
	}
	void collision()
	{
		srand(time(NULL));
		for (int i = 0; i < BLOCKS_COUNT; i++)
		{
			if (blc[i] == false) {

				if (bY <= blockYmax[i] + 10 && bY >= blockYmin[i] - 10 && bX >= blockXmin[i] + 12 && bX <= blockXmax[i] - 15)
				{
					a *= -1;
					destroySprite(Block[i]);
					blc[i] = true;
					blockint--;
					if (aY == -100) {
						aY = blockYmax[i]-80;
						aX = blockXmax[i]-60;
					}
				}
				if (bY <= blockYmax[i] && bY >= blockYmin[i] + 8 && bX >= blockXmin[i]-8 && bX <= blockXmax[i]+10)
				{
					X *= -1;
					destroySprite(Block[i]);
					blc[i] = true;
					blockint--;
					if (aY == -100) {
						aY = blockYmax[i]-80;
						aX = blockXmax[i]-60;
					}
				}
			}
		}
	}

	
	void drawBlocks(int d,int lr,int y)
	{
		int ww = 150;
		int hh = 55;
		for (int i = 0; i < 5; i++)
		{
			blockXmin[i] = ww;
			blockXmax[i] = ww + 80;
			blockYmin[i] = hh+d;
			blockYmax[i] = hh + 40+d;
			ww += 90;
		}
		ww = 150;
		for (int i = 5; i < 10; i++)
		{
			blockXmin[i] = ww;
			blockXmax[i] = ww + 80;
			blockYmin[i] = hh+60+d;
			blockYmax[i] = hh + 40+60+d;
			ww += 90;
		}
		ww = 150;
			blockXmin[10] = ww-110+y;
			blockXmax[10] = ww + 80-90+y;
			blockYmin[10] = hh;
			blockYmax[10] = hh + 40;
			ww += 540;
			blockXmin[11] = ww - 90 + y;
			blockXmax[11] = ww + 80 - 90 + y;
			blockYmin[11] = hh;
			blockYmax[11] = hh + 40;
		for (int i = 0; i < 5; i++)
		{
			drawSprite(Block[i], blockXmin[i], hh+d);
		}
		for (int i = 5; i < 10; i++)
		{
			drawSprite(Block[i], blockXmin[i], hh+60+d);

		}
			drawSprite(BlockTask[10], blockXmin[10]+y, hh);
			drawSprite(BlockTask[11], blockXmin[11] + y, hh);

		for (int i = 0; i < 10; i++)

		if ((blockYmax[i] >= pY && blockXmax[i] >= pX) || blockYmax[i] >= h)//closing
		{
			std::cout << "LOST";
			Close();
			//no idea how to make resuming game
		}
	}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = 800;
		height = 600;
		fullscreen = false;
	}
	bool run = true;
	void load()
	{
		getScreenSize(w, h);

		pX = (w / 2) - 75;
		pY = (h / 2) + (h / 4);

		bX = (w / 2) - 10;
		bY = (h / 2) + h / 5;

		leftEdgePlatform = w / 2 - 75;
		rightEdgePlatform = w / 2 + 75;

		Mouse = createSprite("59-Breakout-Tiles.png");
		Platform = createSprite("50-Breakout-Tiles.png");
		Ball = createSprite("62-Breakout-Tiles.png");
		for (int i = 0; i < BLOCKS_COUNT; i++) {
			Block[i] = createSprite("01-Breakout-Tiles.png");
			setSpriteSize(Block[i], 80, 40);
		}
		for (int i = 10; i < 12; i++) {
			BlockTask[i] = createSprite("06-Breakout-Tiles.png");
			setSpriteSize(BlockTask[i], 80, 40);
		}
		for (int i = 0; i < 5; i++)
		{
			ability[i] = createSprite("46-Breakout-Tiles.png");
			setSpriteSize(ability[i], 50, 20);
		}
		setSpriteSize(Ball, 20, 20);
		setSpriteSize(Platform, 150, 40);
		setSpriteSize(Mouse, 50, 50);
		showCursor(false);
		onMouseMove(0, 0, 0, 0);
	}
	virtual bool Init() {
		if (run == true)
		{
			load();
			run = false;
		}
		for (int i = 0; i < 8; i++)
		if (blockint == 0 && (pY==bY+i|| pY == bY-i || pY == bY)) //finish
		{
			load();
		}
		return true;
	}

	virtual void Close() {
	}
	virtual bool Tick() {
		Sleep(40);
		p++;

		count += delta;
		if (count == min || count == max) delta = -delta;
		
		if (p % 10 == 7)
		{
			down++;
			lr++;
		}

		drawBlocks(down,lr,count);
		drawSprite(Mouse, sX, sY);
		drawSprite(Platform, pX, pY);
		drawSprite(Ball, bX, bY);
		drawSprite(ability[abilitynext], aX, aY + p);
		if (aY+p== pY&& aX >= leftEdgePlatform && aX <= rightEdgePlatform)//catching ability
		{
			destroySprite(ability[abilitynext]);
			aY = -100;
			abilitynext++;
			p = 0;
		}
		if (aY == -90)
		{
			destroySprite(ability[abilitynext]);
			aY = -100;
			abilitynext++;
			p = 0;
		}
		collision();
		collisionT();
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


			a = (float)(dirY - bY) / (float)(dirX - bX); //linear function
		


			isMouseClicked = true;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::LEFT)
		{
			if (pX > -95) {
				leftEdgePlatform -= 60;
				rightEdgePlatform -= 60;
				pX -= 60;
			}
		}
		if (k == FRKey::RIGHT)
		{
			if (pX < 745) {
				leftEdgePlatform += 60;
				rightEdgePlatform += 60;
				pX += 60;
			}
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
