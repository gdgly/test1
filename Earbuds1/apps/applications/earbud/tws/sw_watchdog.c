// #include "sink_main_task.h"
// #include "sink_events.h"

#include <vm.h>
#include "av_headset.h"
#include "sw_watchdog.h"
#include "public.h"

#ifdef ENABLE_WATCHDOG
void watchdogReset(void)
{
        VmSoftwareWdKick(WD_TIMEOUT_SECONDS);
        MessageSendLater(appGetAppTask(), AV_SYS_RST_WATCH_DOG, 0 ,D_SEC(WD_KICK_TIME_SECONDS));
}
#endif
