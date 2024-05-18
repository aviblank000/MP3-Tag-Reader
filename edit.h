#ifndef EDIT
#define EDIT


#include "types.h" // Contains user defined data types.

typedef struct _Edit
{
    /*file info*/
    FILE *fptr_file;
    char *file_name;
    char op[2];

    char *change_text;
    int size_change_text;

    long tag_size; // entire meta container size.

    char *tag;

} Edit;

/* Function Definition */
Status read_validate_file_name_edit(char *argv[], Edit *editInfo);

/* Open files*/
Status open_file_edit(Edit *editInfo);

/* Do required checks*/
Status do_checks_edit(Edit *editInfo);

/* Check for ID3 and version*/
Status check_id3_ver_edit(Edit *editInfo);

/* Check option*/
Status check_option(char *argv[], Edit *editInfo);

/* do_edit */
Status do_edit(Edit *editInfo);

/* to edit the tag data */
Status tag_edit(Edit *editInfo, char *tag);

#endif
