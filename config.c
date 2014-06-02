/************************************************************
*
* @File: config.c
* @Goal:
* @Author: Roger Fernandez (st20723)
* @Date: October 18th, 2013
*
*************************************************************/

// Own libraries
#include "libs.h"

/*************************************************************
*
* @Name: print_config
* @Goal: Print configuration abstract data type values
* @Arg: In: configuration = configuration data
       Out: None
* @Ret: Nothing
*
**************************************************************/

void print_config (struct config *configuration)
{
    char str[1024];
    bzero(str, 1024);
    write (1, configuration->ip, strlen (configuration->ip));
    write (1, "\n", strlen ("\n"));
    bzero(str, 1024);
    sprintf (str, "%d\n", configuration->port);
    write (1, str, strlen (str));
}

/*************************************************************
*
* @Name: init_config
* @Goal: Get configuration values
* @Arg: In: configuration = configuration data
       Out: configuration = configuration data
* @Ret: Nothing
*
**************************************************************/

void init_config (struct config **configuration)
{
    int fd = 0, n = 0, i = 0, j = 0, count = 0;
    char cursor, buff[2048], tmp[2048], *data;
    bzero(buff, 2048);
    bzero(tmp, 2048);
    if ((fd = open ("config.dat", O_RDONLY)) < 0)
    {
        perror ("Error");
        exit (EXIT_FAILURE);
    }
    *configuration = malloc (sizeof (struct config));
    n = read (fd, buff, sizeof (buff));
    for (i = 0; i < n; i++)
    {
        cursor = buff[i];
        if (cursor == '\n')
        {
            tmp[j++] = '\0';
            data = strdup (tmp);
            switch (count)
            {
                case 0: // ip
                    (*configuration)->ip = data;
                break;
                case 1: // port
                    (*configuration)->port = atoi (data);
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
