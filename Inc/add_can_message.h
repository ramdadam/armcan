#ifndef ADDCANMESSAGE_H_
#define ADDCANMESSAGE_H_

class CAddCanMessageView : public CEventListener{
public:
    EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction);
    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*);
    void showAddFrame();

    void hideAddFrame();

    void setSliderPosition(int pos);

    void hideSlider();

    void showSlider();

    void createAddFrame();

    uint8_t getFormData(can_gui_form_data *);

    void showVirtualKeyboard();

    void hideVirtualKeyboard();

    GHandle getAcceptButton() {
        return ghAcceptButton;
    }
    GHandle getBackButton() {
        return ghBackButton;
    }
    GHandle getIsRemoteFlag() {
        return ghAddIsRemote;
    }
private:
    GHandle ghFrame1 = 0;
    GHandle ghKeyboard = 0;
    GHandle ghIDTextEdit = 0;
    GHandle ghLabel1 = 0;
    GHandle ghLabel2 = 0;
    GHandle ghMSBLabel = 0;
    GHandle ghLSBLabel = 0;
    GHandle ghSlider1 = 0;
    uint16_t lastPos = 0;
    GHandle ghTexteditContainer = NULL;
    GHandle ghDataTextEdits[8] = {NULL};
    bool_t byteOrderLabelVisible = 0;

    GHandle ghBackButton = 0;
    GHandle ghAcceptButton = 0;

    GHandle ghAddIsRemote = 0;
    gdispImage addImage;
};


#endif