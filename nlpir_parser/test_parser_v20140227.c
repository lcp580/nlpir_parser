/*-------------------------------------------------------------------------
 *
 * test_parser.c
 *	  Simple example of a text search parser
 *
 * Copyright (c) 2007-2013, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  contrib/test_parser/test_parser.c
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

//#include "tsearch/ts_cache.h"
#include "tsearch/ts_utils.h"



#include "fmgr.h"
#include "utils/geo_decls.h"	/* for point type */
#include "utils/builtins.h"//cstring_to_text
#include <string.h>  

#include "NLPIR_C.h"
#pragma comment(lib,"NLPIR.lib")

char* strndup(const char *in, size_t n)

{
   
	char *newbuf=(char *)malloc(n+1);
   //  strncpy(newbuf,s,n);//����utf8�������n-1����Ϊn,�������һ�����ֳ�������
	strncpy(newbuf,in,strlen(in));
    *(newbuf+n)='\0';//�������Ĺؼ�
	return newbuf;
	
	
}
char *strndup2(const char *in2)
{
	char *a=0;
	int len=strlen(in2);
    a=(char *)malloc(len);

	strncpy_s(a,sizeof(a),in2,strlen(in2));
//	free(a);

	return a;
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
	char        *sege;    //��ʱ����ִʵĽ����
	char        *res,head;
} ParserState;

/* copy-paste from wparser.h of tsearch2 */
typedef struct
{
	int			lexid;
	char	   *alias;
	char	   *descr;
} LexDescr2;

/*
 * prototypes
 */
PG_FUNCTION_INFO_V1(testprs_start);
PGDLLEXPORT  /* Insert this line before every extension function */
Datum		testprs_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(testprs_getlexeme);
PGDLLEXPORT
Datum		testprs_getlexeme(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(testprs_end);
PGDLLEXPORT
Datum		testprs_end(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(testprs_lextype);
PGDLLEXPORT
Datum		testprs_lextype(PG_FUNCTION_ARGS);

//const char *in="�����й��ˣ������Ժ���ʡ������";
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//static int handle=1;

const char *sResult;//���صķ�������ı��������ÿո�ָ�����һ���ַ�����ָ��
const char *splitWords2;
/* bak tokenize
const char *tokenize(const char *intext)
{
//const char *sResult=NULL;
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

sResult=NLPIR_ParagraphProcess(intext,0);

return sResult;
}
*/
static const char *
tokenize(const char *intext)
{
//char *sResult=NULL;����������ĳ�������������.���صĽ�������Ϊȫ�ֱ����ŵ���������
NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

sResult=NLPIR_ParagraphProcess(intext,0);//ֱ����������intext�����ᵼ�·ִʽ������������.

// =strlen(sResult);


return sResult;//���Ա�����������������ִʺ���
}


/*chinese_tokenize(text ) */
//PG_FUNCTION_INFO_V1(chinese_tokenize);
//PGDLLEXPORT
Datum
chinese_tokenize(PG_FUNCTION_ARGS)
{
	/* error �ִʽ���������ȷ������ַ�Ϊ����.ԭ���ڷִ�tokenize��������ǰ��Ĵ���
	text *t=PG_GETARG_TEXT_P(0);//�������������Ϊ�ı�text
	text *result_t = (text*)palloc(6*VARSIZE(t));//������Ϊ�ı�����text

	result_t =(text*)tokenize((const char*)t);

	PG_RETURN_TEXT_P(result_t);
	*/
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	//int len;
	//char *s;//����������е�ԭʼ�ַ������Ƶ����s�ַ���ָ���У��ǵ�Ϊ������ڴ�ռ䲢��������ַ���׼Ϊ\0
	const char *splitWords;
	
	text *in =PG_GETARG_TEXT_PP(0);
    char *s= (char *)palloc(VARSIZE(in));
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	//char *in =(char *)PG_GETARG_POINTER(0);
	//len=VARSIZE(in)-VARHDRSZ;
	//s = (char *)palloc(len+1);
	SET_VARSIZE(s, VARSIZE(in));
	
	memcpy(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);//��srcָ���ַΪ��ʼ��ַ������n���ֽڵ����ݸ��Ƶ���destinָ���ַΪ��ʼ��ַ�Ŀռ��ڡ���strcpy��ȣ�memcpy����������'\0'�ͽ���������һ���´����n���ֽڡ�
	
	//s=strndup2(in);
	s[VARSIZE(in)-VARHDRSZ]='\0';//�������Ĺؼ�
	
	//splitWords=(char *)palloc(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���
	
	//splitWords=tokenize(s);
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	splitWords=NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��	

	pfree(s);

	PG_RETURN_TEXT_P(cstring_to_text(splitWords));

}

/*
CREATE OR REPLACE FUNCTION paragraphprocess(text) RETURNS text
AS 'nlpir_parser.dll', 'ParagraphProcess'
LANGUAGE c ;
*/
/*selct  ParagraphProcess('�����й���') */

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	//text *in=PG_GETARG_TEXT_P(0);//�������Ϊ�ı�����
	char *in =(char *)PG_GETARG_POINTER(0);
	int len;
	char *s;//����������е��ַ���ָ���ݿ��������ַ���ָ��	
	const char *splitWords;

	
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//�������Ĺؼ�
	
	splitWords=(char *)palloc(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���
	

	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	splitWords=NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��	
	//NLPIR_Exit();//error
	PG_RETURN_TEXT_P(cstring_to_text(splitWords));//������ؽ��
	//PG_RETURN_CSTRING(splitWords);
	//NLPIR_Exit();//ok
}

static const char *
ParagraphProcess2( char *intxt)
{
	char *in=NULL;//�������Ϊ�ı�����
	int len;
	char *s;
	const char *splitWords;
	const char *out=NULL;

	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//�������Ĺؼ�
	
	splitWords=(char *)palloc(4*strlen(s)+1);	
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	splitWords=NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��
	
	out = splitWords;//���ַ�����ת��Ϊ�ı�����
	return out;//���Ϊ�ı�����
}
//PG_FUNCTION_INFO_V1(testprs_start1);
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
testprs_start(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));

	pst->buffer = (char *) PG_GETARG_POINTER(0);//arg1;
	pst->len = PG_GETARG_INT32(1);//arg2;
	pst->pos = 0;

	PG_RETURN_POINTER(pst);//return α����internal����������Щֻ�������ݿ�ϵͳ�ڲ������õĺ�������Щ�����������û���SQL��ѯ�ﱻֱ�ӵ��á��������������һ��internal ���͵Ĳ�����SQL��ѯ�Ͳ���ֱ�ӵ�������ֻ���ں�����������һ�� internal���͵Ĳ���������£����ܽ������ķ���ֵ�����Ͷ���Ϊinternal���ͣ�һ��Ҫ������������
}

