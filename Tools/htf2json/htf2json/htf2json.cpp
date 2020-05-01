// htf2json.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cstdlib>

#define BUFFER_SIZE       (4<<10)
char writebuf[BUFFER_SIZE], readbuf[BUFFER_SIZE];
int _tmain(int argc, _TCHAR* argv[])
{
	int ret = 0;
	char *inF, *outF, *ptr;
	FILE *infp = NULL, *outfp = NULL;
	int rdlen, wrlen;
	unsigned int keyid = 0, first_line = 1;

	if(argc < 2) {
		printf("Usage:%s infile outfile", argv[0]);
		return -1;
	}

	inF = argv[1], outF = argv[2];

	fopen_s(&infp, inF, "r");
	fopen_s(&outfp, outF, "w+");

	if(!infp || !outfp) {
		printf("Open File Error:%s %s\n", inF, outF);
		ret = -2; goto out;
	}

	// write head
	wrlen = sprintf_s(writebuf, "{\n  \"type\":\"文件\",\n  \"info\":\n  [");
	fwrite(writebuf, wrlen, 1, outfp);

	// write item
	while(1) {
		memset(readbuf, 0, BUFFER_SIZE);
		fgets(readbuf, BUFFER_SIZE, infp);
	//	printf("R=%s", readbuf);

		rdlen = strlen(readbuf);
		if(rdlen == 0) break;

		if(rdlen < 5) continue;

		for(int i = 0; i < 4; i++) {
			if(readbuf[rdlen-1] == '\n' || readbuf[rdlen-1] == ']' || readbuf[rdlen-1] == ' ') {
				readbuf[rdlen-1] = '\0';
				rdlen -= 1;
			}
		}
	//	printf("S=%s\n", readbuf);
		
		if((ptr = strstr(readbuf, "PSID=")) != NULL) {		 // # PSID=0x2400, capID=0x0048, UCID=0x0000
			ptr += 5;
			keyid = strtoul(ptr, NULL, 16);
		}
		else if((ptr = strstr(readbuf, "-> [ ")) != NULL) {   // # -> [ 1001 0000 0006 4000 ....
			ptr += 5;
			if(0 != keyid)
				wrlen = sprintf_s(writebuf, "\n    {\n      \"type\": \"0x%04x\", \n      \"data\": \"%s\"\n    }",	keyid, ptr);
		}
		else if(strncmp(readbuf, "0x0", 3) == 0) {             // 0x004C80 = [ 01 10 00 00 ...
			if(keyid != 0) {
				if(0 == first_line)
					fwrite(",", 1, 1, outfp);
				fwrite(writebuf, wrlen, 1, outfp);

				first_line = 0;
			}
			keyid = 0;
			memset(writebuf, 0, sizeof(writebuf));
		}
		else if(strncmp(readbuf, "# 0x0", 5) == 0) {             // # 0x004C80 = [ 01 10 00 00 ...
			keyid = 0;
			memset(writebuf, 0, sizeof(writebuf));
		}
	}

	// write tail
	wrlen = sprintf_s(writebuf, "\n  ]\n}");
	fwrite(writebuf, wrlen, 1, outfp);

out:
	if(infp) fclose(infp), infp = NULL;
	if(outfp) fclose(outfp), outfp = NULL;

	printf("Convert %s to %s %s\n", inF, outF, (ret) ? "FAIL" : "SUCCESS");

	return ret;
}

