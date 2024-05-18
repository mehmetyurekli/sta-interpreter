#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARIABLES 100
#define MAX_TEXT_LENGTH 256
#define MAX_TOKEN_NUMBER 1000

typedef enum
{
    INT,
    STRING
} varType;

typedef struct
{
    char name[MAX_TEXT_LENGTH];
    varType type;
    union
    {
        int intValue;
        char textValue[MAX_TEXT_LENGTH];
    } value;
} Variable;

typedef struct
{
    char type[MAX_TEXT_LENGTH];
    char value[MAX_TEXT_LENGTH];
} Token;

Variable variables[MAX_VARIABLES];
int variableIndex = 0;

char *replace( // taken from https://stackoverflow.com/questions/779875/what-function-is-to-replace-a-substring-from-a-string-in-c
    char const *const original,
    char const *const pattern,
    char const *const replacement)
{
    size_t const replen = strlen(replacement);
    size_t const patlen = strlen(pattern);
    size_t const orilen = strlen(original);

    size_t patcnt = 0;
    const char *oriptr;
    const char *patloc;

    for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
    {
        patcnt++;
    }

    {
        size_t const retlen = orilen + patcnt * (replen - patlen);
        char *const returned = (char *)malloc(sizeof(char) * (retlen + 1));

        if (returned != NULL)
        {
            char *retptr = returned;
            for (oriptr = original; (patloc = strstr(oriptr, pattern)); oriptr = patloc + patlen)
            {
                size_t const skplen = patloc - oriptr;
                strncpy(retptr, oriptr, skplen);
                retptr += skplen;
                strncpy(retptr, replacement, replen);
                retptr += replen;
            }
            strcpy(retptr, oriptr);
        }
        return returned;
    }
}

