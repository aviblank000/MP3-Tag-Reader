#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "edit.h"
#include "view.h"

/* Function Definitions */

Status read_validate_file_name_edit(char *argv[], Edit *editInfo)
{
    if (strstr(argv[4], ".mp3"))
        editInfo -> file_name = argv[4];
    else
        return e_failure;

    editInfo -> change_text = argv[3];
    editInfo -> size_change_text = strlen(editInfo -> change_text);

    return e_success;
}

Status open_file_edit(Edit *editInfo)
{
    editInfo -> fptr_file = fopen(editInfo -> file_name, "r+");

    if (editInfo -> fptr_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: unable to open file %s\n", editInfo -> file_name);
        return e_failure;
    }

    return e_success;
}

Status do_checks_edit(Edit *editInfo)
{
    if (open_file_edit(editInfo) == e_success)
        printf("Open files is successfull\n");
    else
    {
        printf("open files was a failure\n");
        return e_failure;
    }

    if (check_id3_ver_edit(editInfo) == e_success)
        printf("Check for version and ID3 was successfull\n");
    else
    {
        printf("Check for version and ID3 was a failure\n");
        return e_failure;
    }

    return e_success;
}

Status check_id3_ver_edit(Edit *editInfo)
{
    char buff_id3[3], buff_ver[2];

    fread(buff_id3, 1, 3,editInfo->fptr_file);

    if (strcmp(buff_id3, "ID3"))
        return e_failure;

    fread(buff_ver, 1, 2, editInfo->fptr_file);

    if (buff_ver[0] != 3)
        return e_failure;

    return e_success;
}

Status check_option(char *argv[], Edit *editInfo)
{
    char *op[] = {"-t", "-a", "-A", "-y", "-c", "-C"};
    
    for (int i = 0; i < 6; i++)
	if (strcmp(argv[2], op[i]) == 0)
	{
	    strcpy(editInfo -> op, op[i]);
	    return e_success;
	}
    
    return e_failure;
}


Status do_edit(Edit *editInfo)
{
    rewind(editInfo -> fptr_file); //  move to beginning of the file.
    fseek(editInfo -> fptr_file, 6, SEEK_SET); // skip ID3, version and flag.
    editInfo -> tag_size = get_size(editInfo -> fptr_file); // get meta container size.

    if (strcmp(editInfo -> op, "-t") == 0)
    {
	if (tag_edit(editInfo, "TIT2") == e_failure)
	    printf("Input Size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else
	    printf("Title modified\n");
    }
    
    if (strcmp(editInfo -> op, "-a") == 0)
    {
	if (tag_edit(editInfo, "TPE1") == e_failure)
	    printf("Input Size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else
	    printf("Artist name modified\n");
    }
    
    if (strcmp(editInfo -> op, "-A") == 0)
    {
	if (tag_edit(editInfo, "TALB") == e_failure)
	    printf("Input Size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else 
	    printf("Album name modified\n");
    }
    
    if (strcmp(editInfo -> op, "-y") == 0)
    {
	if (tag_edit(editInfo, "TYER") == e_failure)
	    printf("Input size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else 
	    printf("Year modified\n");
    }

    if (strcmp(editInfo -> op, "-c") == 0)
    {
	if (tag_edit(editInfo, "TCON") == e_failure)
	    printf("Input Size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else 
	    printf("Content modified\n");
    }
    
    if (strcmp(editInfo -> op, "-C") == 0)
    {
	if (tag_edit(editInfo, "COMM") == e_failure)
	    printf("Input Size exceed!!\n");
	else if (editInfo -> tag == NULL )
	    printf("Tag does not exist\n");
	else 
	    printf("Comment modified\n");
    }
    
    return e_success;
}

Status tag_edit(Edit *editInfo, char *tag)
{
    char bufftag[4], cl = 0;
    int size, flag = 1, i;

    rewind(editInfo -> fptr_file); // move to beginning of the file.
    fseek(editInfo -> fptr_file, 10, SEEK_SET); // skip 10 bytes of header content.
    fread(bufftag, 1, 4, editInfo -> fptr_file); // read 4 bytes, which is the tag.
    size = get_size(editInfo -> fptr_file); // tag size.
    fseek(editInfo -> fptr_file, 2, SEEK_CUR); // move 2 bytes of flag.

    while(flag)
    {
        if (strcmp(bufftag, tag) == 0) // check tag.
        {
	    if (size > editInfo -> size_change_text) // checking the tag size and input size.
	    {
		fseek(editInfo -> fptr_file, 1, SEEK_CUR); // move 1 byte of padding.		
		for (i = 0; i < size - 1; i++)
		    fwrite(&cl, 1, 1, editInfo -> fptr_file); // set all bytes to zero.
		fseek(editInfo -> fptr_file, -(size - 1), SEEK_CUR); // return to starting position.
		fwrite(editInfo -> change_text, editInfo -> size_change_text, 1, editInfo -> fptr_file); // writing to change_text to  mp3 file.
		flag = 0;		
	    }
	    else
		return e_failure; 
        }
        else
        {
            fseek(editInfo -> fptr_file, size, SEEK_CUR); // move to next tag.
            fread(bufftag, 1, 4, editInfo -> fptr_file); // read the next tag header.
            size = get_size(editInfo -> fptr_file); //  get the size of the tag.
            fseek(editInfo -> fptr_file, 2, SEEK_CUR); // move 2 bytes of flag.
        }

        if (ftell(editInfo -> fptr_file) >= editInfo -> tag_size) // if tag does not exist within the container.
        {
            editInfo -> tag = NULL;
            break;
        }
    }
    return e_success;
}
