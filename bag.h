#ifndef BAG_H
#define BAG_H

#include <stddef.h>
#include "set.h"
#include "hashtable.h"
#include "crawler.h"

typedef struct bag_t{
	webpage_t *page;
	struct bag_t *next;
} bag_t;

// Initialize a bag
bag_t *init_bag(void);

// Function to add items to the bag
void addToBag(bag_t *bag, webpage_t *newPage);

// Function to check if the bag is empty for dequeuing
int isBagEmpty(const bag_t *bag);

// Function to remove head item from queue to retrieve the next URL to be processed
webpage_t *dequeueFromBag(bag_t *head);

#endif