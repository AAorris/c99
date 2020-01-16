// tablatal utility
// current function: tablatal file to json
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int COLUMNS = 5;
int LINESIZE = 512;  // max size

// allocate the offset from beginning of a line to
// the beginning of the column. We'll be printing
// substrings of the line read from the file w/o
// allocating any new memory (besides pointers here)
int* allocateColumnStarts(char** header, int columns) {
    int* ret = (int*) malloc(sizeof(int) * columns);
    int acc = 0;
    for (int i=0; i<columns; i++) {
        ret[i] = acc;
        if (i != columns)
            acc += strlen(header[i]);
    }
    return ret;
}

char** parseHeader(char* line, int columns) {
    // header(3):
    // first                 [ s]econd[ t]hird
    // ^----------------------^
    //                         ^-------^
    //                                  ^----^
    char** header = (char**) malloc(sizeof(char*) * columns);
    int column = 0;
    int start = 0;
    int end = 1;
    while (column < columns) {
        if (line[end] != ' ' && line[end - 1] == ' ') {  // [ x]
            header[column] = (char*) malloc(sizeof(char) * ((end - start) + 1));
            strncpy(header[column], line + start, end - start); // x---x
            start = end;
            column += 1;
        }
        end += 1;
    }
    return header;
}

// tbtlen is like strlen but it will ignore trailing spaces
// that means "hi   " has a tbtlen of 2 instead of 5
int tbtlen(char* string, int size) {
    int lastChar = 0;
    for(int i=0; i<size; i++) {
        char c = *(string + i);
        if (c == '\n')
            break;
        if (c != ' ' && c != '\0')
            lastChar = i;
    }
    return lastChar + 1;
}

// given an open file, dump json to stdout
// usage example: ./tbl table.txt | jq '.[] | .title'
void presentJson(FILE *file) {
    char* line = (char*) malloc(sizeof(char) * LINESIZE);
    size_t columnSpace; // available space for current column
    int started = 0;
    fgets(line, LINESIZE, file);
    char** header = parseHeader(line, COLUMNS);
    int* starts = allocateColumnStarts(header, COLUMNS);
    printf("[");
    while (fgets(line, LINESIZE, file)) {
        if (started)
            printf(",\n");
        printf("{");
        for(int i=0; i<COLUMNS; i++) {
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
    for (int i = 0; i < COLUMNS; i++) {
        free(header[i]);
    }
    free(header);
}

// main function.
// here we open a hard coded file and then present it as json.
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