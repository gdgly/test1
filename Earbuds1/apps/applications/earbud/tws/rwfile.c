/*
 * 描述：文件的打开、读、写、关闭、删除
 * 时间：2020年2月26日
*/
#include "rwfile.h"

extern void pfree(void *ptr);


static int writeFileSink(Sink sink, void *buf, int len)
{
    uint16  offset;
    uint8* map_address;
    int lensink = SinkSlack(sink);

    DEBUG_LOG("%d\n", lensink);
    if(len > lensink){
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
    if(!(SinkFlush(sink, len)))
        return 1;

    return 0;
}

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
    }
    SourceClose(file_source);
    printf("fileSize = %d\n",fileSize);
    return fileSize;
}

//写数据到文件
void WriteFile(FILE_INDEX findex, void *buf, int len);
void WriteFile(FILE_INDEX findex, void *buf, int len)
{
    Sink fsink;
    fsink = StreamFileSink(findex);
    writeFileSink(fsink, buf, len);
    SinkClose(fsink);
}
/*
 * 打开文件
 * rwflag: 1(写文件)
 *         0(读文件)
 */
FileCPtr FileOpen(char *fname, int rwflag)
{
    FileCPtr fCtrl = (FileCPtr)PanicUnlessMalloc(sizeof(FileCtrl));
    memcpy(fCtrl->fName, fname, strlen(fname) + 1);
    printf("name %s,%d",fCtrl->fName,strlen(fCtrl->fName));

    fCtrl->fIndex = FileFind(FILE_ROOT, fCtrl->fName, strlen(fCtrl->fName));
    DEBUG_LOG("FileFind ret=%d", fCtrl->fIndex);
    if (rwflag == 0)
    {
        if (FILE_NONE == fCtrl->fIndex) /* 没有找到文件 */
        {
            pfree(fCtrl);
            return NULL;
        }
        fCtrl->fsize = get_file_size(fCtrl->fIndex);
    }
    else if (rwflag == 1)
    {
        if (fCtrl->fIndex != FILE_NONE)
        {
            FileDelete(fCtrl->fIndex);
            DEBUG_LOG("FileDelete ret=%d", fCtrl->fIndex);
        }
        fCtrl->fIndex=FileCreate(fCtrl->fName, (uint16)strlen(fCtrl->fName));
        DEBUG_LOG("Filecreate ret=%d", fCtrl->fIndex);
        fCtrl->fsize = 0;
    }

    fCtrl->sink = StreamFileSink(fCtrl->fIndex);
    fCtrl->offset = 0;
    return fCtrl;
}


FILE_INDEX OpenFile_1(void)
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
/*
 * 写文件
 * 返回：写入文件的长度
 */
int FileWrite(FILE_INDEX findex, uint8 *buffer, int length)
{
    Sink fsink;
    fsink = StreamFileSink(findex);
    writeFileSink(fsink, buffer, length);
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
    UNUSED(len);
    printf(("----\r\n "));
    for(i=0;i<50;i++)
    {
       printf("%x ",data[i]);
    }
    printf(("----\r\n "));
}
/*
 * 把文件内数据读出来
 */
void ReadFile_2(FILE_INDEX findex)
{
    Source fsource;
    uint8* map_address;
    uint16 rLen;

    DEBUG_LOG("read FILE_INDEX ret=%d", findex);
    fsource = StreamFileSource(findex);
    map_address = (uint8 *)SourceMap(fsource);
    rLen = SourceSize(fsource);
    DEBUG_LOG("index = %d,map_addres = %x ,len =%d",findex,map_address,rLen);
    if(rLen)
    {
        DebugData(map_address,rLen);
        SourceDrop(fsource,rLen);/* 文件不能太大，不然也会panic */
    }

    SourceClose(fsource);

}

/* 测试使用 */
const char tfilename[]="/rwfs/Test1.bin";
const uint8 testdata[]={
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

void TestReadFile_test(void)
{
    FILE_INDEX file_index;
    Source fsource;
    uint8* map_address;
    uint16 rLen;

    file_index=FileFind(FILE_ROOT, tfilename, (uint16)strlen(tfilename));

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
    file_index=FileFind(FILE_ROOT, tfilename, (uint16)strlen(tfilename));
    DEBUG_LOG("FileFind-w ret=%x\n", file_index);
    if(file_index == FILE_NONE)
    {
        file_index=FileCreate(tfilename, (uint16)strlen(tfilename));
        DEBUG_LOG("Filecreate ret=%x\n", file_index);
    }
    return file_index;
}
void testw(void);
void testw(void)
{
    WriteFile(OpenFile(), (void *)testdata, sizeof(testdata));
}
//查找文件,返回该文件索引号
FILE_INDEX FindFileIndex(void);
FILE_INDEX FindFileIndex(void)
{
    FILE_INDEX file_index;
    file_index=FileFind(FILE_ROOT, tfilename, (uint16)strlen(tfilename));
    DEBUG_LOG("FileFind-w ret=%x\n", file_index);
    return file_index;
}
//获取文件大小
void FileSize(void);
void FileSize(void){
    get_file_size(FindFileIndex());
}
