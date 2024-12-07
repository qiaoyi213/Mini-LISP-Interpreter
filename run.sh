lex src/main.l
mv lex.yy.c build/
yacc -d src/main.y
mv y.tab.c build/
mv y.tab.h build/
gcc build/lex.yy.c build/y.tab.c -ll -ly -o build/main.out  
./build/main.out
