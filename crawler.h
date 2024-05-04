#ifndef CRAWLER_H
#define CRAWLER_H

#include <stddef.h>
#include "set.h"
#include "hashtable.h"

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;




#endif
