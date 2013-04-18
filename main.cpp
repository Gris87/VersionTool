#include <stdio.h>
#include <ctime>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc!=4 && argc!=5)
    {
        printf("Usage:\n");
        printf("VersionTool.exe MAJOR MINOR BUILD_FILE [FILE]\n");

        return 1;
    }

    // ----------------------------------------------------------------------------

    int buildNumber=0;

    FILE *file;

    file=fopen(argv[3], "r");

    if (file)
    {
        fscanf(file, "%d", &buildNumber);
        fclose(file);
    }

    // ----------------------------------------------------------------------------

    ++buildNumber;

    // ----------------------------------------------------------------------------

    file=fopen(argv[3], "w");
    fprintf(file, "%d", buildNumber);
    fclose(file);

    if (argc==5)
    {
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);

        char buffer[80];
        strftime(buffer, 80, "%a %B %d, %Y  %H:%M:%S", timeinfo);



        file=fopen(argv[4], "w");

        fprintf(file, "#ifndef VERSION_H\n");
        fprintf(file, "#define VERSION_H\n");
        fprintf(file, "\n");
        fprintf(file, "#define BUILD_DATE \"%s\"\n", buffer);
        fprintf(file, "#define VERSION_MAJOR %s\n", argv[1]);
        fprintf(file, "#define VERSION_MINOR %s\n", argv[2]);
        fprintf(file, "#define VERSION_BUILD %d\n", buildNumber);
        fprintf(file, "#define VERSION_FULL \"%s.%s.%d\"\n", argv[1], argv[2], buildNumber);
        fprintf(file, "\n");
        fprintf(file, "#endif // VERSION_H\n");

        fclose(file);
    }

    return 0;
}

