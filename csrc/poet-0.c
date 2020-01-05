/*
 * poet.c - a program to write poems
 *
 * poet.c is a simple program which takes a plaintext source file and returns a
 * randomly/programmatically generated poem.
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

/* Dev notes:
* The state of development is unclear. Apparent poems are produced,
* apparently even using real words found in the dictionary as they appear.
* Yet... The Python version tends to make much more sense. The reason the state
* is unclear is that I do not know whether this is a feature or a bug.
* Furthermore, I cannot determine the cause of this behavior. The reads
* seem correct during debugging, the writes as well. This may be a side-effect
* of how fast C is, cramming dozens of rand() calls into a second or less.
* One option is to switch from a quick, light trie of characters to an array of
* words. However, this will make the program significantly heavier.
* On verra.
*/

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


int to_continue(node *ptr);
int free_all(node *ptr);


int main(int argc, char **argv)
{
    /* Check for proper usage */
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

    /* Variables */
    int line_length = argc > 2 ? strtod(argv[2], NULL) : 12;
    int stanza_length = argc > 3 ? strtod(argv[3], NULL) : 12;
    int total_length = argc > 4 ? strtod(argv[4], NULL) : 3;

    int i;
    int j;
    int k;

    /* Storage */
    struct node *root = calloc((size_t) 1, sizeof(struct node));
    if (root == NULL) 
    {
        fprintf(stderr, "Unable to allocate memory\n");
        return 3;
    }

    char buf[MAX_STR];

    /* Read in the source file */
    struct node *ptr = root;
    char c;
    int index;

    while (fscanf(source, "%s", buf) != EOF)
    {
        /* Iterate over the buffer, storing characters appropriately */
        i = 0;
        do
        {
            c = buf[i];
            i++;

            switch (c)
            {
                /* Null-terminator indicates end of word */
                case '\0':
                    ptr->is_word = 1;
                    break;
                
                case '\'':
                    /* Check if the comma index exists or make it; switch */
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
                    /* Make sure this is a lowercase ANSI character */
                    index = c % BASE_CHAR;
                    if (index > 25)
                    {
                        fprintf(stderr, "%c is not a valid character\n", c);
                        return 4;
                    }

                    /* Store at the character's index in the English alphabet */
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

    /* Seed random number generator */
    srand(time(NULL));
    
    /* Iterate over: stanzas, lines, words */
    int ll;
    int to_write;
    for (i = 0; i < total_length; i++)
    {
        for (j = 0; j < stanza_length; j++)
        {
            /* Set actual line length randomly based on the input */
            ll = (rand() % line_length) + 1;

            for (k = 0; k < ll; k++)
            {
                while (!ptr->is_word || to_continue(ptr))
                {
                    /* Generate a random character */
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

/* Simply decide whether to continue when there are more words using this one */
int to_continue(node *ptr)
{
    int i;
    int is_more_word;

    for (i = 0; i < N; i++)
    {
        if (ptr->children[i] != NULL)
        {
            is_more_word = 1;
        }
    }

    if (is_more_word && rand() % 4 != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* Clean up after yourself */
int free_all(node *ptr)
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