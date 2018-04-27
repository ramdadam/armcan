#ifndef EDIT_CAN_MESSAGE_H_
#define EDIT_CAN_MESSAGE_H_
#define MAX_TRANSMIT_TIMERS 10
static GHandle ghFrame;
static GHandle ghEditIDLabel;
static GHandle ghEditDataValue;
static GHandle ghEditCheckbox;
static GHandle ghEditCycleLabel;
static GHandle ghEditCycleTextEdit;
static GHandle ghEditCycleMsLabel;
static uint8_t showCyclicBox = 0;
void editCanMessage(can_gui_package* package, uint8_t useAlloc);
void hideCyclicTextbox();
void showCyclicTextbox();
void saveEditForm();
void deleteEditForm();
void immediateDeleteTimer(GTimer* timer);
#endif