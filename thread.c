/************************************************************
*
* @File: thread.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 30th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"
#include "adts.h"

/*************************************************************
*
* @Name: write_socket
* @Goal: Send a message to client
* @Arg: In: client_socket
            frame_info = frame
       Out: None
* @Ret: Nothing
*
**************************************************************/

void write_socket (int client_socket, char destination[7], char type, char message[100])
{
    struct frame frame_info;
    // Create message frame
    bzero (frame_info.origin, 7);
    strcpy (frame_info.origin, "server");
    bzero (frame_info.destination, 7);
    strcpy (frame_info.destination, destination);
    frame_info.type = type;
    bzero (frame_info.data, 100);
    strcpy(frame_info.data, message);
    //Send frame
    write (client_socket, &frame_info, MAX_FRAME);
}

/*************************************************************
*
* @Name: parse_data_one_arg
* @Goal: Parse frame's data
* @Arg: In: frame_info = frame
            room_name = room's name
       Out: room_name = room's name
* @Ret: Nothing
*
**************************************************************/

void parse_data_one_arg (struct frame frame_info, char **room_name)
{
    int j = 0;
    size_t i = 0;
    char cursor, tmp[2048], *data;
    bzero(tmp, 2048);
    for (i = 0; i < strlen(frame_info.data) + 1; i++)
    {
        cursor = (frame_info.data)[i];
        if (cursor == '\0')
        {
            data = strdup (tmp);
            (*room_name) = data;
            j = 0;
        }
        else
        {
            tmp[j++] = cursor;
        }
    }
}

/*************************************************************
*
* @Name: parse_data_two_args
* @Goal: Parse frame's data
* @Arg: In: frame_info = frame
            room_name = room's name
            capacity = room's capacity
       Out: name = room's name
            capacity = room's capacity
* @Ret: Nothing
*
**************************************************************/

