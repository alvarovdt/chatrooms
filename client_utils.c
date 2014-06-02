/************************************************************
*
* @File: client_utils.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"
#include "adts.h"
#include "linkedlist.c"
#include "user.c"
#include "config.c"
#include "thread.c"

void select_handler(struct user *user_info, int socket_fd)
{
    int n = 0;
    struct frame frame_info;
    fd_set set;
    // Select features
    FD_ZERO (&set);
    FD_SET (socket_fd, &set);
    FD_SET (0, &set);
    while (1)
    {
        if (select (1024, &set, NULL, NULL, NULL) == -1)
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }
        else
        {
            // Client socket
            if (FD_ISSET (socket_fd, &set))
            {
                // Read socket
                n = 0;
                bzero (&frame_info, MAX_FRAME);
                bzero (frame_info.origin, 7);
                n += read (socket_fd, frame_info.origin, 7);
                bzero (frame_info.destination, 7);
                n += read (socket_fd, frame_info.destination, 7);
                n += read(socket_fd, &frame_info.type, 1);
                bzero (frame_info.data, 100);
                n += read (socket_fd, frame_info.data, 100);
                if (n != MAX_FRAME)
                {
                    write (1, "Client disconnected\n", strlen ("Client disconnected\n"));
                    close (socket_fd);
                }
                else
                {
                    write (1, frame_info.data, strlen(frame_info.data));
                }
                // Reload select
                FD_ZERO (&set);
                FD_SET (socket_fd, &set);
                FD_SET (0, &set);
            }
            // Client keyboard
            else if (FD_ISSET (0, &set))
            {
                check_command (user_info, &socket_fd);
                // Reload select
                FD_ZERO (&set);
                FD_SET (socket_fd, &set);
                FD_SET (0, &set);
            }
        }
    }
}

/*************************************************************
*
* @Name: exit_room
* @Goal: Exit room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void exit_room_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'X';
    bzero (frame_info.data, 100);
    // Copy command arguments
    for (i = length; i < strlen(command); i++)
    {
        frame_info.data[j] = command[i];
        j++;
    }
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: talk
* @Goal: Talk inside room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void talk_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'T';
    bzero (frame_info.data, 100);
    // Message is larger than protocol message limitation
    if ((strlen(command) - 1) > 100)
    {
        // Copy command arguments
        for (i = length; i < strlen (command); i++)
        {
            if (j == 99)
            {
                frame_info.data[j] = command[i];
                j = 0;
                //Send frame
                write (*socket_fd, &frame_info, MAX_FRAME);
                bzero (frame_info.data, 100);
            }
            else if (i == (strlen (command) - 1))
            {
                frame_info.data[j++] = command[i];
                frame_info.data[j] = '\n';
                //Send frame
                write (*socket_fd, &frame_info, MAX_FRAME);
                bzero (frame_info.data, 100);
            }
            else
            {
                frame_info.data[j++] = command[i];
            }
        }
    }
    else
    {
        bzero (frame_info.data, 100);
        // Copy command arguments
        for (i = length; i < strlen(command); i++)
        {
            frame_info.data[j++] = command[i];
        }
        frame_info.data[j] = '\n';
        //Send frame
        write (*socket_fd, &frame_info, MAX_FRAME);
    }
}

/*************************************************************
*
* @Name: enter_room
* @Goal: Enter room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void enter_room_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'N';
    bzero (frame_info.data, 100);
    // Copy command arguments
    for (i = length; i < strlen(command); i++)
    {
        frame_info.data[j] = command[i];
        j++;
    }
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: modify_room
* @Goal: Modify room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void modify_room_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'M';
    bzero (frame_info.data, 100);
    // Copy command arguments
    for (i = length; i < strlen(command); i++)
    {
        frame_info.data[j] = command[i];
        j++;
    }
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: delete_room
* @Goal: Delete room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void delete_room_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'D';
    bzero (frame_info.data, 100);
    // Copy command arguments
    for (i = length; i < strlen(command); i++)
    {
        frame_info.data[j] = command[i];
        j++;
    }
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: create_room
* @Goal: Create room
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_room_request (int *socket_fd, struct user *user_info, char *command, size_t length)
{
    struct frame frame_info;
    size_t i = 0, j = 0;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'Z';
    bzero (frame_info.data, 100);
    // Copy command arguments
    for (i = length; i < strlen(command); i++)
    {
        frame_info.data[j] = command[i];
        j++;
    }
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: show_rooms
* @Goal: Show rooms
* @Arg: In: socket_fd = server port
            user_info = server ip
       Out: None
* @Ret: Nothing
*
**************************************************************/

