/*-------------------------------------------------------------------------
 *
 * nlpir_parser.c
 *	  Simple example of a text search parser
 *
 * Copyright (c) 2007-2013, PostgreSQL Global Development Group
 *
 * IDENTIFICATION
 *	  contrib/nlpir_parser/nlpir_parser.c
 *
 *-------------------------------------------------------------------------
 */
//#include "nlpir_parser.h"
#include <string.h>


//#include <windows.h>
#include "postgres.h"


//#include "fmgr.h"
#include "tsearch/ts_cache.h"
//#include "utils/geo_decls.h"	/* for point type */
#include "tsearch/ts_locale.h"
#include "tsearch/ts_utils.h"
#include "utils/builtins.h"//cstring_to_text


//#include "tsearch/ts_cache.h"
//#include "tsearch/ts_utils.h"
//#include "utils/builtins.h"



#include "NLPIR_C.h"
#pragma comment(lib,"NLPIR.lib")

#define NTBS_SIZE 5

#define INIT_STR_EXT 50

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
//typedef struct ParserState  ParserState;
/*
typedef struct 
{
	char	   *buffer;			/* text to parse */
//	int			len;			/* length of the text in buffer */
//	int			pos;			/* position of the parser */
//	char        *sege;    //临时保存分词的结果集
	//char        *res,head;
//}ParserState;
//typedef struct ParserState_s  ParserState;

typedef struct ParserState
{
	char *buffer;
	int len;
	int pos;
	char *sege;
}ParserState;



/* copy-paste from wparser.h of tsearch2 */

typedef struct
{
	int			lexid;
	char	   *alias;
	char	   *descr;
} LexDescr2;

struct result_t1
{
	const char *result_string;
	int len_string;

};
typedef struct result_t1 *res_t1;

/*
typedef struct result_t *scws_res_t;
struct result_t//分词的结果集
{
  int start; //start position,词语在输入句子中的开始位置
  int length; //length,词语的长度
  char  sPOS[POS_SIZE];//word type，词性ID值，可以快速的获取词性表
  int	iPOS;//词性标注的编号
  int word_ID; //该词的内部ID号，如果是未登录词，设成0或者-1
  int word_type; //区分用户词典;1，是用户词典中的词；0，非用户词典中的词
  int weight;//word weight,read weight
 };
 */
//typedef struct result_tokenize_s *nlpir_reslult_tokenize_s;
typedef struct 
{
	       char *splitWords_s;
	       int splitWords_len;
}result_tokenize_s;

//const struct  result_tokenize_s *tokenize_v2(const char *intext);
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

//const char *in="我是中国人，我来自湖北省江陵县";
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//static int handle=1;

const char *sResult;//返回的分析后的文本，包含用空格分隔开的一串字符串的指针
const char *splitWords2;

int len_ok=0;
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
static const char  *
tokenize(const char *intext,int tokentext_length)
{
	
//char *sResult=NULL;如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

sResult=NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

// =strlen(sResult);
tokentext_length=strlen(sResult);

return sResult;//测试表明，错不在上面这个分词函数

}
/*
const struct  result_tokenize_s *tokenize_v2(const char *intext)
{
	
	struct result_tokenize_s *pVecResult;
//char *sResult=NULL;如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

(pVecResult->splitWords_s)=(char *)NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

// =strlen(sResult);
(pVecResult->splitWords_len)=Int32GetDatum(strlen(pVecResult->splitWords_s));

return pVecResult;//测试表明，错不在上面这个分词函数

}
*/
/*
struct result_tokenize_s *
//static const char *result_tokenize_s
//tokenize_s(const char *intext,int tokentext_length)
tokenize_s( const char *intext)
{
result_tokenize_s *my= (result_tokenize_s *)palloc0(sizeof( struct result_tokenize_s));
//struct result_tokenize_s my;
//const char *sResult=NULL;如果保存结果的常量在这里会出错.返回的结果最后作为全局变量放到函数体外
NLPIR_Init(sDataPath,1,0);//初始化分词函数

//sResult=NLPIR_ParagraphProcess(intext,0);//直接这样导入intext参数会导致分词结果出错，或乱码.

// =strlen(sResult);
//tokentext_length=strlen(sResult);
my->splitWords_s= NLPIR_ParagraphProcess(intext,0);
//tokentext_length =strlen(my->splitWords_s);
my->splitWords_len= strlen(my->splitWords_s);
//return sResult;//测试表明，错不在上面这个分词函数

return my;
}

*/



