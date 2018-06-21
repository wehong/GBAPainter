/*

	paint.c

	small paint example for gameboy advance

*/

#include "gba.h"
#include "data.h"
#include "keypad.h"

#define BG0_ENABLE		0x100
#define BG1_ENABLE		0x200
#define BG2_ENABLE		0x400
#define BG3_ENABLE		0x800
#define OBJ_ENABLE		0x1000
#define WIN0_ENABLE		0x2000
#define WIN1_ENABLE		0x4000
#define WINOBJ_ENABLE	0x8000

#define OBJ_MAP_LINEAR  0x40
#define OBJ_MAP_2D      0x0
#define BACK_BUFFER     0x10
	
#define MODE0			0x0
#define MODE1			0x1
#define MODE2			0x2
#define MODE3			0x3
#define MODE4			0x4
#define MODE5			0x5


#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))
#define SCREENWIDTH 240
#define SCREENHEIGHT 160

u16* ScreenBuffer = (u16*)0x6000000;
u16 dcolor[5] = { 0, 0x001f, 0x03e0, 0x7e00, 0x7fff };

int cx = 120, cy = 80, cc = 4;
u16 tmpdot;

void SetMode(u16 mode)
{
	REG_DISPCNT = mode;
}

void clear()
{
	int x, y;

	for(x=0;x<240;x++)
	{
		for(y= 0;y<160;y++)
		{
			ScreenBuffer[x+y*SCREENWIDTH] = RGB16(0,0,0);
		}
	}
}

void restore(int x, int y)
{
	ScreenBuffer[x+y*SCREENWIDTH] = (u16)tmpdot;
}

void plot(int x, int y, int c)
{
	tmpdot = ScreenBuffer[x+y*SCREENWIDTH];
	ScreenBuffer[x+y*SCREENWIDTH] = dcolor[c];
}

int Get_Input(void)
{
	int d = -1;
	int c;
	int val;

	if(!((*KEYS) & KEY_UP))
	{
		restore(cx,cy);
		if(cy>0) cy--;
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_DOWN))
	{
		restore(cx,cy);
		if(cy<SCREENHEIGHT-1) cy++;
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_LEFT))
	{
		restore(cx,cy);
		if(cx>0) cx--;
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_RIGHT))
	{
		restore(cx,cy);
		if(cx<SCREENWIDTH-1) cx++;
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_A))
	{
		tmpdot = dcolor[cc];
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_B))
	{
	}
	if(!((*KEYS) & KEY_R))
	{
		cc++;
		if(cc>4) cc=0;
		restore(cx, cy);
		plot(cx, cy, cc);
	}
	if(!((*KEYS) & KEY_L))
	{
		cc--;
		if(cc<0) cc=4;
		restore(cx, cy);
		plot(cx, cy, cc);
	}
	for(val=0;val<9999;val++);
	return d;
}

int main()
{
   	SetMode(MODE3 | BG2_ENABLE);

	clear();
	plot(cx,cy,cc);
	while(1)
	{
		Get_Input();
	}
}
