#ifndef COMMAND_H
#define COMMAND_H

#include "fssim_header.h"
#include "my_string.h"

typedef struct
{
    my_string command_name; // mkdir, ls, etc
    my_string command_args;
    boolean valid; // is the command valid or not?
} command;

//***************************************************************************************************************************************************************//
//     function call        : command_constructor()
//     function description : this function should get called directly after a command struct is declared
//     function inputs      : command*
//     function outputs     : indirectly edits command
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void command_constructor(command* input_command)
{
    string_constructor(&input_command->command_name);
    string_constructor(&input_command->command_name);
    input_command->valid = FALSE;
}

//***************************************************************************************************************************************************************//
//     function call        : initialize_command()
//     function description : this function will deallocate all memory associated with the command struct
//     function inputs      : command*
//     function outputs     : indirectly edits command
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void initialize_command(command* input_command)
{
    string_destructor(&input_command->command_name); // deletes all memory in the command name data member
    string_destructor(&input_command->command_args); // deletes all memory in the command argument data member
    input_command->valid = FALSE;
}

//***************************************************************************************************************************************************************//
//     function call        : parse_command_input_string()
//     function description : this function will parse the input string into the command and argument parts
//     function inputs      : char* X3
//     function outputs     : populates 2 strings
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void parse_command_input_string(char* input_string, char* input_command_s, char* input_argument_s)
{
    int i = 0, j = 0, k = 0;
    boolean del_hit = FALSE;

    for (i = 0; (input_string[i] != '\0') && (input_string[i] != '\n'); ++i) // go past all of the leading spaces
    {
        if (input_string[i] != ' ')
        {
            break;
        }
    }

    for (k = 0; (input_string[i] != '\0') && (input_string[i] != '\n'); ++i, ++k)
    {
        if (input_string[i] != ' ')
        {
            input_command_s[k] = input_string[i];
        }
        else // if we hit the delimeter
        {
            ++i;
            del_hit = TRUE; // mark the delimeter hit as true
            break; // 
        }
    }

    for (; (input_string[i] != '\0') && (input_string[i] != '\n'); ++i) // go past all of the in between spaces
    {
        if (input_string[i] != ' ')
        {
            break;
        }
    }

    if (del_hit == TRUE)
    {
        for (j = 0; (input_string[i] != '\0') && (input_string[i] != '\n') && (input_string[i] != ' '); ++j, ++i)
        {
            input_argument_s[j] = input_string[i];
        }
    }


}

//***************************************************************************************************************************************************************//
//     function call        : validate_command()
//     function description : this function will validate command
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

boolean validate_command(char* command_name)
{
    int i = 0;
    for (i = 0; i < NUM_COMMANDS; ++i)
    {
        if (strcmp(command_name, cmd[i]) == 0)
        {
            return TRUE;
        }
        else if (strcmp("0", cmd[i]) == 0)
        {
            error_thrower(COMMAND_NOT_FOUND_ERROR, command_name);
            return FALSE;
        }
    }

    return FALSE;
}

//***************************************************************************************************************************************************************//
//     function call        : validate_arbitraty_argument_command()
//     function description : this function will validate weather or not the argument exists and if it violates certian duplicate path conditions
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : all global variables are correctly defined
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

