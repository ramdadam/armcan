#include "gfx.h"
#include "main_view.h"

void* mainThread(void* param)
{

	/* Execute this until we shall be terminated */
	while (1) {
		initMainPage();
	}

	/* Don't return anything (or return something) */
}


int main()
{
	initMainPage();

    gfxThreadCreate(0, 512, NORMAL_PRIORITY, mainThread, 0);

	return 0;
}
