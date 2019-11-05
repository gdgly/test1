#ifndef SW_WATCHDOG_H
#define SW_WATCHDOG_H

#include "public.h"

#ifdef ENABLE_WATCHDOG
void watchdogReset(void);
#else
#define watchdogReset() {}
#endif

#define WD_TIMEOUT_SECONDS        40
#define WD_KICK_TIME_SECONDS      10

#endif // SW_WATCHDOG_H
