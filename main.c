#include <stdio.h>
#include <string.h>
#include "types.h"
#include "view.h"
#include "edit.h"

void print_error();
void print_helpmenu();

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
	print_error();
	return -1;
    }
     
    if (argc == 2 && (strcmp(argv[1], "--help") == 0))
	print_helpmenu();
    else if (argc == 2)
	print_error();

    if (argc == 3)
    {
	if (check_operation(argv) == view)
	{
	    View viewInfo;
	    if (read_validate_file_name(argv, &viewInfo) == v_failure)
	    {
		printf("Enter valid file name\n");
		return -1;
	    }
	    else
	    {
		if (do_checks(&viewInfo) == v_failure)
		    return -1;

		display_info(&viewInfo);
	    }
	}
	else
	    print_error();
    }

    if (argc == 4 || argc > 6)
	    print_error();

    if (argc == 5)
    {
       	if (check_operation(argv) == edit)
	{
	    Edit editInfo;
	    if (read_validate_file_name_edit(argv, &editInfo) == e_failure)
	    {
		printf("Enter valid filename\n");
		return -1;
	    }
	    else
	    {
		if (do_checks_edit(&editInfo) == e_failure)
		    return -1;

		if (check_option(argv, &editInfo) == e_failure)
		    printf("Enter valid option\n");

		do_edit(&editInfo);
	    }
	}
	else
	    print_error();
    }
    return 0;
}

void print_error()
{
    for (int i = 0; i < 50; i++)
	printf("-");
    printf("\nERROR: ./a.out : Invalid arguments\n");
    printf("USAGE: \n");
    printf("To view please pass like: ./a.out -v mp3filename\n");
    printf("To edit please pass like: ./a.out -e -t/-a/-A/-y/-c/-C changing_text mp3filename\n");
    printf("To get help pass like: ./a.out --help\n");
    for (int j = 0; j < 50; j++)
	printf("-");
    putchar('\n');
}

void print_helpmenu()
{
    for (int i = 0; i < 50; i++)
	printf("-");
    printf("\nHELP MENU\n");
    printf("1) -v -> to view mp3 file contents.\n");
    printf("2) -e -> to edit mp3 file contents.\n");
    printf("\t2.1) -t -> to edit song title.\n");
    printf("\t2.2) -a -> to edit artist name.\n");
    printf("\t2.3) -A -> to edit album name.\n");
    printf("\t2.4) -y -> to edit year.\n");
    printf("\t2.5) -c -> to edit content type.\n");
    printf("\t2.6) -C -> to edit comment.\n");
    for (int i = 0; i < 50; i++)
	printf("-");
    putchar('\n');
}
