#include "pagedir.h"
#include "crawler.h"
#include "bag.h"
#include <sys/stat.h>
#include <dirent.h>

int docID = 1;

bool pagedir_init(const char *pageDirectory) {
	struct stat st;

	// Checks the status of the directory -> if DNE, then create one
	if (stat(pageDirectory, &st) == -1) {

		if (mkdir(pageDirectory, 0700) != 0) {
			fprintf(stderr, "Error: Could not create directory %s.\n", pageDirectory);
			return false;
		}
		printf("Directory %s successfully created.\n", pageDirectory);
	}

	else {
		printf("Error: Directory %s already exists.", pageDirectory);
	}

	// Create a file named any_name.crawler in the directory
    char markerFilename[300];
    snprintf(markerFilename, sizeof(markerFilename), "%s/any_name.crawler", pageDirectory);
    FILE *markerFile = fopen(markerFilename, "w");

    if (markerFile == NULL) {
        perror("Error creating marker file");
        fprintf(stderr, "Error: Could not create marker file %s.\n", markerFilename);
        return false;
    }

    fclose(markerFile);

    printf("Crawler marker file created in %s\n", pageDirectory);
	return true;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	const char *directoryname = "data";

    // Check if the page and pageDirectory are valid
    if (page == NULL || pageDirectory == NULL) {
        fprintf(stderr, "Error: Invalid arguments for pagedir_save.\n");
        return;
    }

    // Construct a filename using the page directory and document ID
    char filename[300]; // Adjust character limit

    sprintf(filename, "%s/%d", directoryname, docID);

    // Open the file to write
    FILE *file = fopen(filename, "w");

    // Checks if file exists
    if (pageDirectory == NULL) {
        perror("Error opening file for writing");
        fprintf(stderr, "Error: Cannot open file %s for writing.\n", filename);
        return;
    }

    // Write webpage info to the file
    fprintf(file, "URL: %s\n", page->url);
    fprintf(file, "Depth: %d\n", page->depth);
    fprintf(file, "HTML: %s\n", page->html);

	// Close file after completion
    if (fclose(file) != 0) {
        perror("Error closing file");
        fprintf(stderr, "Error: Cannot close file %s\n", filename);
        return;
    }

	// Success
    printf("Webpage info saved successfully to %s\n", filename);
    printf("%s has finished writing.\n", filename);

	docID++;
}