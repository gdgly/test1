
#ifndef EARBUDS1_ATTR_H
#define EARBUDS1_ATTR_H

#define MAX_PAYLOAD 32

#define AttrPhone 0X01
///以后有属性，添加到这里

// 0             1             2                N
// +-------------+-------------+-----/ /--------+
// |     LEN     |     ATTR    |    Payload     |
// +-------------+-------------+-----/ /--------+
// len = attr's length + payload's length

struct Starot_Attr_t {
    struct Starot_Attr_t *next;
    uint8 len;  // len = 1(attr) + payload(N)
    uint8 attr;
    uint8 payload[1];
};

typedef struct Starot_Attr_t StarotAttr;

uint8 *attrEncode(StarotAttr *list, int* outLen);

StarotAttr *attrDecode(uint8 *data, int len);

StarotAttr *attrMalloc(StarotAttr* parent, uint8 payloadSize);

void attrFree(StarotAttr *attr);

#endif //EARBUDS1_ATTR_H
