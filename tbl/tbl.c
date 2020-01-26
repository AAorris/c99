// tablatal utility
// current function: tablatal file to json
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int LINESIZE = 512;  // max size
// allocate the offset from beginning of a line to
// the beginning of the column. We'll be printing
// substrings of the line read from the file w/o
// allocating any new memory (besides pointers here)
int* allocateColumnStarts(char** header) {
	int* ret = (int*) calloc(32, sizeof(int));
	int acc = 0;
	for (int i=0; i<32; i++) {
		if (!header[i]) return ret;
		ret[i] = acc;
		if (header[i])
			acc += strlen(header[i]);
	}
	return ret;
}

char** parseHeader(char* line) {
	// header(3):
	// first				 [ s]econd[ t]hird
	// ^----------------------^
	//						   ^-------^
	//									^----^
	char** header = (char**) calloc(32, sizeof(char*));
	int column = 0;
	int start = 0;
	int end = 1;
	while (column < 32) {
		if ((line[end] != ' ' && line[end - 1] == ' ') || line[end] == '\n') {	// [ x]
			header[column] = (char*) malloc(sizeof(char) * ((end - start) + 1));
			strncpy(
				header[column],
				line + start,
				(line[end] == '\n' ? end + 1 : end) - start
			); // x---x
			start = end;
			column += 1;
			if (line[end] == '\n') return header;  // end of header
		}
		end += 1;
	}
	return header;
}

// tbtlen is like strlen but it will ignore trailing spaces
// that means "hi	" has a tbtlen of 2 instead of 5
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

void presentSql(FILE *file, char* tableName) {
	char* line = (char*) malloc(sizeof(char) * LINESIZE);
	fgets(line, LINESIZE, file);
	char** header = parseHeader(line);
	int* starts = allocateColumnStarts(header);
	size_t columnSpace;
	int started = 0;
	printf("create table %s (\n", tableName);
	for (int i=0; header[i] && i<32; i++) {
		printf(" %.*s varchar(%lu)", tbtlen(header[i], strlen(header[i]) - 1), header[i], strlen(header[i]));
		if (header[i+1]) printf(",\n");
	}
	printf(");\ninsert into %s (", tableName);
	for (int i=0; header[i] && i<32; i++) {
		printf("%.*s", tbtlen(header[i], strlen(header[i]) - 1), header[i]);
		if (!header[i+1]) break;
		printf(", ");
	}
	printf(") values\n");
	while (fgets(line, LINESIZE, file)) {
		if (started)
			printf(",\n");
		printf(" (");
		for (int i=0; &starts[i] && header[i] && i<32; i++) {
			char* column = line + starts[i];
			char* headerColumn = header[i];
			columnSpace = strlen(headerColumn) - 1;
			printf("\"%.*s\"", tbtlen(column, columnSpace), column);
			if (header[i+1]) printf(",");
		}
		started = 1;
		printf(")");
	}
	printf(";");
}

void tblnam(char* filename, char* dest) {
	for (int i=0; i<32; i++) {
		if (filename[i] == '.') break;
		dest[i] = filename[i];
	}
}

// main function.
// here we open a hard coded file and then present it as json.
int main(int argc, char *argv[]) {
	FILE * file;
	if (argc == 1) {
		printf("Usage: tbl file.tbtl\n");
		return 1;
	}
	
	file = fopen(argv[1], "r");
	if (!file) {
		printf("EOPEN\n");
		return 1;
	}

	char* tablename = (char*) calloc(32, sizeof(char));
	tblnam(argv[1], tablename);
	presentSql(file, tablename);
	fclose(file);
	free(tablename);
	return 0;
}
