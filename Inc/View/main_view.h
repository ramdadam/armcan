#ifndef MAIN_VIEW_H_
#define MAIN_VIEW_H_

class CMainView {
    friend void redrawTables(void *cMainViewParam);

public:
    void initMainPage();
    void addRxCanPackage(can_gui_package* package);
    void notifySdCardChanges();
protected:
    void createTable(void);

    void createTabset(void);

    void showMainpage();

    void hideMainpage();

    void refreshActiveTab();

private:
    void takeScreenshot(CCanView*);

    void triggerRxRedraw();

    void triggerTxRedraw();

    void triggerCanSettingsUpdate();

    GHandle ghTabset = 0;
    GHandle txTabPage;
    GHandle rxTabPage;
    GHandle canSettingsTabPage;
    GHandle sdSettingsTabPage;

    /* Views */
    CEditMessageView editMessageView;
    CRxCanView rxView;
    CTxCanView txView;
    CCanSettingsView canSettingsView;
    CSdSettingsView sdSettingsView;
    CAddCanMessageView addMessageView;

    bool disableActiveTabRefresh = false;

};

void initMainPage();

#endif