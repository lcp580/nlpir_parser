#include "NLPIR_C.h"
#include <stdio.h>
#pragma comment (lib,"NLPIR.lib")
const char *sSentence="�����й���\0";
int main()
{
	char b[]="\0";
	//void SplitGBK();
	char a[]="�� �� �й� �� ";	
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
	//�ִ�
	
	sResult=NLPIR_ParagraphProcess(sSentence,0);	
	i2=strlen(sResult);
	printf("�ִʳ���ɹ�\n");
    printf(sResult);
	strncpy(b,sResult,strlen(sResult));
	printf("\n%d",i2);
	NLPIR_Exit();
	getchar();
	return 1;
}

void  SplitGBK()
{

   // const char *sSentence="�����й���";
	const char *sResult="O";
	const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
	if(!NLPIR_Init(sDataPath,0,0))
	{
		printf("NLPIR Init fail!\n");
		return;
	}
	else
		printf("NLPIR INIT SUCCESS!\n");
	//�ִ�
	
	sResult=NLPIR_ParagraphProcess(sSentence,1);
	printf("�ִʳ���ɹ�\n");
    printf(sResult);
}
