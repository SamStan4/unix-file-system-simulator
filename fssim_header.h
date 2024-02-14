#ifndef FSSIM_HEADER_H
#define FSSIM_HEADER_H

#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

#include <stdio.h>            
#include <stdlib.h>  
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include "boolean.h"
#include "my_string.h"
#include "linked_list.h"

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

#define NUM_COMMANDS 20

#define NODE_NAME_LENGTH 64
#define NODE_TIME_LENGTH 64

#define DIRECTORY_TYPE 'D'
#define FILE_TYPE 'F'
#define DIRECTORY_TYPE_S "D"
#define FILE_TYPE_S "F"

#define MY_FILE_SYSTEM_SAVE "fssim_save.txt"
#define FILE_SYSYTEM_LINE_ONE "type-time created-path\n"
#define COMMNAD_LIST "command_list.txt"

// error codes

#define COMMAND_NOT_FOUND_ERROR 0
#define EMPTY_COMMAND_ARGUMENT_ERROR 1
#define DUPLICATE_PATH_ERROR 2
#define DUPLICATE_FILE_ERROR 3
#define SLASH_IN_STRING_ERROR 4
#define INVALID_PATH_ERROR 5
#define DIRECTORY_NOT_FOUND_ERROR 6
#define NOT_A_DIRECTORY_ERROR 7
#define DOESNT_NEED_ARGUMENT_ERROR 8
#define FILE_NOT_FOUND_ERROR 9
#define DIRECTORY_NOT_EMPTY_ERROR 10
#define DELETE_ROOT_ERROR 11
#define NOT_A_FILE_ERROR 12
#define NOT_FILE_FORMAT_ERROR 13
#define COMMAND_LIST_ERROR 14

// commands
#define MKDIR_S "mkdir"
#define RMDIR_S "rmdir"
#define LS_S "ls"
#define CD_S "cd"
#define PWD_S "pwd"
#define CREAT_S "creat"
#define RM_S "rm"
#define RELOAD_S "reload"
#define SAVE_S "save"
#define QUIT_S "quit"
#define HELP_S "help"
#define CLEAR_S "clear"
#define EXIT_S "exit"
#define EGG_S "egg"


typedef struct node {
	char  name[NODE_NAME_LENGTH];       // node's name string
	char  time[NODE_TIME_LENGTH];
	char  type;
	struct node *child, *sibling, *parent;
} NODE;

NODE *root = NULL; 
NODE *cwd = NULL;
char *cmd[NUM_COMMANDS] = {};  // fill with list of commands

//***************************************************************************************************************************************************************//
//     function call        : initialize_string_all_null()
//     function description : this function will accept a string and its size and make all of the characters the null character
//     function inputs      : char*, int
//     function outputs     : indirectly changes the string
//     preconditions        : n <= the actual size of the string
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void initialize_string_all_null(char* string, int size)
{
	for (int i = 0; i < size; ++i)
	{
		string[i] = '\0';
	}
}

//***************************************************************************************************************************************************************//
//     function call        : make_node()
//     function description : this function will malloc and initialize a new node
//     function inputs      : void
//     function outputs     : Node*
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

NODE* make_node(void)
{
	int i = 0;
	NODE* new_node = (NODE*)malloc(sizeof(NODE));

	if (new_node != NULL)
	{
		for(i = 0; i < NODE_NAME_LENGTH; ++i)
		{
			new_node->name[i] = '\0';
			new_node->time[i] = '\0';
		}

		new_node->type = '\0';

		new_node->child = NULL;
		new_node->parent = NULL;
		new_node->sibling = NULL;
	}

	return new_node;
}

//***************************************************************************************************************************************************************//
//     function call        : search_node()
//     function description : this function will search the children of the cwd for a node with the same name as the node_name
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : cwd is defined
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

boolean search_node(char* node_name)
{
	NODE* current_node = cwd->child;

	while (current_node != NULL)
	{
		if (strcmp(node_name, current_node->name) == 0) // match
		{
			return TRUE;
		}

		current_node = current_node->sibling;
	}

	return FALSE;
}

//***************************************************************************************************************************************************************//
//     function call        : search_node_and_return()
//     function description : this function will search the children of the cwd for a node with the same name as the node_name, if found the node will be returned
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : cwd is defined
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

