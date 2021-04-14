/* Compiler class 02
* LimChangHwan, BangSungWon
* Implemention of a lexical analyzer
* deadline : 4/18
*/

#include <stdio.h>

// input, output file name
char input[10] = "test.c";
char output[10] = "test.out";

// input, output file pointer
FILE* infile = NULL;
FILE* outfile;

// token names
enum Token { VTYPE=1, INT, CHAR, STR, BOOL, ID, VALUE, COMP, ARITH, ASSIGN, KEYWORD, LPARAN, RPARAN, LBRACE, RBRACE, LBRACKET, RBRACKET, SEMI, SEPAR };

// variable lastToken get token name, last get token ID. 
int lastToken = 0;
char last = ' ';

// lexer (read one character)
char lexer() {
    char lex = fgetc(infile);
    if (lex == '\n')
        linenum++;
    return lex;
}

// kind of error with comment
unsigned int linenum;
int error(int num) {
    switch (num) {
        // no end ' with start '
    case 0:
        fputs("No character end symbol \' ", outfile);
        break;
        // wrong input
    case 5:
        fputs("No String end symbol \" ", outfile);
        break;
    case 10:
        fputs("Undefined symbol ", outfile);
        break;
    }
    fputs("in ", outfile);
    fputs(linenum, outfile);
    fputs("line. ", outfile);
    return num;
}


// check character is number.
int isDigit(char c) {
    if ('0' <= c && c <= '9')
        return 1;
    return 0;
}

// check character is char type.
int isChar(char c) {
    if (('a' <= c && c <= 'z') || ('A'<= c && c <= 'Z'))
        return 1;
    return 0;
}

// check character is whitespace.
int isSpace(char c)  {
    switch(c){
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return 1;
    default:
        return 0;
    }
}

// check character is int type.
int isInteger() {
    char copy[100] = { last };
    last = lexer();
    for (int i = 1; isDigit(last);i++) {
        copy[i] = last;
        last = lexer();
    }
    if (copy[0] == '-' && copy[1] == '0') {
        lastToken = INT;
        fputs("<ARITH, ->", outfile);
        fputs("<INT, 0>", outfile);
    }
    else {
        lastToken = INT;
        fputs("<", outfile);
        fputs("INT, ", outfile);
        fputs(copy, outfile);
        fputs(">", outfile);
    }
    return 1;
}

// all characters in "" are string.  
int isString() {
    char copy[100] = { last };
    last = lexer();
    for (int i = 1; last != '\"' && i < 100; i++) {
        copy[i] = last;
        if (feof(infile))
            return 0;
        last = lexer();
    }
    lastToken = STR;
    last = lexer();
    fputs("<",outfile);
    fputs("STR, ",outfile);
    fputs(copy,outfile);
    fputs(">",outfile);
    return 1;
}

// check characters is identifier.
int isIdentifier(){
    char copy[100] = { last };
    last = lexer();
    for (int i = 1; i < 100; i++) {
        if (!(isChar(last) || isDigit(last) || last == '_'))
            break;
        copy[i] = last;
        last = lexer();
    }
    lastToken = ID;    
    fputs("<",outfile);
    fputs("ID, ",outfile);
    fputs(copy,outfile);
    fputs(">",outfile);
    return 1; 
}

