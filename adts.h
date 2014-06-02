#ifndef __ADTS_H_
#define __ADTS_H_

/************************************************************
*
* @File: adts.h
* @Goal: Store data types and prototypes
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Defines
#define MAX_FRAME 115

// Global variables
int ssocket_fd;
int id_queue;
char *room_name;

// Abstract data types
struct q_message
{
    long id;
    char origin[7];
    char destination[7];
    char type;
    char data[100];
};

struct thread
{
    int client_socket;
    struct room *rooms;
    struct node *users;
    int id_queue;
    int id_user;
};

struct room
{
    char *admin;
    char *name;
    int capacity;
    int room_id;
    int admin_id;
    struct room *next;
    struct node *users;
};

struct frame
{
    char origin[7];
    char destination[7];
    char type;
    char data[100];
};

struct user
{
    char *username;
    char *passwd;
};

struct config
{
    char *ip;
    int port;
};

struct node
{
    char *user;
    char *passwd;
    int id;
    int connected;
    struct node *next;
};

// Prototypes
void *mailbox_reader (void *arg);
void *queue_handler (void *arg);
void write_socket (int client_socket, char destination[7], char type, char message[100]);
void parse_data_one_arg (struct frame frame_info, char **room_name);
void parse_data_two_args (struct frame frame_info, char **room_name, int *capacity);
void delete_room_node (struct room **head_ref, char *roomname);
void delete_room (int client_socket, struct frame frame_info, struct room **rooms);
void exit_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list);
void talk_room (int client_socket, struct frame frame_info, int id_queue, struct room *head);
void enter_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list);
void modify_room (int client_socket, struct frame frame_info, struct room **rooms);
void create_room (int client_socket, struct frame frame_info, struct room **rooms, struct node *list);
void show_rooms (int client_socket, struct frame frame_info, struct room *head);
void exit_room_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void talk_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void enter_room_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void modify_room_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void delete_room_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void create_room_request (int *socket_fd, struct user *user_info, char *command, size_t length);
void show_rooms_request (int *socket_fd, struct user *user_info);
void *dedicated_server (void *arg);
void clean_buffer ();
void get_port_ip (int *port, char **ip, char *command, size_t length);
void connect_server(struct user *user_info, char *command, size_t length, int *socket_fd);
void perform_command (int cmd_num, char *command, size_t length, struct user *user_info, int *socket_fd);
void check_command (struct user *user_info, int *socket_fd);
void create_auth_confirmation_frame (struct frame *frame_info, struct user *user_info);
void create_auth_error_confirmation_frame (struct frame *frame_info, struct user *user_info);
void create_auth_frame (struct frame *frame_info);
void create_conn_error_frame (struct frame *frame_info);
void create_auth_error_frame (struct frame *frame_info);
void create_login_frame (struct frame *frame_info, struct user *user_info);
int check_frame (struct frame frame_info);
void print_user_info (struct user *user_info);
void get_user_info(struct user **user_info);
int connection_handler (struct config *configuration, struct node *head, struct room *rooms);
void print_config (struct config *configuration);
void init_config (struct config **configuration);
int users_linkedlist_length (struct node *head);
void delete_node (struct node **head_ref, char *user);
void append_room (struct room **head_ref, char *admin, char *name, int capacity, int room_id, int admin_id);
void linkedlist_disconnect_user (struct node **head_ref, int id);
void append_node (struct node **head_ref, char *user, char *passwd, int id, int connected);
int linkedlist_check_connected (struct node **head_ref, char *user, char *passwd);
char * linkedlist_get_username (struct node *head, int id);
int linkedlist_search_userexist (struct room *head, char *username, char *roomname);
void linkedlist_get_room_usersid (struct room *head, char *name, int **ids);
int linkedlist_delete_room(struct room **head_ref, char *username, char *roomname);
int linkedlist_exit_room(struct room **head_ref, char *username, char *roomname, struct node *list);
int linkedlist_enter_room(struct room **head_ref, char *username, char *roomname, struct node *list);
int linkedlist_modify_room(struct room **head_ref, char *username, char *roomname, int capacity);
int linkedlist_search_userid (struct node *head, char *user);
int linkedlist_search_roomid (struct room *head);
int linkedlist_search_room (struct room *head, char *name);
int linkedlist_search_user (struct node *head, char *user, char *passwd);
void rooms_linkedlist_print (struct room *head);
void user_linkedlist_print (struct node *head);
int rooms_linkedlist_create (struct room **rooms);
int user_linkedlist_create (struct node **list);
int main ();

#endif
