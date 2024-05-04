#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

// Helper function to create a new set node
static setnode_t *setnode_new(const char *key, void *item) {
    setnode_t *new_node = (setnode_t *)malloc(sizeof(setnode_t));
    if (new_node == NULL) {
        return NULL;  // Memory allocation error
    }

    // Copy the key and set the item
    new_node->key = strdup(key);
    if (new_node->key == NULL) {
        free(new_node);
        return NULL;  // Memory allocation error
    }
    new_node->item = item;
    new_node->next = NULL;

    return new_node;
}

// Create a new (empty) set; return NULL if error
set_t *set_new(void) {
    set_t *set = (set_t *)malloc(sizeof(set_t));
    if (set == NULL) {
        return NULL;  // Malloc error
    }

    set->head = NULL;
    return set;
}

// Insert item, identified by a key (string), into the given set
bool set_insert(set_t *set, const char *key, void *item) {
    if (set == NULL || key == NULL || item == NULL) {
        return false;  // Invalid parameters
    }

    // Check if the key already exists in the set
    setnode_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return false;  // Key already exists
        }
        current = current->next;
    }

    // Create a new node and insert it at the beginning of the list
    setnode_t *new_node = setnode_new(key, item);
    if (new_node == NULL) {
        return false;  // Error in creating a new node
    }

    new_node->next = set->head;
    set->head = new_node;
    return true;
}

// Return the item associated with the given key
void *set_find(set_t *set, const char *key) {
    if (set == NULL || key == NULL) {
        return NULL;  // Invalid parameters
    }

    // Traverse the list to find the key
    setnode_t *current = set->head;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->item;  // Key found
        }
        current = current->next;
    }

    return NULL;  // Key not found
}

// Print the whole set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (set == NULL || fp == NULL) {
        return;  // Invalid parameters
    }

    setnode_t *current = set->head;
    while (current != NULL) {
        itemprint(fp, current->key, current->item);
        current = current->next;
    }
}

// Iterate over all items in the set
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (set == NULL || itemfunc == NULL) {
        return;  // Invalid parameters
    }

    setnode_t *current = set->head;
    while (current != NULL) {
        itemfunc(arg, current->key, current->item);
        current = current->next;
    }
}

// Helper function to delete a set node
/*static void setnode_delete(setnode_t *node, void (*itemdelete)(void *item)) {
    if (node == NULL) {
        return;
    }

    // Delete the key and item
    free(node->key);
    if (itemdelete != NULL) {
        itemdelete(node->item);
    }

    // Free the memory allocated for the node
    free(node);
}*/

// Delete the whole set
void set_delete(set_t *set, void (*itemdelete)(void *item)) {
    if (set == NULL) {
        return;  // Invalid parameter
    }

    setnode_t *current = set->head;
    while (current != NULL) {
        setnode_t *next = current->next;

        // Free the key and the node
        free(current->key);
        if (itemdelete != NULL) {
            itemdelete(current->item);
        }
        free(current);

        current = next;
    }

    // Free the memory allocated for the set
    free(set);
}