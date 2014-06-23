/*http://see.xidian.edu.cn/cpp/html/1572.html*/
# include <stdio.h>
# include <string.h>

int  main ()
{
  //  char * source_str = "THIS IS THE SOURCE STRING" ;
	char  * source_str="我是中国人我来自中国湖北省江陵县";
    char dest_strl[40]= {0}, dest_str2[40]= {0};
    /* Use strncpy() to copy only the first 11 characters.*/
    strncpy(dest_strl, source_str, strlen(source_str));
    printf("How about that! dest-strl is now: '%s'!!!\n", dest_strl);
    /* Now, use strncpy() to copy only the last 13 characters. */
    strncpy(dest_strl, source_str + (strlen(source_str)-13) , 13);
    printf("Whoa! dest_str2 is now: '%s'!!!\n",dest_str2);
	getchar();	
	return 0;
}