void show_rooms_request (int *socket_fd, struct user *user_info)
{
    struct frame frame_info;
    // Create login frame
    bzero (frame_info.origin, 7);
    strcpy(frame_info.origin, user_info->username);
    bzero (frame_info.destination, 7);
    strcpy(frame_info.destination, "server");
    frame_info.type = 'S';
    bzero (frame_info.data, 100);
    strcpy(frame_info.data, "show rooms");
    //Send frame
    write (*socket_fd, &frame_info, MAX_FRAME);
    write (1, "\n", 1);
}

/*************************************************************
*
* @Name: get_port_ip
* @Goal: Connect to server
* @Arg: In: port = server port
            ip = server ip
            command = command inputed
            length = comman length
       Out: port = server port
            ip = server ip
* @Ret: Nothing
*
**************************************************************/

void get_port_ip (int *port, char **ip, char *command, size_t length)
{
    int count = 0, j = 0;
    char tmp[100], *data, cursor;
    size_t i = 0;
    bzero(tmp, 100);
    if (length < strlen (command))
    {
        for (i = length; i <= strlen (command); i++)
        {
            cursor = command[i];
            if (cursor == ':' || cursor == '\0')
            {
                tmp[j++] = '\0';
                data = strdup (tmp);
                switch (count)
                {
                    case 0: // port
                        *port = atoi(data);
                    break;
                    case 1: // ip
                        (*ip) = data;
                    break;
                }
                j = 0;
                count++;
            }
            else
            {
                tmp[j++] = cursor;
            }
        }
    }
}

/*************************************************************
*
* @Name: connect_server
* @Goal: Connect to server
* @Arg: In: command = command inputed
            length = command length
       Out: None
* @Ret: Returns 0 if all when well, -1 otherwise
*
**************************************************************/

void connect_server (struct user *user_info, char *command, size_t length, int *socket_fd)
{
    int port = 0, frame_end = 0, n = 0;
    char *ip;
    uint16_t tmp_port;
    struct sockaddr_in stDireccionServidor;
    struct frame frame_info;
    get_port_ip (&port, &ip, command, length);
    //Check port
    if (port < 1024 || port > 65535)
    {
        write (1, "Comunication Error. Ending session\n", strlen ("Comunication Error. Ending session\n"));
    }
    tmp_port = port;
    //Socket creation
    *socket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (*socket_fd < 0)
    {
        write (1, "Comunication Error. Ending session\n", strlen ("Comunication Error. Ending session\n"));
    }
    //Struct creation
    memset (&stDireccionServidor, 0, sizeof(stDireccionServidor));
    stDireccionServidor.sin_family = AF_INET;
    stDireccionServidor.sin_port = htons (tmp_port);
    stDireccionServidor.sin_addr.s_addr = inet_addr (ip);
    //Server connection
    if (connect (*socket_fd, &stDireccionServidor, sizeof (stDireccionServidor)) < 0)
    {
        write (1, "Comunication Error. Ending session\n", strlen("Comunication Error. Ending session\n"));
    }
    else
    {
        // Create login frame
        create_login_frame (&frame_info, user_info);
        //Send frame
        write (*socket_fd, &frame_info, MAX_FRAME);
        write (1, "\n", 1);
        // While connection is up
        frame_end = 0;
        while (!frame_end)
        {
            n = 0;
            bzero (&frame_info, MAX_FRAME);
            bzero (frame_info.origin, 7);
            n += read (*socket_fd, frame_info.origin, 7);
            bzero (frame_info.destination, 7);
            n += read (*socket_fd, frame_info.destination, 7);
            n += read(*socket_fd, &frame_info.type, 1);
            bzero (frame_info.data, 100);
            n += read (*socket_fd, frame_info.data, 100);
            if (n != MAX_FRAME)
            {
                frame_end = 1;
                perror("Error");
            }
            else
            {
                if (frame_info.type == 'E')
                {
                    write(1, "Authentication error, try again\n", strlen("Authentication error, try again\n"));
                    // Create confirmed authentication error frame
                    create_auth_error_confirmation_frame (&frame_info, user_info);
                    //Send frame
                    write (*socket_fd, &frame_info, MAX_FRAME);
                    write (1, "\n", 1);
                    frame_end = 1;
                    close (*socket_fd);
                }
                else if (frame_info.type == 'A')
                {
                    write(1, "Connected successfully, welcome to LSHangIn\n", strlen("Connected successfully, welcome to LSHangIn\n"));
                    // Create authentication confirmation frame
                    create_auth_confirmation_frame (&frame_info, user_info);
                    //Send frame
                    write (*socket_fd, &frame_info, MAX_FRAME);
                    write (1, "\n", 1);
                    frame_end = 1;
                }
            }
        }
    }
}

