// win_cDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "postgres.h"
#include "fmgr.h"
#include "executor/executor.h"	/* for GetAttributeByName() */
#include "utils/geo_decls.h"	/* for point type */

#include "NLPIR_C.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#pragma comment(lib , "NLPIR.lib")

//#define VARATT_SIZEP(_PTR)		\
//		VARATT_SIZEP_DEPRECATED(PTR)
//#define NLPIR_PARSER_API extern "C" __declspec(dllexport)

#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif

const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";
static void init()
{
if(!NLPIR_Init(sDataPath,1,0))//�����ڵ�ǰ��·���£�Ĭ��ΪGBK����ķִ�
	{
		//printf("ICTCLAS INIT FAILED!\n");
		//return;
		ereport(ERROR,
				(errcode(ERRCODE_INTERNAL_ERROR),
				 errmsg("Chinese Parser Lib SCWS could not init!\"%s\"",""
				       )));
	}
}
PG_FUNCTION_INFO_V1(SplitGBK);
PGDLLEXPORT
Datum
SplitGBK(PG_FUNCTION_ARGS)
{//�ִ���ʾ
    
	const char *sample1=PG_GETARG_CSTRING(0);
	const char *sResult1=PG_GETARG_CSTRING(0);
	//text *sResult1=PG_GETARG_TEXT_P(0);
	init();
	//��ʼ���ִ����
	
	//NLPIR_SetPOSmap(PKU_POS_MAP_SECOND);//���ô��Ա�ע���ϵ����ͣ�Ĭ��Ϊ������������ע��ϵ
	//sample1 = "���ĳɳ��ķ���ѧϰ�����й�֤ȯ�г�";
	sResult1=NLPIR_ParagraphProcess(sample1,1); 
	printf("%s\n", sResult1); 	
	 //PG_RETURN_TEXT_P(sResult1);
	 PG_RETURN_CSTRING(sResult1);	
}

/*
int main()
{
	//char *sample2=PG_GETARG_CSTRING(0);
	//SplitGBK(sample2);
	//printf(sDataPath);
	printf("test nlpir 2014 ok!");
	return 0;
}
*/

//Datum hello( PG_FUNCTION_ARGS );

PG_FUNCTION_INFO_V1( hello );
PGDLLEXPORT
Datum
hello( PG_FUNCTION_ARGS )
{
   // variable declarations
   char greet[] = "Hello, ";
   text *towhom;
   int greetlen;
   int towhomlen;
   text *greeting;
   int  greeting_size;

   // Get arguments.  If we declare our function as STRICT, then
   // this check is superfluous.
   if(PG_ARGISNULL(0)) {
      PG_RETURN_NULL();
   }
   towhom = PG_GETARG_TEXT_P(0);

   // Calculate string sizes.
    greetlen = strlen(greet);  
   towhomlen = VARSIZE(towhom)-VARHDRSZ;

   // Allocate memory and set data structure size.
   greeting = (text *)palloc( greetlen + towhomlen );
   //VARATT_SIZEP( greeting ) = greetlen + towhomlen  + VARHDRSZ;//postgres 7.4
   greeting_size=greetlen+towhomlen+VARHDRSZ;
   SET_VARSIZE(greeting,greeting_size);

   // Construct greeting string.
   strncpy( VARDATA(greeting), greet, greetlen );
   strncpy( VARDATA(greeting) + greetlen,
            VARDATA(towhom),
            towhomlen );

   PG_RETURN_TEXT_P( greeting );
}
