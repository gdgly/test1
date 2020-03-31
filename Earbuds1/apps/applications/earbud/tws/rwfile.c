/*
 * 描述：文件的打开、读、写、关闭、删除
 * 时间：2020年2月26日
 * 修改记录:
 * 2020年3月5日10:37:14
 *  修改 SinkFlush为SinkFlushBlocking
*/
#include "rwfile.h"


/*
 * 获取文件大小
 * @findex: 文件索引号
 * @return: 文件大小
 */
int16 getFileSize(FILE_INDEX findex)
{
    Source file_source;
    uint16 source_size = 0;
    uint16 fileSize    = 0;

    file_source = StreamFileSource(findex);
    if (!file_source) return -1;
    while((source_size = SourceSize(file_source)) != 0)
    {
        fileSize += source_size;
        SourceDrop(file_source,source_size);
        source_size *= 100;
        while(source_size--)
        {
            printf("");
        }
    }
    SourceClose(file_source);
    printf("fileSize = %d\n",fileSize);

    return fileSize;
}

/*
 * 查找文件
 * @file_name: 文件名字,必须是/rwfs/目录下的
 * @return: 该文件索引号
 */
FILE_INDEX FindFileIndex(char *file_name)
{
    FILE_INDEX file_index;

    file_index = FileFind(FILE_ROOT, file_name, (uint16)strlen(file_name));
    return file_index;
}

/*
 * 打开文件
 * @file_name: 文件名字，必须是/rwfs/目录下的,如果文件存在就先删除再创建。
 * @return: 该文件索引号
 */
FILE_INDEX FileOpen(char * file_name)
{
    FILE_INDEX file_index = 0;

    file_index = FindFileIndex(file_name);
    if (file_index != FILE_NONE)
    {
        FileDelete(file_index);
        DEBUG_LOG("FileDelete ret=%u\n", file_index);
    }
    file_index = FileCreate(file_name, (uint16)strlen(file_name));
    DEBUG_LOG("FileCreate ret=%u\n", file_index);

    return file_index;
}


/*
 * 将sink里的数据写入文件里
 * @sink: 缓存
 * @buffer: 需要写入的内容
 * @len: 需要写入内容的长度
 * @return: 成功写入内容的长度
 */
static int writeFileSink(Sink sink, void *buffer, int len)
{
    uint16 offset;
    uint8* map_address = NULL;
    int lensink = SinkSlack(sink);

    if (len > lensink)
    {
        DEBUG_LOG("malloc falied\n");
        return 0;
    }
    map_address = SinkMap(sink);
    do
    {
        /* Claim space in the sink, getting the offset to it */
        offset = SinkClaim(sink, len);
    }while(offset == 0xFFFF);

    /* Copy the string into the claimed space */
    memcpy(map_address+offset, buffer, len);

    /* Flush the data out to the uart */
    if (!SinkFlushBlocking(sink, len))
        return 0;

    return len;
}
/*
 * 写文件
 * @findex: 文件索引号
 * @buffer: 需要写入的内容
 * @length: 需要写入内容的长度
 * @return: 写入文件的长度
 */
int FileWrite(FILE_INDEX findex, uint8 *buffer, int length)
{
    Sink fsink;
    int ret = 0;

    fsink = StreamFileSink(findex);
    ret = writeFileSink(fsink, buffer, length);
    SinkClose(fsink);

    return ret;
}

/*
 *  删除文件
 * @fname: 文件名字
 *
 * @return:
 *       0:删除成功
 *      -1:没有该文件，删除失败
 */
int FileCancel(char *fname)
{
    FILE_INDEX   fIndex = FileFind(FILE_ROOT, fname, strlen(fname));;

    if (FILE_NONE != fIndex)
    {
        FileDelete(fIndex);
        return 0;
    }

    return -1;
}

/*
 * 关闭文件
 *
 * @return:
 *      TRUE:卸载成功
 *      FALSE:卸载失败
 */
int FileClose(void)
{
    return FileSystemUnmount("/rwfs/");
}

/*
 * 写入文件;更新APP下发的文件标志文件
 * @value: 写入文件的值
 * @return: 成功写入文件的字节长度
 */

uint8 FileWriteOk(uint8 value)
{
    FILE_INDEX index;
    uint8 buffer[1];
    uint8 ret;

    buffer[0] = value;
    index = FileOpen(FILE_NAME_OK);
    ret = FileWrite(index, buffer, 1);

    return ret;
}
/*
 * 读取文件;读取APP是否下发成功的标记的文件
 * @return: 读出一个字节的内容
 */
uint8 FileReadOk(void)
{
    FILE_INDEX file_index;
    Source fsource;
    uint8* map_address;
    uint16 rLen;
    uint8 ret;

    file_index=FileFind(FILE_ROOT, FILE_NAME_OK, (uint16)strlen(FILE_NAME_OK));
    if (file_index == FILE_NONE)
        return 0;
    fsource = StreamFileSource(file_index);
    rLen = SourceSize(fsource);
    map_address = (uint8 *)SourceMap(fsource);
    ret = map_address[0];
    DEBUG_LOG("File has %d bytes\n", rLen);
    SourceDrop(fsource,rLen);
    SourceClose(fsource);

    return ret;
}
uint16 FileRead(FILE_INDEX findex,Source *file_source,uint8 *map_address);
uint16 FileRead(FILE_INDEX findex,Source *file_source,uint8 *map_address)
{
    UNUSED(findex);
    uint16 source_size;
    uint16 source_size_ret = 0;

    while((source_size = SourceSize(*file_source)) != 0)
    {
        source_size_ret = source_size;
        map_address = (uint8 *)SourceMap(*file_source);
        SourceDrop(*file_source,source_size);
        source_size *= 100;
        while(source_size--)
        {
            printf("");
        }
    }

    return source_size_ret;
}
//test
void test_read(void);
void test_read(void)
{
    /* 下面代码可以循环读取,SourceDrop可能会panic */
    uint16 source_size,fileSize = 0;
    uint8 *map_address;
    Source file_source;
    uint32 check_sum = 0;
    FILE_INDEX findex = FindFileIndex(FILE_LOG);

    if (findex == FILE_NONE)
    {
        DEBUG_LOG("can't find file %s",FILE_LOG);
        return;
    }
    PanicNull((file_source = StreamFileSource(findex)));
    while((source_size = SourceSize(file_source)) != 0)
    {
        map_address = (uint8 *)SourceMap(file_source);
        for (int i = 0; i< source_size;i++)
        {
            check_sum += map_address[i];
        }
        fileSize += source_size;
        SourceDrop(file_source,source_size);
        source_size *= 100;
        while(source_size--)
        {
            printf("");
        }
    }
    DEBUG_LOG("check_sum %u",check_sum);
    SourceClose(file_source);
    printf("fileSize = %d\n",fileSize);
}
int FileCancel1(void);
int FileCancel1(void)
{
    FILE_INDEX   fIndex = FileFind(FILE_ROOT, FILE_LOG, strlen(FILE_LOG));;

    if (FILE_NONE != fIndex)
    {
        FileDelete(fIndex);
        return 0;
    }

    return -1;
}
