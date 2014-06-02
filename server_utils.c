/************************************************************
*
* @File: server_utils.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"
#include "adts.h"
#include "linkedlist.c"
#include "config.c"
#include "thread.c"

/*************************************************************
*
* @Name: create_auth_frame
* @Goal: Creates an authentication frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_auth_frame (struct frame *frame_info)
{
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, frame_info->origin);
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, "server");
    frame_info->type = 'A';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, "Authentication correct");
}

/*************************************************************
*
* @Name: create_conn_error_frame
* @Goal: Creates a connection error frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_conn_error_frame (struct frame *frame_info)
{
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, frame_info->origin);
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, "server");
    frame_info->type = 'E';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, "Connection error");
}

/*************************************************************
*
* @Name: create_auth_error_frame
* @Goal: Creates an authentication error frame
* @Arg: In: frame_info = frame structure
            user_info = user structure
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_auth_error_frame (struct frame *frame_info)
{
    bzero (frame_info->destination, 7);
    strcpy(frame_info->destination, frame_info->origin);
    bzero (frame_info->origin, 7);
    strcpy(frame_info->origin, "server");
    frame_info->type = 'E';
    bzero (frame_info->data, 100);
    strcpy(frame_info->data, "Authentication error");
}

/*************************************************************
*
* @Name: check_frame
* @Goal: Check frame received
* @Arg: In: frame_info = frame structure
       Out: None
* @Ret: Returns 1 if all data is correct, 0 otherwise
*
**************************************************************/

int check_frame (struct frame frame_info)
{
    if (strcmp(frame_info.destination, "server") != 0
        || frame_info.destination[0] == '\0'
        || frame_info.type == '\0'
        || frame_info.data[0] == '\0')
    {
        return (0);
    }
    return (1);
}

/* ***********************************************************
*
* @Name: RSIInt
* @Goal: Ctrl+C interrupt handler.
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void RSIInt (void)
{
    write(1, "\nBye bye\n", strlen("\nBye bye\n"));
    close (ssocket_fd);
    msgctl (id_queue,IPC_RMID, NULL);
    exit (EXIT_FAILURE);
}

/*************************************************************
*
* @Name: connection_handler()
* @Goal: Handle client connections
* @Arg: In: list = linked list
       Out: None
* @Ret: Returns 0 if all when well, -1 otherwise
*
**************************************************************/

int connection_handler (struct config *configuration, struct node *list, struct room *rooms)
{
    int aux = 0, n = 0, s = 0, first = 1, tr = 1, client_socket = 0;
    uint16_t tmp_port = 0;
    struct sockaddr_in socket_addr, client_addr;
    struct frame frame_info;
    struct thread thread_info;
    pthread_t t1, t2;
    pthread_attr_t attr;
    key_t key;
    // Check port
    aux = configuration->port;
    if ( aux < 1024 || aux > 65535)
    {
        perror ("port");
        return (EXIT_FAILURE);
    }
    tmp_port = aux;
    // Socket connection
    ssocket_fd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ssocket_fd < 0)
    {
        perror ("socket");
        return (EXIT_FAILURE);
    }
    // Socket address
    bzero (&socket_addr, sizeof (socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons (tmp_port);
    socket_addr.sin_addr.s_addr = INADDR_ANY;
    // Socket address linking
    // kill "Address already in use" error message
    if (setsockopt (ssocket_fd, SOL_SOCKET, SO_REUSEADDR, &tr, sizeof (int)) == -1)
    {
        perror ("setsockopt");
        return (EXIT_FAILURE);
    }
    if (bind (ssocket_fd, (void *) &socket_addr, sizeof (socket_addr)) < 0)
    {
        perror ("bind");
        close (ssocket_fd);
        return (EXIT_FAILURE);
    }
    listen (ssocket_fd, 10);
    write (1, "Server connected\n", strlen ("Server connected\n"));
    signal (SIGINT, (void *) RSIInt);
    key = ftok ("/bin/ls", 2);
    if (key == -1)
    {
        perror("ftok");
        exit(-1);
    }
    id_queue = msgget(key,IPC_CREAT | 0660);
    if (id_queue == -1)
    {
        perror("msgget");
        exit(-1);
    }
    while (1)
    {
        // Client socket
        socklen_t c_len = sizeof (client_addr);
        client_socket = accept (ssocket_fd, (void *) &client_addr, &c_len);
        if (client_socket < 0)
        {
            perror("accept");
            close (ssocket_fd);
            return (EXIT_FAILURE);
        }
        // Read client frames
        n = 0;
        bzero (&frame_info, MAX_FRAME);
        bzero (frame_info.origin, 7);
        n += read (client_socket, frame_info.origin, 7);
        bzero (frame_info.destination, 7);
        n += read (client_socket, frame_info.destination, 7);
        n += read(client_socket, &frame_info.type, 1);
        bzero (frame_info.data, 100);
        n += read (client_socket, frame_info.data, 100);
        if (n != MAX_FRAME)
        {
            write (1, "Client disconnected\n", strlen ("Client disconnected\n"));
            close (client_socket);
        }
        else
        {
            if (!check_frame (frame_info))
            {
                write (1, "Error communicacio\n", strlen("Error communicacio\n"));
                // Create auth error frame
                create_conn_error_frame (&frame_info);
                //Send frame
                write (client_socket, &frame_info, MAX_FRAME);
            }
            else
            {
                if (frame_info.type == 'P')
                {
                    if (!linkedlist_search_user (list, frame_info.origin, frame_info.data) || linkedlist_check_connected (&list, frame_info.origin, frame_info.data))
                    {
                        // Create auth error frame
                        create_auth_error_frame (&frame_info);
                        //Send frame
                        write (client_socket, &frame_info, MAX_FRAME);
                    }
                    else
                    {
                        // Create auth frame
                        create_auth_frame (&frame_info);
                        //Send frame
                        write (client_socket, &frame_info, MAX_FRAME);
                        thread_info.client_socket = client_socket;
                        thread_info.rooms = rooms;
                        thread_info.users = list;
                        thread_info.id_queue = id_queue;
                        thread_info.id_user = linkedlist_search_userid (thread_info.users, frame_info.destination);
                        write (1, "Client connected\n", strlen ("Client connected\n"));
                        // Dedicated server
                        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                        s = pthread_create (&t1, &attr, dedicated_server, (void *) &thread_info);
                        if (s != 0)
                        {
                            socket_error (s, "pthread_create");
                        }
                        if (first)
                        {
                            // Queue reader
                            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                            s = pthread_create (&t2, &attr, queue_reader, (void *) &thread_info);
                            if (s != 0){
                                socket_error (s, "pthread_create");
                            }
                            first = 0;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
