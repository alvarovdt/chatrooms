/************************************************************
*
* @File: client.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: November 4th, 2013
*
*************************************************************/

// Own libraries
#include "client_utils.c"

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
    int socket_fd = 0;
    struct user *user_info = NULL;
    struct config *configuration = NULL;
    // Configuration intitialization
    init_config (&configuration);
    // User initialization
    get_user_info (&user_info);
    // Check command (only once)
    check_command (user_info, &socket_fd);
    // Handle select
    select_handler(user_info, socket_fd);
    exit (EXIT_SUCCESS);
}