NODE* search_node_and_return(char* node_name)
{
	NODE* current_node = cwd->child;

	while (current_node != NULL)
	{
		if (strcmp(node_name, current_node->name) == 0) // match
		{
			return current_node;
		}

		current_node = current_node->sibling;
	}

	return NULL;
}

//***************************************************************************************************************************************************************//
//     function call        : validate_path()
//     function description : this function will determine if a given path is present in the file system, it returns the node if so
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : root is defined
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley


NODE* validate_path(char* input_path) // check if this thing works?
{
	int layers = 0, i = 0;
	NODE* current_node = root->child;
	char current_search_key[50] = {'\0'};

	cut_string(input_path, current_search_key, '/', 50);

	layers = num_occurences(input_path, '/'); // finds the number of layers that we are going to need to travel in the file system


	if (strcmp(input_path, "/") == 0) // if we are given the root
	{
		return root;
	}

	while (current_node != NULL)
	{
		if (strcmp(current_node->name, current_search_key) == 0)
		{
			++i; // update the count that we are on


			if (i == layers) // if we have gone deep enough, return
			{
				return current_node;
			}

			cut_string(NULL, current_search_key, '/', 50); // update the node that we are looking for
			current_node = current_node->child;

		}
		else
		{
			current_node = current_node->sibling;
		}

		
	}

	return NULL;

}

//***************************************************************************************************************************************************************//
//     function call        : destroy_file_system_tree()
//     function description : this function will deallocate all memory associated with the file syetem tree
//     function inputs      : char*
//     function outputs     : void
//     preconditions        : root is defined and initialized
//     postconditions       : the file system is already saved and no longer needed
//     created              ; 2/9/2024
//     last edited          : 2/9/2024
//     programmer           : Sam Stanley

void destroy_file_system_tree_helper(NODE* current_node)
{
	if (current_node != NULL)
	{
		destroy_file_system_tree_helper(current_node->sibling);
		destroy_file_system_tree_helper(current_node->child);
		free(current_node);
	}
}

void destroy_file_system_tree(void)
{
	destroy_file_system_tree_helper(root);
	root = NULL;
	cwd = NULL;
}

//***************************************************************************************************************************************************************//
//     function call        : save_file_system_in_file()
//     function description : this function will save the file system inside of a given file
//     function inputs      : FILE*
//     function outputs     : void
//     preconditions        : root is defined
//     postconditions       : file is opened
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void save_file_system_in_file_helper(FILE* outfile, NODE* current_node, my_string* current_path)
{
	my_string *local_path;

	if ((outfile != NULL) && (current_node != NULL))
	{
		local_path = string_copy_constructor(current_path);
		if (current_node->parent == root)
		{
			fprintf(outfile, "%c-%s-%s%s\n", current_node->type, current_node->time, local_path->string_ptr, current_node->name);
		}
		else
		{
			fprintf(outfile, "%c-%s-%s/%s\n", current_node->type, current_node->time, local_path->string_ptr, current_node->name);
		}
		
		if (current_node->parent != root)
		{
			string_addition_operator_c(local_path, '/');
		}
		string_addition_operator_s(local_path, current_node->name);

		save_file_system_in_file_helper(outfile, current_node->child, local_path);
		save_file_system_in_file_helper(outfile, current_node->sibling, current_path);

		string_destructor(local_path);
	}
}

void save_file_system_in_file(FILE* outfile)
{
	my_string current_path;
	string_constructor(&current_path);

	if (outfile != NULL) // make sure the file is open
	{
		fprintf(outfile, FILE_SYSYTEM_LINE_ONE);
		string_assignment_operator(&current_path, "/");
		//fprintf(outfile, "/\n");
		save_file_system_in_file_helper(outfile, root->child, string_copy_constructor(&current_path));
		string_destructor(&current_path);
	}
}



//***************************************************************************************************************************************************************//
//     function call        : insert_into_tree_with_list()
//     function description : this function will insert a node into the file tree usign a linked list
//     function inputs      : linked_list*, NODE*
//     function outputs     : void
//     preconditions        : root is defined, the linked list is a valid path
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley


