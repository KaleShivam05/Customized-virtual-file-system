////////////////////////////////////////////////////////////////
//
//  Header file Inclusion
//
////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h> 
#include<stdbool.h>
#include<string.h>
#include<sys/stat.h>

////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
////////////////////////////////////////////////////////////////

#define MAXFILESIZE 50    // Maximum file size that we allow in the project
#define MAXOPENFILES 20   // max open file in the project 
#define MAXINODE 5        // max file we create

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2

////////////////////////////////////////////////////////////////
//
//  User Defined Macros for Eroor handling
//
////////////////////////////////////////////////////////////////

#define ERR_INVALID_PAREMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

///////////////////////////////////////////////////////////////
//
//  User Defined structuers
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//
//  Structure Name :  BootBlock
//  Description    :  Holde the information to boot the os
//
////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

////////////////////////////////////////////////////////////////
//
//  Structure Name :  SuperBlock
//  Description    :  Holde the information about file system
//
////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInode;
};

//////////////////////////////////////////////////////////////////
//
//  Structure Name :  Inode
//  Description    :  Holde the information about the file
//
//////////////////////////////////////////////////////////////////

#pragma pack(1) // used to avoid padding
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int FileType;
    int ActualFileSize;
    int ReferenceCount;
    int Permission;
    char *Buffer;    // virtual data block
    struct Inode *next; // point to next inode
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

//////////////////////////////////////////////////////////////////
//
//  Structure Name :  FileTable
//  Description    :  Holde the information about the opened file
//
//////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode; // point to Inode structuer
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

//////////////////////////////////////////////////////////////////
//
//  Structure Name :  UAREA
//  Description :     Holde the information about the process
//
//////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];   // name of process
    PFILETABLE UFDT[MAXOPENFILES]; // array of pointer which point filetable
};

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

///////////////////////////////////////////////////////////////
//
//  Function Name : InitialiseUAREA
//  Description   : It is used to initialise UAREA members
//  Author        : Shivam Rajendra kale
//  Date          : 13-01-2026
//
////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");

    int i = 0;

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }
    
    printf("Marvellous CVFS : UAREA gets Initilise Succesfully\n");
}

////////////////////////////////////////////////////////////////
//
//  Function Name : InitialiseSuperBlock
//  Description   : It is used to initialise SuperBlock members
//  Author        : Shivam Rajendra kale
//  Date          : 13-01-2026
//
////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInode = MAXINODE;

    printf("Marvellous CVFS : SuperBlock gets Initilise Succesfully\n");
}

////////////////////////////////////////////////////////////////
//
//  Function Name : CreateDILB
//  Description   : It is used to create linkedlist of inodes
//  Author        : Shivam Rajendra kale
//  Date          : 13-01-2026
//
////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->FileType = 0;
        newn->ActualFileSize = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL) // LL is empty
        {
            head = newn;
            temp = head;
        }
        else             // LL contain atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }

    }

    printf("Marvellous CVFS : DILB created sucesfully\n");
}

////////////////////////////////////////////////////////////////
//
//  Function Name : StartAuxillaryDataInitilisation
//  Description   : It is used to call such function which are
//                    used to initilise auxillary data
//  Author :        Shivam Rajendra kale
//  Date   :        13-01-2026
//
////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done\n");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary Data initialise succesfully\n");
}

///////////////////////////////////////////////////////////////
//
//  Function Name : ListOfFile
//  Description   : It is used to display all files
//  Input         : Nothing
//  Output        : Nothing 
//  Author        : Shivam Rajendra kale
//  Date          : 16-01-2026
//
////////////////////////////////////////////////////////////////

void ListofFile()
{
    PINODE temp = head;

    printf("------------------------------------------------------------------------------\n");
    printf("----------------------------Marvellous CVFS Files-----------------------------\n");
    printf("------------------------------------------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp->ReferenceCount > 0)
        {
            printf("%d : %s\n",temp->InodeNumber,temp->FileName);
        }
        temp = temp->next;
    }

    printf("------------------------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////
//
//  Function Name : IsFileExist
//  Description   : It is used to check whether file is already exit or not
//  Input         : It acepts file name 
//  Output        : bool 
//  Author        : Shivam Rajendra kale
//  Date          : 16-01-2026
//
////////////////////////////////////////////////////////////////

bool IsFileExite(
                    char *name // file name
)
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }

    return bFlag;
}

