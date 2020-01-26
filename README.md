# C99 Experiments

## TBL : Tablatal Utility

Tablatal utility. Currently parses an example tablatal file and outputs SQL.

### Quick Start

```
cd tbl
make cc run # compile c code and run

# After building
./tbl example.tablatal.txt  # output sqlite code for the table

# Create an sqlite database

sqlite3 db.sqlite3 <<< `./tbl example.tablatal.txt`

# Run an sqlite session
sqlite3
>>> .tables;
>>> select * from example;
```
