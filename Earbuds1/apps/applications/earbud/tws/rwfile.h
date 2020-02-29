#ifndef RWFILE_H
#define RWFILE_H

#include "av_headset.h"
#include <pmalloc.h>

/*  */
#define FILE_NAME "/rwfs/firmware.bin"
/*
 * 定义文件结构类型
 */
typedef struct tagFILECTRL {
    char         fName[24];
    FILE_INDEX   fIndex;         // 文件的标记号码

    Sink         sink;
    uint8*       map_address;    // 分配的文件地址
    uint32       offset;         // 读取位置/写入
    uint32       fsize;          // 总文件大小
    uint32       checksum;       // 写文件时记录CHECKSUM
}FileCtrl, *FileCPtr;

FileCPtr FileOpen(char *fname, int rwflag);       //1 w, 0 r
int FileWrite(FileCPtr fCtrl, uint8 *buffer, int length);
int FileRead(FileCPtr fCtrl, uint8 *buffer, int length);
int FileClose(FileCPtr fCtrl);
int FileCancel(char *fname);            // 删除文件

void TestWriteFile_test(void);
void TestReadFile_test(void);

void ReadFile_2(FILE_INDEX findex);
#endif //RWFILE_H

