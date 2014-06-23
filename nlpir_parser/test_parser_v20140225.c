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

//const char *in="我是中国人，我来自湖北省江陵县";
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//static int handle=1;

const char *sResult;//返回的分析后的文本，包含用空格分隔开的一串字符串的指针

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
//const char *sResult;如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

sResult=NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

return sResult;//测试表明，错不在上面这个分词函数
}


PG_FUNCTION_INFO_V1(chinese_tokenize);
PGDLLEXPORT
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
	int len;
	char *s;//将输入参数中的原始字符串复制到这个s字符串指针中，记得为其分配内存空间并设置最后字符标准为\0
	const char *splitWords;
		
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	char *in =(char *)PG_GETARG_POINTER(0);
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//解决乱码的关键
	
	//splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	
	splitWords=tokenize(s);

	PG_RETURN_TEXT_P(cstring_to_text(splitWords));
	

}



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
	NLPIR_Exit();//ok
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


/*
 * functions
 */
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
testprs_start(PG_FUNCTION_ARGS)
{	
	
	ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//为分析器分配内存空间	
	
	const char *splitWords;
	//pst->buffer = (char *) PG_GETARG_TEXT_P(0);	//错误:  invalid memory alloc request size 3101994908
	pst->buffer=(char *)PG_GETARG_POINTER(0);// 输入参数之一，即字符串例如:我是中国人 sql命令为：SELECT to_tsvector('testcfg','我是中国人');	//？如果输入一个空的字符串怎么处理
	
	NLPIR_Init(sDataPath,1,0);//初始化分词函数
   
	splitWords = NLPIR_ParagraphProcess(pst->buffer,0);//这是产生\x01代码的错误之源头.强制将const char *转换为char*.运行出现内存非法访问。编译/连接能过的
	
	pst->sege=strndup(splitWords,strlen(splitWords));//推荐拷贝
	
	
	pst->buffer=pst->sege;
	
	//pst->buffer=(char *)splitWords;
//	pst->len = PG_GETARG_INT32(1);//分词的最后字符掉了一个字"人"，结果不准确的罪祸所在.'中国':3 '我':1 '是':2
	pst->len=strlen(pst->buffer);//解决分词结果的最后字符漏掉的关键所在，不在其他原因，原来就在这里!正确为：'中国':3 '人':4 '我':1 '是':2  //输入参数之二，后面的函数需要从此获得参数作为其输入。
//	*((pst->buffer)+strlen(pst->buffer)+1)='\0';//解决乱码的关键
	pst->pos = 0;//再次对同一个句子分词时，第一个字符为\0。这也不对啊

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