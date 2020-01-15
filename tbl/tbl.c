// tablatal utility
// current function: tablatal file to json
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// manually insert header components here.
// white space is important. Later, this can be parsed out.
char* HEADER[] = {
    "title                  ",
    "summary                                       ",
    "tags            ",
    "month     ",
    "url                                                                                                                     "
};
int COLUMNS = 5;

// while we make you provide the number of columns,
// we'll calculate how long the line is in total.
// later, we should get this from the header line.
int calculateLineLength() {
    int ret = 0;
    for (int i=0; i<COLUMNS; i++) {
        ret += strlen(HEADER[i]);
    }
    return ret;
}

// allocate the offset from beginning of a line to
// the beginning of the column. We'll be printing
// substrings of the line read from the file w/o
// allocating any new memory (besides pointers here)
int* allocateColumnStarts() {
    int* ret = (int*) malloc(sizeof(int) * (COLUMNS + 1));
    int acc = 0;
    for (int i=0; i<COLUMNS + 1; i++) {
        ret[i] = acc;
        if (i != COLUMNS)
            acc += strlen(HEADER[i]);
    }
    return ret;
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
// usage example: ./tbl | jq '.[] | .title'
void presentJson(FILE *file) {
    int lineSize = calculateLineLength();
    char* line = (char*) malloc(sizeof(char) * lineSize);
    int* starts = allocateColumnStarts();
    size_t columnSpace; // available space for current column
    int started = 0;
    fgets(line, lineSize, file); //
    fgets(line, lineSize, file); // skip header 
    printf("[");
    while (fgets(line, lineSize, file)) {
        if (started)
            printf(",\n");
        printf("{");
        for(int i=0; i<COLUMNS; i++) {
            char* column = line + starts[i];
            columnSpace = strlen(HEADER[i]) - 1;
            if (i > 0) printf(",");
            printf("\"%.*s\":", tbtlen(HEADER[i], columnSpace), HEADER[i]);
            printf("\"%.*s\"", tbtlen(column, columnSpace), column);
        }
        started = 1;
        printf("}");
    }
    printf("]");
    free(line);
    free(starts);
}

// main function.
// here we open a hard coded file and then present it as json.
int main(int argc, char *argv[]) {
    FILE *file = fopen("example.tablatal.txt", "r");
    if (!file) {
        printf("EOPEN example.tablatal.txt\n");
        return 1;
    }
    presentJson(file);
    fclose(file);
    return 0;
}