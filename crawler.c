#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "hashtable.h"
#include "pagedir.h"
#include "crawler.h"
#include "curl.h"
#include "url.h"
#include "bag.h"


/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */
static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
	// Assign value to pointers
	*seedURL = argv[1];
	*pageDirectory = argv[2];
	*maxDepth = atoi(argv[3]);
	
	// Check if there are valid number of args
	if (argc < 4) {
		fprintf(stderr, "Error: Invalid number of arguments.\n Usage: %s <seedURL> <pageDirectory> <maxDepth>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (*maxDepth <= 0) {
		fprintf(stderr, "Error: maxDepth must be a positive integer.");
		exit(EXIT_FAILURE);
	}
}

/**
 * Scans a webpage for URLs.
 * This should check for double and single quotes, as well as white spaces.
 * It should also exclude the url if it has a '#' in the beginning
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {
	const char startPattern[] = "<a href=\"";
	const char altStartPattern[] = "<a href='";
	const char endPattern[] = "\"";
	const char altEndPattern[] = "\'";
	const char blankSpaceStart[] = "<a href=' '";
	const char blankSpaceEnd[] = " ";
	const char exclude[] = "<a href='#'";



	while (strlen(page->html) != 0) {
		if (startsWith(page->html, (char*) &startPattern[0]) && strcmp(startPattern, exclude) != '\0') {
			const char *substringStart = page->html + strlen(startPattern);
			page->html += strlen(startPattern);
			while (strlen(page->html) != 0 && strncmp(page->html, (char*) &endPattern[0], strlen(endPattern)) != 0) {
				page->html++;
			}
			if (strncmp(page->html, (char*) &endPattern[0], strlen(endPattern)) == 0) {
				
				char url[page->html - substringStart + 1];
				strncpy(url, substringStart, page->html - substringStart);
				url[page->html - substringStart] = '\0';

				if (isInternalURL(page->url, url)) {
					char *normalizedURL = normalizeURL(page->url, url);

					if (!hashtable_find(pagesSeen, normalizedURL)) {
						webpage_t *insert = malloc(sizeof(webpage_t));
						insert->url = normalizedURL;
						insert->depth = page->depth + 1;

						hashtable_insert(pagesSeen, normalizedURL, insert);
						addToBag(pagesToCrawl, insert);
					}
				}
			}
		}
		if (startsWith(page->html, (char*) &altStartPattern[0]) && strcmp(altStartPattern, exclude) != '\0') {
			const char *altSubstringStart = page->html + strlen(altStartPattern);
			page->html += strlen(altStartPattern);
			while (strlen(page->html) != 0 && strncmp(page->html, (char*) &altEndPattern[0], strlen(altEndPattern)) != 0) {
				page->html++;
			}
			if (strncmp(page->html, (char*) &altEndPattern[0], strlen(altEndPattern)) == 0) {
				
				char url[page->html - altSubstringStart + 1];
				strncpy(url, altSubstringStart, page->html - altSubstringStart);
				url[page->html - altSubstringStart] = '\0';

				if (isInternalURL(page->url, url)) {
					char *normalizedURL = normalizeURL(page->url, url);

					if (!hashtable_find(pagesSeen, normalizedURL)) {
						webpage_t *insert = malloc(sizeof(webpage_t));
						insert->url = normalizedURL;
						insert->depth = page->depth + 1;

						hashtable_insert(pagesSeen, normalizedURL, insert);
						addToBag(pagesToCrawl, insert);
					}
				}
			}
		}

		if (startsWith(page->html, (char*) &blankSpaceStart[0]) && strcmp(blankSpaceStart, exclude) != '\0') {
			const char *blankSubstringStart = page->html + strlen(blankSpaceStart);
			page->html += strlen(blankSpaceStart);
			while (strlen(page->html) != 0 && strncmp(page->html, (char*) &blankSpaceEnd[0], strlen(blankSpaceEnd)) != 0) {
				page->html++;
			}
			if (strncmp(page->html, (char*) &blankSpaceEnd[0], strlen(blankSpaceEnd)) == 0) {
				
				char url[page->html - blankSubstringStart + 1];
				strncpy(url, blankSubstringStart, page->html - blankSubstringStart);
				url[page->html - blankSubstringStart] = '\0';

				if (isInternalURL(page->url, url)) {
					char *normalizedURL = normalizeURL(page->url, url);

					if (!hashtable_find(pagesSeen, normalizedURL)) {
						webpage_t *insert = malloc(sizeof(webpage_t));
						insert->url = normalizedURL;
						insert->depth = page->depth + 1;

						hashtable_insert(pagesSeen, normalizedURL, insert);
						addToBag(pagesToCrawl, insert);
					}
				}
			}
		}
		page->html++;
	}
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
	hashtable_t *ht = hashtable_new(999);
	webpage_t *web = malloc(sizeof(webpage_t));

	web->url = seedURL;
	web->depth = 0;

	hashtable_insert(ht, seedURL, web);

	bag_t *head = init_bag();

	addToBag(head, web);

	int docID = 1;
	while (!isBagEmpty(head)) {
		webpage_t *web = dequeueFromBag(head);
		web->html = download(web->url, &web->length);

		if (web->html != NULL) {
			pagedir_save(web, pageDirectory, docID);

			if (web->depth != maxDepth) {
				pageScan(web, head, ht);
			}
		}
		free(web);
	}
	free(ht);
	free(head);
}

int main(const int argc, char *argv[]) {
	char *seedURL;
	char *pageDirectory;
	int maxDepth;

	parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

	crawl(seedURL, pageDirectory, maxDepth);

	return EXIT_SUCCESS;
}
