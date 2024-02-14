#ifndef WRAPPER_H
#define WRAPPER_H

#include "fssim_header.h"
#include "my_string.h"
#include "command.h"

//***************************************************************************************************************************************************************//
//     function call        : pause_terminal()
//     function description : this function will pause the terminal until the user presses the enter key
//     function inputs      : void
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void pause_terminal(void)
{
    char c;
    printf("    press "MAGENTA"ENTER"RESET" to continue...");
    c = getchar();
    printf("\n\n");
}

//***************************************************************************************************************************************************************//
//     function call        : clear_terminal()
//     function description : this function will clear the terminal of all text
//     function inputs      : void
//     function outputs     : void
//     preconditions        : CLEAR is defined correctly
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void clear_terminal(void)
{
	system(CLEAR);
}

//***************************************************************************************************************************************************************//
//     function call        : pause_terminal()
//     function description : this function will pause the terminal until the user presses the enter key
//     function inputs      : void
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley
//     resources            : https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c <-- this helped me figure out how to use the sys/ioctl.h library
//                            https://stackoverflow.com/questions/5141960/get-the-current-time-in-c <-- this helped me figure out how to get the system time from the time.h library

void make_terminal_pretty(char* current_path)
{
    struct winsize window_dim;

    ioctl(0, TIOCGWINSZ, &window_dim); // <-- this puts the size of the terminal into the struct that we named window_dim

	int i = 0, j = 0;

    //printf("\n\nHere are the rows --> %d\nHere are the cols --> %d\n", window_dim.ws_row, window_dim.ws_col);\

	time_t current_time = time(NULL);
	char* current_time_string = ctime(&current_time); // put the time into a string
	current_time_string[strlen(current_time_string) - 1] = '\0'; // <-- make the last element in the character array the NULL character

	// at this point, we have both the time and the size of the terminal, so we may begin making the terminal theme

	printf(GREEN);
	for (i = 0; i < (window_dim.ws_col - strlen(current_time_string) - 5); ++i)
	{
		printf("-");
	}
	printf("("MAGENTA"%s"GREEN")---", current_time_string);

	if ((current_path == NULL) || (strlen(current_path) == 0)) // if there is nothing in the current path, this will be an error
	{
		printf("\n|\n--("RED"NO PATH DETECTED"GREEN")--> ");
	}
	else
	{
		printf("\n|\n--("MAGENTA"%s"GREEN")--> ", current_path);

	}
}

//***************************************************************************************************************************************************************//
//     function call        : alien_graphic()
//     function description : this function prints out an alien double flipping you off
//     function inputs      : void
//     function outputs     : void
//     preconditions        : all defined strings are defined somewhere
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void alien_graphic(void)
{
	printf(MAGENTA""
		"                _____                  \n"
		"             ,-\"     \"-.               \n"
		"            /"GREEN" o       o "MAGENTA"\\              \n"
		"           /"GREEN"   \\     /   "MAGENTA"\\             \n"
		"          /"GREEN"     )-\"-(     "MAGENTA"\\            \n"
		"         /"GREEN"     ( "CYAN"6 6"GREEN" )     "MAGENTA"\\           \n"
		"        /"GREEN"       \\ \" /       "MAGENTA"\\          \n"
		"       /"GREEN"         )=(         "MAGENTA"\\         \n"
		"      /"GREEN"   o  .--\" - \"--.  o   "MAGENTA"\\        \n"
		"     /"GREEN"    I  /  -   -  \\  I    "MAGENTA"\\       \n"
		" .--("GREEN"    (_}y/\\       /\\y{_)    "MAGENTA")--.   \n"
		"(    \".___"GREEN"l\\/"MAGENTA"__"GREEN"\\_____/"MAGENTA"__"GREEN"\\/l"MAGENTA"___. \"   )  \n"
		" \\                                 /   \n"
		"  \" -._     "CYAN"o "YELLOW"O "RED"o "MAGENTA"O "RED"o "YELLOW"O "CYAN"o"MAGENTA"     _.- \"    \n"
		"      '--Y--.___________.--Y--'        \n\n"RESET);

}

//***************************************************************************************************************************************************************//
//     function call        : file_system_sim()
//     function description : this function will start the unix file system simulator and run it
//     function inputs      : void
//     function outputs     : void
//     preconditions        : global variables are all set
//     postconditions       : none
//     created              ; 2/6/2024
//     last edited          : 2/6/2024
//     programmer           : Sam Stanley

void file_system_sim(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// variables

	my_string current_path;

	command current_command;

	boolean go_again = TRUE;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// initializations

	initialize();

	initialize_command(&current_command);

	string_constructor(&current_path); // this will initialize the string

	string_assignment_operator(&current_path, root->name); // set the current path to be the root

	load_command_list();

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// running the simulator

	clear_terminal();

	while (go_again)
	{
		//clear_terminal();

		make_terminal_pretty(current_path.string_ptr);

		get_user_command(&current_command);

		if (current_command.valid == TRUE)
		{
			go_again = execute_command(&current_command, &current_path);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// deallocating all memory

	initialize_command(&current_command);

	string_destructor(&current_path);

	destroy_command_list();

	destroy_file_system_tree();

	clear_terminal();
}

//***************************************************************************************************************************************************************//

#endif