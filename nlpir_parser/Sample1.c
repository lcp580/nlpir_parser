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
	//vc2010����c���򣬱���������������ʹ�ã����򱨴��м�!
//Sample1:Sentence or paragraph lexical analysis only one result
char sSentence[2000];
const char* sResult;
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";


if(!NLPIR_Init(sDataPath,0,0))//����Data�ļ����ڵ�·����ע�����ʵ������޸ġ�../��ʾDATAĿ¼�ڹ��̸�Ŀ¼��
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
