#include "gfx.h"
#include "Inc/View/main_view.h"
#include <stdio.h>

extern gfxQueueGSync* canTransmitQueue;
extern gfxQueueGSync* canReceiveQueue;

void* mainThread(void* param)
{

	/* Execute this until we shall be terminated */
	while (1) {
		initMainPage();
	}

	/* Don't return anything (or return something) */
}

void* transmitThread(void* _) {
	while(1) {
        gfxQueueGSyncItem* package_queued = gfxQueueGSyncGet(canTransmitQueue, TIME_INFINITE);
		gfxQueueGSyncPut(canReceiveQueue, package_queued);
	} 
	return (threadreturn_t)0;
}

int main()
{
	canTransmitQueue = (gfxQueueGSync*) gfxAlloc(sizeof(gfxQueueGSync));
	canReceiveQueue = (gfxQueueGSync*) gfxAlloc(sizeof(gfxQueueGSync));
	gfxQueueGSyncInit(canTransmitQueue);
	gfxQueueGSyncInit(canReceiveQueue);
    // gfxThreadCreate(0, 512, NORMAL_PRIORITY, mainThread, 0);
    gfxThreadCreate(NULL, 128, NORMAL_PRIORITY, transmitThread, 0);
	initMainPage();
	while(1);
	

	return 0;
}
