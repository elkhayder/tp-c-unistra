#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "lzw.h"

#define MAX_FILENAME_LENGTH 1024

int main(int argc, char **argv)
{
    int i, j, k, l;

    char
        input_filename[MAX_FILENAME_LENGTH],
        output_filename[MAX_FILENAME_LENGTH];

    FILE *input_file, *output_file, *temp_file;
    long input_file_size;

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

    /* If output file name was given */
    if (argc == 3)
    {
        strcpy(output_filename, argv[2]);
    }
    else
    {
        /* Concat file extension */
        strcpy(output_filename, argv[1]);
        strcat(output_filename, ".lzw");
    }

    printf("Input: %s, Output: %s\n", input_filename, output_filename);

    /* Open input file */
    if ((input_file = fopen(input_filename, "rb")) == 0)
    {
        fprintf(stderr, ERR_FILE_MISSING, input_filename);
        exit(EXIT_FAILURE);
    }

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

    /*
     * Open temp file
     */
    if (!(temp_file = fopen(TEMP_FILENAME, "wb")))
    {
        fprintf(stderr, ERR_FILE_CREATION, TEMP_FILENAME);
        exit(EXIT_FAILURE);
    }

    /*
     * Used in group counting
     */
    unsigned int couples_count[128][128];
    unsigned int ***triplets_count = malloc(128 * sizeof(unsigned int **));

    /*
     * Initiate an empty groups count
     */
    for (i = 0; i < 128; i++)
    {
        triplets_count[i] = malloc(128 * sizeof(unsigned int *));
        for (j = 0; j < 128; j++)
        {
            couples_count[i][j] = 0;
            triplets_count[i][j] = calloc(128, sizeof(unsigned int));
        }
    }

    unsigned int c;

    /**
     * Check if all characters are ASCII < 128, exit if not
     */
    rewind(input_file);
    while ((c = getc(input_file)) != EOF)
    {
        if (c >= 128)
        {
            fprintf(stderr, ERR_NON_ASCII_FILE);
            exit(EXIT_FAILURE);
        }
    }

    /*
     * Read file size
     */
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);

    /*
     * Count groups
     */
    rewind(input_file);
    unsigned int buffer[2];
    buffer[0] = fgetc(input_file);
    buffer[1] = fgetc(input_file);
    while ((i = fgetc(input_file)) != EOF)
    {
        couples_count[buffer[0]][buffer[1]]++;
        triplets_count[buffer[0]][buffer[1]][i]++;

        /**
         * Shift buffer values
         */
        buffer[0] = buffer[1];
        buffer[1] = i;
    }

    /*
     * Most used couples
     []{
        is_triplet:  0/1,
        count,
        i,
        j,
        k
     }
     */
    int most_used[128][4];

    for (i = 0; i < 128; i++)
        for (j = 0; j < 4; j++)
            most_used[i][j] = 0;

    unsigned int last_max_value = 0;
    int last_i;
    int last_j;
    int last_k;
    int last_type = 0;

    for (l = 0; l < 128; l++)
    {
        last_max_value = 0;
        for (i = 0; i < 128; i++)
        {
            for (j = 0; j < 128; j++)
            {
                for (j = 0; j < 128; j++)
                {
                }
                if (couples_count[i][j] >= last_max_value)
                {
                    bool already_inserted = false;
                    /* Loop through previous saved state if the value is already inserted */
                    for (l = 0; l < l; l++)
                    {
                        if (most_used[l][0] == i && most_used[l][1] == j)
                        {
                            already_inserted = true;
                            break;
                        }
                    }
                    /**
                     * Save the current values if they are not already in the list
                     */
                    if (!already_inserted)
                    {
                        last_max_value = couples_count[i][j];
                        last_i = i;
                        last_j = j;
                    }
                }
            }
        }

        /**
         * Add saved i & j to the most used chars list
         */
        most_used[l][0] = last_i;
        most_used[l][1] = last_j;
    }

    /*
     * Log
     */
    for (i = 0; i < 128; i++)
        fprintf(temp_file,
                "\"%c%c\",%d\n",
                most_used[i][0],
                most_used[i][1],
                couples_count[most_used[i][0]][most_used[i][1]]);

    /**
     * Save input file name to output file header
     * We add 1 to the string length to also include the EOS char
     */
    fwrite(input_filename, sizeof(char), strlen(input_filename) + 1, output_file);

    /*
     * Header file for decompressinog
     */
    for (i = 0; i < 128; i++)
    {
        fprintf(output_file, "%c%c", most_used[i][0], most_used[i][1]);
    }

    rewind(input_file);

    /**
     * Read file content
     */
    while ((i = fgetc(input_file)) != EOF)
    {
        /**
         * Get next char
         */
        j = fgetc(input_file);

        /**
         * This variable will be used in case the couple is most used
         */
        int line = 0;

        for (l = 0; l < 128; l++)
        {
            /**
             * Loop through all most used couple to check if the current couple is in it
             */
            if (most_used[l][0] == i && most_used[l][1] == j)
            {
                /**
                 * If the couple exists, save it address and quit the loop
                 */
                line = 128 + l;
                break;
            }
        }

        /**
         * IF the couple exists
         */
        if (line != 0)
        {
            /**
             * Write the couple address + 128 as a single char
             */
            fprintf(output_file, "%c", line);
        }
        else
        {
            /**
             * Write the first char and seek the file one char back for the next iter
             */
            fprintf(output_file, "%c", i);
            fseek(input_file, -1, SEEK_CUR);
        }

        /**
         * If the last + 1 char is EOF break the loop
         */
        if (j == EOF)
            break;
    }

    fclose(input_file);
    fclose(output_file);
    fclose(temp_file);

    /**
     * Free used memory
     */
    for (i = 0; i < 128; i++)
    {
        for (j = 0; j < 128; j++)
        {
            free(triplets_count[i][j]);
        }
        free(triplets_count[i]);
    }
    free(triplets_count);

    return EXIT_SUCCESS;
}