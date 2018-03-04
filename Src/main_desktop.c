#include "gfx.h"
extern void initMainPage(void);

void mainThread(void* param)
{

	/* Execute this until we shall be terminated */
	while (1) {
		initMainPage();
	}

	/* Don't return anything (or return something) */
	return 0;
}


void main()
{
	initMainPage();

    gfxThreadCreate(0, 512, NORMAL_PRIORITY, mainThread, 0);

	return 0;
}
