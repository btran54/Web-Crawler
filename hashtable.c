#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

static int hashFunction(const char *key);

// Define a struct for Hashtable

// Simple hash function for strings
static int hashFunction(const char *key) {
    int hash = 0;
    while (*key != '\0') {
        hash = (31 * hash) + (*key);
        key++;
    }
    return hash;
}

// Create a new (empty) hashtable; return NULL if error
hashtable_t *hashtable_new(const int num_slots) {
    if (num_slots <= 0) {
        return NULL;  // Invalid number of bins
    }

    hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
    if (ht == NULL) {
        return NULL;  // Memory allocation error
    }

    ht->num_bins = num_slots;
    ht->bins = (set_t **)malloc(num_slots * sizeof(set_t *));
    if (ht->bins == NULL) {
        free(ht);
        return NULL;  // Memory allocation error
    }

    // Initialize each bin with a new set
    for (int i = 0; i < num_slots; i++) {
        ht->bins[i] = set_new();
        if (ht->bins[i] == NULL) {
            // Error in creating a set
            // Clean up and return NULL
            for (int j = 0; j < i; j++) {
                set_delete(ht->bins[j], NULL);
            }
            free(ht->bins);
            free(ht);
            return NULL;
        }
    }
    return ht;
}

// Insert item, identified by key, into the hashtable
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;  // Invalid parameters
    }

    // Determine the bin for the key using a modulus
    int bin_index = abs(hashFunction(key)) % ht->num_bins;
    
    // Insert item into the corresponding bin
    bool insertion_success = set_insert(ht->bins[bin_index], key, item);

    if (insertion_success) {
        printf("Item with key '%s' successfully inserted into hashtable.\n", key);
    }

    else {
        fprintf(stderr, "Error: Failed to insert item with key '%s' into hashtable.\n", key);
    }

    return insertion_success;
}

// Return the item associated with the given key
void *hashtable_find(hashtable_t *ht, const char *key) {
    if (ht == NULL || key == NULL) {
        return NULL;  // Invalid parameters
    }

    // Determine the bin for the key using a simple modulus hash function
    int bin_index = abs(hashFunction(key)) % ht->num_bins;

    // Find the item in the corresponding bin (set)
    return set_find(ht->bins[bin_index], key);
}

// Print the whole table
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)) {
    if (ht == NULL || fp == NULL) {
        return;  // Invalid parameters
    }

    for (int i = 0; i < ht->num_bins; i++) {
        set_print(ht->bins[i], fp, itemprint);
    }
    
}

// Iterate over all items in the table
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)) {
    if (ht == NULL || itemfunc == NULL) {
        return;  // Invalid parameters
    }

    for (int i = 0; i < ht->num_bins; i++) {
        set_iterate(ht->bins[i], arg, itemfunc);
    }
}

// Helper function to delete a hashtable bin (set)
static void delete_bin(set_t *bin, void (*itemdelete)(void *item)) {
    if (bin == NULL) {
        return;
    }

    set_delete(bin, itemdelete);
}

// Delete the whole hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)) {
    if (ht == NULL) {
        return;  // Invalid parameter
    }

    // Delete each bin (set) in the hashtable
    for (int i = 0; i < ht->num_bins; i++) {
        delete_bin(ht->bins[i], itemdelete);
    }

    // Free the memory allocated for the array of bins and the hashtable structure
    free(ht->bins);
    free(ht);
}