boolean insert_into_tree_with_list(linked_list* list, NODE* new_node)
{
	linked_list_node* current_list_ptr = list->head_ptr;
	NODE* tree_ptr = root->child;

	if (list->head_ptr == list->tail_ptr) // the item is in the root directory
	{
		new_node->parent = root;
		new_node->sibling = root->child;
		root->child = new_node;
		return TRUE;
	}

	while ((current_list_ptr->link != list->tail_ptr) && (tree_ptr != NULL))
	{
		if (strcmp(tree_ptr->name, current_list_ptr->data) == 0) // found the child path to take
		{
			current_list_ptr = current_list_ptr->link;
			tree_ptr = tree_ptr->child;
			break;
		}
		else
		{
			tree_ptr = tree_ptr->sibling;
		}
	}

	if (tree_ptr == NULL)
	{
		//free (new_node);
		return FALSE;
	}
	else
	{
		new_node->parent = tree_ptr;
		new_node->sibling = tree_ptr->child;
		tree_ptr->child = new_node;
		return TRUE;
	}
}

//***************************************************************************************************************************************************************//
//     function call        : load_system_from_file()
//     function description : this function will load a file system from a text file
//     function inputs      : FILE*
//     function outputs     : void
//     preconditions        : root is defined, infile is opened in read mode
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void load_system_from_file(FILE* infile)
{
	char input[500] = {'\0'}, current_read[50] = {'\0'}, * token = NULL;
	NODE* current_node = NULL;
	int i = 0;
	linked_list local_list;
	linked_list_constructor(&local_list);

	if (infile != NULL)
	{
		fgets(input, 500, infile); // get the header of the file

		initialize_string_all_null(input, 500); // make the string all null characters again

		while (fgets(input, 500, infile))
		{
			replace_occurences(input, '\n', '\0', 500);

			current_node = make_node();
			
			if (strcmp(DIRECTORY_TYPE_S, strtok(input, "-")) == 0) // we are dealing with directory type
			{
				current_node->type = DIRECTORY_TYPE;
			}
			else // we are dealing with a file type
			{
				current_node->type = FILE_TYPE;
			}

			token = strtok(NULL, "-");

			strcpy(current_node->time, token); // this gets the time

			i = locate_nth_occurrence(input, '\0', 2, 500); // there are now two null characters in our string and we want to fid the second one

			cut_string(&input[i + 1], current_read, '/', 50);

			insert_at_back(&local_list, current_read);

			initialize_string_all_null(current_read, 50);

			while (cut_string(NULL, current_read, '/', 50) != 0) // while there is still shit to be read
			{
				insert_at_back(&local_list, current_read);
				initialize_string_all_null(current_read, 50);
			}

			/// now we have the populated linked list

			strcpy(current_node->name, local_list.tail_ptr->data);

			if (insert_into_tree_with_list(&local_list, current_node) == FALSE)
			{
				return;
			}

			initialize_string_all_null(input, 500); // initialize the characters

			linked_list_destructor(&local_list);
		}
	}
}

//***************************************************************************************************************************************************************//
//     function call        : error_thrower()
//     function description : this function will display messages to the user when there is an error thrown
//     function inputs      : char*, int
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void error_thrower(int error_code, char* string)
{
	printf(RED);

	struct winsize window_dim;

	int i = 0;

    ioctl(0, TIOCGWINSZ, &window_dim); // <-- this puts the size of the terminal into the struct that we named window_dim

	for (i = 0; i < window_dim.ws_col - 18; ++i)
	{
		printf("-");
	}

	if (error_code < 10)
	{
		printf("(ERROR CODE: %d)---", error_code);

		printf("|\n--(ERROR CODE: %d)--> ", error_code);

	}
	else
	{
		printf("(ERROR CODE: %d)--", error_code);

		printf("|\n--(ERROR CODE: %d)--> ", error_code);
	}

	/////////////////////////////////////////////////////////////////// here is where we deal with the actual errors

	switch(error_code)
	{
	case (COMMAND_NOT_FOUND_ERROR):
		printf("command name \"%s\" not found, type \"help\" for list of commands", string);
		break;
	case (EMPTY_COMMAND_ARGUMENT_ERROR):
			printf("command \"%s\" requires an argument", string);
		break;
	case (DUPLICATE_PATH_ERROR):
			printf("\"%s\" already exists in the current directory", string);
		break;
	case (DUPLICATE_FILE_ERROR):
			printf("file name \"%s\" already exists", string);
		break;
	case (SLASH_IN_STRING_ERROR):
			printf("cannot have a '/' present in the argument for \"%s\"", string);
		break;
	case (INVALID_PATH_ERROR):
			printf("input path \"%s\" does not exist", string);
		break;
	case (DIRECTORY_NOT_FOUND_ERROR):
			printf("\"%s\" directory not found", string);
		break;
	case (NOT_A_DIRECTORY_ERROR):
			printf("\"%s\" is a file NOT a directory", string);
		break;
	case (DOESNT_NEED_ARGUMENT_ERROR):
			printf("command name \"%s\" does not take an argument", string);
		break;
	case (FILE_NOT_FOUND_ERROR):
			printf("\"%s\" file not found", string);
		break;
	case (DIRECTORY_NOT_EMPTY_ERROR):
			printf("the directory \"%s\" is NOT empty", string);
		break;
	case (DELETE_ROOT_ERROR):
			printf("you can NOT delete the root directory");
		break;
	case (NOT_A_FILE_ERROR):
			printf("\"%s\" is a directory NOT a file", string);
		break;
	case (NOT_FILE_FORMAT_ERROR): 
			printf("\"%s\" does not follow file system format", string);
		break;
	case (COMMAND_LIST_ERROR):
			printf("cannot save file system to \"%s\"", string);
		break;


	}

	/*

	*/

	printf("\n");

	printf(RESET);
}

