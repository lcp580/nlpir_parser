#include <stdio.h>
#include <string.h>


#include "NLPIR_C.h"

#pragma comment(lib,"NLPIR.lib")

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";


#define NTBS_SIZE 5


static const char *tokenize(const char *intext1);


//strcpy
/*
int main()
{
	const char *sSentence="我是中国人";
	char *copy_sSentence;
	const char *sResult;
	char new_sResult[14];
	int sSentence_len;
	int sResult_len;
	int new_sResult_len;
	int copy_sSentence_len;

	copy_sSentence=(char *)malloc(strlen(sSentence)+1);
	
	//strncpy(copy_sSentence,sSentence,strlen(sSentence));
	
	memcpy(copy_sSentence,sSentence,strlen(sSentence));
	*((copy_sSentence)+strlen(sSentence))='\0';

	NLPIR_Init(sDataPath,0,0);
		
	sResult=NLPIR_ParagraphProcess(copy_sSentence,0);	
	

	//free(copy_sSentence);
	//strncpy(new_sResult,sResult,sizeof(sResult)-1);
	//new_sResult[sizeof(new_sResult)-1]='\0';

	//copy_sSentence_len=strlen(copy_sSentence_len);
	sSentence_len=strlen(sSentence);
	sResult_len = strlen(sResult);
	new_sResult_len=strlen(new_sResult);

	printf("源字符串长度为:%d\n",sSentence_len);
	printf("分词后的字符串长度为：%d\n",sResult_len);
	printf("复制后的结果数组的长度为：%d\n",new_sResult_len);
	printf("%s\n",sResult);	
	printf("复制后的字符串为：%s\n",copy_sSentence);
	NLPIR_Exit();
	return 0;

}
*/
int main()
{
 const char *sSentence="我是中国人";
 const char *mysResult;
 
 int mysResult_len;
 //mysResult=(char *)malloc(4*strlen(sSentence)+1);

 NLPIR_Init(sDataPath,0,0);//初始化分词函数
 mysResult=NLPIR_ParagraphProcess(sSentence,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

  mysResult_len=strlen(mysResult);
 //mysResult=(char *)tokenize(sSentence);
 printf("%s\n",mysResult);
 printf("%d\n",mysResult_len);
 return 0;

}

static const char *
tokenize(const char *intext)
{
const char *sResult=NULL;//如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

sResult=NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

// =strlen(sResult);


return sResult;//测试表明，错不在上面这个分词函数
}



//strncpy_s
/*
int main()
{
	//char *source = "0123456789abcdef";
	char *source ="我是中国人";
	char a[NTBS_SIZE];
	int len;
	int source_len;

	
	if(source)
	{
	errno_t err=strncpy_s(a,sizeof(a),source,16);
	if(err != 0)
	   {
		printf("Handle error");
	   }
	}
	else
	{
		printf("Handle NULL string condition");
	}
	
	len = strlen(a);
	source_len=strlen(source);

	printf("%s\n",a);
	printf("%d\n",len);
	printf("%d\n",source_len);
	return 0;

}
*/

//strncpy
/*
int main()
{
	//char *source = "0123456789abcdef";
	char *source ="我是中国人";
	char ntbs[NTBS_SIZE];
	int len;
	int source_len;

	strncpy(ntbs,source,sizeof(ntbs)-1);
	ntbs[sizeof(ntbs)-1] ='\0';
	len = strlen(ntbs);
	source_len=strlen(source);

	printf("%s\n",ntbs);
	printf("%d\n",len);
	printf("%d\n",source_len);
	return 0;

}
*/