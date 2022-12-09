#include "Response.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h> 
#include <errno.h> 

char * render_static_file(char * fileName ,_Bool use_virtual_iomap) {
    if  (use_virtual_iomap == true ) 
    {
        return mmfs_rendering(fileName)  ; 
    }

	FILE* file = fopen(fileName, "r");

	if (file == NULL) {
		return NULL;
	}else {
		printf("%s does exist \n", fileName);
	}

    size_t  fsize  = filesize(fileName)  ;  

	char*  contents = malloc(sizeof(char) * (fsize+1));
	char ch;
	int i = 0; 
	while((ch = fgetc(file)) != EOF) {
		contents[i] = ch;
		i++;
	}
	fclose(file);
	return contents;
}

char *mmfs_rendering ( char *fileName )  {

    int file_fd  = open ( fileName ,  O_RDONLY ) ; 
    
    if (file_fd  == -1 ) 
    {
        return NULL ; 
    } 

    int fsize = filesize(fileName) ; 
    char  *filem  = mmap ( NULL  , fsize  , PROT_READ ,  MAP_PRIVATE , file_fd, 0 ) ;
    
    if ( filem == MAP_FAILED )
    {    
        return   NULL ; 
    } 
    
    char *contents  =  malloc(sizeof(char)  * fsize +1 ) ;
    memcpy(contents ,  filem , fsize +1 ) ; 
    munmap(filem  , fsize ) ; 
    return contents ; 
}

static  int  filesize (char * filename ) { 
    //! I supposed the  file already exist 
    struct stat  statbuff  ; 
    
    if ( stat(filename , &statbuff) == -1 ) 
    {
        fprintf(stderr , "File Status Unknow \n") ;  
        perror("stat") ; 
        exit(EXIT_FAILURE) ; 
    }
     
    return  statbuff.st_size ; 
}


