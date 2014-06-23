#include "postgres.h"
#include "fmgr.h"
#include "executor/executor.h"	/* for GetAttributeByName() */
#include "utils/geo_decls.h"	/* for point type */

#include "utils/builtins.h"//cstring_to_text

#include "NLPIR_C.h"
//#include "strndup.h"

#pragma comment(lib , "NLPIR.lib")

static int handle=1;

char *strndup(const char *s, size_t n)
{
   // char* newbuf = (char*)malloc(strnlen(s, n) + 1);
	char *newbuf=(char *)malloc(n);
    strncpy(newbuf,s,n);//����utf8�������n-1����Ϊn,�������һ�����ֳ�������
    return newbuf;
}

#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
void _PG_init(void);
void _PG_fini(void);

 void _PG_init(void)
{
	handle=NLPIR_Init(sDataPath,1,0);
if(!handle)//�����ڵ�ǰ��·���£�Ĭ��ΪGBK����ķִʣ���encode=0,��encode=1��Ϊutf8.postgresqlĬ����utf8����.
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
				 errmsg("Chinese Parser Lib NLPIR lcp580 ok init!\"%s\"",""
				       )));
}
 void _PG_finit(void)
 {
   if(handle=1)
	 NLPIR_Exit();

 }

PG_FUNCTION_INFO_V1(tokenize);
PGDLLEXPORT
Datum
tokenize(PG_FUNCTION_ARGS)
{
	text *in = PG_GETARG_TEXT_P(0);
	//char *in =PG_GETARG_CSTRING(0);
	char *s = NULL;
//	char *s=(char *)palloc(VARSIZE(in));
	char *t = NULL;

	//s=pnstrdup(in,VARSIZE(in)-VARHDRSZ);
#define VAR_STRLEN(S) (VARSIZE(S) - VARHDRSZ)

	s = strndup(VARDATA(in), VAR_STRLEN(in));
	
  //  s=strncpy(s, (char*)in ,strlen((char*)in));

#undef VAR_STRLEN	
	
	_PG_init();
	t=(char *)NLPIR_ParagraphProcess(s,0);
	free(s);
	//PG_RETURN_TEXT_P((text *)t);
	//text *out;
	//out=char2text(t);		
	PG_RETURN_TEXT_P(cstring_to_text(t));
   //PG_RETURN_TEXT_P(DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(pstrdup(t)))));
  // NLPIR_Exit();
	}

char *dup_pgtext(text *what)
{
    size_t len = VARSIZE(what)-VARHDRSZ;
    char *dup =(char *)palloc(len+1);
    memcpy(dup, VARDATA(what), len);
    dup[len] = 0;
    return dup;
}
