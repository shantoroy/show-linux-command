#include<stdio.h>                   // for standard input and output
#include<stdlib.h>                  // for using library variables and functions
#include<unistd.h>                  // for using system call wrapper functions e.g. fork/pipe
#include<sys/types.h>               // for including system functions
#include<sys/wait.h>                // for including system functions
#include<string.h>                  // for string operations
#include<ctype.h>                   // for testing and mapping type of characters
#include<stdbool.h>                 // for handling boolean operations
#include<fcntl.h>                   // for file controlling options including read/write

// function removes the first character of a string
char *remove_first_char (char *data) 
{
    char *str;
    if (strlen(data) == 0)
    {
        str = data;
    }
    else
    {
        str = data + 1;                                 // stores value from the second character
    }
    return str;
}

// function converts a string into integer
int str_2_int(char *data)
{
    int value;
    sscanf(data, "%d", &value);                         // sscanf stores formatted integer value of data in value 
    return value;
}

// function checks if a string starts with a substring
bool startswith(char *string, char *substring)
{
    // strncmp returns 0 if length of substring matches to start part of the string
    if(strncmp(string, substring, strlen(substring)) == 0) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// function checks if a string is an int
bool isadigit(char *string)
{
    if (!isdigit(string[0]))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// main function starts from here...
int main(int args, char *argv[])
{
    pid_t pid;
    int mypipefd[2];
    int ret;
    char file_name[100];
    int n_flag;
    int n=20;
    
    char* path_list = malloc(sizeof(char)* 1024);
    // getenv provides all paths separated with a ":" through PATH variable
    path_list = getenv("PATH"); 
    char *path = strtok(path_list, ":");                // strtok tokenizes strings separated by a ":"
    char **custom_argv;                                 // for customizing the argv array

    // Comparing the number of arguments scenario
    if(args < 3)
        {
            printf("Must provide at least 2 arguments!!!\nSee more using 'man show'\n");
            exit(1);
        }
    else if(args == 3)
    {
        custom_argv = &argv[1];                         // use a custom argv to start from the command
    }
    else
    {
        if(startswith(argv[1],"-") == 1)
        {
            // removes - from the flag and converts it into an integer
            n = str_2_int(remove_first_char(argv[1]));  // get value for n_flag
            custom_argv = &argv[2];                     // use a custom argv to start from the command
        }
        else if(startswith(argv[1],"-") != 1)
        {
            if(isadigit(argv[1]) == 1)
            {
                printf("Bad Flag!!!\nSee more using 'man show'\n");
                exit(1);
            }
            else
            {
                custom_argv = &argv[1];
            }
        }
        else
        {
            custom_argv = &argv[1];
        }
    }

    // return filename if command file exists in path
    while (path!=NULL)
    {
        sprintf(file_name, "%s/%s", path, custom_argv[0]);
        //file = path + '/' + argv[1];
        if (!access(file_name, X_OK))                   // access checks for executable file (X_OK)
        {
            break;
        }
        path = strtok(NULL, ":");                       // strtok checks all path until it is NULL
    }

    // if command not valid/found, print error message and exit
    if(access(file_name, X_OK))
    {
        printf("Command not found!!!\nSee more using 'man show'\n");
        exit(1);
    }
    
    // initiates the pipe
    ret = pipe(mypipefd);                               // initiates the pipe

    // returns error message if the pipe fails
    if(ret == -1)
    {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();                                       // initiates a fork

    if(pid == 0)
    {   
        // child proces
        close(mypipefd[0]);   // closes unused read in
        dup2(mypipefd[1], STDOUT_FILENO);               // Make output go to pipe
        //char *extensions[3] = {file_name, argv[2], NULL}; --> it's not a good practice
        execv( file_name, custom_argv );                // executes execv with the filename and arguments are later in argv
    }
    // Parent Process
    close(mypipefd[1]);                                 // closes unused write in
    wait(NULL);
    close(0);
    dup2(mypipefd[0], STDIN_FILENO);                    // dup into stdin
    close(mypipefd[0]);

    char *line = NULL;
    size_t size = 200;
    char str[100];

    // Open a hidden file in tmp directory and store result line by line
    FILE *fp = fopen("/tmp/.os.txt", "w");              // opens the file in write mode
    if (getline(&line, &size, stdin) == -1)            // if fails to get line by line from stdin
    {
        char *text = "No line\n";
        fputs(text, fp);                                // writes in the file
        fclose(fp);                                     // closes the file
    } 
    else                                                // if succeeds to get line by line from stdin
    {
        for(int j=1; j<=1000; ++j)
        {
            while(fgets(str, sizeof(str), stdin) != NULL )
            {
                fputs(str, fp);                         // writes in the file
            }
        }
        fclose(fp);                                     // closes the file
    }

    FILE *file = fopen("/tmp/.os.txt", "r");            // opens the file in read mode
    int start = 1;
    // Don't know why but directly using n sets a default value of 10
    // So storing n in a different flag variable named n_flag
    n_flag = n;                                         // Number of lines per request
    int end = n_flag;
    char user_choice;
    if ( file != NULL )
    {
        char line[1000];                                // or other suitable maximum line size
        while (fgets(line, sizeof line, file) != NULL)  // read a line 
        {
            if(start<=end)
            {
                printf("%s", line);
            }
            else
            {
                // better to use read/write instead of printf/scanf/getch
                int	input = open("/dev/tty", O_RDWR);  // opens a terminal with read and write permission
                write(input, "Enter 'y' to continue, other to quit: ", 38);  // 38 characters to write
	            char byte;
                ssize_t size = read(input, &byte, sizeof(char)*2);           // two characters include any character and Enter

                // collects user input
                if(byte == 'y')
                {
                    printf("%s", line);
                    end = end + n_flag;
                }
                else
                {
                    break;
                }
            }
            start++;
        }
        fclose(file);
    }
    remove("/tmp/.os.txt");                             // removes the file
    // free(path_list);                                 // frees the allocated memory
    close(mypipefd[0]);
    close(mypipefd[1]);
    return 0;
   }