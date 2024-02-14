#ifndef MY_STRING_H
#define MY_STRING_H

//#include "lab_one_header.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
    char* string_ptr; // the actual string stored here
    int length; // the number of characters present in the string

} my_string;

//***************************************************************************************************************************************************************//
//     function call        : initialize_string()
//     function description : this function will initialize the my_string struct so that the string is not a dangling pointer
//     function inputs      : my_string*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void string_constructor(my_string* str)
{
    str->string_ptr = NULL;
    str->length = 0;
}

//***************************************************************************************************************************************************************//
//     function call        : string_destructor()
//     function description : this function will delete the string and make the pointer NULL
//     function inputs      : my_string*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

void string_destructor(my_string* str)
{
    if (str->string_ptr != NULL) // make sure that the string is not empty
    {
        free(str->string_ptr); // deallocate all memory present here
        str->string_ptr = NULL; // set the pointer to be NULL
        str->length = 0; // set the length to be zero
    }
}

//***************************************************************************************************************************************************************//
//     function call        : string_copy_constructor
//     function description : this function will make a copy of the given stirng
//     function inputs      : my_string*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

my_string* string_copy_constructor(my_string* input)
{
    my_string* new_string = (my_string*)malloc(sizeof(my_string));

    if (new_string != NULL) // make sure malloc worked
    {
        new_string->string_ptr = (char*)calloc(input->length + sizeof(char), sizeof(char));

        if (new_string->string_ptr != NULL)
        {
            for (int i = 0; i <= input->length; ++i)
            {
                new_string->string_ptr[i] = input->string_ptr[i];
            }

            new_string->length = input->length;
        }
        else
        {
            free(new_string);
        }
    }

    return new_string;
}

//***************************************************************************************************************************************************************//
//     function call        : string_assignment_operator
//     function description : this function will make update the string pointer to point to a string containing new_string
//     function inputs      : my_string*, char*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

boolean string_assignment_operator(my_string* str, char* new_string)
{
    char* new_string_ptr = NULL;
    int i = 0;
    
    if (str->string_ptr != NULL) // if the string is not empty, empty it
    {
        free(str->string_ptr);
        str->string_ptr = NULL;
    }

    // by this point the string is empty, so now we will put the new string into it


    new_string_ptr = (char*)calloc((strlen(new_string) + 1), (sizeof(char))); // allocate enough memory so that the the string fits with a single null character at the end

    if (new_string_ptr != NULL)
    {
        // put the contents of the new string into new_string pointer

        for (i = 0; i < strlen(new_string); ++i) // coppies all of the contents over
        {
            new_string_ptr[i] = new_string[i];
        }

        new_string_ptr[i] = '\0'; // set the last character to be the null character

        str->string_ptr = new_string_ptr; // set the string pointer to point to the new string pointer

        str->length = strlen(new_string); // set the size of the string to be the size of the string that was passed into the function

        return TRUE; // return true
    }
    else
    {
        return FALSE; // return false if the memory was not allocated
    }



}

//***************************************************************************************************************************************************************//
//     function call        : string_addition_operator_s
//     function description : this function will concatinate two strings
//     function inputs      : my_string*, char*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

boolean string_addition_operator_s(my_string* str, char* new_string)
{
    int old_length_e = 0, old_length_n, new_length = 0, i = 0, j = 0;

    old_length_e = strlen(str->string_ptr);

    old_length_n = strlen(new_string);

    new_length = old_length_e + old_length_n + 1; // plus one is for the null character

    str->string_ptr = (char*)realloc(str->string_ptr, new_length);

    if (str->string_ptr != NULL)
    {
        for (i = old_length_e, j = 0; j < old_length_n ; ++j, ++i)  // ORIGIONALLY HAD >> j < old_length - 1 <<    <--- might cause issues? ////////////////////////////
        {
            str->string_ptr[i] = new_string[j];
        }

        str->string_ptr[i] = '\0'; // make the last element of the string the null character
        str->length = new_length - 1; // get rid of the null character's spot in the length

        return TRUE;
    }
    else // the memory was not found
    {
        return FALSE;
    }
}

//***************************************************************************************************************************************************************//
//     function call        : string_addition_operator_c
//     function description : this function will concatinate a string with a character
//     function inputs      : my_string*, char
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

boolean string_addition_operator_c(my_string* str, char character)
{
    int new_length = 0;

    new_length = strlen(str->string_ptr) + 2; // +1 for new character, +1 for the NULL character

    str->string_ptr = (char*)realloc(str->string_ptr, new_length); // reallocate this

    if (str->string_ptr != NULL) // was realloc successful
    {
        str->string_ptr[new_length - 2] = character;
        str->string_ptr[new_length - 1] = '\0';
        str->length += 1;

        return TRUE;
    }
    else
    {
        return FALSE;
    }


}

//***************************************************************************************************************************************************************//
//     function call        : string_subtraction_operator
//     function description : this function will concatinate a string with a character
//     function inputs      : my_string*, char*
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/7/2024
//     last edited          : 2/7/2024
//     programmer           : Sam Stanley

boolean delimeter_hit(char* str, char* delimeters)
{
    int i = 0;

    for (; i < strlen(delimeters); ++i)
    {
        if (*str == delimeters[i])
        {
            return TRUE;
        }
    }

    return FALSE;
}

