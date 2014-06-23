#include "postgres.h"
#include "fmgr.h"
#include "executor/executor.h"	/* for GetAttributeByName() */
#include "utils/geo_decls.h"	/* for point type */

#include "NLPIR_C.h"

#pragma comment(lib , "NLPIR.lib")


#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
static void init()
{
if(!NLPIR_Init(sDataPath,0,0))//数据在当前根路径下，默认为GBK编码的分词
	{
		//printf("ICTCLAS INIT FAILED!\n");
		//return;
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("Chinese Parser Lib NLPIR SCWS could not init!\"%s\"",""
				       )));
	}
else
//	printf("NLPIR DATA INIT OK!");
ereport(NOTICE,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("Chinese Parser Lib NLPIR SCWS lcp580 ok init!\"%s\"",""
				       )));
}

PG_FUNCTION_INFO_V1(PG_init);
PGDLLEXPORT
Datum
PG_init(PG_FUNCTION_ARGS)
{
//char Result[]="NLPIR DATA INIT OK!";
char *sResult=PG_GETARG_CSTRING(0);
//sResult=Result;
init();
PG_RETURN_CSTRING(sResult);
}

/*
PG_FUNCTION_INFO_V1(SplitGBK);
PGDLLEXPORT
Datum
SplitGBK(PG_FUNCTION_ARGS)
{//分词演示
    	
	const char *sSentence=PG_GETARG_CSTRING(0);
    int32 sResult_size= VARSIZE(sSentence); 
	
	const char *sResult =(char*)palloc(VARSIZE(sResult_size));
	      char *new_sResult=(char*)palloc(VARSIZE(sResult));
    ereport(NOTICE,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("Chinese Parser Lib NLPIR SCWS lcp580 error init!\"%s\"",""
				       )));
		  SET_VARSIZE(sResult,VARSIZE(sResult_size));
	//char *new_result=PG_GETARG_CSTRING(2);
	//int32 sResult1_size=VARSIZE(sResult1);
	
	unsigned int nPaLen=strlen(sample1);
	char* sRst=0;   //用户自行分配空间，用于保存结果；
	sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
	int nRstLen=0; //分词结果的长度

	int32 sample1_size=VARSIZE(sample1) - VARHDRSZ;
	int32 sResult1_size=VARSIZE(sResult1)-VARHDRSZ;
	

	//init();
	//text *sResult1=PG_GETARG_TEXT_P(0);	
	//初始化分词组件	
	//NLPIR_SetPOSmap(PKU_POS_MAP_SECOND);//设置词性标注集合的类型，默认为计算所二级标注体系
	//sample1 = "开心成长的烦恼学习深造中国证券市场";
	// sResult1=NLPIR_ParagraphProcess(sample1,1); 
	//sResult =(char *)NLPIR_ParagraphProcess(sSentence,1);	
	// new_result=(char*)palloc(sResult1_size*6);
	
	memcpy(new_sResult,sResult,VARSIZE(new_sResult));
	 //printf("%s\n", sResult1); 	
	 //PG_RETURN_TEXT_P(sResult1);
	// PG_RETURN_CSTRING(new_result);
	 //PG_RETURN_CSTRING(new_sResult);
	PG_RETURN_TEXT_P(new_sResult);
}
*/
PG_FUNCTION_INFO_V1(SplitGBK);
	PGDLLEXPORT
	Datum
	SplitGBK(PG_FUNCTION_ARGS)
{
	const char *sSentence=PG_GETARG_CSTRING(0);
	int32 sResult_size= VARSIZE(sSentence)*6; 
	
	const char *sResult =(char*)palloc(VARSIZE(sResult_size));
	SET_VARSIZE(sResult, VARSIZE(sResult));
	//      char *new_sResult=(char*)palloc(VARSIZE(sResult));

	init();

	memcpy((void*)sResult,(void *)(sSentence+'my'),VARSIZE(sResult));
	ereport(NOTICE,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("test test test testtest test init!\"%s\"",""
				       )));
	PG_RETURN_CSTRING(sResult);
	
}


//int NLPIR_Init(const char *sDataPath＝0,int encode＝0,char *sLicenceCode＝0);
PG_FUNCTION_INFO_V1(PG_NLPIR_Init);
PGDLLEXPORT
Datum
PG_NLPIR_Init(PG_FUNCTION_ARGS)
{
	char *sDataPath=PG_GETARG_CSTRING(0);
	int32 endcode=PG_GETARG_INT32(0);
	char *sLicenceCode=PG_GETARG_CSTRING(1);
	int32 result=PG_GETARG_INT32(3);
	result=NLPIR_Init(sDataPath,endcode,sLicenceCode);
	PG_RETURN_INT32(result);
}

//NLPIR_API const char *NLPIR_ParagraphProcess(const char *sParagraph, int bPOStagged＝1);
PG_FUNCTION_INFO_V1(PG_NLPIR_ParagraphProcess);
PGDLLEXPORT
Datum
PG_NLPIR_ParagraphProcess(PG_FUNCTION_ARGS)
{
	char *sParagraph=PG_GETARG_CSTRING(0);
	int32 bPOStagged=PG_GETARG_INT32(0);
	const char *sResult=PG_GETARG_CSTRING(1);
	sResult=NLPIR_ParagraphProcess(sParagraph,bPOStagged);
	PG_RETURN_CSTRING(sResult);
}
//NLPIR_API  BOOL NLPIR_Exit();

PG_FUNCTION_INFO_V1(PG_NLPIR_Exit);
PGDLLEXPORT
	Datum
	PG_NLPIR_Exit(PG_FUNCTION_ARGS)
{
	BOOL bResult=PG_GETARG_BOOL(0);
	bResult=NLPIR_Exit();
	PG_RETURN_BOOL(bResult);
}

/* By Reference, Variable Length */

PG_FUNCTION_INFO_V1(copytext);
PGDLLEXPORT
Datum
copytext(PG_FUNCTION_ARGS)
{
	text	   *t = PG_GETARG_TEXT_P(0);

	/*
	 * VARSIZE is the total size of the struct in bytes.
	 */
	text	   *new_t = (text *) palloc(VARSIZE(t));

	SET_VARSIZE(new_t, VARSIZE(t));

	/*
	 * VARDATA is a pointer to the data region of the struct.
	 */
	memcpy((void *) VARDATA(new_t),		/* destination */
		   (void *) VARDATA(t), /* source */
		   VARSIZE(t) - VARHDRSZ);		/* how many bytes */
	PG_RETURN_TEXT_P(new_t);
}

PG_FUNCTION_INFO_V1(copychar);
PGDLLEXPORT
Datum
copychar(PG_FUNCTION_ARGS)
{
	char	   *t = PG_GETARG_CSTRING(0);

	/*
	 * VARSIZE is the total size of the struct in bytes.
	 */
	char	   *new_t = (char *) palloc(VARSIZE(t));

	SET_VARSIZE(new_t, VARSIZE(t));

	/*
	 * VARDATA is a pointer to the data region of the struct.
	 */
	memcpy((void *) VARDATA(new_t),		/* destination */
		   (void *) VARDATA(t), /* source */
		   VARSIZE(t) - VARHDRSZ);		/* how many bytes */
	PG_RETURN_TEXT_P(new_t);
}