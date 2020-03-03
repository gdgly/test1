#ifndef RWFILE_H
#define RWFILE_H

#include "av_headset.h"
#include <pmalloc.h>

/*  */
#define FILE_NAME "/rwfs/firmware.bin"

FILE_INDEX FileOpen(char * file_name);
FILE_INDEX FindFileIndex(char * file_name);
int FileWrite(FILE_INDEX findex, uint8 *buffer, int length);
int FileClose(void);
int FileCancel(char *fname);            // 删除文件

#endif //RWFILE_H

