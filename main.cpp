#include <stdio.h>
#include <ctime>
#include <string.h>

void printUsage()
{
    printf("Usage:\n");
    printf("VersionTool.exe MAJOR MINOR BUILD_FILE [FILE]\n");
    printf("VersionTool.exe -I VERSION_FILE FILE\n");
}

#define PRINT_USAGE \
    printUsage(); \
    return 1;

int main(int argc, char **argv)
{
    if (argc<2)
    {
        PRINT_USAGE;
    }

    if (strcmp(argv[1], "-I")==0)
    {
        if (argc!=4)
        {
            PRINT_USAGE;
        }

        // ----------------------------------------------------------------------------

        // Modify version in installer config
        {
            char version[20];

            // Get version
            {
                FILE *file=fopen(argv[2], "r");

                if (file==0)
                {
                    printf("File \"%s\" not found", argv[2]);
                    return 2;
                }

                char line[256];

                while (!feof(file))
                {
                    memset(line, 0, 256);
                    fgets(line, 256, file);

                    if (strncmp(line, "#define VERSION_FULL \"", 22)==0)
                    {
                        int len=strlen(line)-24;
                        memcpy(version, line+22, len);
                        version[len]=0;

                        break;
                    }
                }

                fclose(file);
            }

            // ----------------------------------------------------------------------------

            // Copy temp file
            {
                FILE *source      = fopen(argv[3], "r");
                FILE *destanation = fopen("temp",  "w");

                if (source==0)
                {
                    printf("File \"%s\" not found", argv[3]);
                    return 2;
                }

                char buffer[4096];
                int size;

                while (size = fread(buffer, 1, 4096, source))
                {
                    fwrite(buffer, 1, size, destanation);
                }

                fclose(source);
                fclose(destanation);
            }

            FILE *tempFile = fopen("temp",  "r");
            FILE *file     = fopen(argv[3], "w");

            if (tempFile==0)
            {
                printf("Temp file not found");
                return 3;
            }

            char line[256];

            while (!feof(tempFile))
            {
                memset(line, 0, 256);
                fgets(line, 256, tempFile);

                char *subString;

                // Replace version
                {
                    subString=strstr(line, "<Version>");

                    if (subString)
                    {
                        char newVersion[256];

                        strcpy(newVersion, "<Version>");
                        strcat(newVersion, version);
                        strcat(newVersion, "</Version>");

                        if (line[strlen(line)-1]=='\n')
                        {
                            strcat(newVersion, "\n");
                        }

                        strcpy(subString, newVersion);

                        goto writeLine;
                    }
                }

                // Replace release date
                {
                    subString=strstr(line, "<ReleaseDate>");

                    if (subString)
                    {
                        time_t seconds = time(NULL);
                        tm* timeinfo = localtime(&seconds);

                        char buffer[15];
                        strftime(buffer, 15, "%Y-%m-%d", timeinfo);



                        char newReleaseDate[256];

                        strcpy(newReleaseDate, "<ReleaseDate>");
                        strcat(newReleaseDate, buffer);
                        strcat(newReleaseDate, "</ReleaseDate>");

                        if (line[strlen(line)-1]=='\n')
                        {
                            strcat(newReleaseDate, "\n");
                        }

                        strcpy(subString, newReleaseDate);

                        goto writeLine;
                    }
                }

                writeLine:
                fputs(line, file);
            }

            fclose(tempFile);
            fclose(file);

            remove("temp");
        }
    }
    else
    {
        if (argc!=4 && argc!=5)
        {
            PRINT_USAGE;
        }

        // ----------------------------------------------------------------------------

        // Generate build file and version.h file
        {
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
            fprintf(file, "%d\n", buildNumber);
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
        }
    }

    return 0;
}