bool isDefinedBefore(Token tokens[], Token currentToken, int currentIndex)
{

    for (int i = 0; i < currentIndex; i++)
    {
        if (strcmp(tokens[i].type, currentToken.type) == 0)
        {
            if (strcmp(tokens[i].value, currentToken.value) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

int getVariableIndex(char name[])
{
    for (int i = 0; i <= variableIndex; i++)
    {
        if (strcmp(variables[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

char *removeDoubleQuotes(char *input)
{
    int len = strlen(input);
    if (len > 0)
        input++;
    if (len > 1)
        input[len - 2] = '\0';
    return input;
}

int doAssignments(Token tokens[], Token currentToken, int i, bool new, varType type)
{
    bool firstVar = true;
    if (new)
    { // currentToken is variable's name
        strcpy(variables[variableIndex].name, currentToken.value);
        variableIndex++;
        firstVar = false;
    }

    int index = getVariableIndex(currentToken.value);
    currentToken = tokens[++i]; // is or comma
    while (true)
    {

        if (strcmp(currentToken.type, "Comma\n") == 0 || strcmp(currentToken.type, "Comma") == 0)
        {                               // only declaration
            currentToken = tokens[++i]; // skip the comma
            continue;
        }
        else if (strcmp(currentToken.type, "Line Seperator\n") == 0 || strcmp(currentToken.type, "Line Seperator") == 0)
        {
            return i;
        }

        if (strcmp(currentToken.type, "Identifier") == 0)
        { // currentToken is variable's name
            if (new)
            {
                if (type == INT)
                {
                    strcpy(variables[variableIndex].name, currentToken.value);
                    variableIndex++;
                }
                else
                {
                    strcpy(variables[variableIndex].name, currentToken.value);
                    variableIndex++;
                }
            }
            index = getVariableIndex(currentToken.value);
            currentToken = tokens[++i]; // is or comma
        }

        else if (strcmp(currentToken.value, "is\n") == 0 || strcmp(currentToken.value, "is") == 0)
        { // is (declaration)
            int total;
            char totalString[MAX_TEXT_LENGTH];

            currentToken = tokens[++i]; // get the first value
            if (type == INT)
            {
                if (strcmp(currentToken.type, "IntConst") == 0)
                {
                    total = atoi(currentToken.value);
                }
                else if (strcmp(currentToken.type, "Identifier") == 0)
                {
                    int identifierIndex = getVariableIndex(currentToken.value);
                    if (identifierIndex >= 0)
                    {
                        if (variables[identifierIndex].type == INT)
                        {
                            total = variables[identifierIndex].value.intValue;
                        }
                        else
                        {
                            printf("invalid operation");
                        }
                    }
                }
                else
                {
                    printf("unsupported operation");
                    return 0;
                }
            }
            else
            {
                if (strcmp(currentToken.type, "String") == 0)
                {
                    strcpy(totalString, removeDoubleQuotes(currentToken.value));
                }
                else if (strcmp(currentToken.type, "Identifier") == 0)
                {
                    int identifierIndex = getVariableIndex(currentToken.value);
                    if (identifierIndex >= 0)
                    {
                        if (variables[identifierIndex].type == STRING)
                        {
                            strcpy(totalString, variables[identifierIndex].value.textValue);
                        }
                        else
                        {
                            printf("invalid operation");
                        }
                    }
                }
                else
                {
                    printf("unsupported operation");
                    return 0;
                }
            }

            while (true)
            { // declaration might be sth like 5+7+1-a or "str1" + "str2"
                currentToken = tokens[++i];
                if (strcmp(currentToken.type, "Line Seperator\n") == 0 || strcmp(currentToken.type, "Line Seperator") == 0)
                {
                    if (type == INT)
                    {
                        if (total >= 0)
                        {
                            variables[index].value.intValue = total;
                            return i;
                        }
                        else
                        {
                            printf("negative value.");
                            variables[index].value.intValue = 0;
                            return -1;
                        }
                    }
                    else
                    {
                        strcpy(variables[index].value.textValue, totalString);
                        return i;
                    }
                }
                if (strcmp(currentToken.type, "Comma\n") == 0 || strcmp(currentToken.type, "Comma") == 0)
                {
                    if (type == INT)
                    {
                        variables[index].value.intValue = total;
                    }
                    else
                    {
                        strcpy(variables[index].value.textValue, totalString);
                    }

                    break;
                }
                char operation = currentToken.value[0];
                currentToken = tokens[++i]; // get the value to be added, substracted etc.
                int value;
                char valueString[MAX_TEXT_LENGTH];

                if (type == INT)
                {
                    if (strcmp(currentToken.type, "IntConst") == 0)
                    {
                        value = atoi(currentToken.value);
                    }
                    else if (strcmp(currentToken.type, "Identifier") == 0)
                    {
                        int identifierIndex = getVariableIndex(currentToken.value);
                        if (identifierIndex >= 0)
                        {
                            if (variables[identifierIndex].type == INT)
                            {
                                value = variables[identifierIndex].value.intValue;
                            }
                            else
                            {
                                printf("invalid operation");
                                return -1;
                            }
                        }
                    }
                    else
                    {
                        printf("unsupported operation");
                        return -1;
                    }
                }
                else
                { // STRING
                    if (strcmp(currentToken.type, "String") == 0)
                    {
                        strcpy(valueString, removeDoubleQuotes(currentToken.value));
                    }
                    else if (strcmp(currentToken.type, "Identifier") == 0)
                    {
                        int identifierIndex = getVariableIndex(currentToken.value);
                        if (identifierIndex >= 0)
                        {
                            if (variables[identifierIndex].type == STRING)
                            {
                                strcpy(valueString, variables[identifierIndex].value.textValue);
                            }
                            else
                            {
                                printf("invalid operation");
                                return -1;
                            }
                        }
                    }
                    else
                    {
                        printf("unsupported operation");
                        return -1;
                    }
                }

                if (operation == '+')
                {
                    if (type == INT)
                    {
                        total += value;
                    }
                    else
                    {
                        sprintf(totalString, "%s%s", totalString, valueString);
                    }
                }
                else if (operation == '-')
                {
                    if (type == INT)
                    {
                        total -= value;
                    }
                    else
                    {
                        strcpy(totalString, replace(totalString, valueString, " "));
                    }
                }
                else if (operation == '/' && type == INT)
                {
                    total /= value;
                }
                else if (operation == '*' && type == INT)
                {
                    total *= value;
                }
                else
                {
                    return -1;
                }
            }
        }
    }
    return i;
}

void doLoops(Token tokens[], int loopStart, int loopEnd, int loopCount);

void interpretTokens(Token tokens[], int tokenCount, int start, int end) // tokenCount actually holds the last index so its count-1
{
    Token currentToken;
    for (int i = start; i <= end; i++)
    {
        currentToken = tokens[i];
        if (currentToken.type[0] == '\0')
        {
            variableIndex--;
            break;
        }

        if (strcmp(currentToken.type, "Identifier") == 0) // existing variables
        {
            int index = getVariableIndex(currentToken.value);
            if (variableIndex < 0)
            {
                printf("variable doesnt exist");
                return;
            }
            else
            {
                i = doAssignments(tokens, currentToken, i, false, variables[index].type);
                if (i < 0)
                {
                    return;
                }
                continue;
            }
        }

        if (strcmp(currentToken.type, "Keyword") == 0)
        {
            if (strcmp(currentToken.value, "int") == 0)
            {
                variables[variableIndex].type = INT;

                currentToken = tokens[++i];
                i = doAssignments(tokens, currentToken, i, true, INT);
                if (i < 0)
                {
                    return;
                }
                continue;
            }
            else if (strcmp(currentToken.value, "text") == 0)
            {
                variables[variableIndex].type = STRING;

                currentToken = tokens[++i]; // skip 'text'
                i = doAssignments(tokens, currentToken, i, true, STRING);
                if (i < 0)
                {
                    return;
                }
                continue;
            }
            else if (strcmp(currentToken.value, "read") == 0)
            {
                currentToken = tokens[++i]; // skip 'read' next could be a promt or variable

                if (strcmp(currentToken.type, "String") == 0) // prompt
                {
                    printf("%s", removeDoubleQuotes(currentToken.value));
                    currentToken = tokens[++i]; // skip the prompt
                    currentToken = tokens[++i]; // skip the comma
                    int readIndex = getVariableIndex(currentToken.value);
                    if (readIndex >= 0)
                    {
                        if (variables[readIndex].type == INT)
                        {
                            int readInt;
                            scanf("%d", &readInt);
                            variables[readIndex].value.intValue = readInt;
                        }
                        else
                        {
                            char readString[MAX_TEXT_LENGTH];
                            scanf("%s", readString);
                            strcpy(variables[readIndex].value.textValue, readString);
                        }
                    }
                    else
                    {
                        printf("VARIABLE NOT FOUND");
                    }
                }
                else if (strcmp(currentToken.type, "Identifier") == 0) // variable
                {
                    int readIndex = getVariableIndex(currentToken.value);
                    if (readIndex >= 0)
                    {
                        if (variables[readIndex].type == INT)
                        {
                            int readInt;
                            scanf("%d", &readInt);
                            variables[readIndex].value.intValue = readInt;
                        }
                        else
                        {
                            char readString[MAX_TEXT_LENGTH];
                            scanf("%s", readString);
                            strcpy(variables[readIndex].value.textValue, readString);
                        }
                    }
                    else
                    {
                        printf("VARIABLE NOT FOUND");
                    }
                }
                else
                {
                    printf("ERROR IN READ");
                    return;
                }
            }

            else if (strcmp(currentToken.value, "write") == 0)
            {
                while (true)
                {
                    currentToken = tokens[++i];
                    if (strcmp(currentToken.type, "String") == 0)
                    {
                        printf("%s", removeDoubleQuotes(currentToken.value));
                    }
                    else if (strcmp(currentToken.type, "Identifier") == 0)
                    {
                        int readIndex = getVariableIndex(currentToken.value);
                        if (readIndex >= 0)
                        {
                            if (variables[readIndex].type == INT)
                            {
                                printf("%d", variables[readIndex].value.intValue);
                            }
                            else
                            {
                                char readString[MAX_TEXT_LENGTH];
                                printf("%s", variables[readIndex].value.textValue);
                            }
                        }
                    }
                    else if (strcmp(currentToken.type, "Comma\n") == 0 || strcmp(currentToken.type, "Comma") == 0)
                    {
                        continue;
                    }
                    else if (strcmp(currentToken.type, "Line Seperator\n") == 0 || strcmp(currentToken.type, "Line Seperator") == 0)
                    {
                        break;
                    }
                }
            }
            else if (strcmp(currentToken.value, "newLine") == 0)
            {
                printf("\n");
                currentToken = tokens[++i]; // line seperator
                continue;
            }
            else if (strcmp(currentToken.value, "loop") == 0)
            {
                currentToken = tokens[++i];                                                                                     // skip the 'loop' keyword
                int loopCount = atoi(currentToken.value);                                                                       // intConst
                currentToken = tokens[++i];                                                                                     // skip the number
                currentToken = tokens[++i];                                                                                     // skip the times word
                if (strcmp(currentToken.type, "LeftCurlyBracket") == 0 || strcmp(currentToken.type, "LeftCurlyBracket\n") == 0) // code block loop
                {
                    currentToken = tokens[++i]; // skip the left curly bracket
                    int loopStart = i;
                    int leftCurlyCount = 1;
                    int rightCurlyCount = 0;
                    while (true)
                    { // get the outer loop ending index

                        if (strcmp(currentToken.type, "LeftCurlyBracket") == 0 || strcmp(currentToken.type, "LeftCurlyBracket\n") == 0)
                        {
                            leftCurlyCount++;
                        }
                        else if (strcmp(currentToken.type, "RightCurlyBracket") == 0 || strcmp(currentToken.type, "RightCurlyBracket\n") == 0)
                        {
                            rightCurlyCount++;
                            if (leftCurlyCount == rightCurlyCount)
                            {
                                break;
                            }
                        }
                        currentToken = tokens[++i];
                    }
                    int loopEnd = --i;

                    doLoops(tokens, loopStart, loopEnd, loopCount);
                    continue;
                }
                else
                {
                    int loopStart = i;
                    while (true)
                    {
                        if (strcmp(currentToken.type, "Line Seperator\n") == 0 || strcmp(currentToken.type, "Line Seperator") == 0)
                        {
                            break;
                        }
                        currentToken = tokens[++i];
                    }
                    int loopEnd = i;
                    doLoops(tokens, loopStart, loopEnd, loopCount);
                    continue;
                }
            }
            else
            { // other keywords will be handled here
                return;
            }
        }
    }
    return;
}

void doLoops(Token tokens[], int loopStart, int loopEnd, int loopCount)
{
    for (int i = 0; i < loopCount; i++)
    {
        interpretTokens(tokens, loopCount - 1, loopStart, loopEnd);
    }
}

int main()
{
    FILE *file = fopen("code.lex", "r");
    if (file == NULL)
    {
        return 0;
    }

    Token tokens[MAX_TOKEN_NUMBER];
    int tokenIndex = 0;

    char line[MAX_TOKEN_NUMBER];

    while (fgets(line, MAX_TOKEN_NUMBER, file))
    { // reading the file line by line and creating tokens.
        char type[MAX_TEXT_LENGTH];
        char value[MAX_TEXT_LENGTH];

        if (sscanf(line, "%[^()] (%[^)])", type, value) == 2)
        {
            strcpy(tokens[tokenIndex].type, type);
            strcpy(tokens[tokenIndex].value, value);
            tokenIndex++;
        }
        else if (sscanf(line, "%[^()] (%[^)])", type, value) == 1)
        {
            strcpy(tokens[tokenIndex].type, type);
            tokenIndex++;
        }
    }

    varType currentType;

    interpretTokens(tokens, tokenIndex, 0, tokenIndex);

    return 0;
}