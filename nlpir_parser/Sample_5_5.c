//nlpir 2014 exmaple 5.5
#include "NLPIR_C.h"
#include <stdio.h>
//#include <string.h>

#pragma comment(lib,"NLPIR.lib")

/*error
int main (int argc, char *argv[])
{
//Sample1:Sentence or paragraph lexical analysis with only one result
	char sSentence[2000];
	const result_t *pVecResult;
	int nCount;
	if(!NLPIR_Init())
	{
		printf("Init fails\n");
		return -1;
	}
	printf("Input sentence now!\n");
	scanf("%s",sSentence);
	while(_stricmp(sSentence,"q")!=0)
	{
	pVecResult=NLPIR_ParagraphProcessA(sSentence,&nCount,true);
	for(int i=0;i<nCount;i++)
	   {
	printf("Start=%d Length=%d Word_ID=%d POS_ID=%D\n",
		pVecResult[i].start,
		pVecResult[i].length,
		pVecResult[i].word_ID,
		pVecResult[i].sPOS);
	    }
	}
	NLPIR_Exit();
	getchar();
	return 0;
}
*/


int main( int argc, char * argv[])
{
	const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
	char sSentence[2000]="我是中国人的好儿子";
	const struct result_t *pVecResult;
	int   nCount;
	int i;//必须先声明再使用，否则int i=0;i<nCount出错.
	if(!NLPIR_Init(sDataPath,1,0))
	{
		printf("Init fails\n");
	return -1;
	}
	pVecResult=NLPIR_ParagraphProcessA(sSentence,&nCount,1);
	
	for(i=0;i<nCount;i++)
	   {
	printf("Start=%d Length=%d Word_ID=%d POS_ID=%D\n",
		pVecResult[i].start,
		pVecResult[i].length,
		pVecResult[i].word_ID,
		pVecResult[i].iPOS);
	    }
	
	NLPIR_Exit();
	getchar();
	return 0;
}

