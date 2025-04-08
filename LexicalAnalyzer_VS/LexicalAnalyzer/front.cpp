// front.cpp - lexical analyzer system for simple arithmetic expressions (C++ version)
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define END_OF_FILE -1

class Lexer {
private:
    int charClass;
    string lexeme;
    char nextChar;
    int lexLen;
    int nextToken;
    ifstream inFile;

public:
    Lexer(const string& filename) {
        inFile.open(filename);
        if (!inFile) {
            cerr << "ERROR - cannot open " << filename << endl;
            exit(1);
        }
        getChar();
    }

    void addChar() {
        if (lexLen <= 98) {
            lexeme += nextChar;
            lexLen++;
        } else {
            cerr << "Error - lexeme is too long" << endl;
        }
    }

    void getChar() {
        if (inFile.get(nextChar)) {
            if (isalpha(nextChar))
                charClass = LETTER;
            else if (isdigit(nextChar))
                charClass = DIGIT;
            else
                charClass = UNKNOWN;
        } else {
            charClass = END_OF_FILE;
        }
    }

    void getNonBlank() {
        while (isspace(nextChar))
            getChar();
    }

    int lookup(char ch) {
        switch (ch) {
            case '=': addChar(); return ASSIGN_OP;
            case '(': addChar(); return LEFT_PAREN;
            case ')': addChar(); return RIGHT_PAREN;
            case '+': addChar(); return ADD_OP;
            case '-': addChar(); return SUB_OP;
            case '*': addChar(); return MULT_OP;
            case '/': addChar(); return DIV_OP;
            default:  addChar(); return END_OF_FILE;
        }
    }

    int lex() {
        lexeme.clear();
        lexLen = 0;
        getNonBlank();

        switch (charClass) {
            case LETTER:
                addChar();
                getChar();
                while (charClass == LETTER || charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = IDENT;
                break;

            case DIGIT:
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = INT_LIT;
                break;

            case UNKNOWN:
                nextToken = lookup(nextChar);
                getChar();
                break;

            case END_OF_FILE:
                nextToken = END_OF_FILE;
                lexeme = "EOF";
                break;
        }

        cout << "Next token is: " << nextToken << ", Next lexeme is: " << lexeme << endl;
        return nextToken;
    }

    bool isEOF() const {
        return nextToken == END_OF_FILE;
    }
};

int main() {
    cout << "processing statment: D = A / F + 2" << endl;

    Lexer lexer("front.in");
    while (!lexer.isEOF()) {
        lexer.lex();
    }

    return 0;
}