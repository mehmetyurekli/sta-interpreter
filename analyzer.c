#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#define MAX_STRING_SIZE 256
#define MAX_IDENTIFIER_SIZE 10
#define MAX_INTEGER_SIZE 8
#define MAX_STACK_SIZE 16

// STACK STRUCTURE TO CHECK THE BALANCE OF CURLY BRACKETS.
char stack[MAX_STACK_SIZE];
int top = -1;

bool isempty()
{
    return (top == -1);
}

bool isfull()
{
    return (top == MAX_STACK_SIZE);
}

char peek()
{
    return stack[top];
}

char pop()
{
    int data;
    if (!isempty())
    {
        data = stack[top];
        top--;
        return data;
    }
    else
    {
        printf("empty stack.\n");
        return '\0';
    }
}

void push(char data)
{
    if (!isfull())
    {
        top = top + 1;
        stack[top] = data;
    }
    else
    {
        printf("cant push. stack is full.\n");
    }
}
// END OF STACK

bool isKeyword(char str[MAX_IDENTIFIER_SIZE]) // Function to check if given string is a keyword.
{
    char keyWords[8][MAX_IDENTIFIER_SIZE] = {"int", "text", "is", "loop", "times", "read", "write", "newLine"};

    for (int i = 0; i < 8; i++)
    {
        if (strcmp(str, keyWords[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    printf("\x1b[31m");
    FILE *outFile = fopen("code.lex", "w");
    FILE *file = fopen("code.sta", "r");
    if (file == NULL)
    {
        return 0;
    }

    char c = '\0';
    char last;
    char next;              // Temp char i use like a peek method.
    char lastBeforeComment; // Last char before a comment has started.
    bool comment = false;   // Is true if last scanned thing is a comment. Necessary if the file ends after a comment.

    do
    {
        if (!comment && c != '\xff' && c != '\n')
        {
            last = c;
        }
        c = fgetc(file);
        if (c == ' ') // skips blank spaces
        {
            continue;
        }

        if (c == '/') // checks if it's a comment or operation.
        {
            comment = false;
            next = fgetc(file);
            if (next == '*') // comment start detected.
            {
                lastBeforeComment = last;
                last = c;
                c = next;
                while (true)
                {
                    if (feof(file))
                    {
                        printf("ERROR! Comment couldn't terminate before EOF.");
                        return 0;
                    }
                    last = c;
                    c = fgetc(file);
                    if (c == '*')
                    {
                        last = c;
                        c = fgetc(file);
                        if (c == '/')
                        { // end of comment
                            last = lastBeforeComment;
                            comment = true;
                            break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
            }
            else
            { // operator '/' detected. 'next' is inserted back to the file.
                ungetc(next, file);
                fprintf(outFile, "Operator(/)\n");
                continue;
            }
        }

        // STRING HANDLING
        else if (c == '"')
        {
            comment = false;
            char string[MAX_STRING_SIZE];          // Creating an array with a size of max supported string lenght.
            memset(string, '\0', MAX_STRING_SIZE); // Setting the array elements to NULL.
            int i = 0;

            while (true)
            {
                if (feof(file))
                {
                    printf("ERROR! String constant couldn't terminate before EOF.\n");
                    return 0;
                }

                last = c;
                c = fgetc(file);

                if (i >= MAX_STRING_SIZE)
                {
                    printf("ERROR! String constant is longer than %d characters.\n", MAX_STRING_SIZE);
                    return 0;
                }

                if (c != '"')
                {
                    string[i++] = c;
                }
                else
                {
                    break;
                }
            }

            fprintf(outFile, "String(\"%s\")\n", string); // String token created.
        }
        // END OF STRING HANDLING

        // IDENTIFIER - KEYWORD HANDLING
        else if (isalpha(c))
        {
            comment = false;
            char word[MAX_IDENTIFIER_SIZE];          // Creating an array with a size of max supported identifier lenght.
            memset(word, '\0', MAX_IDENTIFIER_SIZE); // Setting the array elements to NULL.
            int i = 0;
            while (true)
            {
                if (i >= MAX_IDENTIFIER_SIZE)
                {
                    printf("ERROR! Max identifier size is %d characters.\n", MAX_IDENTIFIER_SIZE);
                    i = 0;
                    return 0;
                }
                word[i++] = c;

                next = fgetc(file);
                if (isspace(next) || next == '.' || next == '{' || next == '}' || next == '+' || next == '-' ||
                    next == '*' || next == '/' || next == ',' || next == '\xff' || next == '\n')
                { // Puts back 'next' to the file if the next char is supported or new line or EOF. They will be handled exclusively.
                    ungetc(next, file);
                    i = 0;
                    break;
                }
                last = c;
                c = next;
                if (ispunct(c) && c != '_')
                { // Unsupported character in an identifier is detected.
                    printf("ERROR! Identifiers can only have alphabetical, numerical and underscore as characters.\n");
                    i = 0;
                    return 0;
                }
            }

            if (isKeyword(word))
            {
                fprintf(outFile, "Keyword(%s)\n", word);
            }
            else
            {
                fprintf(outFile, "Identifier(%s)\n", word);
            }
        }
        // END OF IDENTIFIER - KEYWORD HANDLING

        // INTEGER HANDLING
        else if (isdigit(c))
        {
            comment = false;
            char integer[MAX_INTEGER_SIZE];          // Creating an array with a size of max supported identifier lenght.
            memset(integer, '\0', MAX_INTEGER_SIZE); // Setting the array elements to NULL.
            int i = 0;
            while (true)
            {
                if (i >= MAX_INTEGER_SIZE)
                {
                    printf("ERROR! Max integer size is %d digits.\n", MAX_INTEGER_SIZE);
                    i = 0;
                    return 0;
                }
                integer[i++] = c;

                next = fgetc(file);
                if (isspace(next) || next == '.' || next == '{' || next == '}' || next == '+' || next == '-' ||
                    next == '*' || next == '/' || next == ',' || next == '\xff' || next == '\n')
                { // Puts back 'next' to the file if the next char is supported or new line or EOF. They will be handled exclusively.
                    ungetc(next, file);
                    i = 0;
                    break;
                }
                last = c;
                c = next;
                if (!isdigit(c))
                { // Non-digit character is detected.
                    printf("ERROR! Integer cannot contain non digit characters.\n");
                    return 0;
                }
            }
            fprintf(outFile, "IntConst(%s)\n", integer);
        }
        // END OF INTEGER HANDLING

        // SPECIAL CASES
        else
        {
            switch (c)
            {
            case '{':
                comment = false;
                fprintf(outFile, "LeftCurlyBracket\n");
                push(c); // Pushing 'c' to the stack to check the balance of brackets.
                break;
            case '}':
                comment = false;
                if (pop() != '{') // Checking the bracket balance.
                {
                    printf("ERROR! Brackets are not balanced.\n");
                    return 0;
                }
                fprintf(outFile, "RightCurlyBracket\n");
                break;
            case '+':
                comment = false;
                fprintf(outFile, "Operator(%c)\n", c);
                break;

            case '-':
                comment = false;
                fprintf(outFile, "Operator(%c)\n", c);
                break;

            case '/':
                comment = false;
                fprintf(outFile, "Operator(%c)\n", c);
                break;

            case '*':
                comment = false;
                fprintf(outFile, "Operator(%c)\n", c);
                break;
            case ',':
                comment = false;
                fprintf(outFile, "Comma\n");
                break;
            case '.':
                comment = false;
                fprintf(outFile, "Line Seperator\n");
                break;
            case '\xff': // EOF case. It will be handled outside the loop.
                continue;

            case '\n':
                if (comment)
                {
                    continue;
                }
                if (last == '\n') // If it is after another new line, continues.
                {
                    continue;
                }
                if (last == '\0') // Last is never defined.
                {
                    continue;
                }
                if (last == ' ') // Last is a space.
                {
                    continue;
                }
                if (last != '.') // New line is after an integer, keyword, identifier etc. meaning that line seperator is missing.
                {
                    // printf("ERROR! Every line must end with a '.'");
                    // return 0;
                    continue;
                }
                continue;

            default:
                printf("ERROR! Unrecognized character - '%c'", c); // Any unsupported character.
                return 0;
            }
        }

    } while (!feof(file));
    if (!isempty())
    {
        printf("ERROR! Brackets are not balanced.");
    }
    fclose(file);
    fclose(outFile);
}
