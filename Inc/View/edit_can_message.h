#ifndef EDIT_CAN_MESSAGE_H_
#define EDIT_CAN_MESSAGE_H_
#define MAX_TRANSMIT_TIMERS 10

class CEditMessageView : public CEventListener {
public:
    virtual EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction);
    virtual EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*);

    void editCanMessage(can_gui_package *package, uint8_t useAlloc);
    GHandle getCyclicRadioSelect(){
        return ghEditRadioSelectCyclic;
    }
    GHandle getOnceRadioSelect(){
        return ghEditRadioSelectOnce;
    }
    GHandle getEditAcceptButton(){
        return ghEditAcceptButton;
    }
    GHandle getEditBackButton(){
        return ghEditBackButton;
    }

    void saveEditForm();

    void deleteEditForm();
    //TODO: make private, see main_view.cpp:145
    void immediateDeleteTimer(GTimer *timer);

    void hideCyclicTextbox();

    void showCyclicTextbox();

private:
    GHandle ghFrame = 0;
    GHandle ghEditIDLabel = 0;
    GHandle ghEditDLCLabel = 0;
    GHandle ghEditDataValue = 0;
    GHandle ghEditCheckbox = 0;
    GHandle ghEditCycleLabel = 0;
    GHandle ghEditCycleTextEdit = 0;
    GHandle ghEditCycleMsLabel = 0;
    uint8_t showCyclicBox = 0;
    can_gui_package *currentPackage = 0;

    GHandle ghEditRadioSelectCyclic = 0;
    GHandle ghEditRadioSelectOnce = 0;
    GHandle ghEditAcceptButton = 0;
    GHandle ghEditBackButton = 0;
};

#endif