boolean string_subtraction_operator(my_string* str, char* delimeters)
{
    int i = strlen(str->string_ptr) - 1;

    for (; i >= 0; --i) // this funds the location of the delimeter
    {
        if (delimeter_hit(&(str->string_ptr[i]), delimeters))
        {
            break;
        }
    }

    if (i < 0) // we did not find a delimeter
    {
        return FALSE;
    }
    else // we did find a delimeter, and i is pointing exactly at the delimeter
    {
        str->string_ptr = (char*)realloc(str->string_ptr, ((i + 1) * sizeof(char)));
        
        if (str->string_ptr != NULL) // check if realloc worked
        {
            str->string_ptr[i] = '\0';
            str->length = i;
            return TRUE;
        }
        else
        {
            str->length = 0;
            return FALSE;
        }
    }


}

boolean string_subtraction_operator_s(my_string* str, char* delimeters)
{
    int i = strlen(str->string_ptr) - 1;

    for (; i >= 0; --i) // this funds the location of the delimeter
    {
        if (delimeter_hit(&(str->string_ptr[i]), delimeters))
        {
            break;
        }
    }

    if (i < 0) // we did not find a delimeter
    {
        return FALSE;
    }
    else // we did find a delimeter, and i is pointing exactly at the delimeter
    {
        str->string_ptr = (char*)realloc(str->string_ptr, ((i + 2) * sizeof(char)));
        
        if (str->string_ptr != NULL) // check if realloc worked
        {
            str->string_ptr[i + 1] = '\0';
            str->length = i + 1;
            return TRUE;
        }
        else
        {
            str->length = 0;
            return FALSE;
        }
    }

}

//***************************************************************************************************************************************************************//
//     function call        : cut_string
//     function description : this function is sort of like a strtok, but without changing the string
//     function inputs      : char*, char*, char, int
//     function outputs     : indirectly updates the contents of the input parameter
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/10/2024
//     last edited          : 2/10/2024
//     programmer           : Sam Stanley

int cut_string(char* current_path, char* destination, char delimeter, int dest_size)
{
    int i = 0;
    static char* current_pointer;

    if (current_path != NULL) // check if this is a new call
    {
        current_pointer = current_path; // if so, update the current pointer
    }

    for (i = 0; i < dest_size; ++i) // pad destination with null characters
    {
        destination[i] = '\0';
    }

    if (*current_pointer == '\0')
    {
        return 0;
    }

    if (current_pointer[0] == delimeter) // if the pointer is pointing at a delimeter, move it one over
    {
        current_pointer += sizeof(char); // put one past the current index
    }

    for (i = 0; (i < dest_size - 1) && (*current_pointer != '\0') && (*current_pointer != delimeter); current_pointer += sizeof(char), ++i)
    {
        destination[i] = *current_pointer;
    }

    if (i == (dest_size - 1)) // if we filled up the entire destination
    {
        for (; (*current_pointer != '\0') && (*current_pointer != delimeter); current_pointer += sizeof(char));
    }

    return 1;
}

//***************************************************************************************************************************************************************//
//     function call        : num_occurences
//     function description : this function returns the number of times a character appears in a c string
//     function inputs      : char*, char
//     function outputs     : int
//     preconditions        : input_string is null terminated
//     postconditions       : none
//     created              ; 2/10/2024
//     last edited          : 2/10/2024
//     programmer           : Sam Stanley

int num_occurences(char* input_string, char search)
{
    int i = 0, j = 0;
    for (i = 0; input_string[i] != '\0'; ++i)
    {
        if (input_string[i] == search)
        {
            ++j;
        }
    }

    return j;
}

//***************************************************************************************************************************************************************//
//     function call        : is_ended_by
//     function description : this function returns 1 if the input string is ended by end, returns 0 ow
//     function inputs      : char*, char*
//     function outputs     : int
//     preconditions        : input_string and end are null terminated
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

int is_ended_by(char* input_string, char* end)
{
    int i = strlen(input_string) - 1,
    j = strlen(end) - 1;

    for (; (i >= 0) && (j >= 0); --i, --j)
    {
        if (input_string[i] != end[j])
        {
            return 0;
        }
    }

    if (j == -1) // did we make it to the end of the end
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//***************************************************************************************************************************************************************//
//     function call        : locate_nth_occurrence
//     function description : this function returns the index of the nth occurrence of a givn character in a string
//     function inputs      : char*, cahr, int, int
//     function outputs     : int
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

int locate_nth_occurrence(char* string, char delimeter, int n, int size)
{
     int i = 0, j = 0; 

     for (i = 0; i < size; ++i)
     {
        if (string[i] == delimeter)
        {
            ++j;

            if (j == n)
            {
                return i;
            }
        }
     }

     return -1;
}

//***************************************************************************************************************************************************************//
//     function call        : replace_occurences
//     function description : this function replaces all occurrences of a given character in a stirng with another given character
//     function inputs      : char*, char, char, int
//     function outputs     : void
//     preconditions        : none
//     postconditions       : none
//     created              ; 2/11/2024
//     last edited          : 2/11/2024
//     programmer           : Sam Stanley

void replace_occurences(char* string, char search, char replacement, int size)
{
    int i = 0;

    for (i = 0; i < size; ++i)
    {
        if (string[i] == search)
        {
            string[i] = replacement;
        }
    }
}

//***************************************************************************************************************************************************************//

#endif