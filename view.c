#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "view.h"

/* Function Definitions */

OperationType check_operation(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
	return view;
    else if (strcmp(argv[1], "-e") == 0)
	return edit; 
    return unsupported;
}

Status read_validate_file_name(char *argv[], View *viewInfo)
{
    if (strstr(argv[2], ".mp3"))
	viewInfo -> file_name = argv[2]; 
    else
	return v_failure;

    return v_success;
}

Status open_file(View *viewInfo)
{
    viewInfo -> fptr_file = fopen(viewInfo -> file_name, "r");

    if (viewInfo -> fptr_file == NULL)
    {
	perror("fopen");
	fprintf(stderr, "ERROR: unable to open file %s\n", viewInfo -> file_name);
	return v_failure;
    }

    return v_success;
}

Status do_checks(View *viewInfo)
{
    if (open_file(viewInfo) == v_success)
	printf("Open files is successfull\n");
    else
    {
	printf("open files was a failure\n");
	return v_failure;
    }

    if (check_id3_ver(viewInfo) == v_success)
	printf("Check for version and ID3 was successfull\n");
    else
    {
	printf("Check for version and ID3 was a failure\n");
	return v_failure;
    }

    return v_success;
}

Status check_id3_ver(View *viewInfo)
{
    char buff_id3[3], buff_ver[2];

    fread(buff_id3, 1, 3, viewInfo->fptr_file);

    if (strcmp(buff_id3, "ID3"))
	return v_failure;

    fread(buff_ver, 1, 2, viewInfo->fptr_file);

    if (buff_ver[0] != 3)
	return v_failure;

    return v_success;
}

Status display_info(View *viewInfo)
{
    rewind(viewInfo -> fptr_file);   
    fseek(viewInfo -> fptr_file, 6, SEEK_SET);
    viewInfo -> tag_size = get_size(viewInfo -> fptr_file);
    
    printf("Title = %s\n", get_tag(viewInfo, "TIT2"));
    printf("Artist = %s\n", get_tag(viewInfo, "TPE1"));
    printf("ALbum = %s\n", get_tag(viewInfo, "TALB"));
    printf("Year = %s\n", get_tag(viewInfo, "TYER"));
    printf("Content type = %s\n", get_tag(viewInfo, "TCON"));
    printf("Comment = %s\n", get_tag(viewInfo, "COMM"));
    
    return v_success;
}

int get_size(FILE *fptr)
{
    char s;
    int i, size = 0;
    for (i = 0; i < 4; i++)
    {
	fread(&s, 1, 1, fptr);
	size = (size << 8) | s;
    }	
    return size;
}

char * get_tag(View *viewInfo, char *tag)
{
    char bufftag[4]; 
    int size, flag = 1;    

    rewind(viewInfo -> fptr_file);
    fseek(viewInfo -> fptr_file, 10, SEEK_SET);
    fread(bufftag, 1, 4, viewInfo -> fptr_file);
    size = get_size(viewInfo -> fptr_file);
    fseek(viewInfo -> fptr_file, 2, SEEK_CUR);
    
    while(flag)
    {
	if (strcmp(bufftag, tag) == 0)
	{
	    viewInfo -> tag = malloc(size);
	    fseek(viewInfo -> fptr_file, 1, SEEK_CUR);
	    fread(viewInfo -> tag, size - 1, 1, viewInfo -> fptr_file);
	    flag = 0;
	}
	else 
	{
	    fseek(viewInfo -> fptr_file, size, SEEK_CUR);
	    fread(bufftag, 1, 4, viewInfo -> fptr_file);
	    size = get_size(viewInfo -> fptr_file);
	    fseek(viewInfo -> fptr_file, 2, SEEK_CUR);
	}
	
	if (ftell(viewInfo -> fptr_file) >= viewInfo -> tag_size)
	{
	    viewInfo -> tag = NULL;
	    break;
	}
    }	 
    return viewInfo -> tag;
}
