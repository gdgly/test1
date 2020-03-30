#ifndef RWFILE_H
#define RWFILE_H

#include "av_headset.h"
#include <pmalloc.h>

/* 升级文件 */
#define FILE_NAME "/rwfs/firmware.bin"
/* 标记升级文件是否下载成功 */
#define FILE_NAME_OK "/rwfs/firmware_ok.bin"
/* 系统日志文件 */
#define FILE_LOG "/rwfs/dgb_log.bin"
#define MAX_LOG_SIZE 20480

FILE_INDEX FileOpen(char * file_name);
FILE_INDEX FindFileIndex(char * file_name);
int FileWrite(FILE_INDEX findex, uint8 *buffer, int length);
int FileClose(void);
int FileCancel(char *fname);

uint8 FileWriteOk(uint8 value);
uint8 FileReadOk(void);
int16 getFileSize(FILE_INDEX findex);
uint16 FileRead(FILE_INDEX findex,Source *file_source,uint8 *map_address);

#endif //RWFILE_H

