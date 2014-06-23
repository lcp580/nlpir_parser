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
   //  strncpy(newbuf,s,n);//对于utf8，必须从n-1纠正为n,否则最后一个汉字出现乱码
	strncpy(newbuf,in,strlen(in));
    *(newbuf+n)='\0';//解决乱码的关键
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
	char        *sege;    //临时保存分词的结果集
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

//const char *in="我是中国人，我来自湖北省江陵县";
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//static int handle=1;

const char *sResult;//返回的分析后的文本，包含用空格分隔开的一串字符串的指针
const char *splitWords2;
/* bak tokenize
const char *tokenize(const char *intext)
{
//const char *sResult=NULL;
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
*/
static const char *
tokenize(const char *intext)
{
//char *sResult=NULL;如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

sResult=NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

// =strlen(sResult);


return sResult;//测试表明，错不在上面这个分词函数
}


/*chinese_tokenize(text ) */
//PG_FUNCTION_INFO_V1(chinese_tokenize);
//PGDLLEXPORT
Datum
chinese_tokenize(PG_FUNCTION_ARGS)
{
	/* error 分词结果的最后不正确，最后字符为乱码.原因不在分词tokenize而在于它前面的代码
	text *t=PG_GETARG_TEXT_P(0);//输入参数，类型为文本text
	text *result_t = (text*)palloc(6*VARSIZE(t));//输出结果为文本类型text

	result_t =(text*)tokenize((const char*)t);

	PG_RETURN_TEXT_P(result_t);
	*/
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	//int len;
	//char *s;//将输入参数中的原始字符串复制到这个s字符串指针中，记得为其分配内存空间并设置最后字符标准为\0
	const char *splitWords;
	
	text *in =PG_GETARG_TEXT_PP(0);
    char *s= (char *)palloc(VARSIZE(in));
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	//char *in =(char *)PG_GETARG_POINTER(0);
	//len=VARSIZE(in)-VARHDRSZ;
	//s = (char *)palloc(len+1);
	SET_VARSIZE(s, VARSIZE(in));
	
	memcpy(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);//由src指向地址为起始地址的连续n个字节的数据复制到以destin指向地址为起始地址的空间内。与strcpy相比，memcpy并不是遇到'\0'就结束，而是一定会拷贝完n个字节。
	
	//s=strndup2(in);
	s[VARSIZE(in)-VARHDRSZ]='\0';//解决乱码的关键
	
	//splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	
	//splitWords=tokenize(s);
	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords=NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	

	pfree(s);

	PG_RETURN_TEXT_P(cstring_to_text(splitWords));

}

/*
CREATE OR REPLACE FUNCTION paragraphprocess(text) RETURNS text
AS 'nlpir_parser.dll', 'ParagraphProcess'
LANGUAGE c ;
*/
/*selct  ParagraphProcess('我是中国人') */

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	//text *in=PG_GETARG_TEXT_P(0);//输入参数为文本类型
	char *in =(char *)PG_GETARG_POINTER(0);
	int len;
	char *s;//将输入参数中的字符串指内容拷贝到此字符串指针	
	const char *splitWords;

	
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//解决乱码的关键
	
	splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	

	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords=NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	
	//NLPIR_Exit();//error
	PG_RETURN_TEXT_P(cstring_to_text(splitWords));//输出返回结果
	//PG_RETURN_CSTRING(splitWords);
	//NLPIR_Exit();//ok
}

static const char *
ParagraphProcess2( char *intxt)
{
	char *in=NULL;//输入参数为文本类型
	int len;
	char *s;
	const char *splitWords;
	const char *out=NULL;

	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//解决乱码的关键
	
	splitWords=(char *)palloc(4*strlen(s)+1);	
	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords=NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针
	
	out = splitWords;//将字符类型转换为文本类型
	return out;//输出为文本类型
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

	PG_RETURN_POINTER(pst);//return 伪类型internal用于声明那些只能在数据库系统内部被调用的函数，这些函数不能在用户的SQL查询里被直接调用。如果函数至少有一个internal 类型的参数，SQL查询就不能直接调用它。只有在函数除至少有一个 internal类型的参数的情况下，才能将函数的返回值的类型定义为internal类型，一定要遵守这条规则。
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
    ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//初始化解析器以便分配内存空间
	NLPIR_Init(sDataPath,1,0);//初始化分词器
	//pst -> s = scws;//初始化分词句柄
	pst->buffer = (char *) PG_GETARG_POINTER(0);//第一个参数，需要分析的文本
	pst->len = PG_GETARG_INT32(1);//注意参数的序号为0、1、2等
	

	pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);//进行分词，返回为字符串指针
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
		pst->sege = strdup(" ");//对分词结果集为空的处理
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
	char *myintext="我是中国人，我来自湖北省江陵县";
	//printf("%s\n",ParagraphProcess2(myintext));
	printf("%s\n",tokenize(myintext));
	return 0;
}
*/