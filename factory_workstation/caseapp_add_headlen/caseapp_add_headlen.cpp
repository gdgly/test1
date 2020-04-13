// caseapp_add_headlen.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "atlconv.h"
#include "shellapi.h"
#include <windows.h>

//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int ret = 0;
	FILE *f_in;FILE *f_out;
	string in_file_path, out_file_path;
	unsigned int length; unsigned char*buf;

	if(argc != 3){
		string str = argv[1];
		printf("usg: %s <input file> <output file>!\n", argv[0]);
		ret = -1;
		goto err_out;
	}else{
		in_file_path = argv[1];
		out_file_path = argv[2];
	}

	if( (f_in = fopen(in_file_path.c_str(), "rb+")) == NULL ){
		printf("input file open failed \n");
		ret = -1;
		goto err_out;
	}
	fseek(f_in,0,SEEK_END);//定位到文件的最后面
	length  = ftell(f_in);
	buf = (unsigned char*)malloc(length);
	fseek(f_in, 0, SEEK_SET);
	fread(buf, length, 1, f_in);
	fclose(f_in);

	unsigned char *len_pos = buf + 0x10 +0xc;
	*len_pos = (length >> 24) & 0xff;
	*(len_pos+1) = (length >> 16) & 0xff;
	*(len_pos+2) = (length >> 8) & 0xff;
	*(len_pos+3) = (length >> 0) & 0xff;

	if( (f_out = fopen(out_file_path.c_str(), "wb+")) == NULL ){
		printf("out file open failed \n");
		ret = -1;
		goto err_buf_out;
	}
	fwrite(buf, length, 1, f_out);
	fclose(f_out);

err_buf_out:
	free(buf);
err_out:
	return ret;
}