/*************************************************************
*
* @Name: perform_command
* @Goal: Perform action based on command number
* @Arg: In: command number
       Out: None
* @Ret: None
*
**************************************************************/

void perform_command (int cmd_num, char *command, size_t length, struct user *user_info, int *socket_fd)
{
    switch (cmd_num) {
        case 0:
            connect_server (user_info, command, length, socket_fd);
        break;
        case 1:
            show_rooms_request (socket_fd, user_info);
        break;
        case 2:
            create_room_request (socket_fd, user_info, command, length);
        break;
        case 3:
            delete_room_request (socket_fd, user_info, command, length);
        break;
        case 4:
            modify_room_request (socket_fd, user_info, command, length);
        break;
        case 5:
            enter_room_request (socket_fd, user_info, command, length);
        break;
        case 6:
            talk_request (socket_fd, user_info, command, length);
        break;
        case 7:
            exit_room_request (socket_fd, user_info, command, length);
        break;
    }
}

/*************************************************************
*
* @Name: check_command
* @Goal: Check if inputed command is correct
* @Arg: In: None
       Out: None
* @Ret: None
*
**************************************************************/

void check_command (struct user *user_info, int *socket_fd)
{
    char buff[2048];
    char cmd1[] = {"$connect"}, cmd2[] = {"$show_rooms"}, cmd3[] = {"$create_room"},
         cmd4[] = {"$delete_room"}, cmd5[] = {"$modify_room"}, cmd6[] = {"$enter_room"},
         cmd7[] = {"$talk"}, cmd8[] = {"$exit_room"};
    size_t cmd1_c = 0, cmd2_c = 0, cmd3_c = 0, cmd4_c = 0,
           cmd5_c = 0, cmd6_c = 0, cmd7_c = 0, cmd8_c = 0;
    size_t i = 0;
    char *commands[8] = {cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8};
    size_t counters[8] = {cmd1_c, cmd2_c, cmd3_c, cmd4_c, cmd5_c, cmd6_c, cmd7_c, cmd8_c};
    int j = 0;
    bzero(buff, 2048);
    read (0, buff, sizeof (buff));
    buff[strlen(buff)-1] = '\0';
    for (i = 0; i < strlen(buff); i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (buff[i] == commands[j][i] && i == counters[j])
            {
                counters[j]++;
                if (counters[j] == strlen(commands[j]))
                {
                    perform_command (j, strdup(buff), strlen(commands[j]), user_info, socket_fd);
                }
            }
        }
    }
}

/*************************************************************
*
* @Name: create_auth_confirmation_frame
* @Goal: Creates an authentication confirmation frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_auth_confirmation_frame (struct frame *frame_info, struct user *user_info)
{
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, user_info->username);
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, "server");
    frame_info->type = 'C';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, "Connected successfully");
}

/*************************************************************
*
* @Name: create_auth_error_confirmation_frame
* @Goal: Creates an authentication error confirmation frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_auth_error_confirmation_frame (struct frame *frame_info, struct user *user_info)
{
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, user_info->username);
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, "server");
    frame_info->type = 'E';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, "Confirmed error");
}

/*************************************************************
*
* @Name: create_login_frame
* @Goal: Creates a login frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_login_frame (struct frame *frame_info, struct user *user_info)
{
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, user_info->username);
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, "server");
    frame_info->type = 'P';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, user_info->passwd);
}