boolean validate_arbitraty_argument_command(char* command_name, char* command_argument)
{
    // STEP 1 --> make sure argument is present
    // STEP 2 --> make sure argument does not result in duplicate path names or files

    
	// creat, mkdir, save

    FILE* file_pointer = NULL;

    if (strlen(command_argument) == 0) // step 1
    {
        error_thrower(EMPTY_COMMAND_ARGUMENT_ERROR, command_name);
        return FALSE;
    }

    if (strcmp(command_name, SAVE_S) == 0) // we need to check if there is already a file with the same name as command_argument
    {
        if (is_ended_by(command_argument, ".txt") == 0)
        {
            strcat(command_argument, ".txt");
        }

        if (strcmp(command_argument, COMMNAD_LIST) == 0)
        {
            error_thrower(COMMAND_LIST_ERROR, command_argument);
            return FALSE;
        }

       return TRUE; // we are just going to overwrite the file
    }
    else // we are dealing with either mkdir or creat
    {
        if (search_node(command_argument) == TRUE)
        {
            error_thrower(DUPLICATE_PATH_ERROR, command_argument);
            return FALSE;
        }
        else
        {
            if (num_occurences(command_argument, '/') > 0) // we dont want to have a / in the name of the argument because this fucks up the path
            {
                error_thrower(SLASH_IN_STRING_ERROR, command_name);
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
    }

}

//***************************************************************************************************************************************************************//
//     function call        : validate_arg_or_no_arg_command()
//     function description : this function will validate weather or not the argument exists and if it violates certian duplicate path conditions
//     function inputs      : char*
//     function outputs     : boolean
//     preconditions        : all global variables are correctly defined
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

/*
    for ls and cd
*/

boolean validate_arg_or_no_arg_command(char* command_name, char* command_argument)
{
    NODE* node = NULL;

    if (strlen(command_argument) == 0) // these functions do not require an argument
    {
        return TRUE;
    }

    /////// first check if it exists locally

    if ((strcmp(command_name, CD_S) == 0) && (strcmp(command_argument, "..") == 0)) // going back one directory
    {
        return TRUE;
    }

    if (search_node(command_argument) == TRUE)
    {
        // check if it is a directory

        node = search_node_and_return(command_argument);
        
        if (node->type == FILE_TYPE) // check if this node is a file
        {
            error_thrower(NOT_A_DIRECTORY_ERROR, command_argument);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    if (strcmp(command_argument, cwd->name) == 0)
    {
        return TRUE;
    }

    /////// now check the entire tree
    
    if (validate_path(command_argument) == NULL)
    {
        error_thrower(INVALID_PATH_ERROR, command_argument);
        return FALSE;
    }
    

    ////////////////////////
    return TRUE;


}

//***************************************************************************************************************************************************************//
//     function call        : validate_no_argument_command()
//     function description : this function will validate commands that take no arguments
//     function inputs      : char* X2
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

boolean validate_no_argument_command(char* command_name, char* command_argument)
{
    if (strcmp(command_argument, "") == 0)
    {
        return TRUE;
    }
    else
    {
        error_thrower(DOESNT_NEED_ARGUMENT_ERROR, command_name);
        return FALSE;
    }
}

//***************************************************************************************************************************************************************//
//     function call        : validate_removal_command()
//     function description : this function will validate commands that can remove directories and files
//     function inputs      : char* X2
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

boolean validate_removal_command(char* command_name, char* command_argument)
{
    NODE* node = NULL;

    if (search_node(command_argument) == FALSE) // didnt find the node that we were looking for
    {
        if (strcmp(command_name, RMDIR_S) == 0)
        {
            error_thrower(DIRECTORY_NOT_FOUND_ERROR, command_argument);
        }
        else
        {
            error_thrower(FILE_NOT_FOUND_ERROR, command_argument);
        }
        return FALSE;
    }

    if (strcmp(command_name, RMDIR_S) == 0) // we are dealing with rmdir
    {
        node = search_node_and_return(command_argument);
        {
            if (node->type == FILE_TYPE)
            {
                error_thrower(NOT_A_DIRECTORY_ERROR, command_argument);
                return FALSE;
            }
            else if (node->child != NULL) // directory is not empty
            {   
                error_thrower(DIRECTORY_NOT_EMPTY_ERROR, command_argument);
                return FALSE;
            }
            else if (strcmp(command_argument, "/") == 0) // wants to delete the root directory
            {
                error_thrower(DELETE_ROOT_ERROR, "");
                return FALSE;
            }
        }
    }
    else // the command is rm
    {
        node = search_node_and_return(command_argument);

        if (node->type == DIRECTORY_TYPE)
        {
            error_thrower(NOT_A_FILE_ERROR, command_argument);
            return FALSE;
        }
    }

    return TRUE;


}

//***************************************************************************************************************************************************************//
//     function call        : validate_reload_argument()
//     function description : this function will validate the reload command argument
//     function inputs      : char* X2
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

boolean validate_reload_argument(char* command_name, char* command_argument)
{
    FILE* infile = NULL;
    char test[50] = {'\0'};

    if (strcmp(command_argument, "") == 0) // empty
    {
        return TRUE;
    }

    if (is_ended_by(command_argument, ".txt") == 0) // check if there is a .txt extension
    {
        strcat(command_argument, ".txt"); // if not, add it
    }

    infile = fopen(command_argument, "r");

    if (infile == NULL) // the file did not open and is not present in the system
    {
        error_thrower(FILE_NOT_FOUND_ERROR, command_argument);
        return FALSE;
    }
    else
    {
        fgets(test, 50, infile);

        if (strcmp(test, FILE_SYSYTEM_LINE_ONE) == 0)
        {
            return TRUE;
        }
        else   
        {
            error_thrower(NOT_FILE_FORMAT_ERROR, command_argument);
            return FALSE;
        }
    }
}

//***************************************************************************************************************************************************************//
//     function call        : validate_command_argument()
//     function description : this function will validate command argument
//     function inputs      : char* X2
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

boolean validate_command_argument(char* command_name, char* command_argument)
{
    if ((strcmp(command_name, MKDIR_S) == 0) || (strcmp(command_name, CREAT_S) == 0) || (strcmp(command_name, SAVE_S) == 0)) // no specific argumnet needed, presence is needed
    {
        if (validate_arbitraty_argument_command(command_name, command_argument) == FALSE)
        {
            return FALSE;
        }
    }
    else if ((strcmp(command_name, CD_S) == 0) || (strcmp(command_name, LS_S) == 0)) // either an argument or no argument
    {
        if (validate_arg_or_no_arg_command(command_name, command_argument) == FALSE)
        {
            return FALSE;
        }
    }
    else if ((strcmp(command_name, PWD_S) == 0) || (strcmp(command_name, HELP_S) == 0) || (strcmp(command_name, CLEAR_S) == 0) || (strcmp(command_name, EGG_S) == 0) || (strcmp(command_name, EXIT_S) == 0) || (strcmp(command_name, QUIT_S) == 0)) // no argument command
    {
        if (validate_no_argument_command(command_name, command_argument) == FALSE)
        {
            return FALSE;
        }
    }
    else if ((strcmp(command_name, RMDIR_S) == 0) || (strcmp(command_name, RM_S) == 0)) // commands that remove shit in the current directory
    {
        if (validate_removal_command(command_name, command_argument) == FALSE)
        {
            return FALSE;
        }
    }
    else if (strcmp(command_name, RELOAD_S) == 0) // loading a file system from a test file
    {
        validate_reload_argument(command_name, command_argument);
    }
    // temp //
    return TRUE;
    // end temp //
}

//***************************************************************************************************************************************************************//
//     function call        : validate_user_command_input()
//     function description : this function will validate command argument
//     function inputs      : char* X2
//     function outputs     : boolean
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

boolean validate_user_command_input(char* command_name, char* command_argument)
{
    if (validate_command(command_name) == FALSE) // command was incorrect
    {
        return FALSE;
    }
    if (validate_command_argument(command_name, command_argument) == FALSE)
    {
        return FALSE;
    }


    // temp //

    return TRUE;

    // end temp //
}

//***************************************************************************************************************************************************************//
//     function call        : get_user_command()
//     function description : this function will get a command from the user and validate it
//     function inputs      : command*
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void get_user_command(command* input_command)
{
    char input_string[100] = {'\0'},
        input_command_s[50] = {'\0'},
        input_argument[50] = {'\0'};

    int i = 0;

    initialize_command(input_command);

    printf(MAGENTA);
    fgets(input_string, 100, stdin); // use a file command on the standard input stream
    printf(GREEN);

    for (i = 0; i < 100; ++i) // gets rid of the new line character on the end of the input_string
    {
        if (input_string[i] == '\n')
        {
            input_string[i] = '\0';
            break;
        }
    }

    parse_command_input_string(input_string, input_command_s, input_argument);

    //printf("command --> %s\nargument --> %s", input_command_s, input_argument);

    if (validate_user_command_input(input_command_s, input_argument) == TRUE) // the command is invalid and we return false
    {
        string_assignment_operator(&input_command->command_name, input_command_s);
        string_assignment_operator(&input_command->command_args, input_argument);
        input_command->valid = TRUE;


    }
}

//***************************************************************************************************************************************************************//
//     function call        : execute_help()
//     function description : this function will print out a list of commands
//     function inputs      : void
//     function outputs     : void
//     preconditions        : global variable cmd is defiend and populated
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void execute_help(void)
{
    int i = 0;

    printf("|\n");

    for (i = 0; (i < NUM_COMMANDS) && (strcmp(cmd[i], "0") != 0); ++i)
    {
        if (strcmp(cmd[i], EGG_S) != 0)
        {
            printf(">----> "MAGENTA"%s"GREEN"\n", cmd[i]);
        }
    }
    printf("|\n");
}

//***************************************************************************************************************************************************************//
//     function call        : execute_egg()
//     function description : easter egg
//     function inputs      : void
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void execute_egg(void)
{
    static int i = 0;
    char message[40] = {'\0'};

    if (i == 0)
    {
        i++;
        strcpy(message, "--congrats, you found me");
    }
    else if (i == 1)
    {
        i++;
        strcpy(message, "--always funnier the second time");
    }
    else if (i == 2)
    {
        i++;
        strcpy(message, "--keeps getting better");
    }
    else
    {
        strcpy(message, "");
    }

    printf("\n");
    	printf(MAGENTA""
		"                _____                  \n"
		"             ,-\"     \"-.               \n"
		"            /"GREEN" o       o "MAGENTA"\\              \n"
		"           /"GREEN"   \\     /   "MAGENTA"\\             \n"
		"          /"GREEN"     )-\"-(     "MAGENTA"\\            %s\n"
		"         /"GREEN"     ( "CYAN"6 6"GREEN" )     "MAGENTA"\\           \n"
		"        /"GREEN"       \\ \" /       "MAGENTA"\\          \n"
		"       /"GREEN"         )=(         "MAGENTA"\\         \n"
		"      /"GREEN"   o  .--\" - \"--.  o   "MAGENTA"\\        \n"
		"     /"GREEN"    I  /  -   -  \\  I    "MAGENTA"\\       \n"
		" .--("GREEN"    (_}y/\\       /\\y{_)    "MAGENTA")--.   \n"
		"(    \".___"GREEN"l\\/"MAGENTA"__"GREEN"\\_____/"MAGENTA"__"GREEN"\\/l"MAGENTA"___. \"   )  \n"
		" \\                                 /   \n"
		"  \" -._     "CYAN"o "YELLOW"O "RED"o "MAGENTA"O "RED"o "YELLOW"O "CYAN"o"MAGENTA"     _.- \"    \n"
		"      '--Y--.___________.--Y--'        \n\n"RESET, message);

    printf("\n");
}

//***************************************************************************************************************************************************************//
//     function call        : execute_mkdir()
//     function description : this function will execute the mkdir command
//     function inputs      : command*, my_string*
//     function outputs     : void
//     preconditions        : command has been validated
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_mkdir(command* input_command, my_string* current_path)
{
    NODE* new_node = make_node();

    time_t current_time = time(NULL);
	char* current_time_string = ctime(&current_time); // put the time into a string
	current_time_string[strlen(current_time_string) - 1] = '\0'; // <-- make the last element in the character array the NULL character

    strncpy(new_node->time, current_time_string, NODE_TIME_LENGTH);
    strncpy(new_node->name, input_command->command_args.string_ptr, NODE_NAME_LENGTH);
    new_node->type = DIRECTORY_TYPE;
    new_node->parent = cwd;

    // insert at front

    new_node->sibling = cwd->child;
    cwd->child = new_node;

    printf("|\n---> \""MAGENTA"%s"GREEN"\" has been added to "MAGENTA"%s"GREEN"\n\n", input_command->command_args.string_ptr, current_path->string_ptr);
}

//***************************************************************************************************************************************************************//
//     function call        : execute_mkdir()
//     function description : this function will execute the mkdir command
//     function inputs      : command*, my_string*
//     function outputs     : void
//     preconditions        : command has been validated
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_ls(command* input_command)
{
    NODE* current_ptr = NULL;

    if ((strcmp(input_command->command_args.string_ptr, cwd->name) == 0) || (strcmp(input_command->command_args.string_ptr, "") == 0)) // stuff that makes the cwd the pointer
    {
        current_ptr = cwd;

    }
    else if (search_node(input_command->command_args.string_ptr) == TRUE) // one of the children
    {
        current_ptr = search_node_and_return(input_command->command_args.string_ptr);
    }
    else // somewhere else in the fucking system
    {
        current_ptr = validate_path(input_command->command_args.string_ptr);
        //current_ptr = current_ptr->child; // i have no idea why this needs to be here, but here it is
    }

    current_ptr = current_ptr->child;

    printf("|\n");

    while (current_ptr != NULL)
    {
        if (current_ptr->type == DIRECTORY_TYPE)
        {
            printf(">--("MAGENTA"%s"GREEN")--("MAGENTA"directory"GREEN")--> "MAGENTA"%s"GREEN"\n", current_ptr->time, current_ptr->name);
        }
        else
        {
            printf(">--("MAGENTA"%s"GREEN")--("MAGENTA"file"GREEN")-------> "MAGENTA"%s"GREEN"\n", current_ptr->time, current_ptr->name);
        }
        current_ptr = current_ptr->sibling;
    }

    printf("|\n");
}

//***************************************************************************************************************************************************************//
//     function call        : execute_pwd()
//     function description : this function will display the path of the current working directory
//     function inputs      : my_string*
//     function outputs     : void
//     preconditions        : command has been validated
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void execute_pwd(my_string* current_path)
{
    printf("|\n>--("MAGENTA"cwd"GREEN")--> "MAGENTA"%s"GREEN"\n|\n", current_path->string_ptr);
}

//***************************************************************************************************************************************************************//
//     function call        : execute_creat()
//     function description : this function will execute the creat command
//     function inputs      : command*, my_string*
//     function outputs     : void
//     preconditions        : input_command is already validated
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_creat(command* input_command, my_string* current_path)
{
    NODE* new_node = make_node();

    time_t current_time = time(NULL);
	char* current_time_string = ctime(&current_time); // put the time into a string
	current_time_string[strlen(current_time_string) - 1] = '\0'; // <-- make the last element in the character array the NULL character

    strncpy(new_node->time, current_time_string, NODE_TIME_LENGTH);
    strncpy(new_node->name, input_command->command_args.string_ptr, NODE_NAME_LENGTH);
    new_node->type = FILE_TYPE;
    new_node->parent = cwd;

    // insert at front

    new_node->sibling = cwd->child;
    cwd->child = new_node;

    printf("|\n---> \""MAGENTA"%s"GREEN"\" has been added to "MAGENTA"%s"GREEN"\n\n", input_command->command_args.string_ptr, current_path->string_ptr);
}

//***************************************************************************************************************************************************************//
//     function call        : execute_quit()
//     function description : this function will execute the quit command
//     function inputs      : void
//     function outputs     : void
//     preconditions        : necesairy files already exist
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_quit()
{
    FILE* outfile = fopen(MY_FILE_SYSTEM_SAVE, "w");
    
    if (outfile != NULL)
    {
        save_file_system_in_file(outfile);
    }

    fclose(outfile);
}

//***************************************************************************************************************************************************************//
//     function call        : execute_save()
//     function description : this function will execute the save command
//     function inputs      : command*
//     function outputs     : void
//     preconditions        : necesairy files already exist
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_save(command* input_command)
{
    FILE* outfile = fopen(input_command->command_args.string_ptr, "w");\

    if (outfile != NULL)
    {
        save_file_system_in_file(outfile);
    }

    printf("|\n--> file system successfully saved in %s\n|\n", input_command->command_args.string_ptr);

    fclose(outfile);
}

//***************************************************************************************************************************************************************//
//     function call        : execute_cd()
//     function description : this function will execute the cd command
//     function inputs      : void
//     function outputs     : void
//     preconditions        : every global variable is defiend
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_cd(command* input_command, my_string* current_path)
{
    if (strcmp(input_command->command_args.string_ptr, "..") == 0) // going back one directory
    {
        if (cwd != root)
        {
            if (cwd->parent == root)
            {
                string_subtraction_operator_s(current_path, "/");
            }
            else
            {
                string_subtraction_operator(current_path, "/");
            }
            cwd = cwd->parent;
        }
    }
    else if (strcmp(input_command->command_args.string_ptr, "") == 0) // going back to the root
    {
        string_assignment_operator(current_path, root->name);
        cwd = root;
    }
    else if (search_node(input_command->command_args.string_ptr) == TRUE) // going forward one directory
    {   
        if (cwd != root)
        {
            string_addition_operator_s(current_path, "/");
        }
        string_addition_operator_s(current_path, input_command->command_args.string_ptr);
        cwd = search_node_and_return(input_command->command_args.string_ptr);
    }
    else if (validate_path(input_command->command_args.string_ptr) != NULL) // going somewhere else in the tree
    {
        string_assignment_operator(current_path, input_command->command_args.string_ptr);
        cwd = validate_path(input_command->command_args.string_ptr);
    }
}

//***************************************************************************************************************************************************************//
//     function call        : execute_rmdir()
//     function description : this function will execute the rmdir command
//     function inputs      : command*
//     function outputs     : void
//     preconditions        : every global variable is defiend
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_rmdir(command* input_command)
{
    NODE* current_node = NULL, * previous_node = NULL;

    current_node = cwd->child;

    while(current_node != NULL)
    {
        if (strcmp(current_node->name, input_command->command_args.string_ptr) == 0) // we have found the node
        {
            if (previous_node == NULL) // at the front of the list
            {
                cwd->child = current_node->sibling;
            }
            else // somewhere else in the list
            {
                previous_node->sibling = current_node->sibling;
            }

            free(current_node);
            return;
        }

        previous_node = current_node;
        current_node = current_node->sibling;
    }
}

//***************************************************************************************************************************************************************//
//     function call        : execute_rm()
//     function description : this function will execute the rmdir command
//     function inputs      : command*
//     function outputs     : void
//     preconditions        : every global variable is defiend
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/8/2024
//     programmer           : Sam Stanley

void execute_rm(command* input_command)
{
    NODE* current_node = NULL, * previous_node = NULL;

    current_node = cwd->child;

    while(current_node != NULL)
    {
        if (strcmp(current_node->name, input_command->command_args.string_ptr) == 0) // we have found the node
        {
            if (previous_node == NULL) // at the front of the list
            {
                cwd->child = current_node->sibling;
            }
            else // somewhere else in the list
            {
                previous_node->sibling = current_node->sibling;
            }

            free(current_node);
            return;
        }

        previous_node = current_node;
        current_node = current_node->sibling;
    }
}

//***************************************************************************************************************************************************************//
//     function call        : execute_reload()
//     function description : this function will execute the reload command
//     function inputs      : command*
//     function outputs     : void
//     preconditions        : every global variable is defiend
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void execute_reload(command* input_command, my_string* current_path) // hard
{
    destroy_file_system_tree(); // kill the current file system
    initialize();
    string_assignment_operator(current_path, "/");

    // hard part, load the system from the file

    if (strcmp(input_command->command_args.string_ptr, "") == 0)
    {
        string_assignment_operator(&input_command->command_args, MY_FILE_SYSTEM_SAVE);
    }

    FILE* infile = fopen(input_command->command_args.string_ptr, "r");

    if (infile != NULL)
    {
        load_system_from_file(infile);
        fclose(infile);
    }
}

//***************************************************************************************************************************************************************//
//     function call        : execute_command()
//     function description : this function will execute the command that the user passed in
//     function inputs      : command*, my_string*
//     function outputs     : void
//     preconditions        : input_command is already validated
//     postconditions       : none
//     created              ; 2/8/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

boolean execute_command(command* input_command, my_string* current_path)
{
    // probably could have used a hash table for this, but ok

    if (strcmp(input_command->command_name.string_ptr, MKDIR_S) == 0) // execute mkdir
    {
        execute_mkdir(input_command, current_path);
    }
    else if (strcmp(input_command->command_name.string_ptr, RMDIR_S) == 0) // execute rmdir
    {
        execute_rmdir(input_command);
    }
    else if (strcmp(input_command->command_name.string_ptr, LS_S) == 0) // execute ls
    {
        execute_ls(input_command);
    }
    else if (strcmp(input_command->command_name.string_ptr, CD_S) == 0) // execute cd
    {
        execute_cd(input_command, current_path);
    }
    else if (strcmp(input_command->command_name.string_ptr, PWD_S) == 0) // execute pwd
    {
        execute_pwd(current_path);
    }
    else if (strcmp(input_command->command_name.string_ptr, CREAT_S) == 0) // execute creat
    {
        execute_creat(input_command, current_path);
    }
    else if (strcmp(input_command->command_name.string_ptr, RM_S) == 0) // execute rm
    {
        execute_rm(input_command);
    }
    else if (strcmp(input_command->command_name.string_ptr, RELOAD_S) == 0) // execute reload ///////////////////////////
    {
        execute_reload(input_command, current_path);
    }
    else if (strcmp(input_command->command_name.string_ptr, SAVE_S) == 0) // execute save
    {
        execute_save(input_command);
    }
    else if (strcmp(input_command->command_name.string_ptr, QUIT_S) == 0) // execute quit
    {
        execute_quit();
        return FALSE;
    }
    else if (strcmp(input_command->command_name.string_ptr, HELP_S) == 0) // execute help
    {
        execute_help();
    }
    else if (strcmp(input_command->command_name.string_ptr, CLEAR_S) == 0) // execute clear
    {
        system(CLEAR);
    }
    else if (strcmp(input_command->command_name.string_ptr, EGG_S) == 0) // execute egg
    {
        execute_egg();
    }
    else if (strcmp(input_command->command_name.string_ptr, EXIT_S) == 0) // exit without saving
    {
        return FALSE;
    }

    return TRUE;
    
}

//***************************************************************************************************************************************************************//

#endif