////////////////////////////////////////////////////////////////
//
//  Function Name : BackupAllFile()
//  Description   : It is used to get a backup of all regulr file
//  Input         : It acepts file descriptor and string
//  Output        : Integer
//  Author        : Shivam Rajendra kale
//  Date          : 16-01-2026
//
////////////////////////////////////////////////////////////////

bool BackupAllFiles()
{
    int i = 0, iRet = 0, fd = 0;

    iRet = mkdir("./Backup",0777);

    if(iRet != 0)
    {
        return false;
    }

    chdir("Backup");

    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            continue;
        }

        if(uareaobj.UFDT[i]->ptrinode->ReferenceCount > 0)
        {
            fd = open(uareaobj.UFDT[i]->ptrinode->FileName, O_CREAT | O_WRONLY | O_APPEND);

            write(fd, uareaobj.UFDT[i]->ptrinode->Buffer, sizeof(uareaobj.UFDT[i]->ptrinode->Buffer));
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////
//
//  Function Name : CreatFile
//  Description   : It is used to create new regulr file
//  Input         : It acepts file name and permissions
//  Output        : fd 
//  Author        : Shivam Rajendra kale
//  Date          : 16-01-2026
//
////////////////////////////////////////////////////////////////

int CreatFile(
                char *name,    // name of new file
                int permission // permission for that file
)
{
    PINODE temp = head;
    int i = 0;

    // if name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PAREMETER;
    }

    // if the permission value is wrong
    // Permission 1 = READ
    // Permission 2 = WRITE
    // Permission 3 = READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PAREMETER;
    }

    // if the inodes are full
    if(superobj.FreeInode == 0)
    {
        return ERR_NO_INODES;
    }

    // if file is already present
    if(IsFileExite(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty inode
    while(temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    if(uareaobj.UFDT[i] == NULL)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    uareaobj.UFDT[i]->Mode = 3;
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;

    uareaobj.UFDT[i]->ptrinode = temp;

    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;
    uareaobj.UFDT[i]->ptrinode->Buffer = (char*)malloc(sizeof(MAXFILESIZE));

    superobj.FreeInode -= 1;

    printf("Total number of remaining : %d\n",superobj.FreeInode);

    return i;
}

////////////////////////////////////////////////////////////////
//
//  Function Name : writeFile()
//  Description   : It is used to write in regulr file
//  Input         : file descriptor and addres of buffer which contain data
//                  size of data that we want to write
//  Output        : Number of bytes succesfully 
//  Author        : Shivam Rajendra kale
//  Date          : 22-01-2026
//
////////////////////////////////////////////////////////////////

int writeFile(  int fd,
                char *Data,
                int size
             )
{
    printf("File Descriptor : %d\n",fd);
    printf("Data  : %s\n",Data);
    printf("bytes : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PAREMETER;
    }

    // Invalid fd to null
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // there is no permission to write;
    // if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE);
    // {
    //     return ERR_PERMISSION_DENIED;
    // }

    // Insuffecient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // write the data into the file

    strncpy((uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset),Data,size);

    // Update the write offset

    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actualFile size

    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;


    return size;
}

////////////////////////////////////////////////////////////////
//
//  Function Name : readFile()
//  Description   : It is used to read data from file
//  Input         : file descriptor and addres of empty buffer
//                  size of data that we want to read
//  Output        : Number of bytes succesfully read
//  Author        : Shivam Rajendra kale
//  Date          : 22-01-2026
//
////////////////////////////////////////////////////////////////

int readFile(   int fd,
                char *data,
                int size
            )
{
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PAREMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PAREMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PAREMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // read the data

    strncpy(data ,(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset),size);

    // update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

////////////////////////////////////////////////////////////////
//
//  Function Name : unlinkFile()
//  Description   : It is used to Delete the file
//  Input         : It acepts file Name
//  Output        : Nothing
//  Author        : Shivam Rajendra kale
//  Date          : 22-01-2026
//
////////////////////////////////////////////////////////////////

int unlinkFile(
                char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PAREMETER;
    }

    if(IsFileExite(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // travel the ufdt
    for(i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName,name) == 0)
            {
                // Deallocate memory of buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // reset all values of inode
                // Dont deallocate memory of inode

                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;
                memset(uareaobj.UFDT[i]->ptrinode->FileName,'\0',sizeof(uareaobj.UFDT[i]->ptrinode->FileName));
                
                //Dealloacte memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to uFDT
                uareaobj.UFDT[i] = NULL;

                // Increase the free inodes count
                superobj.FreeInode++;

                break; // IMP

            }// End of if

        }// End of if

    }// End of for

    return EXECUTE_SUCCESS;

}// End of function
