#include "postgres.h"
#include "miscadmin.h"
#include "fmgr.h"
#include "utils/geo_decls.h"	/* for point type */
#include "utils/builtins.h"//cstring_to_text

#include "NLPIR_C.h"
#pragma comment(lib,"NLPIR.lib")

#ifdef PG_MODULE_MAGIC
	PG_MODULE_MAGIC;
#endif


const char *sDataPath="C:\\Program Files\\PostgreSQL\\9.3\\share\\tsearch_data";

PG_FUNCTION_INFO_V1(ParagraphProcess);
PGDLLEXPORT
Datum
ParagraphProcess(PG_FUNCTION_ARGS)
{
	text *in=PG_GETARG_TEXT_P(0);
	int len;
	char *s;
	const char *splitWords;
	text *out=NULL;


	len=VARSIZE(in)-VARHDRSZ;
	s = (char *)palloc(len+1);
	memcpy(s,VARDATA(in),len);
	*(s+len)='\0';//解决乱码的关键
	
	splitWords=(char *)palloc(4*strlen(s)+1);
	
	NLPIR_Init(sDataPath,1,0);

	splitWords=NLPIR_ParagraphProcess(s,0);
	
	out= cstring_to_text(splitWords);
	//pfree(splitWords);
	PG_RETURN_TEXT_P(out);


	/*
	bool endl=true;
	HINSTANCE hinstLib;
	MYPROC1 ProcAddr1;
	MYPROC2 ProcAddr2;
	int fFreeResult;
	bool fRunTimeLinkSuccess=FALSE;
	char *splitWords=(char *)palloc(4*strlen(s)+1);
	text *out;
	//Get a handle to the DLL module
	hinstLib=LoadLibrary("ICTCLAS.dll")
	//IF the handle is valid ,try to get the function address.
	if(hinstLib!=NULL)
	{
		ProcAddr1=(MYPROC1)GetProcAddress(hinstLib,"Init");
		if(fRunTimeLinkSuccess=(ProcAddr1!=NULL))
		{
			(ProcAddr2)(s,splitWords);
		}
	}
	//Free the DLL module.
	fFreeResult=FreeLibrary(hinstLib);
	{
		out=char2text(splitWords);
		pfree(splitWords);
		PG_RETURN_TEXT_P(out);
	}
	*/

}
/*
CREATE FUNCTION ParagraphProcess(text) RETURNS  text
	AS '$lib/TSearch2','ParagraphProcess'
	LANGUAGE 'C' with (isstrict,iscachable);
*/

/*

CREATE FUNCTION ParagraphProcess(text) RETURNS  text
	AS 'nlpir_parser.dll','ParagraphProcess'
	LANGUAGE C;
	*/

/*test ok 末尾再也没能乱码啦!注意代码32行
select ParagraphProcess('我是中国人，我生于湖北省江陵县');
*/
