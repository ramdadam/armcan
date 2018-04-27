


#define TX_MAX_PACKAGES 25

static can_gui_package_array txCanContainer = 0;
static uint32_t txCanContainerSize = 0;

GHandle createTxCanViewTable(GHandle* parent);
void deleteTxCanViewTable();
can_gui_package* getTxSelectedCANPackage();
void createButtonGroup(GHandle* parent);
int8_t putTxCanPackage(can_gui_package* package, uint8_t allowPackageDeletion);