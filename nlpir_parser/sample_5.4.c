//nlpir 2014 exmaple 5.5
#include "NLPIR_C.h"
#include <stdio.h>
#include <string.h>

#pragma comment(lib,"NLPIR.lib")

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";

int main (int argc, char *argv[])
{
//Sample1:Sentence or paragraph lexical analysis with only one result
	char sSentence[2000];
	const char *sResult;
	if(!NLPIR_Init(sDataPath,1,0))
	{
		printf("Init fails\n");
		return -1;
	}
	printf("Input sentence now!\n");
	scanf("%s",sSentence);
	while(_stricmp(sSentence,"q")!=0)
	{	
	sResult=NLPIR_ParagraphProcess(sSentence,1);		
	printf("%s\nInput string now('q' to quit)!\n",sResult);
	scanf("%s",sSentence);
	}		
	NLPIR_Exit();
	getchar();
	return 0;
}