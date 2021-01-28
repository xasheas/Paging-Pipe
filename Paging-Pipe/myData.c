
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#ifdef __APPLE__
#include <termios.h>
#else
#include <termio.h>
#endif


#define MAX_LINE_SIZE 500
#define MAX_LINE_NUMBER 100
#define LINE_TURN 24
#define READ_END 0
#define WRITE_END 1


int main(int argc, char *argv[])
{
    FILE *fp = getFile(argv[1]);


    if (argc == 2)
    {
        readAllFile(fp);
    }

    else if (!strcmp(argv[2], "=") && !strcmp(argv[3], "myMore") && argc == 4)
    {
        char buffer[MAX_LINE_SIZE];
        char inputChar;
        int isFinished = 0;

        char write_msg[MAX_LINE_NUMBER][MAX_LINE_SIZE];

        do
        {
            pid_t pid;
            int fd[2];
            pipe(fd);
            pid = fork();


            if (pid < 0)
            {
                fprintf(stderr, "Catallandirma dogru bir Sekilde calistirilamadi.");
                return 1;
            }


            if (pid == 0)
            {
                close(fd[WRITE_END]);

                char read_end_pipe[10];
                sprintf(read_end_pipe, "%d", fd[READ_END]);

                char *arguments[3] = {argv[3], read_end_pipe, NULL};


                if (execv(argv[3], arguments) == -1)
                {
                    perror("myMore programi baslatilamadi.");
                    return -1;
                }
            }


            if (pid > 0)
            {
                int lineCount = 0;


                for (int i = 0; i < LINE_TURN; ++i)
                {

                    if (fgets(buffer, sizeof(buffer), fp) > 0)
                    {
                        strcpy(write_msg[i], buffer);
                        lineCount++;
                    }

                    else
                    {
                        isFinished = 1;
                        break;
                    }
                }

                close(fd[READ_END]);
                write(fd[WRITE_END], &write_msg, lineCount * MAX_LINE_SIZE);
                close(fd[WRITE_END]);

                wait(NULL);
            }

            if (isFinished == 0)
                printf("\n ### Devam Etmek icin SPACE Tusuna Basin ### \n\n");
            else
            {
                printf("\n ### Dosya Basariyla okundu ### \n\n");
                return 0;
            }

            inputChar = getch();

        } while (isFinished != 1 && inputChar != 'q');
    }
    else
    {
        printf("\nKullanim Hatasi!\nDosyanın tamamini okumak icin:\t\t\"./myData <Metin Dosyasi Yolu>\"\nDosyayi satir satir okumak icin:\t\"./myData <Metin Dosyasi Yolu> = myMore\"\n");
        return 1;
    }

    return 0;
}






int getch(void)
{
    struct termios oldattr, newattr;
    int ch;

    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

    return ch;
}



int fileSize(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    int fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    return fileSize;
}


FILE *getFile(char *filePath)
{
    FILE *fp;
    fp = fopen(filePath, "r");

    if (fp != NULL)
        return fp;

    else
    {
        perror("Error");
        exit(1);
    }
}


int readAllFile(FILE *fp)
{
    int byteOfFile = fileSize(fp);
    char *records = malloc(byteOfFile + 1);

    if (!fp)
        return -1;

    while (fgets(records, byteOfFile, fp) != NULL)
        printf("%s", records);

    fclose(fp);

    return 0;
}


