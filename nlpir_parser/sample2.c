#include "NLPIR_C.h"
#include <stdio.h>
#pragma comment (lib,"NLPIR.lib")
const char *sSentence="我是中国人\0";
int main()
{
	char b[]="\0";
	//void SplitGBK();
	char a[]="我 是 中国 人 ";	
	int i1=sizeof(a);
	int i2;
	const char *sResult=NULL;
	const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
	if(!NLPIR_Init(sDataPath,0,0))
	{
		printf("NLPIR Init fail!\n");
		return;
	}
	else
		printf("NLPIR INIT SUCCESS!\n");
	//分词
	
	sResult=NLPIR_ParagraphProcess(sSentence,0);	
	i2=strlen(sResult);
	printf("分词程序成功\n");
    printf(sResult);
	strncpy(b,sResult,strlen(sResult));
	printf("\n%d",i2);
	NLPIR_Exit();
	getchar();
	return 1;
}

void  SplitGBK()
{

   // const char *sSentence="我是中国人";
	const char *sResult="O";
	const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
	if(!NLPIR_Init(sDataPath,0,0))
	{
		printf("NLPIR Init fail!\n");
		return;
	}
	else
		printf("NLPIR INIT SUCCESS!\n");
	//分词
	
	sResult=NLPIR_ParagraphProcess(sSentence,1);
	printf("分词程序成功\n");
    printf(sResult);
}
