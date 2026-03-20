////////////////////////////////////////////////////////////////
//
//  Header file Inclusion
//
////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h> 
#include<string.h>
#include "Helper.h"

////////////////////////////////////////////////////////////////
//
//  Function Name : DisplayHelp
//  Description   : It is used to help page
//  Author        : Shivam Rajendra kale
//  Date          : 14-01-2026
//
////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    int fd = 0, iRet = 0;
    char Buffer[1024] = {'\0'};

    fd = open("help.txt",O_RDONLY);

    if(fd == -1)
    {
        printf("Unable to help you due to some internall work\n");
    }

    while(iRet = read(fd,Buffer, sizeof(Buffer)) == 0)
    {
        printf("%s\n",Buffer);
    }
}

////////////////////////////////////////////////////////////////
//
//  Function Name : ManPageDisplay
//  Description   : It is used to display man page
//  Author        : Shivam Rajendra kale
//  Date          : 14-01-2026
//
////////////////////////////////////////////////////////////////

void ManPageDisplay(char name[])
{
    if(strcmp("ls",name) == 0)
    {
        printf("About : It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man",name) == 0)
    {
        printf("About : It is used to display manual page\n");
        printf("Usage : man command_name\n");
        printf("command_name : It is the name of command");
    }
    else if(strcmp("exit",name) == 0)
    {
        printf("About : It is used to terminet the shell\n");
        printf("Usage : exit\n");
    }
    else if(strcmp("clear",name) == 0)
    {
        printf("About : It is used to clear the shell\n");
        printf("Usage : clear\n");
    }
    else 
    {
        printf("No manual entry for %s\n",name);
    }
}
