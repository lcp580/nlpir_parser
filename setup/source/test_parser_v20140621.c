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
//typedef struct ParserState  ParserState;
/*
typedef struct 
{
	char	   *buffer;			/* text to parse */
//	int			len;			/* length of the text in buffer */
//	int			pos;			/* position of the parser */
//	char        *sege;    //��ʱ����ִʵĽ����
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
struct result_t//�ִʵĽ����
{
  int start; //start position,��������������еĿ�ʼλ��
  int length; //length,����ĳ���
  char  sPOS[POS_SIZE];//word type������IDֵ�����Կ��ٵĻ�ȡ���Ա�
  int	iPOS;//���Ա�ע�ı��
  int word_ID; //�ôʵ��ڲ�ID�ţ������δ��¼�ʣ����0����-1
  int word_type; //�����û��ʵ�;1�����û��ʵ��еĴʣ�0�����û��ʵ��еĴ�
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

//const char *in="�����й��ˣ������Ժ���ʡ������";
const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
//static int handle=1;

const char *sResult;//���صķ�������ı��������ÿո�ָ�����һ���ַ�����ָ��
const char *splitWords2;

int len_ok=0;
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
static const char  *
tokenize(const char *intext,int tokentext_length)
{
	
//char *sResult=NULL;����������ĳ�������������.���صĽ�������Ϊȫ�ֱ����ŵ���������
NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

sResult=NLPIR_ParagraphProcess(intext,0);//ֱ����������intext�����ᵼ�·ִʽ������������.

// =strlen(sResult);
tokentext_length=strlen(sResult);

return sResult;//���Ա�����������������ִʺ���

}
/*
const struct  result_tokenize_s *tokenize_v2(const char *intext)
{
	
	struct result_tokenize_s *pVecResult;
//char *sResult=NULL;����������ĳ�������������.���صĽ�������Ϊȫ�ֱ����ŵ���������
NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

(pVecResult->splitWords_s)=(char *)NLPIR_ParagraphProcess(intext,0);//ֱ����������intext�����ᵼ�·ִʽ������������.

// =strlen(sResult);
(pVecResult->splitWords_len)=Int32GetDatum(strlen(pVecResult->splitWords_s));

return pVecResult;//���Ա�����������������ִʺ���

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
//const char *sResult=NULL;����������ĳ�������������.���صĽ�������Ϊȫ�ֱ����ŵ���������
NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

//sResult=NLPIR_ParagraphProcess(intext,0);//ֱ����������intext�����ᵼ�·ִʽ������������.

// =strlen(sResult);
//tokentext_length=strlen(sResult);
my->splitWords_s= NLPIR_ParagraphProcess(intext,0);
//tokentext_length =strlen(my->splitWords_s);
my->splitWords_len= strlen(my->splitWords_s);
//return sResult;//���Ա�����������������ִʺ���

return my;
}

*/



/*chinese_tokenize(text ) */
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
	//int len;
	//char *s;//����������е�ԭʼ�ַ������Ƶ����s�ַ���ָ���У��ǵ�Ϊ������ڴ�ռ䲢��������ַ���׼Ϊ\0
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
	memcpy(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);//��srcָ���ַΪ��ʼ��ַ������n���ֽڵ����ݸ��Ƶ���destinָ���ַΪ��ʼ��ַ�Ŀռ��ڡ���strcpy��ȣ�memcpy����������'\0'�ͽ���������һ���´����n���ֽڡ�
	//(s,VARDATA(in),VARSIZE(in)-VARHDRSZ);
	// StrNCpy(s,in,strlen(in));
	//s=strndup2(in);
	s[VARSIZE(in)-VARHDRSZ]='\0';//�������Ĺؼ�
//	*(s+strlen(in))='\0';
	//splitWords=(char *)palloc(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���
	
	//splitWords=tokenize(s);
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	splitWords =NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��	


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
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//ͨ��text_to_cstring�������������text����ת��Ϊ��null��β��C�ַ��������ǹؼ���!!!!
	//out_txt =(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(in_text));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok ����ǿ��ת�����ܽ�һ�£������������Ϊtext�������±������������char����Ҳû����null��\0��Ϊ��β����
	//out_txt=tokenize(in_txt);//error L
	//len =Int32GetDatum(strlen(out_txt));
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	char *in_txt =(char *)PG_GETARG_POINTER(0);//cstring 
	char *out_txt;	
	int len_txt=0;
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//ͨ��text_to_cstring�������������text����ת��Ϊ��null��β��C�ַ��������ǹؼ���!!!!
	//out_txt =(char *)tokenize(in_txt);//ok
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִʺ���

    out_txt =(char *)NLPIR_ParagraphProcess(in_txt,0);//ֱ����������intext�����ᵼ�·ִʽ������������.
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
	char *s;//��ʱ�洢���ƺ�������ַ�������\0��β
	int  len;
	char *out_txt;

