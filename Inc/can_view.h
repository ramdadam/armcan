#ifndef AC_VIEW_CAN_VIEW
#define AC_VIEW_CAN_VIEW
class CCanView {
public:

    GHandle createBaseTableWidget(GHandle* parent, uint16_t width = 480, uint16_t height = 272);
    void deleteTableWidget();
    void redrawTable();
    void setWaitingLabelVisibility(bool show);
protected:
    GHandle table_view;
    void addRow(char* item);
private:
    GHandle ghPleaseWaitLabel;

};
#endif
