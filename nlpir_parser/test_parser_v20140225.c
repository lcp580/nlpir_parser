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
	char        *sege;    
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
//const char *sResult;����������ĳ�������������.���صĽ�������Ϊȫ�ֱ����ŵ���������
NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

sResult=NLPIR_ParagraphProcess(intext,0);//ֱ����������intext�����ᵼ�·ִʽ������������.

return sResult;//���Ա�����������������ִʺ���
}


PG_FUNCTION_INFO_V1(chinese_tokenize);
PGDLLEXPORT
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
	int len;
	char *s;//����������е�ԭʼ�ַ������Ƶ����s�ַ���ָ���У��ǵ�Ϊ������ڴ�ռ䲢��������ַ���׼Ϊ\0
	const char *splitWords;
		
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	char *in =(char *)PG_GETARG_POINTER(0);
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//�������Ĺؼ�
	
	//splitWords=(char *)palloc(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���
	
	splitWords=tokenize(s);

	PG_RETURN_TEXT_P(cstring_to_text(splitWords));
	

}



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
	NLPIR_Exit();//ok
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


/*
 * functions
 */
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
testprs_start(PG_FUNCTION_ARGS)
{	
	
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//Ϊ�����������ڴ�ռ�	
	
	const char *splitWords;
	//pst->buffer = (char *) PG_GETARG_TEXT_P(0);	//����:  invalid memory alloc request size 3101994908
	pst->buffer=(char *)PG_GETARG_POINTER(0);// �������֮һ�����ַ�������:�����й��� sql����Ϊ��SELECT to_tsvector('testcfg','�����й���');	//���������һ���յ��ַ�����ô����
	
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���
   
	splitWords = NLPIR_ParagraphProcess(pst->buffer,0);//���ǲ���\x01����Ĵ���֮Դͷ.ǿ�ƽ�const char *ת��Ϊchar*.���г����ڴ�Ƿ����ʡ�����/�����ܹ���
	
	pst->sege=strndup(splitWords,strlen(splitWords));//�Ƽ�����
	
	
	pst->buffer=pst->sege;
	
	//pst->buffer=(char *)splitWords;
//	pst->len = PG_GETARG_INT32(1);//�ִʵ�����ַ�����һ����"��"�������׼ȷ���������.'�й�':3 '��':1 '��':2
	pst->len=strlen(pst->buffer);//����ִʽ��������ַ�©���Ĺؼ����ڣ���������ԭ��ԭ����������!��ȷΪ��'�й�':3 '��':4 '��':1 '��':2  //�������֮��������ĺ�����Ҫ�Ӵ˻�ò�����Ϊ�����롣
//	*((pst->buffer)+strlen(pst->buffer)+1)='\0';//�������Ĺؼ�
	pst->pos = 0;//�ٴζ�ͬһ�����ӷִ�ʱ����һ���ַ�Ϊ\0����Ҳ���԰�

	//NLPIR_ParagraphProcess(pst->buffer,0);
	//NLPIR_ParagraphProcessA(pst->buffer,&(pst->len),0);

	//(pst -> res) = (scws_res_t)-1;

	//pfree(s);
	PG_RETURN_POINTER(pst);	
	NLPIR_Exit();
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


PGDLLEXPORT
Datum
testprs_end(PG_FUNCTION_ARGS)
{
	
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	//NLPIR_Exit();error
	pfree(pst);
	
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

/*
int main()
{
	tokenize(in);
	return 1;
}
*/