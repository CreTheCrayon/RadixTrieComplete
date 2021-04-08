#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "functions.h"

int main() {
    struct node * root = calloc(1, sizeof(struct node));
    struct node * counter = calloc(1, sizeof(struct node));
    char cmd;
    char word[35];
    char printWord[35];
    memset(printWord, '\0', 35*sizeof(char));

    do {
        do {
            scanf("%c", &cmd);
        } while(isspace(cmd));

        switch (cmd) {
            case 'i': // insert a word
                // read word to insert
                scanf("%s", word);

                //insert word
                insert(word, root, counter);

                break;

            case 's':  //search for a word
                scanf("%s", word);

                if(search(word, root)) {
                    printf("%s is present\n", word);
                }
                else {
                    printf("%s is missing \n", word);
                }
                break;

            case 'n':  //print the number of nodes
                printf("Trie contains %d nodes \n", counter->nodeCount);
                break;

            case 'w':  //print the number of words
                printf("Trie contains %d words \n", counter->wordCount);
                break;

            case 'e':  // empty the tree
                emptyTree(root, root);
                counter->nodeCount = 0;
                counter->wordCount = 0;
                break;

            case 'p':  // print all words in the tree
                printWords(root, printWord);
                memset(printWord, '\0', 35*sizeof(char));
                break;

            case 'q':
                //free memory before quitting
                emptyTree(root, root);
                free(root);
                free(counter);
                break;

            default:
                printf("Oi! That's not a legal command\n");
        }

    } while (cmd != 'q');

    return 0;
}