void parse_data_two_args (struct frame frame_info, char **room_name, int *capacity)
{
    int j = 0, count = 0;
    size_t i = 0;
    char cursor, tmp[2048], *data;
    bzero(tmp, 2048);
    for (i = 0; i < strlen(frame_info.data) + 1; i++)
    {
        cursor = (frame_info.data)[i];
        if (cursor == ':' || cursor == '\0')
        {
            data = strdup (tmp);
            switch (count)
            {
                case 0: // name
                {
                    (*room_name) = data;
                }
                break;
                case 1: // capacity
                {
                    if (capacity != 0)
                    {
                        (*capacity) = atoi (data);
                    }
                }
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

/*************************************************************
*
* @Name: delete_room
* @Goal: Responds to delete room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void delete_room (int client_socket, struct frame frame_info, struct room **rooms)
{
    char *room_name, str[100];
    bzero (str, 100);
    // Get frame data '<name>:<capacity>'
    parse_data_one_arg(frame_info, &room_name);
    // Search if user is the room's admin
    if (!linkedlist_delete_room(rooms, frame_info.origin, room_name))
    {
        sprintf (str, "Not enough privileges to delte room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'D', str);
    }
    else
    {
        sprintf (str, "Successfully deleted room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'D', str);
    }
}

/*************************************************************
*
* @Name: exit_room
* @Goal: Responds to exit room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void exit_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list)
{
    char *room_name, str[100];
    bzero (str, 100);
    // Get frame data '<name>'
    parse_data_one_arg (frame_info, &room_name);
    if (linkedlist_search_userexist (*rooms, frame_info.origin, room_name))
    {
        sprintf (str, "Room %s doesn't exist\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'X', str);
    }
    else if (!linkedlist_exit_room (rooms, frame_info.origin, room_name, list))
    {
        sprintf (str, "Not inside room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'X', str);
    }
    else
    {
        sprintf (str, "Successfully disconnected from room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'X', str);
    }
}

/*************************************************************
*
* @Name: talk_room
* @Goal: Responds to talk room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void talk_room (int client_socket, struct frame frame_info, int id_queue, struct room *rooms)
{
    char *room_name, str[100];
    bzero (str, 100);
    // Get frame data '<name>:<capacity>'
    parse_data_two_args (frame_info, &room_name, 0);
    // Search if room exists
    struct q_message message;
    message.id = 1;
    bzero (message.origin, 7);
    strcpy (message.origin, frame_info.origin);
    bzero (message.destination, 7);
    strcpy (message.destination, frame_info.destination);
    message.type = frame_info.type;
    bzero (message.data, 100);
    strcpy (message.data, frame_info.data);
    if (msgsnd (id_queue, (struct msgbuff*) &message, sizeof (message) - sizeof (long), IPC_NOWAIT) != 0)
    {
        perror ("msgsnd");
    }
}

/*************************************************************
*
* @Name: enter_room
* @Goal: Responds to enter room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void enter_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list)
{
    char *room_name, str[100];
    bzero (str, 100);
    // Get frame data '<name>'
    parse_data_one_arg (frame_info, &room_name);
    if (linkedlist_search_userexist (*rooms, frame_info.origin, room_name))
    {
        sprintf (str, "Already inside room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'S', str);
    }
    else if (!linkedlist_enter_room (rooms, frame_info.origin, room_name, list))
    {
        write_socket (client_socket, frame_info.origin, 'N', "Room is full or doesn't exist\n\n");
    }
    else
    {
        sprintf (str, "Successfully connected to room %s\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'S', str);
    }
}

/*************************************************************
*
* @Name: modify_room
* @Goal: Responds to modify room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void modify_room (int client_socket, struct frame frame_info, struct room **rooms)
{
    char *room_name, str[100];
    int capacity;
    bzero (str, 100);
    // Get frame data '<name>:<capacity>'
    parse_data_two_args (frame_info, &room_name, &capacity);
    // Search if user is the room's admin
    if (!linkedlist_modify_room (rooms, frame_info.origin, room_name, capacity))
    {
        sprintf (str, "Not enough privileges to modify room %s capacity\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'M', str);
    }
    else
    {
        sprintf (str, "Successfully modified room %s capacity\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'M', str);
    }
}

/*************************************************************
*
* @Name: create_room
* @Goal: Responds to create room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void create_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list)
{
    char *room_name, str[100];
    int capacity = 0, room_id = 0, user_id = 0;
    bzero (str, 100);
    // Get frame data '<name>:<capacity>'
    parse_data_two_args (frame_info, &room_name, &capacity);
    // Search for existing room
    if (!linkedlist_search_room(*rooms, room_name))
    {
        // Create a room id based on id on the room
        room_id = linkedlist_search_roomid (*rooms);
        // Get user id from users linkedlist
        user_id = linkedlist_search_userid (list, frame_info.origin);
        // Add a new room to the room's linkedlist
        if (capacity > 0)
        {
            append_room (rooms, frame_info.origin, room_name, capacity, room_id, user_id);
            sprintf (str, "Successfully created room %s\n\n", room_name);
            write_socket (client_socket, frame_info.origin, 'Z', str);
        }
        else
        {
            sprintf (str, "Room %s cannot be created\n\n", room_name);
            write_socket (client_socket, frame_info.origin, 'Z', str);
        }
    }
    else
    {
        sprintf (str, "Room %s already exists\n\n", room_name);
        write_socket (client_socket, frame_info.origin, 'Z', str);
    }
}

/*************************************************************
*
* @Name: show_rooms
* @Goal: Responds to show room request
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void show_rooms (int client_socket, struct frame frame_info, struct room *head)
{
    char str[100];
    int count = 0;
    struct room *curr = head->next;
    bzero (str, 100);
    write_socket (client_socket, frame_info.origin, 'S', "The following rooms are available:\n");
    while (curr != NULL)
    {
        sprintf (str, "[%s]-[%d/%d]\n\n", curr->name, users_linkedlist_length (curr->users), curr->capacity);
        write_socket (client_socket, frame_info.origin, 'S', str);
        curr = curr->next;
        count++;
    }
    if (count == 0)
    {
        write_socket (client_socket, frame_info.origin, 'S', "There is no currently rooms available\n\n");
    }
}

/*************************************************************
*
* @Name: socket_error
* @Goal: Creates a dedicated server
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void socket_error (int s, const char *msg)
{
    char str[2048];
    sprintf (str, "%s: %s\n\n", msg, strerror (s));
    write(1, str, strlen(str));
    exit (EXIT_FAILURE);
}

/*************************************************************
*
* @Name: message_reader
* @Goal: reads a message from specific mailbox
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void *message_reader (void *arg)
{
    int out = 0;
    struct thread thread_info;
    struct q_message message;
    struct frame frame_info;
    thread_info = *(struct thread*)(arg);
    while (!out)
    {
        if (msgrcv (thread_info.id_queue, (struct msgbuff*) &message, sizeof (message) - sizeof (long), thread_info.id_user + 1, 0) > 0)
        {
            // Create message frame
            bzero (frame_info.origin, 7);
            strcpy (frame_info.origin, "server");
            bzero (frame_info.destination, 7);
            strcpy (frame_info.destination, "client");
            frame_info.type = 'T';
            bzero (frame_info.data, 100);
            strcpy(frame_info.data, message.data);
            //Send frame
            write (thread_info.client_socket, &frame_info, MAX_FRAME);
        }
        else
        {
            out = 1;
        }
    }
    return (void *) 0;
}

/*************************************************************
*
* @Name: dedicated_server
* @Goal: Creates a dedicated server
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void *queue_reader (void *arg)
{
    int i = 0, j = 0, k = 0, out = 0, start = 1, *ids = NULL;
    struct thread thread_info;
    struct q_message message;
    char room[100], data[100], str[100];
    thread_info = *(struct thread*)(arg);
    bzero (room, 100);
    while (!out)
    {
        if (msgrcv (thread_info.id_queue, (struct msgbuff *) &message, sizeof (message) - sizeof (long), 1, 0) > 0)
        {
            // Initialize message backup
            bzero (data, 100);
            // Copy message to backup
            strcpy (data, message.data);
            i = 0;
            // Get the room from message data
            while (message.data[i] != ':' && start)
            {
                room[i] = message.data[i];
                i++;
            }
            linkedlist_get_room_usersid (thread_info.rooms, room, &ids);
            if (ids != NULL)
            {
                for (i = 1; i <= ids[0]; i++)
                {
                    if (ids[i] != linkedlist_search_userid (thread_info.users, message.origin))
                    {
                        message.id = ids[i] + 1;
                        if (start)
                        {
                            // Initalize message
                            bzero (message.data, 100);
                            // Copy initial message
                            bzero (str, 100);
                            sprintf (str, "[%s] %s: ", room, message.origin);
                            strcpy (message.data, str);
                            // Send message's first part
                            if (msgsnd (thread_info.id_queue, (struct msgbuff *) &message, sizeof (message) - sizeof (long), IPC_NOWAIT) != 0)
                            {
                                out = 1;
                            }
                            // Initialize message
                            bzero (message.data, 100);
                            // Copy message backup
                            for (k = strlen(room) + 1, j = 0; k < 100; k++, j++)
                            {
                                message.data[j] = data[k];
                            }
                        }
                        // Send message's second part
                        if (msgsnd (thread_info.id_queue, (struct msgbuff *) &message, sizeof (message) - sizeof (long), IPC_NOWAIT) != 0)
                        {
                            out = 1;
                        }
                    }
                }
            }
            free (ids);
            start = 0;
            for (i = 0; i < 100; i++)
            {
                if (message.data[i] == '\n')
                {
                    start = 1;
                    i = 100;
                    bzero (room, 100);
                }
            }
        }
        else
        {
            out = 1;
        }
    }
    return (void *) 0;
}

/*************************************************************
*
* @Name: socket_reader
* @Goal: reads from the socket
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void *socket_reader (void *arg)
{
    int n = 0, out = 0;
    struct thread thread_info;
    struct frame frame_info;
    thread_info = *(struct thread*)(arg);
    while (!out)
    {
        n = 0;
        bzero (&frame_info, MAX_FRAME);
        bzero (frame_info.origin, 7);
        n += read (thread_info.client_socket, frame_info.origin, 7);
        bzero (frame_info.destination, 7);
        n += read (thread_info.client_socket, frame_info.destination, 7);
        n += read (thread_info.client_socket, &frame_info.type, 1);
        bzero (frame_info.data, 100);
        n += read (thread_info.client_socket, frame_info.data, 100);
        if (n != MAX_FRAME)
        {
            out = 1;
            write (1, "Client disconnected thread\n", strlen ("Client disconnected thread\n"));
            linkedlist_disconnect_user (&thread_info.users, thread_info.id_user);
            close (thread_info.client_socket);
        }
        else
        {
            switch (frame_info.type)
            {
                case 'S':
                    show_rooms (thread_info.client_socket, frame_info, thread_info.rooms);
                break;
                case 'Z':
                    create_room (thread_info.client_socket, frame_info, &thread_info.rooms, thread_info.users);
                break;
                case 'M':
                    modify_room (thread_info.client_socket, frame_info, &thread_info.rooms);
                break;
                case 'N':
                    enter_room (thread_info.client_socket, frame_info, &thread_info.rooms, thread_info.users);
                break;
                case 'T':
                    talk_room (thread_info.client_socket, frame_info, thread_info.id_queue, thread_info.rooms);
                break;
                case 'X':
                    exit_room (thread_info.client_socket, frame_info, &thread_info.rooms, thread_info.users);
                break;
                case 'D':
                    delete_room (thread_info.client_socket, frame_info, &thread_info.rooms);
                break;
            }
        }
    }
    return (void *) 0;
}

/*************************************************************
*
* @Name: dedicated_server
* @Goal: Creates a dedicated server
* @Arg: In: None
       Out: None
* @Ret: Nothing
*
**************************************************************/

void *dedicated_server (void *arg)
{
    int s = 0;
    struct thread thread_info;
    pthread_t t1,t2;
    thread_info = *(struct thread*)(arg);
    // Message reader
    s = pthread_create (&t1, NULL, message_reader, (void *) &thread_info);
    if (s != 0)
    {
        socket_error (s, "pthread_create");
    }
    // Socket reader
    s = pthread_create (&t2, NULL, socket_reader, (void *) &thread_info);
    if (s != 0)
    {
        socket_error (s, "pthread_create");
    }
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return (void *) 0;
}
