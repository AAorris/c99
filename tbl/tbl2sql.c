// Stream a tablatal file in and SQL out.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tbl.h"

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
