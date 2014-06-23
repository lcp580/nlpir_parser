//Example1
//#include "NLPIR_C.h"
/*
#include "postgres.h"
#include "fmgr.h"
#include "utils/geo_decls.h"
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NLPIR_C.h"

#pragma comment(lib, "NLPIR.lib")


int main(int argc,char* argv[])
{
	//vc2010编译c程序，变量必须先声明后使用，否则报错。切记!
//Sample1:Sentence or paragraph lexical analysis only one result
char sSentence[2000];
const char* sResult;
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";


if(!NLPIR_Init(sDataPath,0,0))//给出Data文件所在的路径，注意根据实际情况修改。../表示DATA目录在工程根目录。
{
printf("Init fail\n");
return -1;
}
printf("Input sentence now('q' to quit)!\n");
scanf("%s",sSentence);
while(_stricmp(sSentence,"q")!=0)
{
sResult=NLPIR_ParagraphProcess(sSentence,1);
printf("%s\nInput string now('q' to quit)!\n",sResult);
scanf("%s",sSentence);
}
NLPIR_Exit();
return 0;
}