/*
 * functions
 */
PG_FUNCTION_INFO_V1(testprs_start1);
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
testprs_start1(PG_FUNCTION_ARGS)
{		
	
	char * str =(char *)PG_GETARG_POINTER(0);
	//int    len = PG_GETARG_INT32(1);
	//char * t = (char *)tokenize(str,len);
     char  *t =(char *)DirectFunctionCall1(ParagraphProcess,PointerGetDatum(str));

	int len =strlen(t);

    return DirectFunctionCall2(testprs_start,PointerGetDatum(t),Int32GetDatum(len));
	// return t;
	//PG_RETURN_TEXT_P(cstring_to_text(t));                              
	
}

/*CREATE FUNCTION testprs_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
*/
//PG_FUNCTION_INFO_V1(testprs_start);
PGDLLEXPORT
Datum 
testprs_start5(PG_FUNCTION_ARGS)
{
    ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//��ʼ���������Ա�����ڴ�ռ�
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���
	//pst -> s = scws;//��ʼ���ִʾ��
	pst->buffer = (char *) PG_GETARG_POINTER(0);//��һ����������Ҫ�������ı�
	pst->len = PG_GETARG_INT32(1);//ע����������Ϊ0��1��2��
	

	pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);//���зִʣ�����Ϊ�ַ���ָ��
	//(pst->buffer) = (pst->sege);

	//pst->len=strlen(pst->sege);
	pst->pos = 0;

	PG_RETURN_POINTER(pst);//return
	
	
}


PG_FUNCTION_INFO_V1(testprs_start_2);//bak start function
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
testprs_start_2(PG_FUNCTION_ARGS)
{
	const char *t=NULL;
	
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	
	pst->buffer = (char *) PG_GETARG_POINTER(0);


	//t=tokenize(pst->buffer);
//	t=ParagraphProcess2(pst->buffer);

	pst->sege=pstrdup(t);
	/*
	if (pst->sege == NULL) {
		pst->sege = strdup(" ");//�Էִʽ����Ϊ�յĴ���
	}
	*/
	pst->buffer=pst->sege;
	pst->len = PG_GETARG_INT32(1);
	pst->pos = 0;

	PG_RETURN_POINTER(pst);
}



PGDLLEXPORT
Datum
testprs_getlexeme(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	int		   *tlen = (int *) PG_GETARG_POINTER(2);
	int			startpos = pst->pos;
	int			type;

	*t = pst->buffer + pst->pos;

	if (pst->pos < pst->len &&
		(pst->buffer)[pst->pos] == ' ')
	{
		/* blank type */
		type = 12;
		/* go to the next non-space character */
		while (pst->pos < pst->len &&
			   (pst->buffer)[pst->pos] == ' ')
			(pst->pos)++;
	}
	else
	{
		/* word type */
		type = 3;
		/* go to the next space character */
		while (pst->pos < pst->len &&
			   (pst->buffer)[pst->pos] != ' ')
			(pst->pos)++;
	}

	*tlen = pst->pos - startpos;

	/* we are finished if (*tlen == 0) */
	if (*tlen == 0)
		type = 0;

	PG_RETURN_INT32(type);
	
}


PG_FUNCTION_INFO_V1(testprs_end1);//bak start function
PGDLLEXPORT
Datum
testprs_end1(PG_FUNCTION_ARGS)
{
		
    ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	pfree(pst);	
	PG_RETURN_VOID();
	//NLPIR_Exit();
}

PGDLLEXPORT
Datum
testprs_end(PG_FUNCTION_ARGS)
{
		
     ParserState * parser = ( ParserState *) PG_GETARG_POINTER(0);
	// NLPIR_Exit();
	 //free(parser->buffer);
	 pfree(parser);
   // DirectFunctionCall1(testprs_end1,
     //                   PointerGetDatum(parser));
	PG_RETURN_VOID();
}


Datum
PGDLLEXPORT
testprs_lextype(PG_FUNCTION_ARGS)
{
	/*
	 * Remarks: - we have to return the blanks for headline reason - we use
	 * the same lexids like Teodor in the default word parser; in this way we
	 * can reuse the headline function of the default word parser.
	 */
	LexDescr2   *descr = (LexDescr2 *) palloc(sizeof(LexDescr) * (2 + 1));

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

/*
int main()
{
	char *myintext="�����й��ˣ������Ժ���ʡ������";
	//printf("%s\n",ParagraphProcess2(myintext));
	printf("%s\n",tokenize(myintext));
	return 0;
}
*/