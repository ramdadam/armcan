//
// Created by mohamad on 01.01.19.
//

#ifndef ARMCAN_CHAT_VIEW_H
#define ARMCAN_CHAT_VIEW_H

#include <Inc/VirtualKeyBoard/vkeyboard.h>
#include <Inc/common/can_gui_package.h>
#include "stm32f7xx_hal_rng.h"

#define CHAT_MESSAGE_BUFFER_LENGTH 10
#define CHAT_MESSAGE_MAX_LENGTH CHAT_MESSAGE_BUFFER_LENGTH - 2
typedef struct {
    uint16_t sender = 0;
    uint16_t currentPos = 0;
    char messageBuffer[CHAT_MESSAGE_BUFFER_LENGTH] = " ";

} Message;

class CChatView : public CEventListener {
public:
    EVENT_ACTION evalEvent(GEvent *, EVENT_ACTION currentAction);

    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent *);

    CChatView();

    void createChatView(GHandle *parent);
    void addMessage();
    void receiveCanMessage(can_gui_package* package);
    void sendMessage();
private:
    RNG_HandleTypeDef hrng;
    Message message;
    uint16_t myId = 0xF;
    VirtualKeyBoard* hexKeyboard;
    VirtualKeyBoard* textKeyboard;
    GHandle parent;
    GHandle ghSendBtn;
    GHandle ghChatConsole;
    GHandle ghReceiverIDTextEdit;
    GHandle ghMessageTextEdit;
    GHandle ghReceiverIDLabel;
    GHandle ghMessageLabel;
};

#endif //ARMCAN_CHAT_VIEW_H