/*chinese_tokenize(text ) */
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
	//int len;
	//char *s;//将输入参数中的原始字符串复制到这个s字符串指针中，记得为其分配内存空间并设置最后字符标准为\0
	 const char *splitWords;
	// size_t len2;
	
	 char *in = PG_GETARG_CSTRING(0);
	// len2=VARSIZE(in)-VARHDRSZ+1;
	// char *in =(char *)PG_GETARG_POINTER(0);//arg1
     char *s= (char *)palloc(VARSIZE(in)-VARHDRSZ+1);
	//  char *s = (char *)malloc(len2);
//	char *s = (char *)malloc(strlen(in)+1);
	//char *in =(char *)PG_GETARG_TEXT_P(0);
	//char *in =(char *)PG_GETARG_POINTER(0);
	//len=VARSIZE(in)-VARHDRSZ;
	//s = (char *)palloc(len+1);
	//SET_VARSIZE(s, VARSIZE(in));
//	strncpy(s,in,strlen(in));error
	memcpy(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);//由src指向地址为起始地址的连续n个字节的数据复制到以destin指向地址为起始地址的空间内。与strcpy相比，memcpy并不是遇到'\0'就结束，而是一定会拷贝完n个字节。
	//(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);
	// StrNCpy(s,in,strlen(in));
	//s=strndup2(in);
	s[VARSIZE(in)-VARHDRSZ]='\0';//解决乱码的关键
//	*(s+strlen(in))='\0';
	//splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	
	//splitWords=tokenize(s);
	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords =NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	


	 pfree(in);
	//free(s);
	//s=NULL;

	//PG_RETURN_TEXT_P(cstring_to_text(splitWords));
	PG_RETURN_CSTRING(splitWords);

}
PG_FUNCTION_INFO_V1(chinese_tokenize_v2);
PGDLLEXPORT
Datum
chinese_tokenize_v2(PG_FUNCTION_ARGS)
{
	
	result_tokenize_s  *res_t=(	result_tokenize_s *)palloc(sizeof(result_tokenize_s));
	//text *in_text =(text *)PG_GETARG_POINTER(0);//text 
	//char *in_txt =(char *)PG_GETARG_POINTER(0);//cstring 
	//char *out_txt;
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//通过text_to_cstring函数将输入参数text类型转换为以null结尾的C字符串。这是关键。!!!!
	//out_txt =(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(in_text));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok 必须强制转换，总结一下：参数中输入的为text类型哪怕表面上输入的是char类型也没能以null即\0作为结尾符。
	//out_txt=tokenize(in_txt);//error L
	//len =Int32GetDatum(strlen(out_txt));
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	char *in_txt =(char *)PG_GETARG_POINTER(0);//cstring 
	char *out_txt;	
	int len_txt=0;
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//通过text_to_cstring函数将输入参数text类型转换为以null结尾的C字符串。这是关键。!!!!
	//out_txt =(char *)tokenize(in_txt);//ok
	NLPIR_Init(sDataPath,1,0);//初始化分词函数

    out_txt =(char *)NLPIR_ParagraphProcess(in_txt,0);//直接这样导入intext参数会导致分词结果出错，或乱码.
	//pst->sege =out_txt;
	//pst->buffer = out_txt;

	//PG_RETURN_DATUM(DirectFunctionCall2(nlpirprs_start1,PointerGetDatum(out_txt),len));
	//PG_RETURN_TEXT_P(cstring_to_text(pst->buffer));//return text DataType
	//PG_RETURN_CSTRING(pst->buffer);//return char * DataType
	len_txt=strlen(out_txt);
	// result_t1 *res_t1= (result_t1*)PG_GETARG_POINTER(0);
	
 
	 res_t->splitWords_s = out_txt;
	 res_t->splitWords_len =len_txt;
	// PG_RETURN_POINTER(res_t);
	//PG_RETURN_CSTRING(out_txt);
	// PG_RETURN_CSTRING(res_t->splitWords_s);
	 PG_RETURN_INT32(res_t->splitWords_len);

}


