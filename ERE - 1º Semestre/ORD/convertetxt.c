/*
    --------------------------------
    Lucas Pupulin Nanni - RA 56242
    Mariana Mendes Dultra - RA 59288
    --------------------------------
*/

#include <stdio.h>
#include <string.h>

#define NL 10
#define CR 13

int main(int argc, char *argv[]) {

    char *mode = argv[1];
    char *inputFileName = argv[2];
    char *outputFileName = argv[3];
    FILE *inputFile;
    FILE *outputFile = fopen(outputFileName, "wb");
    char c;

    // arguments counter error
    if (argc < 4) {
        printf ("\n > ERROR: too few arguments [mode] [input] [output]\n");
        return 1;
    }
    else if (argc > 4) {
        printf ("\n > ERROR: too many arguments [mode] [input] [output]\n");
        return 1;
    }

    // mode error
    if (strcmp(mode, "wl") != 0 && strcmp(mode, "lw") != 0) {
        printf("\n > ERROR: mode should be wl or lw\n");
        return 1;
    }

    // file error
    if ((inputFile = fopen(inputFileName, "rb")) == NULL) {
        printf("\n > ERROR: %s: no such file or directory\n", inputFileName);
        return 1;
    }

    // windows to linux
    if (strcmp(mode, "wl") == 0) {
        while ((c = fgetc(inputFile)) != EOF) {
            if (c != CR) {
                fputc(c, outputFile);
            }
        }
    }

    // linux to windows
    else {
        while ((c = fgetc(inputFile)) != EOF) {
            if (c == NL) {
                fputc(CR, outputFile);
            }
            fputc(c, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    printf("\n > File %s was sucessfully converted to %s with %s mode!\n", inputFileName, outputFileName, mode);

    return 0;
}
