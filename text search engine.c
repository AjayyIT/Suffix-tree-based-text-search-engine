#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 256

// Define a Trie node structure
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    int index; // Stores the starting index of the substring
} TrieNode;

// Function to create and initialize a new node
TrieNode* createNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        newNode->children[i] = NULL;
    }
    newNode->index = -1;
    return newNode;
}

// Function to insert a suffix into the trie
void insertSuffix(TrieNode* root, const char* suffix, int originalIndex) {
    TrieNode* curr = root;
    int len = strlen(suffix); // Calculate length once
    
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)suffix[i];
        if (curr->children[c] == NULL) {
            curr->children[c] = createNode();
        }
        curr = curr->children[c];
        
        // Store the index as we pass through to allow substring matching
        if (curr->index == -1) {
            curr->index = originalIndex;
        }
    }
}

// Function to search for a substring in the text
void search(TrieNode* root, const char* query) {
    TrieNode* curr = root;
    int len = strlen(query);
    
    for (int i = 0; i < len; i++) {
        unsigned char c = (unsigned char)query[i];
        if (curr->children[c] == NULL) {
            printf("No occurrences of '%s' found.\n", query);
            return;
        }
        curr = curr->children[c];
    }
    
    // Because we store the index at every step, we can find substrings anywhere
    printf("Occurrence of '%s' found starting at text index %d.\n", query, curr->index);
}

// Optional: Function to free the memory to prevent memory leaks
void freeTrie(TrieNode* root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            freeTrie(root->children[i]);
        }
    }
    free(root);
}

int main() {
    char text[1000];
    printf("Enter the text: ");
    if (fgets(text, sizeof(text), stdin) == NULL) return 1;
    
    // Remove newline character safely
    text[strcspn(text, "\n")] = '\0';  

    TrieNode* root = createNode();
    int textLen = strlen(text);
    
    // Insert all suffixes
    for (int i = 0; i < textLen; i++) {
        insertSuffix(root, text + i, i);
    }

    char query[100];
    printf("Enter the query: ");
    if (scanf("%99s", query) == 1) { // Prevent query buffer overflow
        search(root, query);
    }

    freeTrie(root);
    return 0;
}