PG_FUNCTION_INFO_V1(out_len);
PGDLLEXPORT
Datum
out_len(PG_FUNCTION_ARGS)
/*
{
	
	char  *in_txt = (char *)PG_GETARG_TEXT_P(0);
	char *s;//临时存储复制后的输入字符串并以\0结尾
	int  len;
	char *out_txt;

	len = VARSIZE(in_txt)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in_txt),len);
	*(s+len)='\0';//解决乱码的关键	
	
	NLPIR_Init(sDataPath,1,0);//初始化分词器

	out_txt= (char *)NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	

	//out_txt=(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(in_txt));
	

	PG_RETURN_INT32(Int32GetDatum(strlen(out_txt)));
}*/
{
	//char *in_txt =(char *)PG_GETARG_TEXT_P(0);
	char *in_txt_1 =(char *)PG_GETARG_POINTER(0);//cstring 
	int len;
	//char *out_txt;

	len =DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in_txt_1));
	
	PG_RETURN_INT32(len);//err 2
}


/*chinese_tokenize(text ) */
/*
CREATE OR REPLACE FUNCTION chinese_tokenize_v2(text) RETURNS text
AS 'nlpir_parser.dll', 'chinese_tokenize_v2'
LANGUAGE c VOLATILE;

or

CREATE OR REPLACE FUNCTION chinese_tokenize_v2(cstring) RETURNS text
AS 'nlpir_parser.dll', 'chinese_tokenize_v2'
LANGUAGE c VOLATILE;

or
CREATE OR REPLACE FUNCTION chinese_tokenize_v2(cstring) RETURNS cstring
AS 'nlpir_parser.dll', 'chinese_tokenize_v2'
LANGUAGE c VOLATILE;

*/



