/*
 * poet.c - a program to write poems
 *
 * poet.c is a simple program which takes a source file, a maximum line length
 * in words, a maximum stanza length in lines, and a maximum total length in
 * stanzas, and returns a randomly/programmatically generated plaintext poem
 * file called poem.txt.
 *
 * The artistic methodology of this program is inspired by John Cage's endeavors
 * to "imitate Nature in her modes of operation," as well as contemporary
 * radical poets.
 *
 * Usage: ./poet source [line_length] [stanza_length] [total_length]
 *
 * All lengths are optional, to be replaced by hard-coded values. These values
 * are TBD. However, all parameters preceding any used parameter must also be
 * used.
 *
 */

/* TODO: it do not work */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 27
#define MAX_STR 80
#define BASE_CHAR 97
#define COMMA N - 1


typedef struct node
{
    int is_word;
    struct node *children[N];
} node;


int free_all(struct node *ptr);


int main(int argc, char **argv)
{
    /* check for proper usage */
    if (argc < 2 || argc > 5)
    {
        printf("Usage: ./poet source [line_length] [stanza_length] [total_length]\n");
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (source == NULL)
    {
        fprintf(stderr, "Unable to open %s\n", argv[1]);
        return 2;
    }

    /* variables */
    int line_length = argc > 2 ? strtod(argv[2], NULL) : 12;
    int stanza_length = argc > 3 ? strtod(argv[3], NULL) : 12;
    int total_length = argc > 4 ? strtod(argv[4], NULL) : 3;

    int i;
    int j;
    int k;

    /* create storage */
    struct node *root = calloc((size_t) 1, sizeof(struct node));
    if (root == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory\n");
        return 3;
    }

    char buf[MAX_STR];

    /* read in the source file */
    struct node *ptr = root;
    char c;
    int index;

    while (fscanf(source, "%s", buf) != EOF)
    {
        /* iterate over the buffer, using the number of read characters as the
           limit */
        i = 0;
        do
        {
            c = buf[i];
            i++;

            switch (c)
            {
                /* null-terminator indicates end of word */
                case '\0':
                    ptr->is_word = 1;
                    break;
                
                case '\'':
                    /* check if the comma index exists and make, then switch */
                    if (ptr->children[COMMA] == NULL)
                    {
                        ptr->children[COMMA] = calloc((size_t) 1, sizeof(struct node));
                        if (ptr->children[COMMA] == NULL)
                        {
                            fprintf(stderr, "Unable to allocate memory\n");
                            return 3;
                        }
                    }
                    ptr = ptr->children[COMMA];
                    break;
                
                default:
                    /* make sure this is a lowercase character */
                    index = c % BASE_CHAR;
                    if (index > 25)
                    {
                        fprintf(stderr, "%c is not a valid character\n", c);
                        return 4;
                    }

                    /* store at the character's index in the English alphabet */
                    if (ptr->children[index] == NULL)
                    {
                        ptr->children[index] = calloc((size_t) 1, sizeof(struct node));
                        if (ptr->children[index] == NULL)
                        {
                            fprintf(stderr, "Unable to allocate memory\n");
                            return 3;
                        }
                    }

                    ptr = ptr->children[index];
            }
        } while (c != '\0');
        ptr = root;
    }

    FILE *poem = fopen("new_poem.txt", "w");
    if (poem == NULL)
    {
        fprintf(stderr, "Unable to open poem.txt\n");
        return 2;
    }

    /* seed random number generator */
    srand(time(NULL));
    
    /* iterate over: stanzas, lines, words */
    int ll;
    int to_write;
    for (i = 0; i < total_length; i++)
    {
        for (j = 0; j < stanza_length; j++)
        {
            /* set actual line length randomly based on the input */
            ll = (rand() % line_length) + 1;

            for (k = 0; k < ll; k++)
            {
                while (!ptr->is_word)
                {
                    to_write = rand() % N;
                    if (ptr->children[to_write] != NULL)
                    {
                        if (to_write == COMMA)
                        {
                            fputc('\'', poem);
                        }
                        else
                        {
                            fputc(to_write + BASE_CHAR, poem);
                        }

                        ptr = ptr->children[to_write];
                    }
                }
                fputc(' ', poem);
                ptr = root;
            }
            fputc('\n', poem);
        }
        fputc('\n', poem);
    }

    free_all(root);

    return 0;
}

/* The footwork function for unloading */
int free_all(struct node *ptr)
{
    int i;
    
    /* Iterate over the given node's children */
    for (i = 0; i < N; i++)
    {
        /* Call this function on any allocated nodes */
        if (ptr->children[i] != NULL)
        {
            free_all(ptr->children[i]);
        }
    }
    /* All possible children have been freed; free this node */
    free(ptr);
    /* Indicate success */
    return 1;
}