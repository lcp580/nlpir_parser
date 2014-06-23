#include<stdio.h>
#include<conio.h>

int copystr ( char *, char *);
main( )
{
    char source[20] ; 
    char destination[20] ;
    printf("Enter the string you want to copy in destination array\n");
    gets(source);
    copystr ( destination, source ) ;
    printf ( "\nsource string = %s", source ) ;
    printf ( "\nDestination string = %s", destination ) ;
    getch();
}

int copystr ( char *d, char *b )
{
     while ( *b != '\0' )
     {
          *d = *b ;
          b++ ;
          d++ ;
     }
     *d = '\0' ;
}