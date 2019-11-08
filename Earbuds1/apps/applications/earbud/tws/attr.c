#include <pmalloc.h>
#include "av_headset.h"
#include "tws/attr.h"

#define MAKE_ATTR_WITH_LEN(TYPE, LEN) TYPE *message = (TYPE *) PanicUnlessMalloc(((sizeof(TYPE) + LEN) / 8 * 8) + ((sizeof(TYPE) + LEN) % 8 > 0 ? 8 : 0));

//#define MAKE_ATTR_WITH_LEN(TYPE, LEN) TYPE *message = (TYPE *) PanicUnlessMalloc(sizeof(TYPE) + LEN);

StarotAttr *attrMalloc(StarotAttr** parent, uint8 payloadSize) {
    int payLoadLen = ((payloadSize > 1) ? (payloadSize - 1) : 0);
    int size = sizeof(StarotAttr) + payLoadLen;
    MAKE_ATTR_WITH_LEN(StarotAttr, payLoadLen);
    memset(message, 0x00, size);
    message->next = *parent;
    message->len = 1 + payloadSize;
    *parent = message;
//    DEBUG_LOG("attr new address is : %x", message);
    return message;
}

void attrFree(StarotAttr *attr, void* data) {
    if (NULL != data) {
//        DEBUG_LOG("data free address is : %x", data);
        pfree(data);
    }
    while (NULL != attr) {
        StarotAttr* next = attr->next;
//        DEBUG_LOG("attr free address is : %x", attr);
        pfree(attr);
        attr = next;
    }
}

uint8 *attrEncode(StarotAttr *list, uint16* outLen) {
    /// 其实application核，类似单线程运行，考虑使用此特点，减少内存分配
    uint16 all = 0;
    StarotAttr* begin = list;
    while (NULL != begin) {
        StarotAttr* next = begin->next;
        all += (begin->len + 1);
        begin = next;
    }
    int s = all / 4 * 4 + (all % 4 > 0 ? 4 : 0);
    uint8* data = (uint8*)PanicUnlessMalloc(s);

    begin = list;
    int pos = 0;
    while (NULL != begin) {
        StarotAttr* next = begin->next;
        data[pos] = begin->len;
        data[pos + 1] = begin->attr;
        if (begin->len > 1) {
            memcpy(data + pos + 2, begin->payload, begin->len - 1);
        }
        pos += (1 + begin->len);
        begin = next;
    }

    *outLen = all;
    return data;
}

StarotAttr *attrDecode(uint8 *data, int len) {
    StarotAttr* res = NULL;
    int pos = 0;
    while (pos < len) {
        if ((len - pos) < 2) {
            break;
        }
        uint8 l = data[pos];
        uint8 a = data[pos + 1];
        if ((len - pos) < (l + 1)) {
            break;
        }

        StarotAttr* ptr = attrMalloc(&res, l - 1);
        ptr->len = l;
        ptr->attr = a;
        if (l >= 2) {
            memcpy(ptr->payload, data + pos + 2, l - 1);
        }

        pos += l + 1;
    }

    return res;
}


