//#include "NLPIR_C.h"
//#include <stdio.h>
//#include <string.h>
/*
testprs_start(internal,int4):

Desc:
  Initialises the parser.
Interface:
  1. Argument:  C-Type: (char *)          (IN)
     Desc: Pointer to the text which we parse
  2. Argument:  C-type: (int)             (IN)
     Desc: length of the text
Return value:
  PG_RETURN_POINTER(pst) where pst is a pointer to our
     selfdefined struct, which contains the parser state
     (let's name it ParserState, so pst is of the type
      (ParserState *))
	  

testprs_getlexeme(internal,internal,internal):
Desc:
  Returns the next token. This procedure will be
  called so long as the procedure return type=0
Interface:
  1. Argument.  C-Type: (ParserState *)   (INOUT)
  2. Argument:  C-Type: (char **)         (OUT)
     Desc: token text
  3. Argument:  C-type: (int *)           (OUT)
     Desc: length of the token text
Return value:
     PG_RETURN_INT32(type);
     Desc: token type
*/




#include "postgres.h"
#include "miscadmin.h"
#include "fmgr.h"
#include "utils/geo_decls.h"	/* for point type */
#include "utils/builtins.h"//cstring_to_text

#include "NLPIR_C.h"
#pragma comment(lib,"NLPIR.lib")

static int handle=1;

char* strndup(const char *s, size_t n)
{
   // char* newbuf = (char*)malloc(strnlen(s, n) + 1);
	char *newbuf=(char *)malloc(n);
    strncpy(newbuf,s,n);//对于utf8，必须从n-1纠正为n,否则最后一个汉字出现乱码
    return newbuf;
}

PG_MODULE_MAGIC;

/*
 * types
 */


/* self-defined type */
typedef struct
{
	char	   *buffer;			/* text to parse */
	int			len;			/* length of the text in buffer */
	int			pos;			/* position of the parser */
//	scws_t s;
//	scws_res_t res,head;	
	result_t s;
	char 		*sege;
} ParserState;

/* copy-paste from wparser.h of tsearch2 */
typedef struct
{
	int			lexid;
	char	   *alias;
	char	   *descr;
} LexDescr;


/*
 * prototypes
 */
PG_FUNCTION_INFO_V1(zhprs_start);


PG_FUNCTION_INFO_V1(zhprs_getlexeme);


PG_FUNCTION_INFO_V1(zhprs_end);


PG_FUNCTION_INFO_V1(zhprs_lextype);


const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
void _PG_init(void);
void _PG_fini(void);

/*
 * functions
 */

 void _PG_init(void)
{
	handle=NLPIR_Init(sDataPath,1,0);
if(!handle)//数据在当前根路径下，默认为GBK编码的分词，即encode=0,若encode=1则为utf8.postgresql默认是utf8编码.
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
   	 NLPIR_Exit();
 }

 PGDLLEXPORT
 Datum	
 nlpirprs_start(PG_FUNCTION_ARGS)
 {
    
	 char *s = NULL;
	 char *t = NULL;
	 ParserState *prs = (ParserState *) palloc(sizeof(ParserState)); 
	 prs->buffer = (char *) PG_GETARG_POINTER(0);
	
#define VAR_STRLEN(S) (VARSIZE(S) - VARHDRSZ)
	s = strndup(VARDATA(prs->buffer), VAR_STRLEN(prs->buffer)); 
#undef VAR_STRLEN	
	
	_PG_init();
	t=(char *)NLPIR_ParagraphProcess(s,0);
  // prs->sege=strdup(t);
	/*
	prs->sege=strndup(VARDATA(t),VARSIZE(t));
  if (prs->sege == NULL) {
		prs->sege = strdup(" ");
	}
	*/
  prs->len = PG_GETARG_INT32(0);
  prs->pos = 0;
  PG_RETURN_POINTER(prs); 
 // PG_RETURN_TEXT_P(DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(pstrdup(t)))));
	
  /*
	 const char *t = NULL;
  ParserState *prs = (ParserState *) palloc(sizeof(ParserState));      
  prs->buffer = (char *) PG_GETARG_POINTER(0);  
   _PG_init();
  t=(char *)NLPIR_ParagraphProcess(prs->buffer,0);
  prs->sege=strdup(t);
  if (prs->sege == NULL) {
		prs->sege = strdup(" ");
	}
  prs->len = PG_GETARG_INT32(1);
  prs->pos = 0;

 // PG_RETURN_POINTER(prs);  
  PG_RETURN_TEXT_P(DatumGetTextP(DirectFunctionCall1(textin, CStringGetDatum(pstrdup(t)))));
 */
 }
 
PGDLLEXPORT
Datum
nlpirprs_getlexeme(PG_FUNCTION_ARGS)
{
	ParserState *prs = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	int		   *tlen = (int *) PG_GETARG_POINTER(2);
	//int			startpos = pst->pos;
	int			type;

	*tlen = prs->pos;
	*t = prs->sege + prs->pos;

	if ((prs->sege)[prs->pos] == ' ')
	{
		/* blank type */
		type = 12;
		/* go to the next non-white-space character */
		while ((prs->sege)[prs->pos] == ' ' &&
			   prs->pos < prs->len)
			(prs->pos)++;
	}
	else
	{
		/* word type */
		type = 3;
		/* go to the next white-space character */
		while ((prs->sege)[prs->pos] != ' ' &&
			   prs->pos < prs->len)
			(prs->pos)++;
	}

	*tlen = prs->pos - *tlen;

	/* we are finished if (*tlen == 0) */
	if (*tlen == 0)
		type = 0;
	
	PG_RETURN_INT32(type);
 
}

PGDLLEXPORT
Datum
nlpirprs_end(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);

	pfree(pst);
	PG_RETURN_VOID();
}

Datum
PGDLLEXPORT
nlpirprs_lextype(PG_FUNCTION_ARGS)
{
	/*
	 * Remarks: - we have to return the blanks for headline reason - we use
	 * the same lexids like Teodor in the default word parser; in this way we
	 * can reuse the headline function of the default word parser.
	 */
	LexDescr   *descr = (LexDescr *) palloc(sizeof(LexDescr) * (2 + 1));

	/* there are only two types in this parser */
	descr[0].lexid = 3;
	descr[0].alias = pstrdup("word");
	descr[0].descr = pstrdup("Word");
	descr[1].lexid = 12;
	descr[1].alias = pstrdup("blank");
	descr[1].descr = pstrdup("Space symbols");
	descr[2].lexid = 0;

	PG_RETURN_POINTER(descr);
}


////main
/*
int main( int argc, char * argv[])
{
	const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
	char sSentence[2000]="我是中国人民的好儿子";
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
*/
