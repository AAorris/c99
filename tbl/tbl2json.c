// Stream a tablatal file in and JSON out.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tbl.h"


void presentJson(FILE *file) {
    char* line = (char*) malloc(sizeof(char) * LINESIZE);
    size_t columnSpace; // available space for current column
    int started = 0;
    fgets(line, LINESIZE, file);
    char** header = parseHeader(line);
    int* starts = allocateColumnStarts(header);
    printf("[");
    while (fgets(line, LINESIZE, file)) {
        if (started)
            printf(",\n");
        printf("{");
        for(int i=0; i<32; i++) {
			if (!header[i]) break;
            char* column = line + starts[i];
            char* headerColumn = header[i];
            // FIXME: move these calculations out of the loops
            columnSpace = strlen(headerColumn) - 1;
            if (i > 0) printf(",");
            printf("\"%.*s\":", tbtlen(headerColumn, columnSpace), headerColumn);
            printf("\"%.*s\"", tbtlen(column, columnSpace), column);
        }
        started = 1;
        printf("}");
    }
    printf("]\n");
    free(line);
    free(starts);
	int i = 0;
	while (header[i])
		free(header[i++]);
    free(header);
}


int main(int argc, char *argv[]) {
    FILE * file;
    if (argc == 1)
        file = fopen("example.tablatal.txt", "r");
    else
        file = fopen(argv[1], "r");
    if (!file) {
        printf("EOPEN\n");
        return 1;
    }
    presentJson(file);
    fclose(file);
    return 0;
}
