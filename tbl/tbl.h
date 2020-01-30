// Tablatal interface.

int LINESIZE;

int* allocateColumnStarts(char** header);

char** parseHeader(char* line);

int tbtlen(char* string, int size);

void tblnam(char* filename, char* dest);
