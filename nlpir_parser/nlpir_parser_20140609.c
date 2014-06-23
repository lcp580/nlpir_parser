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


#include "NLPIR_C.h"
#pragma comment(lib,"NLPIR.lib")

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
	char       *sege;
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
PG_FUNCTION_INFO_V1(nlpirprs_start);
PGDLLEXPORT  /* Insert this line before every extension function */
Datum		nlpirprs_start(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(nlpirprs_getlexeme);
PGDLLEXPORT
Datum		nlpirprs_getlexeme(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(nlpirprs_end);
PGDLLEXPORT
Datum		nlpirprs_end(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(nlpirprs_lextype);
PGDLLEXPORT
Datum		nlpirprs_lextype(PG_FUNCTION_ARGS);

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
static int handle=1;
const char *sResult;

const char *tokenize(char *intext){

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

sResult=NLPIR_ParagraphProcess(intext,0);

return sResult;
}

/*
 * functions
 */
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
nlpirprs_start(PG_FUNCTION_ARGS)
{
	const char *t=NULL;
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	
	pst->buffer = (char *) PG_GETARG_POINTER(0);

	t=tokenize(pst->buffer);

	pst->sege=pstrdup(t);
	if (pst->sege == NULL) {
		pst->sege = strdup(" ");//对分词结果集为空的处理
	}

	pst->buffer=pst->sege;
	pst->len = PG_GETARG_INT32(1);
	pst->pos = 0;

	PG_RETURN_POINTER(pst);
}


PGDLLEXPORT
Datum
nlpirprs_getlexeme(PG_FUNCTION_ARGS)
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

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	//text	   *in = PG_GETARG_TEXT_P(0);//输入参数为文本类型,例如：巴拿马和美国都是国家地区，汉族是一个民族。
	char *in =(char *)PG_GETARG_POINTER(0);
	int len;
	char *s;//将输入参数中的字符串指内容拷贝到此字符串指针	
	char *splitWords;
	text *out;//输出结果为文本类型
	
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	//*(s+len)='\0';//解决乱码的关键
	s[len]='\0';
	
	//splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	
	//splitWords=(char *)malloc(strlen(in)+1);

	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords= (char *)NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	
	//NLPIR_Exit();//error
	out=cstring_to_text(splitWords);

	//pfree(splitWords);
	PG_RETURN_TEXT_P(out);//输出返回结果	
	//pfree(splitWords);
	//NLPIR_Exit();//ok
}
