#ifndef __NPIR_H__
#define __NPIR_H__
//以下头文件必须加上，不然/TC即以c编译时会出错。找了好久才查找到此原因。
#include <windows.h>


#ifdef NLPIR_EXPORTS
#define NLPIR_API __declspec(dllexport)
#else
#define NLPIR_API __declspec(dllimport)
#endif

/*
#ifdef OS_LINUX
	#define  extern "C" __declspec(dllimport)  
#else
#ifdef NLPIR_EXPORTS
#define __declspec(dllimport) extern "C"  __declspec(dllexport)
#else
#define __declspec(dllimport) extern "C" __declspec(dllimport)
#endif
#endif
#if defined(ICTCLAS_JNI_EXPORTS)||defined(KEYEXTRACT_EXPORTS)||defined(NLPIR_JNI_EXPORTS)||defined(LJSUMMARY_EXPORTS)||defined(LJSUMMARY_JNI_EXPORTS)||defined(DOCEXTRACTOR_EXPORTS)
	#define __declspec(dllimport) 
#endif
*/



//以下是通用于/TC和/TP即，编译为C代码和编译为C＋＋代码的头文件。网上查找资料得知。
#ifdef __cplusplus 
extern "C" { 
#endif

//一段代码

//#define  NLPIR_API  __declspec(dllimport)
/*
#define POS_MAP_NUMBER 4 //add by qp 2008.11.25
#define ICT_POS_MAP_FIRST 1  //计算所一级标注集
#define ICT_POS_MAP_SECOND 0 //计算所二级标注集
#define PKU_POS_MAP_SECOND 2 //北大二级标注集
#define PKU_POS_MAP_FIRST 3	//北大一级标注集
#define  POS_SIZE 40
	
#define GBK_CODE 0//默认支持GBK编码
#define UTF8_CODE GBK_CODE+1//UTF8编码
#define BIG5_CODE GBK_CODE+2//BIG5编码
#define GBK_FANTI_CODE GBK_CODE+3//GBK编码，里面包含繁体字
*/
#define  POS_SIZE 40

	/* data structures */
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

NLPIR_API  int NLPIR_Init(const char *sDataPath＝0,int encode＝0,char *sLicenceCode＝0);

NLPIR_API  BOOL NLPIR_Exit();

NLPIR_API const char *NLPIR_ParagraphProcess(const char *sParagraph, int bPOStagged＝1);
//NLPIR_API text *NLPIR_ParagraphProcess(const char *sParagraph, int bPOStagged＝1);

//NLPIR_API const result_t *NLPIR_ParagraphProcessA(const char *sParagraph,int *pResultCount,BOOL bUserDict);注意vc2010实现的是c89标准，没有bool这个类型，所以要改用int类型代替
NLPIR_API const struct result_t *NLPIR_ParagraphProcessA(const char *sParagraph,int *pResultCount, int bUserDice);

#ifdef __cplusplus 
} 
#endif 
#endif
