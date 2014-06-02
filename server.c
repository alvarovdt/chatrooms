    /************************************************************
*
* @File: server.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: September 20th, 2013
*
*************************************************************/

// Own libraries
#include "server_utils.c"

/*************************************************************
*
* @Name: main
* @Goal: Choose option and proccess information by user choice
* @Arg: In: argv = argument values array
*           argc = argument count array
* @Ret: Returns 0 if all when wll, -1 otherwise
*
**************************************************************/

int main (void)
{
    struct node *list = NULL;
    struct room *rooms = NULL;
    struct config *configuration = NULL;
    // User linked list initialization
    if (user_linkedlist_create(&list) < 0)
    {
        perror("user_linkedlist_create");
        exit (EXIT_FAILURE);
    }
    if (rooms_linkedlist_create(&rooms) < 0)
    {
        perror("room_linkedlist_create");
        exit (EXIT_FAILURE);
    }
    // Configuration initialization
    init_config(&configuration);
    // Handle connections
    connection_handler(configuration, list, rooms);
    exit (EXIT_SUCCESS);
}
