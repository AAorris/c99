clean:
	rm tbl

json:
	gcc -g -Wall tbl.c tbl2json.c -o tbl2json

sql:
	gcc -g -Wall tbl.c tbl2sql.c -o tbl2sql

run:
	./tbl example.tablatal.txt

debug:
	gdb --args ./tbl example.tablatal.txt
