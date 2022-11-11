#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "unlzw.h"

#define MAX_FILENAME_LENGTH 1024

int main(int argc, char **argv)
{
    int i;
    unsigned char c;

    char
        input_filename[MAX_FILENAME_LENGTH],
        output_filename[MAX_FILENAME_LENGTH] = "",
        original_filename[MAX_FILENAME_LENGTH] = "";

    FILE *input_file, *output_file;

    /* Afficher nom d'executable */
    printf("Executable name: %s\n", argv[0]);

    /* Afficher les paramètres passés */
    for (i = 1; i < argc; i++)
    {
        printf("Param n°%d: %s\n", i, argv[i]);
    }

    /* Verification des nombres des parametres */
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, ERR_SYNTAX);
        exit(EXIT_FAILURE);
    }

    /* Save input_filename */
    strcpy(input_filename, argv[1]);

    /* Open input file */
    if ((input_file = fopen(input_filename, "rb")) == 0)
    {
        fprintf(stderr, ERR_FILE_MISSING, input_filename);
        exit(EXIT_FAILURE);
    }

    /**
     * Read original filename from compressed file header
     */
    i = 0;
    do
    {
        c = getc(input_file);
        original_filename[i] = c;
        i++;
    } while (c != '\0');

    /* If output file name was given */
    if (argc == 3)
    {
        strcpy(output_filename, argv[2]);
    }
    else
    {
        strcpy(output_filename, original_filename);
    }

    printf("Input: %s, Output: %s\n", input_filename, output_filename);

    /*
     * Check if output file already exists
     */
    if ((output_file = fopen(output_filename, "r")))
    {
        fclose(output_file);
        int x = 0;

        /*
         * Keep asking for users response untill a valid Y/n
         */
        do
        {
            printf(WARN_OUTPUT_EXISTS, output_filename);

            x = tolower(fgetc(stdin));

            while (fgetc(stdin) != LF)
                ;
        } while (x != ANSWER_YES && x != ANSWER_NO);

        /*
         * Quit if user denies
         */
        if (x != ANSWER_YES)
        {
            fprintf(stderr, EXITING);
            exit(EXIT_FAILURE);
        }
    }

    /*
     *   Open output file in write mode
     */
    if (!(output_file = fopen(output_filename, "wb")))
    {
        fprintf(stderr, ERR_FILE_CREATION, output_filename);
        exit(EXIT_FAILURE);
    }

    char header_couples[128 * 2];

    /**
     *  Read header couples
     */
    for (i = 0; i < 128 * 2; i++)
    {
        header_couples[i] = getc(input_file);
    }

    /**
     * Read input file content
     */
    while (true)
    {
        /**
         * Get this char
         */
        c = getc(input_file);

        /**
         * Break if the last char read was EOF
         */
        if (feof(input_file))
            break;

        /**
         * Check if char >= 128
         */
        if (c & 0x80)
        {
            /**
             * Get the chars in the header at addresses (c - 128) * 2 and (c - 128) * 2 + 1
             */
            fprintf(output_file, "%c%c", header_couples[(c & 0x7F) * 2], header_couples[(c & 0x7F) * 2 + 1]);
        }
        else
        {
            /**
             * Print the char as it is
             */
            fprintf(output_file, "%c", c);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return EXIT_SUCCESS;
}