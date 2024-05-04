#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bag.h"
#include "crawler.h"


bag_t *init_bag(void) {
    bag_t *bag = (bag_t *)malloc(sizeof(bag_t));
    bag->next = NULL;
    return bag;
}
 
void addToBag(bag_t *bag, webpage_t *newPage) {
    bag_t *iter = bag;

    while (iter != NULL && iter->next != NULL) {
        iter = iter->next;
    }

    bag_t *newBag = init_bag();
    newBag->next = NULL;
    iter->next = newBag;
        
    newBag->page = newPage;
}

int isBagEmpty(const bag_t *bag) {
	return bag->next == NULL;
}

webpage_t *dequeueFromBag(bag_t *head) {
	if (!isBagEmpty(head)) {
		
        bag_t *next = head->next;
        head->next = head->next->next;

        webpage_t *page = next->page;

        free(next);
		return page;
	}

	else {
		// In case where bag is empty
		fprintf(stderr, "Error: Cannot dequeue from an empty bag.\n");
		return NULL;
	}
}