/*
CREATE OR REPLACE FUNCTION paragraphprocess(text) RETURNS text
AS 'nlpir_parser.dll', 'ParagraphProcess'
LANGUAGE C;
*/
/*selct  ParagraphProcess('我是中国人') */

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	text	   *in = PG_GETARG_TEXT_P(0);//输入参数为文本类型,例如：巴拿马和美国都是国家地区，汉族是一个民族。
	//char *in =(char *)PG_GETARG_POINTER(0);
	int len;
	char *s;//将输入参数中的字符串指内容拷贝到此字符串指针	
	char *splitWords;
	text *out;//输出结果为文本类型
	
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//解决乱码的关键
	
	splitWords=(char *)palloc(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串
	

	NLPIR_Init(sDataPath,1,0);//初始化分词器

	splitWords= (char *)NLPIR_ParagraphProcess(s,0);//进行分词，返回为字符串指针	
	//NLPIR_Exit();//error
	out=cstring_to_text(splitWords);
	//pfree(splitWords);
	PG_RETURN_TEXT_P(out);//输出返回结果
	pfree(splitWords);
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
CREATE OR REPLACE FUNCTION  nlpirprs_start_v2(cstring) RETURNS cstring
AS 'nlpir_parser.dll', 'nlpirprs_start'
LANGUAGE c VOLATILE;

or

CREATE OR REPLACE FUNCTION  nlpirprs_start_v2(cstring) RETURNS int
AS 'nlpir_parser.dll', 'nlpirprs_start'
LANGUAGE c VOLATILE;


PG_FUNCTION_INFO_V1(nlpirprs_start1);
PGDLLEXPORT  /* Insert this line before every extension function */


//int len=0;
Datum
nlpirprs_start(PG_FUNCTION_ARGS)
{	
	
	 const  char *splitWords=NULL;//分词得到的字符串指针。初始化为空
	 char *mybuffer;//保存分词结果字符串的一块临时的动态内存空间，这个字符串指针的结尾字符将设置为\0.
	 int length;//分词结果字符串的长度，不包含最后一个结束标志符\0的长度
	 ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//text	   *in = PG_GETARG_TEXT_P(0);//输入参数为文本类型,例如：巴拿马和美国都是国家地区，汉族是一个民族。
	 pst->buffer = (char *) PG_GETARG_POINTER(0);//第一个参数，需要分析的文本。输入参数为字符类型
	
//	 int  len;
	//char *s;//将输入参数中的字符串指内容拷贝到此字符串指针	
	//size_t str_size =INIT_STR_EXT;
	//char *splitWords;//text *out;//输出结果为文本类型
	//char *splitWords = (char *)malloc(str_size);
	//*splitWords='\0';//set 1st character to null
	//const struct  result_tokenize_s *my;
	//my=tokenize_v2(in);
	//splitWords=my->splitWords_s;
	//len=my->splitWords_len;
    //text *in_text =(text *)PG_GETARG_POINTER(0);
	//char *in_text =(char *)PG_GETARG_POINTER(0);
	//char *out_txt;
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//通过text_to_cstring函数将输入参数text类型转换为以null结尾的C字符串。这是关键。!!!!
	//out_txt =(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(in_text));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok 必须强制转换，总结一下：参数中输入的为text类型哪怕表面上输入的是char类型也没能以null即\0作为结尾符。
	//out_txt=tokenize(in_txt);//error L
	//len =Int32GetDatum(strlen(out_txt));
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//  len=VARSIZE(in)-VARHDRSZ;
	//len=strlen(in);
	//  s = (char *)palloc(len+1);
	//  memcpy(VARDATA(s),VARDATA(in),len);
	//memcpy(s,in,len);
	//  *(s+len)='\0';//解决乱码的关键
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//splitWords=(char *)palloc0(4*strlen(s)+1);//为出入结果分配足够的内存空间以便保存即将分析后的字符串	
	//SET_VARSIZE(splitWords,strlen(splitWords));
	
	 NLPIR_Init(sDataPath,1,0);//初始化分词器gbk
	 //strcat_s(splitWords,str_size,NLPIR_ParagraphProcess(in,0));
	 //splitWords=(char *)NLPIR_ParagraphProcess(in,0);//进行分词，返回为字符串指针	
   // len = Int32GetDatum(strlen(splitWords));
	//NLPIR_Exit();//error
	//out=cstring_to_text(splitWords);
	//char  *in_txt =(char *) PG_GETARG_POINTER(0);
	//out_txt=(char *)tokenize(pstrdup(in_txt));//error L
	//out_txt=(char *)(DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in_text_v1)));//通过text_to_cstring函数将输入参数text类型转换为以null结尾的C字符串。这是关键。!!!!
	//  splitWords = (char *)(DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in)));
	//pst->sege =out_txt;
	  //pst->buffer = (char *) PG_GETARG_POINTER(0);
	 // pst->buffer=splitWords;
	  //const  char *splitWords=NULL;
	  splitWords=NLPIR_ParagraphProcess(pst->buffer,0);//调用分词函数，得到分词结果字符串，注意返回类型为const char*类型。这里不能保证以\0标志结尾
	  length=strlen(splitWords);//分词结果字符串的长度
	
	  if((mybuffer=(char*)malloc(length+1))==NULL)//动态分配一块连续内存的空间，长度为分词结果字符串的长度+1。多分配一个字符空间的目标是为预留一个标志符\0.这里还判断是否分配内存成功
	  {
		  ereport(NOTICE,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("malloc failed!.\n\"%s\"",""
				       )));

	  }
	  strcpy(mybuffer,splitWords);//将分词结果字符串拷贝到刚分配的指针指向的空间，但要注意结尾字符\0是不会拷贝。所以下面我们要手工设置其结尾标志符\0
	   *(mybuffer+length)='\0';//解决乱码的关键.必须将指针指向的分词结果字符串的最后一个字符设置为\0标志的结尾符.解决于20140622 13:05
	  pst->sege=mybuffer;//将分词指针指向得到的分词结果字符串的指针
	 // pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);
	 // len_ok= strlen(pst->sege);
	 // pst->len = len_ok;
	 // len = strlen(pst->sege);
	//  pst->len = len;
	 // pst->len =Int32GetDatum(strlen(pst->sege));
	 // pst->len=strlen(splitWords);
	  // pst->len = PG_GETARG_INT32(1);//Desc: length of the text 出错在这里啊。第二个参数
	   pst->len= length;//第二个参数.分词结果字符串的长度.
	  // pst->len = strlen(pst->sege);
	 // pst->len =len;
	 // len = Int32GetDatum(strlen(pst->buffer));
	 // len  += pg_mblen(splitWords);//error
	  //pst->len =54;//19,ok 我 是 中国 人 \0	 
	 //   pst->len =8;//我是,即一个汉字=4个字节，utf8
	//pst->len=17;//12/4=3个汉字，即我是中，而结果却是，我是中国
	 // pst->len=Int32GetDatum(strlen(splitWords));
	//   pst->len=Int32GetDatum(strlen(pst->sege));
	//pst->len =CALCDATASIZE(pst->buffer, strlen(splitWords));
	
	//len_1=VARSIZE(pst->buffer)-VARHDRSZ;
	 //pst->len =strlen(out_txt)*sizeof(out_txt);//eror 字符串太长
	  pst->pos = 0;

	//PG_RETURN_DATUM(DirectFunctionCall2(nlpirprs_start1,PointerGetDatum(out_txt),len));
	 PG_RETURN_POINTER(pst);
	// PG_RETURN_CSTRING(pst->buffer);
	// PG_RETURN_INT32(Int32GetDatum(strlen(pst->sege)));
	// PG_RETURN_INT32(Int32GetDatum(pst->len));
	//  PG_RETURN_CSTRING(pst->sege);
	 //  PG_RETURN_INT32(len);
	//pfree(splitWords);
	//pfree(in);
	//NLPIR_Exit();//ok

}