//***************************************************************************************************************************************************************//
//     function call        : initialize()
//     function description : this function initializes all of the global variables in the program
//     function inputs      : void
//     function outputs     : indirectly changes global variables
//     preconditions        : none
//     postconditions       : none
//     created              ; ???
//     last edited          : ???
//     programmer           : ???

int initialize()
{
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = DIRECTORY_TYPE;
	cwd = root;

	//printf("Filesystem initialized!\n");
}

//***************************************************************************************************************************************************************//
//     function call        : initialize_command_list()
//     function description : this function will initialize the command list global variable "cmd"
//     function inputs      : void
//     function outputs     : indirectly changes the global variable cmd
//     preconditions        : cmd is defined correctly
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void initialize_command_list(void)
{
	int i = 0; 

	for (i = 0; i < NUM_COMMANDS; ++i)
	{
		cmd[i] = (char*) calloc (2, sizeof(char));

		if (cmd[i] != NULL)
		{
			cmd[i][0] = '0';
			cmd[i][1] = '\0';
		}
	}
}

//***************************************************************************************************************************************************************//
//     function call        : load_command_list()
//     function description : this function will load all of the commands into the command list
//     function inputs      : void
//     function outputs     : indirectly changes the global variable cmd
//     preconditions        : cmd is defined correctly
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void load_command_list(void)
{
	FILE* command_file_stream = NULL;
	char read_string[50] = {'\0'};
	int i = 0, num_commands;

	initialize_command_list();

	command_file_stream = fopen("command_list.txt", "r");

	if (command_file_stream == NULL)
	{
		printf(RED"\n\nERROR OPENING COMMAND FILE\n\n"RESET);
	}
	else
	{
		//printf(GREEN"\n\nCOMMNAD FILE OPENED\n\n"RESET);

		while (fgets(read_string, 50, command_file_stream)) // while we are not at the end of the file
		{

			for (i = 0; (read_string[i] != '\0') && (i < 50); ++i) // gets rid of the new line character if ther is one
			{
				if (read_string[i] == '\n')
				{
					read_string[i] = '\0';
					break;
				}
			}

			cmd[num_commands] = (char*)realloc(cmd[num_commands], ((strlen(read_string)) * (sizeof(char))));

			if (cmd[num_commands] != NULL) // if it was successfully moved
			{
				for (i = 0; i <= strlen(read_string); ++i)
				{
					cmd[num_commands][i] = read_string[i];
				}
			}

			initialize_string_all_null(read_string, 50);

			++num_commands;
		}
	}

	fclose(command_file_stream);
}

//***************************************************************************************************************************************************************//
//     function call        : destroy_command_list()
//     function description : this function will deallocate all dynamic memory associated with cmd
//     function inputs      : void
//     function outputs     : indirectly changes the global variable cmd
//     preconditions        : cmd is defined correctly
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void destroy_command_list(void)
{
	int i = 0;

	for (; i < NUM_COMMANDS; ++i)
	{
		free(cmd[i]);
		cmd[i] = NULL;
	}
}

//***************************************************************************************************************************************************************//

#endif