#ifndef VIEW
#define VIEW

#include "types.h" // Contains user defined data types.

typedef struct _View
{
    /*file info*/
    FILE *fptr_file;
    char *file_name;

    char *tag;
    long tag_size;


} View;

/*Check operation type*/
OperationType check_operation(char *argv[]);

/* Open file */
Status open_file(View *viewInfo);

/*Check file name*/
Status read_validate_file_name(char *argv[], View *viewInfo);

/*Do required checks*/
Status do_checks(View *viewInfo);

/* Check for ID3 and version*/
Status check_id3_ver(View *viewInfo);

/* Display information*/
Status display_info(View *viewInfo);

/* get size */
int get_size(FILE *fptr);

/* get tag*/
char* get_tag(View *viewInfo, char *tag);

#endif
