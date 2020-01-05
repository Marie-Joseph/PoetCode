/* poet.c - put header here */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sysexits.h>


#define MAX_WORDS 150000
#define MAX_CHARS 47


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
    size_t read;

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
    FILE *fp = fopen("../dicts/large", "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open dictionary\n");
        exit(EX_NOINPUT);
    }

    counter = 0;
    while ((read = fscanf(fp, "%s", buf)) != EOF) {
        word_list[counter] = calloc(MAX_CHARS, sizeof(char));
        strcpy(word_list[counter], buf);
        counter++;
    }
    fclose(fp);

    /* Open and write to output file */
    FILE *pp = fopen("new_poem.txt", "w");
    if (pp == NULL) {
        fprintf(stderr, "Unable to create poem file\n");
        exit(EX_CANTCREAT);
    }

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

    for (i = 0; i < counter; i++) {
        free(word_list[i]);
    }

    exit(EX_OK);
}