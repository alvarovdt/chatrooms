/************************************************************
*
* @File: linkedlist.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"

/*************************************************************
*
* @Name: linkedlist_disconnect_user
* @Goal: Disconnect user
* @Arg: In: list = linked list
            id = user's id
        Out: None
* @Ret: Nothing
*
**************************************************************/

void linkedlist_disconnect_user (struct node **head_ref, int id)
{
    struct node *curr = *head_ref;
    while (curr != NULL)
    {
        if (curr->id == id)
        {
            curr->connected = 0;
        }
        curr = curr->next;
    }
}

/*************************************************************
*
* @Name: linkedlist_check_connected
* @Goal: Check is user is connected
* @Arg: In: list = linked list
       Out: None
* @Ret: Return 1 if found, 0 otherwise
*
**************************************************************/

int linkedlist_check_connected (struct node **head_ref, char *user, char *passwd)
{
    struct node *curr = *head_ref;
    while (curr != NULL)
    {
        if ((strcmp (curr->user, user) == 0) && (strcmp (curr->passwd, passwd) == 0))
        {
            if (curr->connected == 0)
            {
                curr->connected = 1;
                return (0);
            }
            else
            {
                return (1);
            }
        }
        curr = curr->next;
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_get_username
* @Goal: Search through linked list's values
* @Arg: In: head = room's linked list
            id = user id
       Out: username = user name
* @Ret: Nothing
*
**************************************************************/

char * linkedlist_get_username (struct node *head, int id)
{
    struct node *curr = head->next;
    while (curr != NULL)
    {
        if (curr->id == id)
        {
            return (curr->user);
        }
        curr = curr->next;
    }
    return (NULL);
}

/*************************************************************
*
* @Name: linkedlist_search_userexist
* @Goal: Search through linked list's values
* @Arg: In: head = room's linked list
            name = room's name
            ids = users ids
       Out: ids = users ids
* @Ret: Nothing
*
**************************************************************/

int linkedlist_search_userexist (struct room *head, char *username, char *room_name)
{
    struct room *curr = head->next;
    struct node *curr_user;
    while (curr != NULL)
    {
        curr_user = curr->users->next;
        if (strcmp (curr->name, room_name) == 0)
        {
            while (curr_user != NULL)
            {
                if (strcmp (curr_user->user, username) == 0)
                {
                    return (1);
                }
                curr_user = curr_user->next;
            }
        }
        curr = curr->next;
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_get_room_usersid
* @Goal: Search through linked list's values
* @Arg: In: head = room's linked list
            name = room's name
            ids = users ids
       Out: ids = users ids
* @Ret: Nothing
*
**************************************************************/

void linkedlist_get_room_usersid (struct room *head, char *name, int **ids)
{
    struct room *curr = head->next;
    struct node *curr_user;
    int i = 1;
    *ids = (int *) malloc (sizeof (int) * (i + 1));
    while (curr != NULL)
    {
        curr_user = curr->users->next;
        if (strcmp (curr->name, name) == 0)
        {
            while (curr_user != NULL)
            {
                (*ids)[i++] = curr_user->id;
                *ids = (int *) realloc (*ids, sizeof (int) * (i + 1));
                curr_user = curr_user->next;
            }
        }
        curr = curr->next;
    }
    (*ids)[0] = i - 1;
}

/*************************************************************
*
* @Name: linkedlist_delete_room
* @Goal: Check if user is the rooms admin
* @Arg: In: head = linked list's head pointer
       Out: None
* @Ret: Returns 1 if user is the room's admin, 0 otherwise
*
**************************************************************/

int linkedlist_delete_room (struct room **head_ref, char *username, char *room_name)
{
    struct room *curr = *head_ref;
    while (curr != NULL)
    {
        // If room found
        if (strcmp(curr->name, room_name) == 0) {
            // If user is admin
            if (strcmp(curr->admin, username) == 0) {
                delete_room_node (head_ref, room_name);
                return (1);
            }
            else
            {
                return (0);
            }
        }
        else
        {
            curr = curr->next;
        }
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_exit_room
* @Goal: Remove user from room user's linkedlist
* @Arg: In: head_ref = linked list's head pointer
            username = user's name
            room_name = room's name
            list = user's linkedlist
       Out: None
* @Ret: Returns 1 if user is the room's admin, 0 otherwise
*
**************************************************************/

int linkedlist_exit_room (struct room **head_ref, char *username, char *room_name, struct node *list)
{
    struct room *current_room = *head_ref;
    struct node *current_user = list;
    while (current_room != NULL)
    {
        // If room found
        if (strcmp(current_room->name, room_name) == 0) {
            while (current_user != NULL)
            {
                // If user found
                if (users_linkedlist_length(current_room->users) < current_room->capacity) {
                    // If user is found in server information
                    if (strcmp(current_user->user, username) == 0) {
                        delete_node(&current_room->users, current_user->user);
                        return (1);
                    }
                    else
                    {
                        current_user = current_user->next;
                    }
                }
                else
                {
                    return (0);
                }
            }
        }
        else
        {
            current_room = current_room->next;
        }
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_enter_room
* @Goal: Add user to room user's linkedlist
* @Arg: In: head = linked list's head pointer
            username = user's name
            room_name = room's name
            list = user's linkedlist
       Out: None
* @Ret: Returns 1 if user is the room's admin, 0 otherwise
*
**************************************************************/

int linkedlist_enter_room (struct room **head_ref, char *username, char *room_name, struct node *list)
{
    struct room *current_room = *head_ref;
    struct node *current_user = list;
    while (current_room != NULL)
    {
        // If room found
        if (strcmp(current_room->name, room_name) == 0) {
            while (current_user != NULL)
            {
                // If user found
                if (users_linkedlist_length(current_room->users) < current_room->capacity) {
                    // If user is found in server information
                    if (strcmp(current_user->user, username) == 0) {
                        append_node(&current_room->users, current_user->user, current_user->passwd, current_user->id, current_user->connected);
                        return (1);
                    }
                    else
                    {
                        current_user = current_user->next;
                    }
                }
                else
                {
                    return (0);
                }
            }
        }
        else
        {
            current_room = current_room->next;
        }
    }
    return (0);
}

/*************************************************************
*
* @Name: users_linkedlist_length
* @Goal: Check if user is the rooms admin
* @Arg: In: head = linked list's head pointer
       Out: None
* @Ret: Returns 1 if user is the room's admin, 0 otherwise
*
**************************************************************/

int linkedlist_modify_room(struct room **head_ref, char *username, char *room_name, int capacity)
{
    struct room *current = *head_ref;
    while (current != NULL)
    {
        // If room found
        if (strcmp(current->name, room_name) == 0) {
            // If user is admin
            if (strcmp(current->admin, username) == 0) {
                current->capacity = capacity;
                return (1);
            }
            else
            {
                return (0);
            }
        }
        else
        {
            current = current->next;
        }
    }
    return (0);
}

/*************************************************************
*
* @Name: users_linkedlist_length
* @Goal: Get users linked list's length
* @Arg: In: head = linked list's head pointer
       Out: None
* @Ret: Nothing
*
**************************************************************/

int users_linkedlist_length (struct node *head)
{
    struct node *curr = head->next;
    int size = 0;
    while (curr != NULL)
    {
        size++;
        curr = curr->next;
    }
    return size;
}

/*************************************************************
*
* @Name: rooms_linkedlist_print
* @Goal: Print rooms linked list's values
* @Arg: In: head = linked list's head pointer
       Out: None
* @Ret: Nothing
*
**************************************************************/

void rooms_linkedlist_print (struct room *head)
{
    struct room *curr = head->next;
    char str[1024];
    while (curr != NULL)
    {
        bzero(str, 1024);
        write (1, curr->admin, strlen (curr->admin));
        write (1, "\n", strlen ("\n"));
        bzero(str, 1024);
        write (1, curr->name, strlen (curr->name));
        write (1, "\n", strlen ("\n"));
        bzero(str, 1024);
        sprintf (str, "%d\n", curr->capacity);
        write (1, str, strlen (str));
        sprintf (str, "%d\n", curr->room_id);
        write (1, str, strlen (str));
        sprintf (str, "%d\n", curr->admin_id);
        write (1, str, strlen (str));
        curr = curr->next;
    }
}

/*************************************************************
*
* @Name: append_room
* @Goal: Add a new room to the linked list
* @Arg: In: head_ref = linked list's head pointer
            admin = admin's name
            name = rooms name
            num_users = number of users
       Out: list = updated linked list
* @Ret: Nothing
*
**************************************************************/

void append_room (struct room **head_ref, char *admin, char *name, int capacity, int room_id, int admin_id)
{
    struct room *current = *head_ref;
    struct room *new_node;
    new_node = malloc (sizeof (struct room));
    new_node->admin = admin;
    new_node->name = name;
    new_node->capacity = capacity;
    new_node->room_id = room_id;
    new_node->admin_id = admin_id;
    new_node->next = NULL;
    new_node->users = NULL;
    append_node(&new_node->users, "", "", 0, 0);
    if (current == NULL)
    {
        *head_ref = new_node;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

/*************************************************************
*
* @Name: linkedlist_search_roomid
* @Goal: Search through linked list's values
* @Arg: In: list = linked list
       Out: None
* @Ret: Return room_id
*
**************************************************************/

int linkedlist_search_userid (struct node *head, char *user)
{
    struct node *curr = head->next;
    while (curr != NULL)
    {
        if ((strcmp(curr->user, user) == 0))
        {
            return (curr->id);
        }
        curr = curr->next;
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_search_roomid
* @Goal: Search through linked list's values
* @Arg: In: list = linked list
       Out: None
* @Ret: Return room_id
*
**************************************************************/

int linkedlist_search_roomid (struct room *head)
{
    struct room *curr = head->next;
    int room_id = 1;
    while (curr != NULL)
    {
        if (curr->room_id == room_id)
        {
            room_id++;
            curr = head->next;
        }
        else
        {
            curr = curr->next;
        }
    }
    return (room_id);
}

/*************************************************************
*
* @Name: linkedlist_search_room
* @Goal: Search through linked list's values
* @Arg: In: list = linked list
       Out: None
* @Ret: Return 1 if found, 0 otherwise
*
**************************************************************/

int linkedlist_search_room (struct room *head, char *name)
{
    struct room *curr = head->next;
    while (curr != NULL)
    {
        if (strcmp(curr->name, name) == 0)
        {
            return (1);
        }
        curr = curr->next;
    }
    return (0);
}

/*************************************************************
*
* @Name: rooms_linkedlist_create
* @Goal: Create rooms linked list
* @Arg: In: list = linked list
       Out: list = linked list
* @Ret: Returns 0 if all when well, -1 otherwise
*
**************************************************************/

int rooms_linkedlist_create (struct room **rooms)
{
    append_room (rooms, "", "", 0, 0, 0);
    return (0);
}

/*************************************************************
*
* @Name: delete_node
* @Goal: Delete a linked list room based on room's name
* @Arg: In: head_ref = linked list's head pointer
            room_name = room to delete
       Out: list = updated linked list
* @Ret: Nothing
*
**************************************************************/

void delete_room_node (struct room **head_ref, char *room_name)
{
    struct room *current = *head_ref;
    struct room *prev = current;
    int found = 0;
    while (current != NULL && found == 0)
    {
        if (strcmp (current->name, room_name) == 0)
        {
            prev->next = current->next;
            current = prev->next;
            found = 1;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

/*************************************************************
*
* @Name: delete_node
* @Goal: Delete a linked list node based on user
* @Arg: In: head_ref = linked list's head pointer
            user = user to delete
       Out: list = updated linked list
* @Ret: Nothing
*
**************************************************************/

void delete_node (struct node **head_ref, char *user)
{
    struct node *current = *head_ref;
    struct node *prev = current;
    int found = 0;
    while (current != NULL && found == 0)
    {
        if (strcmp (current->user, user) == 0)
        {
            prev->next = current->next;
            current = prev->next;
            found = 1;
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

/*************************************************************
*
* @Name: append_node
* @Goal: Add a new node to the linked list
* @Arg: In: head_ref = linked list's head pointer
            user = user's name
            passwd = user's password
            id = user's id
       Out: list = updated linked list
* @Ret: Nothing
*
**************************************************************/

void append_node (struct node **head_ref, char *user, char *passwd, int id, int connected)
{
    struct node *current = *head_ref;
    struct node *new_node;
    new_node = malloc (sizeof (struct node));
    new_node->user = user;
    new_node->passwd = passwd;
    new_node->id = id;
    new_node->connected = connected;
    new_node->next = NULL;
    if (current == NULL)
    {
        *head_ref = new_node;
    }
    else
    {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

/*************************************************************
*
* @Name: linkedlist_search_user
* @Goal: Search through linked list's values
* @Arg: In: list = linked list
       Out: None
* @Ret: Return 1 if found, 0 otherwise
*
**************************************************************/

int linkedlist_search_user (struct node *head, char *user, char *passwd)
{
    struct node *curr = head->next;
    while (curr != NULL)
    {
        if ((strcmp(curr->user, user) == 0) && (strcmp(curr->passwd, passwd) == 0))
        {
            return (1);
        }
        curr = curr->next;
    }
    return (0);
}

/*************************************************************
*
* @Name: linkedlist_update
* @Goal: Update linked list's values
* @Arg: In: list = linked list
            checklist = updated linked list
       Out: list = updated linked list
* @Ret: Nothing
*
**************************************************************/

void linkedlist_update (struct node **list, struct node **checklist)
{
    struct node *curr1 = *list; // Initial list
    struct node *curr2 = *checklist; // Current list
    struct node *prev = NULL;
    int found;
    // Check if add or modified node
    while (curr2 != NULL)
    {
        while (curr1 != NULL)
        {
            // Check if user exists in both lists
            if (strcmp (curr2->user, curr1->user) == 0)
            {
                found = 1;
                // Check if password in both list is diferent
                if (strcmp (curr2->passwd, curr1->passwd) != 0)
                {
                    curr1->passwd = curr2->passwd;
                }
                // Check if id in both list is diferent
                if (curr2->id != curr1->id)
                {
                    curr1->id = curr2->id;
                }
                curr2 = curr2->next;
                curr1 = *list;
            }
            else
            {
                found = 0;
                prev = curr2;
                curr1 = curr1->next;
            }
            if (curr2 == NULL || curr1 == NULL)
            {
                if (found == 0)
                {
                    append_node (list, prev->user, prev->passwd, prev->id, prev->connected);
                }
                curr1 = NULL;
                curr2 = NULL;
            }
        }
    }
    // Check if delete
    curr1 = *list;
    curr2 = *checklist;
    prev = NULL;
    while (curr1 != NULL)
    {
        while (curr2 != NULL)
        {
            if (strcmp (curr2->user, curr1->user) == 0)
            {
                found = 1;
                curr1 = curr1->next;
                curr2 = *checklist;
            } else {
                found = 0;
                prev = curr1;
                curr2 = curr2->next;
            }
            if (curr2 == NULL || curr1 == NULL)
            {
                if (found == 0)
                {
                    delete_node (list, prev->user);
                }
                curr1 = NULL;
                curr2 = NULL;
            }
        }
    }
}

/*************************************************************
*
* @Name: user_linkedlist_print
* @Goal: Print user linked list's values
* @Arg: In: head = linked list's head pointer
       Out: None
* @Ret: Nothing
*
**************************************************************/

void user_linkedlist_print (struct node *head)
{
    struct node *curr = head->next;
    char str[1024];
    while (curr != NULL)
    {
        bzero(str, 1024);
        write (1, curr->user, strlen (curr->user));
        write (1, "\n", strlen ("\n"));
        bzero(str, 1024);
        write (1, curr->passwd, strlen (curr->passwd));
        write (1, "\n", strlen ("\n"));
        bzero(str, 1024);
        sprintf (str, "%d\n", curr->id);
        write (1, str, strlen (str));
        curr = curr->next;
    }
}

/*************************************************************
*
* @Name: user_linkedlist_create
* @Goal: Create user linked list
* @Arg: In: list = linked list
       Out: list = linked list
* @Ret: Returns 0 if all when well, -1 otherwise
*
**************************************************************/

int user_linkedlist_create (struct node **list)
{
    int fd = 0, n = 0, i = 0, j = 0, count = 0, id = 0;
    char cursor, buff[2048], tmp[2048], *data, *user, *passwd;
    bzero(buff, 2048);
    bzero(tmp, 2048);
    if ((fd = open ("users.dat", O_RDONLY)) < 0)
    {
        perror ("Error");
        exit (EXIT_FAILURE);
    }
    n = read (fd, buff, sizeof (buff));
    // Ghost node
    append_node (list, "", "", 0, 0);
    for (i = 0; i < n; i++)
    {
        cursor = buff[i];
        if (cursor != ':')
        {
            tmp[j++] = cursor;
        }
        if (cursor == ':' || cursor == '\n')
        {
            tmp[j++] = '\0';
            data = strdup (tmp);
            switch (count)
            {
                case 0: // user
                    user = data;
                break;
                case 1: // passwd
                    passwd = data;
                break;
                case 2: // id
                    id = atoi (data);
                    append_node (list, user, passwd, atoi (data), 0);
                break;
            }
            j = 0;
            count++;
        }
        if (cursor == '\n')
        {
            j = 0;
            count = 0;
        }
    }
    return (0);
}
