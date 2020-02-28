/*
 * 描述：文件的打开、读、写、关闭、删除
 * 时间：2020年2月26日
*/
#include "rwfile.h"

//extern void pfree(void *ptr);

/*
 * 获取文件大小
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
    printf("fileSize = %d\n",fileSize);
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
    if ( !fCtrl )return NULL;

    memcpy(fCtrl->fName, fname, strlen(fname) + 1);
    printf("name %s,%d\n",fCtrl->fName,strlen(fCtrl->fName));
    fCtrl->fIndex = FileFind(FILE_ROOT, fCtrl->fName, strlen(fCtrl->fName));
#if 1
    fCtrl->sink = StreamFileSink(fCtrl->fIndex);
    fCtrl->sou_map_address = (uint8 *)SourceMap(StreamFileSource(fCtrl->fIndex));
    fCtrl->sou_len = SourceSize(StreamFileSource(fCtrl->fIndex));

    if (rwflag == 0)
    {
        if (FILE_NONE == fCtrl->fIndex) /* 没有找到文件 */
        {
            pfree(fCtrl);
            return NULL;
        }
        /* 总文件大小 */
        fCtrl->fsize = get_file_size(fCtrl->fIndex);
        DEBUG_LOG("FileFind-r fIndex = %d\n", fCtrl->fIndex);
    }
    else if (rwflag == 1)
    {
        if(fCtrl->fIndex == FILE_NONE)
        {
           FileDelete(fCtrl->fIndex);
           fCtrl->fIndex=FileCreate(fCtrl->fName, (uint16)strlen(fCtrl->fName));
        }
        fCtrl->fsize = 0;
        DEBUG_LOG("FileFind-w fIndex = %d\n", fCtrl->fIndex);
    }

#endif

#if 0
    if (0 == rwflag) /* readonly */
    {
        if (FILE_NONE == fCtrl->fIndex) /* 没有找到文件 */
        {
            pfree(fCtrl);
            return NULL;
        }
        fCtrl->map_address = (uint8 *)FileMap(fCtrl->fIndex, 0, FILE_MAP_SIZE_ALL);
        /* 总文件大小 */
        fCtrl->fsize = get_file_size(fCtrl->fIndex);
        DEBUG_LOG("FileFind-r index=%d\n", fCtrl->fIndex);
    }
    else /* write */
    {
        if (fCtrl->fIndex != FILE_NONE)
            FileDelete(fCtrl->fIndex);

        fCtrl->fIndex = (uint16)FileCreate(fCtrl->fName, (uint16)strlen(fCtrl->fName));
        fCtrl->fsize = 0;
        DEBUG_LOG("FileFind-w index=%d\n", fCtrl->fIndex);
    }
    fCtrl->sink = StreamFileSink(fCtrl->fIndex);
#endif
    fCtrl->offset = 0;
    DEBUG_LOG("sink = %d, index = %d",fCtrl->sink, fCtrl->fIndex);
    return fCtrl;
}

/*
 * 写文件
 * 返回：写入文件的长度
 */
int FileWrite(FileCPtr fCtrl, uint8 *buffer, int length)
{
    Sink fsink;
    fsink = StreamFileSink(fCtrl->fIndex);
    uint16  offset;
    uint8* map_address;
    int lensink = SinkSlack(fsink);

    if(length > lensink)
    {
        DEBUG_LOG("malloc falied\n");
        return 0;
    }
    map_address = SinkMap(fsink);
    do
    {
        /* Claim space in the sink, getting the offset to it */
        offset = SinkClaim(fsink, length);
    }while(offset == 0xFFFF);

    /* Copy the string into the claimed space */
    memcpy(map_address+offset, buffer, length);

    /* Flush the data out to the uart */
    if(!(SinkFlush(fsink, length)))
        return -1;

    SinkClose(fsink);
    return length;
}


/*
 * 读文件
 * RET：> 0 返回读取到的字节数
 *      -1  失败
 *
 */
int FileRead(FileCPtr fCtrl, uint8 *buffer, int length)
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

static void DebugData(uint8 *data,uint8 len)
{
    uint8 i;
    printf(("----\r\n "));
    for(i=0;i<len;i++)
    {
       printf("%x ",data[i]);
    }
    printf(("----\r\n "));
}
//把文件内数据读出来
void ReadFile_2(FILE_INDEX findex)
{
    Source fsource;
    uint8* map_address;
    uint16 rLen;

    PanicNull((fsource = StreamFileSource(findex)));
    map_address = (uint8 *)SourceMap(fsource);
    rLen = SourceSize(fsource);
    if(rLen)
    {
        DebugData(map_address,rLen);
        SourceDrop(fsource,rLen);
    }

    SourceClose(fsource);

}