// token 
int findToken(){
    // remove WHITESPACE
    while (isSpace(last))
        last = lexer();

    // semicolon
    if(last == ';'){
        lastToken = SEMI;
        fputs("<",outfile);
        fputs("SEMI",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // left paren
    else if(last == '('){
        lastToken = LPARAN;
        fputs("<",outfile);
        fputs("LPAREN",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // right paren
    else if(last == ')'){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("RPARAN",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // left brace
    else if(last == '{'){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("LBRACE",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // right brace
    else if(last == '}'){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("RBRACE",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // left bracket
    else if(last == '['){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("LBRACKET",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // right bracket
    else if(last == ']'){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("RBRACKET",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // saparate
    else if(last == ','){
        lastToken = RPARAN;
        fputs("<",outfile);
        fputs("SAPARATE",outfile);
        fputs(">",outfile);
        last = lexer();
    }
    // 0
    else if (last == '0'){
        lastToken = INT;
        fputs("<",outfile);
        fputs("INT, ",outfile);
        fputc(last,outfile);
        fputs(">",outfile);
        last = lexer();
    }
    
    // Single character
    else if (last == '\'') {
        last = lexer();
        if(last == '\\')
            last = lexer();
        if (lexer() != '\'')
            return error(0);
        lastToken = CHAR;
        fputs("<",outfile);
        fputs("CHAR, ",outfile);
        fputc(last,outfile);
        fputs(">",outfile);
        last = lexer();
    }

    // Literal String
    else if (last == '\"') {
        if (!isString(last))
            return error(5);
    }

    // identifier _
    else if(last == '_'){
        isIdentifier(last);
    }

    // signed integer or operation
    else if (last == '-') {
        // if last = value, id, ), 0 => operation
        if ((lastToken == VALUE || lastToken == ID) || lastToken == RPARAN){
            lastToken = ARITH;    
            fputs("<",outfile);
            fputs("ARITH, ",outfile);
            fputc(last,outfile);
            fputs(">",outfile);
            last = lexer();
        }
        // else signed integer
        else{
            isInteger(last);
        }
    }
 
    // start with char
    else if(isChar(last)){
        char copy[100] = {0};
        for(int i =0;(isChar(last)||isDigit(last)) || (last == '_');i++){
           copy[i]=last;
           last = lexer();
        }
        // KEYWORD int
        if ((copy[0] == 'i' && copy[1] == 'n') && (copy[2] == 't' && copy[3] == 0))
            lastToken = VTYPE;
        // KEYWORD char
        else if (((copy[0] == 'c' && copy[1] == 'h') && (copy[2] == 'a' && copy[3] == 'r')) && copy[4] == 0)
            lastToken = VTYPE;
        // KEYWORD boolean
        else if ((((copy[0] == 'b' && copy[1] == 'o') && (copy[2] == 'o' && copy[3] == 'l')) && ((copy[4] == 'e' && copy[5] == 'a') && (copy[6] == 'n' && copy[7] == 0))))
            lastToken = VTYPE;
        // KEYWORD String
        else if (((copy[0] == 'S' && copy[1] == 't') && (copy[2] == 'r' && copy[3] == 'i')) && ((copy[4] == 'n' && copy[5] == 'g') && copy[6] == 0))
            lastToken = VTYPE;
        // KEYWORD true
        else if (((copy[0] == 't' && copy[1] == 'r') && (copy[2] == 'u' && copy[3] == 'e')) && copy[4] == 0)
            lastToken = BOOL;
        // KEYWORD false
        else if (((copy[0] == 'f' && copy[1] == 'a') && (copy[2] == 'l' && copy[3] == 's')) && (copy[4] == 'e' && copy[5] == 0))
            lastToken = BOOL;
        // KEYWORD return
        else if (((copy[0] == 'r' && copy[1] == 'e') && (copy[2] == 't' && copy[3] == 'u')) && ((copy[4] == 'r' && copy[5] == 'n') && copy[6] == 0))
            lastToken = KEYWORD;
        // KEYWORD if
        else if ((copy[0] == 'i' && copy[1] == 'f') && copy[2] == 0)
            lastToken = KEYWORD; 
        // KEYWORD else 
        else if (((copy[0] == 'e' && copy[1] == 'l') && (copy[2] == 's' && copy[3] == 'e')) && copy[4] == 0)
            lastToken = KEYWORD;
        // KEYWORD while
        else if (((copy[0] == 'w' && copy[1] == 'h') && (copy[2] == 'i' && copy[3] == 'l')) && (copy[4] == 'e' && copy[5] == 0))
            lastToken = KEYWORD;
        // KEYWORD class
        else if (((copy[0] == 'c' && copy[1] == 'l') && (copy[2] == 'a' && copy[3] == 's')) && (copy[4] == 's' && copy[5] == 0))
            lastToken = KEYWORD;
        // rests are ID
        else
            lastToken = ID;
        fputs("<",outfile);
        if (lastToken == VTYPE)
            fputs("VTYPE, ", outfile);
        else if (lastToken == ID)
            fputs("ID, ", outfile);
        else if (lastToken == BOOL)
            fputs("BOOL, ", outfile);
        else if (lastToken == KEYWORD)
            fputs("KEYWORD, ",outfile);
        fputs(copy,outfile);
        fputs(">",outfile);
     }

    // start with digit
    else if(isDigit(last)){
        isInteger(last);
    }

    //
    else {
        char copy[3] = { 0 };
        if ((last == '<' || last == '>')) {
            copy[0] = last;
            lastToken = COMP;
             if((last = lexer()) != '='){  // < >
                  fputs("<",outfile);
                  fputs("COMP, ",outfile);
                  fputc(last,outfile);
                  fputs(">",outfile);
             }
             else{
                 copy[1] = last;    // <= >=
                 fputs("<",outfile);
                 fputs("COMP, ",outfile);
                 fputs(copy,outfile);
                 fputs(">",outfile);
                 last = lexer();
             }
        }
        else if (last == '='){
            copy[0] = last;
            if ((last = lexer()) != '='){   // =
                lastToken = ASSIGN;
                fputs("<",outfile);
                fputs("ASSIGN",outfile);
                fputs(">",outfile);
            }
            else {
                lastToken = COMP;    // ==
                copy[1] = last;
                fputs("<",outfile);
                fputs("COMP",outfile);
                fputs(copy,outfile);
                fputs(">",outfile);
                last = lexer();
            }
        }

        else if (last == '!'){
            copy[0] = last;
            
            // !
            if ((last = lexer()) != '=')
                return error(10);
            
            // !=
            else {
                lastToken = COMP;
                copy[1] = last;
                fputs("<",outfile);
                fputs("COMP",outfile);
                fputs(copy,outfile);
                fputs(">",outfile);
                last = lexer();
            }
        }

        // character +, -, *, /
        else if (last == '+' || last == '-' || last == '*' || last == '/') {
            lastToken = ARITH;
            fputs("<",outfile);
            fputs("ARITH, ",outfile);
            fputc(last,outfile);
            fputs(">",outfile);
            last = lexer();
        }

        // undefined character input
        else {
            error(10);
            last = lexer();
        }
    }
    return 1;
}



// 출력시 받으면서 출력 끝나고 한번에 출력

int main()
{
    // input and output files open
    fopen_s(&infile, input,"r");
    fopen_s(&outfile, output, "w");

    // there is no input file
    if (!infile) {
        printf("NO INPUT FILE");
        return 0;
    }

    // lex first character
    last = lexer();
    linenum = 1;
    while (!feof(infile)) {
        findToken();
    }

    // close files
    fclose(infile);
    fclose(outfile);
}