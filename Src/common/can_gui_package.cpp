#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "can_gui_package.h"

void buildStringInCanGuiPackage(can_gui_package *package) {
    for (char &i : package->displayText) {
        i = 0;
    }
    // package->displayText = {0};
    char idString[30] = {0};
    sprintf(idString, "ID: 0x%X | ", package->id);
    size_t idStrLen = strlen(idString);
    strncat(package->displayText, idString, idStrLen);
    package->dataPosStart += idStrLen;

    if (package->isRemote) {
        strncat(package->displayText, "R | ", 2);
        package->dataPosStart += 2;

    } else {
        char dlcString[13] = {0};
        snprintf(dlcString, 13, "DLC: %d | ", package->dlc);
        size_t dlcStrLength = strlen(dlcString);
        strncat(package->displayText, dlcString, dlcStrLength);
        package->dataPosStart += dlcStrLength;


        char dataString[30] = {0};
        if (package->dlc > 0) {
            const char *dataDescription = "D: ";
            const char *format = "%02X | ";
            const char *formatDpp = "%02X:";
            size_t pos = strlen(dataDescription);
            strncat(&dataString[0], dataDescription, pos);

            package->dataPosStart += pos;

            for (uint8_t i = 0; i < package->dlc; i++) {
                char buffer[8] = {0};
                if (i == package->dlc - 1) {
                    snprintf(buffer, 7, format, package->data.data_b[i]);
                    strncat(&dataString[pos], buffer, 5);
                    pos += 2;
                } else {
                    snprintf(buffer, 5, formatDpp, package->data.data_b[i]);
                    strncat(&dataString[pos], buffer, 3);
                    pos += 3;
                }
            }
        }

        strncat(package->displayText, dataString, strlen(dataString));
    }
    if (package->count != 0) {
        char packageCount[10] = {0};
        snprintf(packageCount, 10, "%d", package->count);
        strncat(package->displayText, packageCount, strlen(packageCount));
    }
}

void increaseOnlyCounter(can_gui_package *package) {
//    char packageCount[10] = {0};
//    snprintf(packageCount, 10, "%d", package->count);
//    strncat(package->displayText + package->dataPosStart, packageCount, strlen(packageCount));
    buildStringInCanGuiPackage(package);

}

void bumpPackageCounter(can_gui_package *package) {
     increaseOnlyCounter(package);
}

void packageToString(can_gui_package *package) {
    buildStringInCanGuiPackage(package);
}

can_gui_package *convertCANFormDataToGuiPackage(can_gui_form_data *package) {
    can_gui_package *guiPackage = (can_gui_package *) gfxAlloc(sizeof(can_gui_package));
    guiPackage->id = package->id;
    guiPackage->dlc = package->dlc;
    guiPackage->isRemote = package->isRemote;
    guiPackage->data.data_l = package->data.data_l;
    guiPackage->timer = nullptr;
    guiPackage->cycle = 0;
    guiPackage->count = 0;
    return guiPackage;
}
