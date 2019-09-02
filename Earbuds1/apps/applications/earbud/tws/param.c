
#include <../av_headset.h>
#include <ps/ps_if.h>
#include <bdaddr.h>
#include <message.h>
#include <panic.h>
#include <ps.h>
#include <stdlib.h>
#include <string.h>
#include <vm.h>

#define PSKEY_CUSTOMER0_VALUE   (200)                         // ID, from trap_api/ps.h

#define PSKEY_PEER_ADDR         (PSKEY_CUSTOMER0_VALUE)       // 保存双方配对耳机蓝牙地址

// taddr: save value;
//        NULL for delete old save
int16 ParamSavePeerAddr( typed_bdaddr *taddr)
{
    uint16 ret, size = sizeof(typed_bdaddr)/2;      // PsStore的第二个参数为words,因此长度应该除2
    if(!taddr)
        size = 0;

    ret = PsStore(PSKEY_PEER_ADDR, taddr, size);
    if(ret != size)
        return -1;

    return size;
}

int16 ParamLoadPeerAddr( typed_bdaddr *taddr)
{
    uint16 ret, size = sizeof(typed_bdaddr)/2;      // PsRetrieve的第二个参数为words,因此长度应该除2
    if(!taddr)
        return -1;

    ret = PsRetrieve(PSKEY_PEER_ADDR, taddr, size);
    if(ret != size)
        return -2;

    return size;
}








