/*
 * functions.c
 *
 * Implementation of a simple, dynamically allocated radix trie.
 *
 * This implementation uses a node structures containing, mainly, an array of
 * node pointers to point to children nodes, and an array of chars to hold the edges.
 *
 * Written by Christian Rondeau - April 2021
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <ctype.h>
 #include "functions.h"

 int search(char sWord[], struct node *root) {

    int length = 0;
    while (sWord[length] != '\0') length++;
    struct node * currNode = root;
    int g = 0; //(index for iterating thru each strEdge without overiterating)
    for (int i = 0; i < length; i++) {
        if (currNode->strEdge[g] != '\0') {
            if (sWord[i] == currNode->strEdge[g]) {
                g++; //(if its "=" then increment edge count)
                continue; //just to avoid entering anything else
            }
            return 0;
        } else if (currNode->children[((int)sWord[i])-97] != NULL) {
            //update node pointer
            currNode = currNode->children[((int)sWord[i])-97];
            g = 1; //(if move on to new node, reset incrementer) (if the edge existed then we can
                   //be sure that the first char is the right char and is present, since it is always placed
                   //with the creation of a node, thus we can skip the first index)
            continue; //just to avoid entering anything else
        } else {
            return 0;
        }
    }
    //outside loop, if it has made it this far, then word is probably
    //present, but as final check
    //we want to check at g since if its a single char, then index [1] will be NULL, and if
    //its iterating thru and reaches end of string, it will always hit a null value at g, since g increments
    //before quitting

    if (currNode->strEdge[g] == '\0') { //(meaning we were at the last index of strEdge)
        if (currNode->wordEnd == 1) return 1;
    }

    return 0;

}


void insert(char sWord[], struct node *root, struct node *counter) {

    int length = 0;
    while (sWord[length] != '\0') length++;

    //Lines 63-83 are basically search
    struct node * currNode = root;
    int isPresent = 1;
    int g = 0;
    int k = 0; //keep track of how many letters we've gone thru for later
    for (int i = 0; i < length; i++) {
        if (currNode->strEdge[g] != '\0') {
            if (sWord[i] != currNode->strEdge[g]) {
                isPresent = 0;
                break;
            }
            g++;
        } else if (currNode->children[((int)sWord[i])-97] != NULL) {
            //update node pointer
            currNode = currNode->children[((int) sWord[i]) - 97];
            g = 1;
        } else {
            isPresent = 0;
            break;
        }
        k++;
    }
    //outside loop, if it has made it this far, then word is probably
    //present

    if (currNode->strEdge[g] == '\0') { //(end of strEdge cases)
        if (isPresent == 0) {
            if (currNode == root) {
                if (g == 0) { //if root strEdge is null
                    int isNull = 1;
                    for (int i=0; i < 25; i++) {
                        if (currNode->children[i] != NULL) isNull = 0;
                    }
                    if (isNull) { //(first word case)
                        //add word to strEdge and mark word
                        strcpy(currNode->strEdge, sWord);
                        currNode->wordEnd = 1;
                        counter->nodeCount++;
                        counter->wordCount++;
                        return;
                    }
                }
            }

            //add extra child node containing word from k till NULL
            struct node * newChild = calloc(1, sizeof(struct node));
            counter->nodeCount++;
            int index = 0;
            for (int i=k; i < length; i++) {
                newChild->strEdge[index] = sWord[i];
                index++;
            }
            currNode->children[((int)(newChild->strEdge[0]))-97] = newChild;
            //update node reference to child
            currNode = newChild;
            counter->wordCount++;
        }
        currNode->wordEnd = 1;
        return;
    }
    else { //(middle or beginning of strEdge cases)
        if (currNode == root) { //(split case, for when root needs to change to blank)
            if (isPresent == 0) {
                if (g == 0) {
                    //need to make new root node with blank strEdge
                    struct node * newRoot = calloc(1, sizeof(struct node));
                    counter->nodeCount++;
                    struct node * rootSave = calloc(1, sizeof(struct node));
                    *rootSave = *root;
                    currNode = rootSave;
                    newRoot->children[((int)(currNode->strEdge[0]))-97] = currNode;


                    //and create new child with given sWord
                    struct node * newChild = calloc(1, sizeof(struct node));
                    counter->nodeCount++;
                    strcpy(newChild->strEdge, sWord);
                    newRoot->children[((int)sWord[0])-97] = newChild;
                    newChild->wordEnd = 1;

                    //reassign root
                    *root = *newRoot;
                    counter->wordCount++;
                    free(newRoot);
                    return;
                }
            }
        }
        //Cut out g to (end of strEdge) of node, and only have latter half of word
        //create new node with strEdge = cut out word
        struct node * otherHalf = calloc(1, sizeof(struct node));
        counter->nodeCount++;
        int index = 0;
        int i = g;
        while (currNode->strEdge[i] != '\0') {
            otherHalf->strEdge[index] = currNode->strEdge[i];
            currNode->strEdge[i] = '\0';
            i++;
            index++;
        }
        //assign currNode's children to otherHalf's children
        for (int b=0; b < 26; b++) {
            otherHalf->children[b] = currNode->children[b];
            currNode->children[b] = NULL;
        }
        currNode->children[((int)(otherHalf->strEdge[0]))-97] = otherHalf;
        otherHalf->wordEnd = 1;

        if (isPresent == 0) {
            //if word is not finished, then currNode should not be marked a word
            currNode->wordEnd = 0;
            //add additional child node to node
            struct node * newChild = calloc(1, sizeof(struct node));
            counter->nodeCount++;
            int index2 = 0;
            for (int b=k; b < length; b++) {
                newChild->strEdge[index2] = sWord[b];
                index2++;
            }
            currNode->children[((int)(newChild->strEdge[0]))-97] = newChild;
            //update node reference to child
            currNode = newChild;
        }
        currNode->wordEnd = 1;
        counter->wordCount++;
    }
}

void emptyTree(struct node *currNode, struct node *root) {

    //if we hit dead end, go back up tree
    if (currNode == NULL) {
        return;
    }

    //look thru children of node
    for (int i = 0; i < 26; i++) {
        emptyTree(currNode->children[i], root);
    }

    //if node is root, we dont want to free it here, bc program might not be done
    if (currNode == root) {
        memset(root->strEdge, '\0', 35*sizeof(char));
        memset(root->children, 0, 26*sizeof(struct node *));
        root->wordEnd = 0;
        return;
    }
    free(currNode);

}

void printWords(struct node *currNode, char currWord[]) {

    if (currNode == NULL) {
        return;
    }

    //Find end of currWord
    int length = 0;
    while (currWord[length] != '\0') length++;

    //add edge to currWord
    int i = 0;
    while (currNode->strEdge[i] != '\0') {
        currWord[length+i] = currNode->strEdge[i];
        i++;
    }
    length = length + i;

    //print word if node is marked word
    if (currNode->wordEnd == 1) {
        printf("%s \n", currWord);
    }

    for (i = 0; i < 26; i++) {
        printWords(currNode->children[i], currWord);
    }

    //remove edge from currWord before traveling back up the tree
    i = 0;
    while (currNode->strEdge[i] != '\0') {
        currWord[length-(i+1)] = '\0';
        i++;
    }

}
