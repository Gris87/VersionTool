#include <stdio.h>
#include <ctime>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc!=4)
    {
        printf("Usage:\n");
        printf("VersionTool.exe MAJOR MINOR FILE\n");

        return 1;
    }

    // ----------------------------------------------------------------------------

    int buildNumber=0;

    FILE *file;

    file=fopen(argv[3], "r");

    if (file)
    {
        char buffer[100];

        while (!feof(file))
        {
            fscanf(file, "%s", buffer);

            if (strcmp(buffer, "VERSION_BUILD")==0)
            {
                fscanf(file, "%d", &buildNumber);
                break;
            }
        }

        fclose(file);
    }

    // ----------------------------------------------------------------------------

    ++buildNumber;

    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);

    char buffer[80];
    char* format = "%a %B %d, %Y  %H:%M:%S";
    strftime(buffer, 80, format, timeinfo);

    // ----------------------------------------------------------------------------

    file=fopen(argv[3], "w");

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

    return 0;
}