/*
 * functions
 */
//PG_FUNCTION_INFO_V1(nlpirprs_start1);
PGDLLEXPORT  /* Insert this line before every extension function */
Datum
nlpirprs_start1(PG_FUNCTION_ARGS)
{		
	
	//int len;
	//char *t =(char *)PG_GETARG_POINTER(0);//arg1参数1
	//char *t = "我 是 中国 人";
	
	//int len=PG_GETARG_INT32(1);
	//char  *str=(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(t));//分词得到的结果,我 是 中国 人   注意text类型没有null结尾符。
	
	//int len1 = PG_GETARG_INT32(1);
	//char *str;
	//char *str_copy;
	//str=(char *)tokenize(t);
	//str_copy=(char *)palloc(VARSIZE(str)-VARHDRSZ+1);
	//str_copy=(char *)malloc(strlen(str)+1);
	//strncpy(str_copy,str,VARSIZE(str)-VARHDRSZ);
	//strncpy(str_copy,str,strlen(str));
	//memcpy(str_copy,VARDATA(str),VARSIZE(str)-VARHDRSZ);//error
	//str_copy[VARSIZE(str)-VARHDRSZ]='\0';
	//len=strlen(str_copy);//err
	//len=PG_GETARG_INT32(1);
	/*
	 * VARSIZE is the total size of the struct in bytes.
	 */
	//char	*new_t = (char *) palloc(VARSIZE(t)-VARHDRSZ+1);

	//SET_VARSIZE(new_t, VARSIZE(t)-VARHDRSZ);

	/*
	 * VARDATA is a pointer to the data region of the struct.
	 */
	//memcpy(VARDATA(new_t),		/* destination */
	//	   VARDATA(t), /* source */
	//	   VARSIZE(t) - VARHDRSZ);		/* how many bytes */
	//PG_RETURN_TEXT_P(new_t);
	//new_t[VARSIZE(t) - VARHDRSZ]='\0';Int32GetDatum(len1)
   // memset(str_copy,'\0',len);
	

	
	// return t;
	//PG_RETURN_TEXT_P(cstring_to_text(t));

	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//初始化解析器以便分配内存空间	
	

	//pst->buffer =(char *)PG_GETARG_POINTER(0);
	//pst->len=14;//我 是 中国 人
	//pst->len = PG_GETARG_INT32(1);
	//pst->len =Int32GetDatum(strlen(str))+VARHDRSZ;
	//pst->pos = 0;
	//PG_RETURN_POINTER(pst);//return 我 是 中国

	
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//初始化解析器以便分配内存空间	
	//text *in_text =(text *)PG_GETARG_POINTER(0);
	char *in_txt =(char *)PG_GETARG_POINTER(0);
	const char *out_txt;
	int len;
	//out_txt=tokenize(text_to_cstring(in_txt));//通过text_to_cstring函数将输入参数text类型转换为以null结尾的C字符串。这是关键。!!!!
	out_txt= (char *)DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in_txt));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok 必须强制转换，总结一下：参数中输入的为text类型哪怕表面上输入的是char类型也没能以null即\0作为结尾符。
	//out_txt=tokenize(in_txt);//error L
	
	//PG_RETURN_DATUM(DirectFunctionCall2(nlpirprs_start1,PointerGetDatum(out_txt),len));
	//PG_RETURN_TEXT_P(cstring_to_text(out_txt));

	//pst->buffer =text_to_cstring(cstring_to_text(out_txt));
	//pst->len = PG_GETARG_INT32(1);
	//pst->len =strlen(out_txt);
	//pst->pos = 0;
	//PG_RETURN_POINTER(pst);
	len=strlen(out_txt);
	PG_RETURN_DATUM(DirectFunctionCall2(nlpirprs_start1,PointerGetDatum(out_txt),len));


}

