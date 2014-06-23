/*
#include<stdio.h>
#include<string.h>
*/
/*http://www.programmingsimplified.com/c/source-code/c-program-copy-strings*/

/*
main()
{
   char source[] = "C program";
   char destination[50];
 
   strcpy(destination, source);
 
   printf("Source string: %s\n", source);
   printf("Destination string: %s\n", destination);
 
   return 0;
}
*/

#include<stdio.h>
 
void copy_string(char*, char*);
 
main()
{
    char source[100], target[100];
 
    printf("Enter source string\n");
    gets(source);
 
    copy_string(target, source);
 
    printf("Target string is \"%s\"\n", target);
 
	getchar();
    return 0;
}
 
void copy_string(char *target, char *source)
{
   while(*source)
   {
      *target = *source;
      source++;
      target++;
   }
   *target = '\0';
}
