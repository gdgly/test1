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
 */
static uint16 get_file_size(FILE_INDEX findex)
{
    Source file_source;
    uint16 source_size,fileSize = 0;

    PanicNull((file_source = StreamFileSource(findex)));
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
 * 查找文件,返回该文件索引号
 */
FILE_INDEX FindFileIndex(char * file_name)
{
    FILE_INDEX file_index;

    file_index = FileFind(FILE_ROOT, file_name, (uint16)strlen(file_name));
    return file_index;
}

/*
 * 打开文件
 * file_name:文件名字，必须是/rwfs/目录下的
 * 如果文件存在就先删除再创建。
 */
FILE_INDEX FileOpen(char * file_name)
{
    FILE_INDEX file_index = 0;

    file_index = FindFileIndex(file_name);
    if (file_index != FILE_NONE)
    {
        FileDelete(file_index);
        DEBUG_LOG("FileDelete ret=%x\n", file_index);
    }
    file_index = FileCreate(FILE_NAME, (uint16)strlen(FILE_NAME));
    DEBUG_LOG("FileCreate ret=%x\n", file_index);

    return file_index;
}


/*
 * 将sink里的数据写入文件里
 */
static int writeFileSink(Sink sink, void *buf, int len)
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
    memcpy(map_address+offset, buf, len);

    /* Flush the data out to the uart */
    if (!SinkFlushBlocking(sink, len))
        return 0;

    return len;
}
/*
 * 写文件
 * 返回：写入文件的长度
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
 * 删除文件
 *  0:删除成功
 * -1:没有该文件，删除失败
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
 */
int FileClose(void)
{
    return FileSystemUnmount("/rwfs/");
}

/*
 *  写入文件是否下发成功的标记的文件
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
 * 读取文件是否下发成功的标记的文件
 */
uint8 FileReadOk(void)
{
    FILE_INDEX file_index;
    Source fsource;
    uint8* map_address;
    uint16 rLen;
    uint8 ret;

    file_index=FileFind(FILE_ROOT, FILE_NAME_OK, (uint16)strlen(FILE_NAME_OK));
    DEBUG_LOG("FileFind-r ret=%d\n", file_index);
    if(file_index == FILE_NONE)
        return 0;
    fsource = StreamFileSource(file_index);
    map_address = (uint8 *)SourceMap(fsource);
    ret = map_address[0];
    rLen = SourceSize(fsource);
    DEBUG_LOG("File has %d bytes\n", rLen);
    SourceDrop(fsource,rLen);
    SourceClose(fsource);

    return ret;
}
/* 测试使用 */
static void DebugData(uint8 *data,uint8 len)
{
    uint8 i;
    UNUSED(len);
    printf(("----\r\n "));
    for(i=0;i<10;i++)
    {
       printf("%x ",data[i]);
    }
    printf(("----\r\n "));
}


const char tfilename[]="/rwfs/Test1.bin";
const uint8 testdata[]={
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    #if 0
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
    0x00,0x01,0x02,0x03,0x04,0x05,0x00,0x01,0x02,0x03,0x04,0x05,
 #endif
};

void TestWriteFile_test(void);
void TestWriteFile_test(void)
{
    FILE_INDEX file_index;
    Sink fsink = 0;
    file_index=FileFind(FILE_ROOT, tfilename, (uint16)strlen(tfilename));

    DEBUG_LOG("FileFind-w ret=%d\n", file_index);
    if(file_index != FILE_NONE)
    {
        FileDelete(file_index);
        DEBUG_LOG("FileDelete ret=%d\n", file_index);
    }
    file_index=FileCreate(tfilename, (uint16)strlen(tfilename));
    DEBUG_LOG("Filecreate ret=%d\n", file_index);
    fsink = StreamFileSink(file_index);
    writeFileSink(fsink,(void *)testdata,sizeof(testdata));
    SinkClose(fsink);

}
void TestDeleteFile_test(void);
void TestDeleteFile_test(void)
{
    FILE_INDEX file_index;
    file_index=FileFind(FILE_ROOT, tfilename, (uint16)strlen(tfilename));

    DEBUG_LOG("FileFind-w ret=%d\n", file_index);
    if(file_index != FILE_NONE)
    {
        FileDelete(file_index);
        DEBUG_LOG("FileDelete ret=%d\n", file_index);
    }
}
void TestReadFile_test(void);
void TestReadFile_test(void)
{
    FILE_INDEX file_index;
    Source fsource;
    uint8* map_address;
    uint16 rLen;

    file_index=FileFind(FILE_ROOT, FILE_NAME, (uint16)strlen(FILE_NAME));

    DEBUG_LOG("FileFind-r ret=%d\n", file_index);
    if(file_index == FILE_NONE)
     return;

    fsource = StreamFileSource(file_index);

    map_address = (uint8 *)SourceMap(fsource);
    rLen = SourceSize(fsource);
    DEBUG_LOG("File has %d bytes\n", rLen);
    if(rLen)
    {
     DebugData(map_address,rLen);
//     SourceDrop(fsource,rLen);
    }

    SourceClose(fsource);

}


//查找并创建文件,返回该文件索引号
FILE_INDEX OpenFile(void);
FILE_INDEX OpenFile(void)
{
    FILE_INDEX file_index;
    file_index=FileFind(FILE_ROOT, FILE_NAME, (uint16)strlen(FILE_NAME));
    DEBUG_LOG("FileFind-w ret=%x\n", file_index);
    if(file_index == FILE_NONE)
    {
        file_index=FileCreate(FILE_NAME, (uint16)strlen(FILE_NAME));
        DEBUG_LOG("Filecreate ret=%x\n", file_index);
    }
    return file_index;
}
void testw(void);
void testw(void)
{
    FileWrite(OpenFile(), (void *)testdata, sizeof(testdata));
}

//获取文件大小
void FileSize(void);
void FileSize(void)
{
    get_file_size(FindFileIndex((void *)testdata));
}
/*
 * 循环读取文件例子
 */

void test_read(void);
void test_read(void)
{
    /* 下面代码可以循环读取,SourceDrop可能会panic */
    uint16 source_size,fileSize = 0;
    uint8 *map_address;
    Source file_source;
    uint32 check_sum = 0;
    FILE_INDEX findex = FindFileIndex(FILE_NAME);

    if (findex == FILE_NONE)
    {
        DEBUG_LOG("can't find file %s",FILE_NAME);
        return;
    }
    PanicNull((file_source = StreamFileSource(findex)));
    while((source_size = SourceSize(file_source)) != 0)
    {
        map_address = (uint8 *)SourceMap(file_source);
//        DebugData(map_address,10);
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
