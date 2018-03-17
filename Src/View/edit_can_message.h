#ifndef EDIT_CAN_MESSAGE_H_
#define EDIT_CAN_MESSAGE_H_
#define MAX_TRANSMIT_TIMERS 10
static GHandle ghFrame;
static GHandle ghEditBackButton;
static GHandle ghEditAcceptButton;
void editCanMessage(can_gui_package* package, uint8_t useAlloc);

#endif