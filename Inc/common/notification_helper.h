#ifndef NOTIFICATION_HELPER
#define NOTIFICATION_HELPER
#include "gfx.h"
static GHandle notificationLabel;
void showMessage(const char* text);
void showError(const char* text);
void initNotifications();
#endif