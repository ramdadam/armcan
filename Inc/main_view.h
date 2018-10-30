#ifndef MAIN_VIEW_H_
#define MAIN_VIEW_H_

class CMainView {
    friend void redrawTables(void *cMainViewParam);

public:
    void initMainPage();
    void addRxCanPackage(can_gui_package* package);
protected:
    void createTable(void);

    void createTabset(void);

    void showMainpage();

    void hideMainpage();

private:

    void triggerRxRedraw();

    void triggerTxRedraw();

    GHandle ghTabset = 0;
    GHandle tabset_page_1;
    GHandle tabset_page_2;

    /* Views */
    CEditMessageView cEditMessageView;
    CRxCanView cRxCanView;
    CTxCanView cTxCanView;
    CAddCanMessageView cAddCanMessageView;

};

void initMainPage();

#endif