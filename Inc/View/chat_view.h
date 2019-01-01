//
// Created by mohamad on 01.01.19.
//

#ifndef ARMCAN_CHAT_VIEW_H
#define ARMCAN_CHAT_VIEW_H

class CChatView : public CEventListener {
public:
    EVENT_ACTION evalEvent(GEvent *, EVENT_ACTION currentAction);

    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent *);

    CChatView();

    void createChatView(GHandle *parent);
    void addMessage();

private:
    GHandle *parent;
    GHandle ghChatConsole;
    GHandle ghReceiverIDTextEdit;
    GHandle ghMessageTextEdit;
    GHandle ghReceiverIDLabel;
    GHandle ghMessageLabel;
};

#endif //ARMCAN_CHAT_VIEW_H
