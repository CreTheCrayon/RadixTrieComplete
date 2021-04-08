/*
 * functions.h
 *
 * Interface for a simple, dynamically allocated radix trie
 *
 * Users of this data structure are expected to keep a pointer to the
 * root of the trie (even if empty), and pass it into trie functions.
 *
 * A root with a null edge and a null node pointer array is taken to mean
 * the trie is empty.
 *
 * Written by Christian Rondeau - April 2021
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct node {
    int wordEnd;
    struct node *children[26];
    char strEdge[35];
    int nodeCount;
    int wordCount;
};

/*
 * Search for word in trie.
 *
 * Parameters:
 * 		*root       -- top node (root) held by calling code.
 *    sWord[] 	  -- word to search for.
 *
 * Returns 1 on success, or 0 if word was not found.
 */
int search(char sWord[], struct node *root);

/*
 * Insert given word into trie
 *
 * Parameters:
 * 		*root 	 -- top node (root) held by calling code.
 *    *counter -- detached node that stores word/node count for other functions
 *
 * Returns nothing.
 */
void insert(char sWord[], struct node *root, struct node *counter);

/*
 * Delete trie, leaving it ready to use again
 *
 * Parameters:
 * 		*root       -- top node (root) held by calling code.
 *    *currNode 	-- root as well, used for comparisons within function
 *
 */
void emptyTree(struct node *currNode, struct node *root);

/*
 * Print all words in trie in lexicographical order
 *
 * Parameters:
 * 		*currNode       -- top node (root) held by calling code.
 *    currword[] 	    -- char array used for storing/printing current word
 *
 */
void printWords(struct node *currNode, char currWord[]);

#endif