/*CREATE FUNCTION nlpirprs_start(internal, int4)
RETURNS internal
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT;
*/
PG_FUNCTION_INFO_V1(nlpirprs_star5);
PGDLLEXPORT
Datum 
nlpirprs_start5(PG_FUNCTION_ARGS)
{
    ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//初始化解析器以便分配内存空间
	NLPIR_Init(sDataPath,0,0);//初始化分词器
	//pst -> s = scws;//初始化分词句柄
	pst->buffer = (char *) PG_GETARG_POINTER(0);//第一个参数，需要分析的文本
	pst->len = PG_GETARG_INT32(1);//注意参数的序号为0、1、2等
	

	pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);//进行分词，返回为字符串指针
	//(pst->buffer) = (pst->sege);

	//pst->len=strlen(pst->sege);
	pst->pos = 0;

	PG_RETURN_POINTER(pst);//return
	
	
}



PGDLLEXPORT
Datum
nlpirprs_getlexeme(PG_FUNCTION_ARGS)
{
	ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	char	  **t = (char **) PG_GETARG_POINTER(1);
	//int		   *tlen = (int *) PG_GETARG_POINTER(2);
	 int  *tlen= (int *)PG_GETARG_INT32(2);//error
	int			startpos = pst->pos;
	int			type;

	//*t = pst->buffer + pst->pos;
	*t =pst->sege + pst->pos;

	if (pst->pos < pst->len &&
	//	(pst->buffer)[pst->pos] == ' ')
	//(pst->sege)[pst->pos] == ' ')
	(pst->sege)[pst->pos] == 0x0020)
	{
		/* blank type */
		type = 12;
		/* go to the next non-space character */
		while (pst->pos < pst->len &&
	//		   (pst->buffer)[pst->pos] == ' ')
	 (pst->sege)[pst->pos] ==0x0020)
			(pst->pos)++;
	}
	else
	{
		/* word type */
		type = 3;
				 
		/* go to the next space character */
		while (pst->pos < pst->len &&
	//		   (pst->buffer)[pst->pos] != ' ')
			    (pst->sege)[pst->pos] != 0x0020)
			(pst->pos)++;
		   
	     }
	  

	*tlen = pst->pos - startpos;

	/* we are finished if (*tlen == 0) */
	//if((pst->sege)== NULL)
	if (*tlen == 0)
	//if(pst->buffer = '\0')//error null
	//	*tlen = 0;
		type = 0;
	
	//if(pst->sege='\0')
	//	    *tlen==0;

	PG_RETURN_INT32(type);
	
	
	
}


PG_FUNCTION_INFO_V1(nlpirprs_end1);//bak start function
PGDLLEXPORT
Datum
nlpirprs_end1(PG_FUNCTION_ARGS)
{
		
    ParserState *pst = (ParserState *) PG_GETARG_POINTER(0);
	pfree(pst);	
	PG_RETURN_VOID();
	//NLPIR_Exit();
}

PGDLLEXPORT
Datum
nlpirprs_end(PG_FUNCTION_ARGS)
{
		
     ParserState * parser = ( ParserState *) PG_GETARG_POINTER(0);
	// NLPIR_Exit();
	 //free(parser->buffer);
	 pfree(parser);
   // DirectFunctionCall1(nlpirprs_end1,
     //                   PointerGetDatum(parser));
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
	LexDescr2   *descr = (LexDescr2 *)palloc(sizeof(LexDescr) * (2 + 1));

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