	len = VARSIZE(in_txt)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in_txt),len);
	*(s+len)='\0';//�������Ĺؼ�	
	
	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	out_txt= (char *)NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��	

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
/*selct  ParagraphProcess('�����й���') */

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	text	   *in = PG_GETARG_TEXT_P(0);//�������Ϊ�ı�����,���磺��������������ǹ��ҵ�����������һ�����塣
	//char *in =(char *)PG_GETARG_POINTER(0);
	int len;
	char *s;//����������е��ַ���ָ���ݿ��������ַ���ָ��	
	char *splitWords;
	text *out;//������Ϊ�ı�����
	
	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//�������Ĺؼ�
	
	splitWords=(char *)palloc(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���
	

	NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���

	splitWords= (char *)NLPIR_ParagraphProcess(s,0);//���зִʣ�����Ϊ�ַ���ָ��	
	//NLPIR_Exit();//error
	out=cstring_to_text(splitWords);
	//pfree(splitWords);
	PG_RETURN_TEXT_P(out);//������ؽ��
	pfree(splitWords);
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
	
	 const  char *splitWords=NULL;//�ִʵõ����ַ���ָ�롣��ʼ��Ϊ��
	 char *mybuffer;//����ִʽ���ַ�����һ����ʱ�Ķ�̬�ڴ�ռ䣬����ַ���ָ��Ľ�β�ַ�������Ϊ\0.
	 int length;//�ִʽ���ַ����ĳ��ȣ����������һ��������־��\0�ĳ���
	 ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//text	   *in = PG_GETARG_TEXT_P(0);//�������Ϊ�ı�����,���磺��������������ǹ��ҵ�����������һ�����塣
	 pst->buffer = (char *) PG_GETARG_POINTER(0);//��һ����������Ҫ�������ı����������Ϊ�ַ�����
	
//	 int  len;
	//char *s;//����������е��ַ���ָ���ݿ��������ַ���ָ��	
	//size_t str_size =INIT_STR_EXT;
	//char *splitWords;//text *out;//������Ϊ�ı�����
	//char *splitWords = (char *)malloc(str_size);
	//*splitWords='\0';//set 1st character to null
	//const struct  result_tokenize_s *my;
	//my=tokenize_v2(in);
	//splitWords=my->splitWords_s;
	//len=my->splitWords_len;
    //text *in_text =(text *)PG_GETARG_POINTER(0);
	//char *in_text =(char *)PG_GETARG_POINTER(0);
	//char *out_txt;
	//out_txt=(char *)tokenize(text_to_cstring(in_text));//ͨ��text_to_cstring�������������text����ת��Ϊ��null��β��C�ַ��������ǹؼ���!!!!
	//out_txt =(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(in_text));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok ����ǿ��ת�����ܽ�һ�£������������Ϊtext�������±������������char����Ҳû����null��\0��Ϊ��β����
	//out_txt=tokenize(in_txt);//error L
	//len =Int32GetDatum(strlen(out_txt));
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//  len=VARSIZE(in)-VARHDRSZ;
	//len=strlen(in);
	//  s = (char *)palloc(len+1);
	//  memcpy(VARDATA(s),VARDATA(in),len);
	//memcpy(s,in,len);
	//  *(s+len)='\0';//�������Ĺؼ�
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));
	//splitWords=(char *)palloc0(4*strlen(s)+1);//Ϊ�����������㹻���ڴ�ռ��Ա㱣�漴����������ַ���	
	//SET_VARSIZE(splitWords,strlen(splitWords));
	
	 NLPIR_Init(sDataPath,1,0);//��ʼ���ִ���gbk
	 //strcat_s(splitWords,str_size,NLPIR_ParagraphProcess(in,0));
	 //splitWords=(char *)NLPIR_ParagraphProcess(in,0);//���зִʣ�����Ϊ�ַ���ָ��	
   // len = Int32GetDatum(strlen(splitWords));
	//NLPIR_Exit();//error
	//out=cstring_to_text(splitWords);
	//char  *in_txt =(char *) PG_GETARG_POINTER(0);
	//out_txt=(char *)tokenize(pstrdup(in_txt));//error L
	//out_txt=(char *)(DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in_text_v1)));//ͨ��text_to_cstring�������������text����ת��Ϊ��null��β��C�ַ��������ǹؼ���!!!!
	//  splitWords = (char *)(DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in)));
	//pst->sege =out_txt;
	  //pst->buffer = (char *) PG_GETARG_POINTER(0);
	 // pst->buffer=splitWords;
	  //const  char *splitWords=NULL;
	  splitWords=NLPIR_ParagraphProcess(pst->buffer,0);//���÷ִʺ������õ��ִʽ���ַ�����ע�ⷵ������Ϊconst char*���͡����ﲻ�ܱ�֤��\0��־��β
	  length=strlen(splitWords);//�ִʽ���ַ����ĳ���
	
	  if((mybuffer=(char*)malloc(length+1))==NULL)//��̬����һ�������ڴ�Ŀռ䣬����Ϊ�ִʽ���ַ����ĳ���+1�������һ���ַ��ռ��Ŀ����ΪԤ��һ����־��\0.���ﻹ�ж��Ƿ�����ڴ�ɹ�
	  {
		  ereport(NOTICE,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("malloc failed!.\n\"%s\"",""
				       )));

	  }
	  strcpy(mybuffer,splitWords);//���ִʽ���ַ����������շ����ָ��ָ��Ŀռ䣬��Ҫע���β�ַ�\0�ǲ��´����������������Ҫ�ֹ��������β��־��\0
	   *(mybuffer+length)='\0';//�������Ĺؼ�.���뽫ָ��ָ��ķִʽ���ַ��������һ���ַ�����Ϊ\0��־�Ľ�β��.�����20140622 13:05
	  pst->sege=mybuffer;//���ִ�ָ��ָ��õ��ķִʽ���ַ�����ָ��
	 // pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);
	 // len_ok= strlen(pst->sege);
	 // pst->len = len_ok;
	 // len = strlen(pst->sege);
	//  pst->len = len;
	 // pst->len =Int32GetDatum(strlen(pst->sege));
	 // pst->len=strlen(splitWords);
	  // pst->len = PG_GETARG_INT32(1);//Desc: length of the text ���������ﰡ���ڶ�������
	   pst->len= length;//�ڶ�������.�ִʽ���ַ����ĳ���.
	  // pst->len = strlen(pst->sege);
	 // pst->len =len;
	 // len = Int32GetDatum(strlen(pst->buffer));
	 // len  += pg_mblen(splitWords);//error
	  //pst->len =54;//19,ok �� �� �й� �� \0	 
	 //   pst->len =8;//����,��һ������=4���ֽڣ�utf8
	//pst->len=17;//12/4=3�����֣��������У������ȴ�ǣ������й�
	 // pst->len=Int32GetDatum(strlen(splitWords));
	//   pst->len=Int32GetDatum(strlen(pst->sege));
	//pst->len =CALCDATASIZE(pst->buffer, strlen(splitWords));
	
	//len_1=VARSIZE(pst->buffer)-VARHDRSZ;
	 //pst->len =strlen(out_txt)*sizeof(out_txt);//eror �ַ���̫��
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
	//char *t =(char *)PG_GETARG_POINTER(0);//arg1����1
	//char *t = "�� �� �й� ��";
	
	//int len=PG_GETARG_INT32(1);
	//char  *str=(char *)DirectFunctionCall1(chinese_tokenize,PointerGetDatum(t));//�ִʵõ��Ľ��,�� �� �й� ��   ע��text����û��null��β����
	
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

	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//��ʼ���������Ա�����ڴ�ռ�	
	

	//pst->buffer =(char *)PG_GETARG_POINTER(0);
	//pst->len=14;//�� �� �й� ��
	//pst->len = PG_GETARG_INT32(1);
	//pst->len =Int32GetDatum(strlen(str))+VARHDRSZ;
	//pst->pos = 0;
	//PG_RETURN_POINTER(pst);//return �� �� �й�

	
	//ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//��ʼ���������Ա�����ڴ�ռ�	
	//text *in_text =(text *)PG_GETARG_POINTER(0);
	char *in_txt =(char *)PG_GETARG_POINTER(0);
	const char *out_txt;
	int len;
	//out_txt=tokenize(text_to_cstring(in_txt));//ͨ��text_to_cstring�������������text����ת��Ϊ��null��β��C�ַ��������ǹؼ���!!!!
	out_txt= (char *)DirectFunctionCall1(chinese_tokenize_v2,PointerGetDatum(in_txt));
	//out_txt=tokenize(text_to_cstring(((text *)in_txt)));//ok ����ǿ��ת�����ܽ�һ�£������������Ϊtext�������±������������char����Ҳû����null��\0��Ϊ��β����
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
    ParserState *pst = (ParserState *) palloc0(sizeof(ParserState));//��ʼ���������Ա�����ڴ�ռ�
	NLPIR_Init(sDataPath,0,0);//��ʼ���ִ���
	//pst -> s = scws;//��ʼ���ִʾ��
	pst->buffer = (char *) PG_GETARG_POINTER(0);//��һ����������Ҫ�������ı�
	pst->len = PG_GETARG_INT32(1);//ע����������Ϊ0��1��2��
	

	pst->sege=(char *)NLPIR_ParagraphProcess(pst->buffer,0);//���зִʣ�����Ϊ�ַ���ָ��
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
	char *myintext="�����й��ˣ������Ժ���ʡ������";
	//printf("%s\n",ParagraphProcess2(myintext));
	printf("%s\n",tokenize(myintext));
	return 0;
}
*/
