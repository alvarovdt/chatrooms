/************************************************************
*
* @File: user.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"

/*************************************************************
*
* @Name: print_user_info
* @Goal: print user information
* @Arg: In: user_info = user structure
       Out: None
* @Ret: None
*
**************************************************************/

void print_user_info (struct user *user_info)
{
    char str[1024];
    bzero(str, 1024);
    sprintf (str, "Username: %s\n", user_info->username);
    write (1, str, strlen (str));
    bzero(str, 1024);
    sprintf (str, "MD5 Password: %s\n", user_info->passwd);
    write (1, str, strlen (str));
}

/*************************************************************
*
* @Name: get_user_info
* @Goal: Get user information
* @Arg: In: user_info = user structure
       Out: user_info = user structure
* @Ret: None
*
**************************************************************/

void get_user_info (struct user **user_info)
{
    char buff[2048], md5_hash[33];
    unsigned char digest[MD5_DIGEST_LENGTH];
    int n = 0, i = 0;

    bzero(buff, 2048);
    bzero(md5_hash, 33);

    *user_info = malloc (sizeof (struct user));

    write (1, "Username: ", strlen ("Username: "));
    n = read (0, buff, sizeof (buff));
    buff[n-1] = '\0';
    (*user_info)->username = strdup (buff);

    bzero(buff, 2048);
    write (1, "Password: ", strlen ("Password: "));
    n = read (0, buff, sizeof (buff));
    buff[n-1] = '\0';

    MD5 ((unsigned char*)&buff, strlen (buff), (unsigned char*)&digest);

    for (i = 0; i < MD5_DIGEST_LENGTH; i++)
        sprintf (&md5_hash[i*2], "%02x", (unsigned int)digest[i]);

    (*user_info)->passwd = strdup (md5_hash);
}
