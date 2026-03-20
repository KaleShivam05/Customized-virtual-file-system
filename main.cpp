////////////////////////////////////////////////////////////////
//
//  Header file Inclusion
//
////////////////////////////////////////////////////////////////

#include<stdio.h>
#include "structers.h"
#include "Helper.h" 

////////////////////////////////////////////////////////////////
//
//  Etry point function of the project
//
////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'},{'\0'},{'\0'},{'\0'}};
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL;
    int iCount = 0;
    int iRet = 0;
    bool bFlag = false;

    StartAuxillaryDataInitilisation();

    printf("------------------------------------------------------------------------------\n");
    printf("----------------------Marvellous CVFS started succesfully---------------------\n");
    printf("------------------------------------------------------------------------------\n");

    // infinite listening shell
    while(1)
    {
        fflush(stdin);
        strcpy(str,"");

        printf("\nMarvellous CVSF : > ");

        fgets(str,sizeof(str),stdin); 

        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3],Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVSF : > exit
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("----------Thank you for using Marvellous CVFS.\n");
                printf("----------Deallocating all the allocated resources...\n");
                printf("------------------------------------------------------------------------------\n");

                break;
            }
            else if(strcmp("ls",Command[0]) == 0) // Marvellous CVSF : > ls
            {
                ListofFile();
            }
            else if(strcmp("help",Command[0]) == 0) // Marvellous CVSF : > help
            {
                DisplayHelp();
            }
            else if(strcmp("clear",Command[0]) == 0) // Marvellous CVSF : > clear
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                 #endif  
            }
            else if(strcmp("backup",Command[0]) == 0) // Marvellous CVSF : > backup
            {
                bFlag = BackupAllFiles();

                if(bFlag == false)
                {
                    printf("Unable to get a backup\n");
                }
                
                if(bFlag == true)
                {
                    printf("-----------Backup gets succesfully-----------\n");
                }
                
            }
            

        } // End of else if 1
        else if(iCount == 2)
        {
            if(strcmp("man",Command[0]) == 0) // Marvellous CVSF : > man creat
            {
                ManPageDisplay(Command[1]);
            }
            else if(strcmp("unlink",Command[0]) == 0) // Marvellous CVSF : > unlink Demo.txt
            {
                iRet = unlinkFile(Command[1]);

                if(iRet == ERR_INVALID_PAREMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }
                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Unable to delete as there is no such file\n");
                }
                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }
            }
            else if(strcmp("write",Command[0]) == 0) // Marvellous CVFS > write 2
            {
                printf("Enter the data that you want to write\n");
                fgets(InputBuffer,MAXFILESIZE,stdin);

                iRet = writeFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);

                if(iRet == ERR_INVALID_PAREMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Unable to write there is no space\n");
                }
                else  
                {
                    printf("%d bytes gets succesfully written\n",iRet);
                }
                
            }
            else 
            {
                printf("There is such command\n");
            }

        } // End of else if 2
        else if(iCount == 3)
        {
            if(strcmp("creat",Command[0]) == 0) // Marvellous CVSF : > creat Demo.txt 3
            {
                iRet = CreatFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PAREMETER)
                {
                    printf("Error : Unable to create as parameters are invalid\n");
                    printf("please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there are no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create as file is already present\n");
                }

                if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to create as file is no space\n");
                }

                if(iRet > 0)
                {
                    printf("File succesfully created with fd : %d\n",iRet);
                }

            }

            if(strcmp("read", Command[0]) == 0) // marvellous cvfs > read 3 10
            {
                EmptyBuffer = (char*)malloc(sizeof(atoi(Command[2])));

                iRet = readFile(atoi(Command[1]),EmptyBuffer,atoi(Command[2]));

                if(iRet == ERR_INVALID_PAREMETER)
                {
                    printf("Error : Invalid Parameters\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : File not exites\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : NO Premission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficiant data\n");
                }
                else
                {
                    printf("Read operation is succesfully\n");
                    printf("Data from file is : %s\n",EmptyBuffer);
                    
                    free(EmptyBuffer);
                }

            }

            else 
            {
                printf("There is no such command\n");
            }

        } // End of else if 3
        else if(iCount == 4)
        {

        } // End of else if 4
        else 
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        }// End of else

    }// End of while

    return 0;

}// End of main