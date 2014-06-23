
#if !defined(__NLPIR_2013_LIB_INCLUDED__)
#define __NLPIR_2013_LIB_INCLUDED__

/*
#ifdef OS_LINUX
	#define  extern "C" NLPIR_API  
#else
#ifdef NLPIR_EXPORTS
#define NLPIR_API extern "C"  __declspec(dllexport)
#else
#define NLPIR_API extern "C" __declspec(dllimport)
#endif
#endif
#if defined(ICTCLAS_JNI_EXPORTS)||defined(KEYEXTRACT_EXPORTS)||defined(NLPIR_JNI_EXPORTS)||defined(LJSUMMARY_EXPORTS)||defined(LJSUMMARY_JNI_EXPORTS)||defined(DOCEXTRACTOR_EXPORTS)
	#define NLPIR_API 
#endif
*/
#ifdef __cplusplus 
extern "C" { 
#endif

//一段代码

#define NLPIR_API  __declspec(dllimport)


//////////////////////////////////////////////////////////////////////////
//
//以下定义为兼容ICTCLAS以前的版本
//
//////////////////////////////////////////////////////////////////////////
#define ICTCLAS_Init NLPIR_Init
#define ICTCLAS_Exit NLPIR_Exit
#define ICTCLAS_ImportUserDict  NLPIR_ImportUserDict
#define ICTCLAS_FileProcess NLPIR_FileProcess
#define ICTCLAS_ParagraphProcess  NLPIR_ParagraphProcess 
#define ICTCLAS_ParagraphProcessA  NLPIR_ParagraphProcessA 
#define ICTCLAS_GetParagraphProcessAWordCount   NLPIR_GetParagraphProcessAWordCount 
#define ICTCLAS_ParagraphProcessAW   NLPIR_ParagraphProcessAW 
#define ICTCLAS_AddUserWord  NLPIR_AddUserWord
#define ICTCLAS_SaveTheUsrDic  NLPIR_SaveTheUsrDic
#define ICTCLAS_DelUsrWord  NLPIR_DelUsrWord
#define ICTCLAS_GetUniProb  NLPIR_GetUniProb
#define ICTCLAS_IsWord  NLPIR_IsWord
#define ICTCLAS_SetPOSmap  NLPIR_SetPOSmap
#define CICTCLAS  CNLPIR
#define GetActiveICTCLAS GetActiveInstance 


#define POS_MAP_NUMBER 4 //add by qp 2008.11.25
#define ICT_POS_MAP_FIRST 1  //计算所一级标注集
#define ICT_POS_MAP_SECOND 0 //计算所二级标注集
#define PKU_POS_MAP_SECOND 2 //北大二级标注集
#define PKU_POS_MAP_FIRST 3	//北大一级标注集
#define  POS_SIZE 40

struct result_t{
  int start; //start position,词语在输入句子中的开始位置
  int length; //length,词语的长度
  char  sPOS[POS_SIZE];//word type，词性ID值，可以快速的获取词性表
  int	iPOS;//词性标注的编号
  int word_ID; //该词的内部ID号，如果是未登录词，设成0或者-1
  int word_type; //区分用户词典;1，是用户词典中的词；0，非用户词典中的词
  int weight;//word weight,read weight
 };

#define GBK_CODE 0//默认支持GBK编码
#define UTF8_CODE GBK_CODE+1//UTF8编码
#define BIG5_CODE GBK_CODE+2//BIG5编码
#define GBK_FANTI_CODE GBK_CODE+3//GBK编码，里面包含繁体字

NLPIR_API int NLPIR_Init(const char * sDataPath=0,int encode=GBK_CODE,const char*sLicenceCode=0);

NLPIR_API bool NLPIR_Exit();

NLPIR_API const char *NLPIR_ParagraphProcess(const char *sParagraph,int bPOStagged=1);

NLPIR_API void NLPIR_ReleaseBuf(const char *sBuf);

NLPIR_API const result_t * NLPIR_ParagraphProcessA(const char *sParagraph,int *pResultCount,bool bUserDict=true);


NLPIR_API int NLPIR_GetParagraphProcessAWordCount(const char *sParagraph);

NLPIR_API void NLPIR_ParagraphProcessAW(int nCount,result_t * result);


NLPIR_API double NLPIR_FileProcess(const char *sSourceFilename,const char *sResultFilename,int bPOStagged=1);


NLPIR_API unsigned int NLPIR_ImportUserDict(const char *sFilename);


NLPIR_API int NLPIR_SaveTheUsrDic();


NLPIR_API int NLPIR_DelUsrWord(const char *sWord);


NLPIR_API double NLPIR_GetUniProb(const char *sWord);

NLPIR_API bool NLPIR_IsWord(const char *sWord);


NLPIR_API const char * NLPIR_GetKeyWords(const char *sLine,int nMaxKeyLimit=50,bool bWeightOut=false);


NLPIR_API const char * NLPIR_GetFileKeyWords(const char *sFilename,int nMaxKeyLimit=50,bool bWeightOut=false);

NLPIR_API const char * NLPIR_GetNewWords(const char *sLine,int nMaxKeyLimit=50,bool bWeightOut=false);


NLPIR_API const char * NLPIR_GetFileNewWords(const char *sFilename,int nMaxKeyLimit=50,bool bWeightOut=false);

NLPIR_API unsigned long NLPIR_FingerPrint(const char *sLine);


NLPIR_API int NLPIR_SetPOSmap(int nPOSmap);

#ifdef OS_LINUX
class  CNLPIR {
#else
class  __declspec(dllexport) CNLPIR {
#endif
	public:
		CNLPIR();
		~CNLPIR();
		double FileProcess(const char *sSourceFilename,const char *sResultFilename,int bPOStagged=1);
		//Process a file，类似于C下的NLPIR_FileProcess
		const char * ParagraphProcess(const char *sLine,int bPOStagged=1); 
		//Process a file，类似于C下的NLPIR_ParagraphProcess
		const result_t * ParagraphProcessA(const char *sParagraph,int *pResultCount,bool bUserDict=true);
		//Process a file，类似于C下的NLPIR_ParagraphProcessA

		void ParagraphProcessAW(int nCount,result_t * result);
		int GetParagraphProcessAWordCount(const char *sParagraph);

		const char * GetKeyWords(const char *sLine,int nMaxKeyLimit,bool bWeightOut);
		//获取关键词
		const char * GetFileKeyWords(const char *sFilename,int nMaxKeyLimit,bool bWeightOut);
		//从文本文件中获取关键词
		const char * GetNewWords(const char *sFilename,int nMaxKeyLimit,bool bWeightOut);
		//获取新词
		const char * GetFileNewWords(const char *sFilename,int nMaxKeyLimit,bool bWeightOut);
		//从文本文件中获取新词

		bool SetAvailable(bool bAvailable=true);//当前线程释放该类，可为下一个线程使用
		bool IsAvailable();//判断当前分词器是否被线程占用
		unsigned int GetHandle()
		{
			return m_nHandle;
		}
private:
		unsigned int m_nHandle;//该成员作为该类的Handle值，由系统自动分配，用户不可修改
		bool m_bAvailable;//该成员作为多线程共享控制的参数，由系统自动分配，用户不可修改

};


NLPIR_API CNLPIR* GetActiveInstance();


NLPIR_API bool NLPIR_NWI_Start();
NLPIR_API int  NLPIR_NWI_AddFile(const char *sFilename);

NLPIR_API bool NLPIR_NWI_AddMem(const char *sText);

NLPIR_API bool NLPIR_NWI_Complete();
NLPIR_API const char * NLPIR_NWI_GetResult(bool bWeightOut=false);
NLPIR_API unsigned int  NLPIR_NWI_Result2UserDict();//新词识别结果转为用户词典,返回新词结果数目

#ifdef __cplusplus 
} 
#endif 
#endif//#define __NLPIR_LIB_INCLUDED__
