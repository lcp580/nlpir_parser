#include "postgres.h"
#include "fmgr.h"
#include "executor/executor.h"	/* for GetAttributeByName() */
#include "utils/geo_decls.h"	/* for point type */

#include "utils/builtins.h"//cstring_to_text

#include "NLPIR_C.h"

#pragma comment(lib , "NLPIR.lib")


#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//const char *sSentence="我是中国人";

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

PG_FUNCTION_INFO_V1(SplitGBK);
PGDLLEXPORT
Datum
SplitGBK(PG_FUNCTION_ARGS)
{
	
	//text	   *t = PG_GETARG_TEXT_P(0);
	char  *t= PG_GETARG_CSTRING(0);
	/*
	 * VARSIZE is the total size of the struct in bytes.
	 */
	text	   *new_t = (text *) palloc(VARSIZE(t));
	init();
//	fprintf(stderr, "This PostgreSQL build does not support threads.\n");
	SET_VARSIZE(new_t, VARSIZE(t)*6);
	new_t=(text *)NLPIR_ParagraphProcess(t,1);
	/*
	 * VARDATA is a pointer to the data region of the struct.
	 */
	
	//memcpy((void *) VARDATA(new_t),		/* destination */
	//	   (void *) VARDATA(t), /* source */
	//	   VARSIZE(t) - VARHDRSZ);		/* how many bytes */
	//
		   PG_RETURN_TEXT_P(new_t);
	}

