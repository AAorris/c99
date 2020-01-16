# C99 Experiments

## TBL : Tablatal Utility

Tablatal utility. Currently parses an example tablatal file and outputs JSON.

### Quick Start

```
cd tbl
make start  # this will build ./tbl

# After building
./tbl
```

### Example Case

Let's count tags in links

- Convert to json
- Use jq to get the tags field
- Split tags on spaces into new lines
- Sort them for uniq
- Use uniq to get unique items with count
- Show the popular ones on top

```
▲ tbl links.tbtl | jq -r '.[] | .tags' | tr ' ' '\n' | sort | uniq -c | sort -r
   7 tech
   4 home
   3 web
   3 ml
   2 media
   1 tools
   1 lisp
   1 game
   1 c
   1 book
```