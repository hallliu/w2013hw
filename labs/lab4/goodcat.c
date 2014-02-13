#include <stdio.h>
#include <stdlib.h>

#define SUCCESS  0
#define E_PARAM  1

int main(int argc, char **argv)
{
    int i, numread;
    FILE *in;
    char buf[100];

    if(argc==0)
    {
        fprintf(stderr, "ERROR: Not enough parameters.\n");
        fprintf(stderr, "Syntax: %s [file1] [file2] ... [fileN]\n", argv[0]);
        exit(E_PARAM);
    }

    for(i=1;i<argc;i++)
    {
        in = fopen(argv[i], "rt");
        if(in==NULL)
            fprintf(stderr, "\n%s: %s: No such file or directory\n", argv[0], argv[i]);
        else while(!feof(in))
        {
            numread = fread(buf, sizeof(char), 100, in);
            if(numread>0 && numread != EOF)
                fwrite(buf, sizeof(char), numread, stdout);
        }
        fclose(in);
    }
    exit(SUCCESS);
}
