#include "av_headset.h"
#include <pmalloc.h>

extern void pfree(void *ptr);

/*
 * 定义文件结构类型
 */
typedef struct tagFILECTRL {
    char         fName[24];
    FILE_INDEX   fIndex;        //文件的标记号码

    Sink         sink;
    uint8*       map_address;    //分配的文件地址
    uint32       offset;         // 读取位置/写入
    uint32       fsize;          // 总文件大小
    uint32       checksum;       // 写文件时记录CHECKSUM
}FileCtrl, *FileCPtr;

FileCPtr FileOpen(char *fname, int rwflag);       //1 w, 0 r
int FileWrite(FileCPtr fCtrl, char *buffer, int length);
int FileRead(FileCPtr fCtrl, char *buffer, int length);
int FileClose(FileCPtr fCtrl);
int FileCancel(char *fname);            // 删除文件

/*
 * 获取去文件大小
 */
static uint16 get_file_size(FILE_INDEX findex)
{
    Source file_source;
    uint16 source_size,fileSize = 0;

    PanicNull((file_source = StreamFileSource(findex)));

    while((source_size = SourceSize(file_source))!=0)   {
        fileSize += source_size;
        SourceDrop(file_source,source_size);
    }
    SourceClose(file_source);
    printf("%d\n",fileSize);
    return fileSize;
}

/*
 * 打开文件
 * rwflag: 1(写文件)
 *         0(读文件)
 */
FileCPtr FileOpen(char *fname, int rwflag)
{
    FileCPtr fCtrl = (FileCPtr)PanicUnlessMalloc(sizeof(FileCtrl));

    if ( !fCtrl )
        return NULL;

    memcpy(fCtrl->fName, fname, strlen(fname));
    fCtrl->fIndex = FileFind(FILE_ROOT, fCtrl->fName, strlen(fCtrl->fName));
    DEBUG_LOG("FileFind-w ret=%x\n", fCtrl->fIndex);

    if (0 == rwflag) /* readonly */
    {
        if (FILE_NONE == fCtrl->fIndex) /* 没有找到文件 */
        {
            pfree(fCtrl);
            return NULL;
        }

        fCtrl->map_address = (uint8*)FileMap(fCtrl->fIndex, 0, FILE_MAP_SIZE_ALL);
        /* 总文件大小 */
        fCtrl->fsize       = get_file_size(fCtrl->fIndex);
    }
    else /* write */
    {
        if(fCtrl->fIndex != FILE_NONE)
            FileDelete(fCtrl->fIndex);

        fCtrl->fIndex = FileCreate(fCtrl->fName, (uint16)strlen(fCtrl->fName));
    }

    fCtrl->offset = 0;

    return fCtrl;
}

/*
 * 写文件
 */
int FileWrite(FileCPtr fCtrl, char *buffer, int length)
{
    uint16  offset;
    uint8* map_address;
    int lensink = SinkSlack(fCtrl->sink);

    DEBUG_LOG("%d\n", lensink);
    if (length > lensink)
    {
        DEBUG_LOG("malloc falied\n");
        return 0;
    }
    map_address = SinkMap(fCtrl->sink);
    do
    {
        /* Claim space in the sink, getting the offset to it */
        offset = SinkClaim(fCtrl->sink, length);
    }while(offset == 0xFFFF);

    /* Copy the string into the claimed space */
    memcpy(map_address + offset, buffer, length);

    /* Flush the data out to the file */
    if (!(SinkFlush(fCtrl->sink, length)))
        return -1;

    return length;
}


/*
 * 读文件
 * RET：> 0 返回读取到的字节数
 *      -1  失败
 *
 */
int FileRead(FileCPtr fCtrl, char *buffer, int length)
{
    int less;

    if (!fCtrl) return -1;

    less = fCtrl->fsize - fCtrl->offset;
    if (less >= length) less = length;

    memcpy(buffer, fCtrl->map_address+fCtrl->offset, less);
    fCtrl->offset += less;

    return less;
}

/*
 * 删除文件
 */
int FileCancel(char *fname)
{
    FILE_INDEX   fIndex = FileFind(FILE_ROOT, fname, strlen(fname));;

    if (FILE_NONE != fIndex) {
        FileDelete(fIndex);
        return 0;
    }

    return -1;
}

/*
 * 关闭文件
 */
int FileClose(FileCPtr fCtrl)
{
    if ( !fCtrl )
        return 0;

    if ( fCtrl->map_address )
        FileUnmap(fCtrl->map_address), fCtrl->map_address = NULL;

    if ( fCtrl->sink )
        SinkClose(fCtrl->sink);

    return FileSystemUnmount("/rwfs");
}

