/* strndup.c
 *https://code.google.com/p/madp-win/source/browse/src/argp-standalone-1.3/strndup.c?r=2d96025e8ad4b150317ff6f0ff8d75c59a83cf97
 */

/* Written by Niels Möller <nisse@lysator.liu.se>
 *
 * This file is hereby placed in the public domain.
 */

//#include <stdlib.h>
//#include <string.h>
#include "stdio.h"
#include "strndup.h"

char *
strndup (const char *s, size_t size)
{
  char *r;
  char *end =(char *)memchr(s, 0, size);
  
  if (end)
    /* Length + 1 */
    size = end - s + 1;
  //  size=end -s + 2;
  r =(char *)malloc(size);

  if (size)
    {
      memcpy(r, s, size-1);
      r[size-1] = '\0';
    }
  return r;
}

char *strndup2(const char *s, size_t n)
{
	size_t len = strnlen(s, n);
	char *new_s = (char *) malloc((len + 1) * sizeof(char));
	if (!new_s)
		return NULL;
	new_s[len] = '\0';
	return (char *) memcpy(new_s, s, len);
}


int main()
{
	char *in="我是中国人，我来自湖北省江陵县";
	int size_in=sizeof(in);
	
	char *out=NULL;
	out=strndup2(in,size_in);
	printf("copy is %s",out);
	getchar();
	return 0;
}
