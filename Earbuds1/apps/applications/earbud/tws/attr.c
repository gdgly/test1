
#include "tws/attr.h"

#define MAKE_ATTR_WITH_LEN(TYPE, LEN) TYPE##_T *message = (TYPE *) malloc(sizeof(TYPE) + LEN);


StarotAttr *attrMalloc(StarotAttr** parent, uint8 payloadSize) {
    int size = sizeof(StarotAttr) + ((payloadSize > 1) ? (payloadSize - 1) : 0);
    MAKE_ATTR_WITH_LEN(StarotAttr, (payloadSize > 1) ? (payloadSize - 1) : 0);
    memset(message, 0x00, size);
    message->next = *parent;
    *parent = message;
    return message;
}

void attrFree(StarotAttr *attr) {
    while (NULL != attr) {
        StarotAttr* next = attr->next;
        free(attr);
        attr = next;
    }
}

uint8 *attrEncode(StarotAttr *list, int* outLen) {
    /// 其实application核，类似单线程运行，考虑使用此特点，减少内存分配
    int all = 0;
    StarotAttr* begin = list;
    while (NULL != begin) {
        StarotAttr* next = begin->next;
        all += (begin->len + 1);
        begin = next;
    }

    uint8* data = (uint8*)malloc(all);

    begin = list;
    int pos = 0;
    while (NULL != begin) {
        StarotAttr* next = begin->next;
        data[pos] = begin->len;
        data[pos + 1] = begin->attr;
        if (next->len > 1) {
            memcpy(pos + 2, begin->payload, len - 1);
        }
        pos += 1 + begin->len;
        begin = next;
    }

    *outLen = all;
    return data;
}

StarotAttr *attrDecode(uint8 *data, int len) {
    StarotAttr* res = NULL;
    StarotAttr* next = NULL;
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

        StarotAttr* ptr = attrMalloc(l - 1);
        ptr->len = l;
        ptr->attr = a;
        if (l > 2) {
            memcpy(ptr->payload, data[pos + 2], l - 1);
        }
        if (NULL == res) {
            res = ptr;
            next = ptr;
        } else {
            next->next = ptr;
            next = ptr;
        }
        pos += l + 1;
    }

    return res;
}


