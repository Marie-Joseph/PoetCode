/* poet.c - program to write poems
 *
 * poet.c is a simple program which takes a plaintext source file and returns a
 * randomly/programmatically generated poem.
 *
 * The artistic methodology of this program is inspired by John Cage's endeavors
 * to "imitate Nature in her modes of operation," as well as contemporary
 * radical poets.
 *
 * TODO:
 * I. Implement user control of poem shape and source
 * II. Set-up Windows/Unix differences
 *  A. Change paths (and drop sysexits.h?) for Windows
 *  B. Use /usr/share/dict/words for harcoded Unix dictionary
 * III. GUI?
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


/* Exit codes based on widespread sysexits.h*/
#define EX_OK 0
#define EX_USAGE 64
#define EX_NOINPUT 66
#define EX_CANTCREAT 73

/* Global constants */
#define MAX_WORDS 150000
#define MAX_CHARS 47


char *genfilen();

int main (int argc, char **argv) {

    /* Error checking */
    /*if (argc != 2) {
        fprintf(stderr, "Usage: ./poet dictionary\n");
        exit(EX_USAGE);
    }*/

    /* Variables and values */
    const int stanzas = 3;
    const int lines = 12;
    const int words = 13;

    int i;
    int j;
    int k;
    int counter;

    /* Storage */
    char *word_list[MAX_WORDS];
    for (i = 0; i < MAX_WORDS; i++) {
        word_list[i] = NULL;
    }
    char buf[MAX_CHARS];

    /* Randomization prep */
    srand(time(NULL));
    int rand_words;

    /* Open and read-in input file */
    FILE *fp = fopen("dicts/large", "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open dictionary\n");
        exit(EX_NOINPUT);
    }

    counter = 0;
    while (fscanf(fp, "%s", buf) != EOF) {
        word_list[counter] = calloc(MAX_CHARS, sizeof(char));
        strcpy(word_list[counter], buf);
        counter++;
    }
    fclose(fp);

    /* Open and write to output file */
    char *filename = genfilen();
    FILE *pp = fopen(filename, "w");
    if (pp == NULL) {
        fprintf(stderr, "Unable to create poem file\n");
        exit(EX_CANTCREAT);
    }

    /* Iterate over: stanzas, lines, words; write words and whitespaces */
    for (i = 0; i < stanzas; i++) {
        for (j = 0; j < lines; j++) {
            rand_words = (rand() % words) + 1;
            for (k = 0; k < rand_words; k++) {
                fputs(word_list[rand() % counter], pp);
                fputc(' ', pp);
            }
            fputc('\n', pp);
        }
        fputc('\n', pp);
    }
    fclose(pp);

    /* Clean up and go home */
    for (i = 0; i < counter; i++) {
        free(word_list[i]);
    }
    free(filename);

    exit(EX_OK);
}

/* Generate a filename based on current time formatted to ISO standard -
* this would be much easier with C99; check Windows compliance */
char *genfilen() {
    /* Prepare time values, name storage, and generate name */
    const time_t t = time(NULL);
    struct tm *local = localtime(&t);

    char *filen = calloc(MAX_CHARS, sizeof(char));
    
    strftime(filen, MAX_CHARS, "poems/C-%Y-%m-%dT%H:%M:%S", local);

    return filen;
}