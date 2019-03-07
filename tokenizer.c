/**
* tokenizer.c - A simple token recognizer. The main file.
* This program will take input from a file and output the specific
* tokens/lexeme to another file. Format how one wishes, as long as the tokens
* are read from left to right, top to bottom. End statements with a semicolon.
* NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
*       program.
*
* @author Kevin Filanowski
* @version 04/08/2018
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"
#include <ctype.h>

// Global pointer to line of input
char *line;

/**
* Main method. Opens the file, grabs tokens, and writes them to a file.
* @param argc - Argument count
* @param argv[] - Array of arguments
*/
int tokenizer(FILE *out_file) {
  char  token[TSIZE];      /* Spot to hold a token, fixed size         */
  char  alpha[TSIZE];      /* Spot to hold numeric characters          */
  char  error[TSIZE];      /* Spot to hold errors                      */
  int   result = FALSE;    /* FALSE if no errors found, TRUE otherwise */

  //Reset token, alpha, and error after every new line.
  strncpy(token, "", TSIZE);
  strncpy(alpha, "", TSIZE);
  strncpy(error, "", TSIZE);

  //Scan tokens for lexical errors.
  while (strlen(strstr(line, token)) > 1) {
    get_token(token); //Get the next token, if any.

    if (strncmp(token, "NULL", TSIZE) == 0) { //Check if a token existed.
      strncpy(token, "", TSIZE);              //Reset the token.
      continue;                               //Move onto next token.
    }

    //Lexical error found!
    if (strncmp(token, "ERROR", TSIZE) == 0) {
      error[0] = line[0]; //Save the lexical error for later printing.
      line++;             //Increment past the unrecognized token.
      strncpy(token, "", TSIZE); //Reset the token.
    } else if (isalpha(token[0])) {
      strncpy(alpha, token, TSIZE);
    }
  }
    if (strlen(error) != 0 || strlen(alpha) != 0) {  //Check Non-Lexemes.
      file_write_error(out_file, alpha, error); //Write Unrecognized Token(s).
      result = TRUE; //Error found.
    }
  return result;
}

/**
* This method writes a formatted error message to the file.
* @param out_file - A FILE type, the type to write the information to.
**/
void file_write_error(FILE *out_file, char *alpha, char* error) {
  int i;
  if (strlen(alpha) != 0) {
    for (i = 0; i < strlen(alpha); i++) {
      line++;
    }
    fprintf(out_file, "===> '%s'\nLexical error: not a lexeme\n\n", alpha);
  } else {
    fprintf(out_file, "===> '%s'\nLexical error: not a lexeme\n\n", error);
    line++;
  }
}

/**
* This method writes formatted string containing a token into a line
* on an output file.
* @param start - An integer representing the start of a new statement.
* Statements are seperated by semicolons.
* @param count - An integer representing the token number in the statement.
* @param token_p - A string containing the token to write to the file.
* @param out_file - A FILE type, the type to write the information to.
**/
void file_write_token(int *start, int *count, char *token_p, FILE *out_file) {
  //Write the lexeme to the file.
  fprintf(out_file, "%s", token_p);

  //At the end of a statement, increase the start counter and reset count.
  if (strcmp(token_p, ";") == 0) {
    fprintf(out_file, "\n");
    *start += 1;
    *count = -1;
  }
}

/**
* This method determines the next token in the input file, and modifies
* the pointer to the token to the corrosponding next token. If no lexeme is
* left in the string varaible line, then the token contains the value "NULL".
* If an invalid lexeme is detected, then the token contains the value "ERROR".
* If "NULL" or "ERROR" appears, it will have to be taken care of in the
* function that calls get_token().
* @param token_p - A pointer to a token string.
*/
void get_token(char *token_p) {
  int i = 0, j = 0;
  //Allocate a temporary string to hold the tokens.
  char *temp = calloc(TSIZE, sizeof(char));

  //Get the rest of the string after a specific token.
  string_after_token(token_p);

  //Skip certain special characters.
  while (line[0]==' ' || line[0]=='\n' || line[0]=='\r' || line[0]=='\t') {
    line++;
  }

  //Scan the character(s) for operators.
  switch (line[i]) {
    case ADD_OP:          temp[i] = ADD_OP;          break;
    case SUB_OP:          temp[i] = SUB_OP;          break;
    case MULT_OP:         temp[i] = MULT_OP;         break;
    case DIV_OP:          temp[i] = DIV_OP;          break;
    case LEFT_PAREN:      temp[i] = LEFT_PAREN;      break;
    case RIGHT_PAREN:     temp[i] = RIGHT_PAREN;     break;
    case EXPON_OP:        temp[i] = EXPON_OP;        break;
    case SEMI_COLON:      temp[i] = SEMI_COLON;      break;
    case LESS_THAN_OP:    temp[i] = LESS_THAN_OP;    break;
    case GREATER_THAN_OP: temp[i] = GREATER_THAN_OP; break;
    case ASSIGN_OP:       temp[i] = ASSIGN_OP;       break;
    case NOT_OP:          temp[i] = NOT_OP;          break;
  }

  //Consider the possibility of the operator containing a second character.
  //But only if the first character is not an INT_LITERAL.
  if (!isdigit(line[i]) && line[i+1] == ASSIGN_OP)
  temp[i+1] = ASSIGN_OP;

  //Scan the character(s) for alphanumerics.
  while (isalpha(line[j])) {
    temp[j] = line[j];
    j++;
  }

  //Scan the character(s) for INT_LITERAL.
  while (isdigit(line[i])) {
    temp[i] = line[i];
    i++;
  }

  //Copy the temporary variable to the token.
  strncpy(token_p, temp, TSIZE);

  //If no token was detected, then there are none left, or it is not a lexeme.
  if (strcmp(token_p, "") == 0) {
    if (*line != '\0') {
      strncpy(token_p, "ERROR", TSIZE);
    } else {
      strncpy(token_p, "NULL", TSIZE);
    }
  }
  free(temp);
  //return token_p;
}

/**
* This method removes all white space, tab, and newline characters from the
* given string and returns the string without those characters. It then
* modifies the original char_p parameter passed in through pointers.
* @param string - A string to remove all delimiters from.
*/
void squeeze_together(char* char_p) {
  int i;
  int j = 0;
  //Allocate a temporary string to hold the new string.
  char *temp = calloc(LINE, sizeof(char));

  //Iterate through the array and remove all tabs, white space, and newline
  //characters.
  for (i = 0; i < strlen(char_p); i++) {
    if (char_p[i] != '\t'  && char_p[i] != ' '  && char_p[i] != '\n'
    && char_p[i]  != '\0'  && char_p[i] != '\v' && char_p[i] != '\b'
    && char_p[i]  != '\f'  && char_p[i] != '\r' && char_p[i] != '\a') {
      temp[j++] = char_p[i];
    }
  }
  //Copy the new temporary line to the parameter passed in.
  strncpy(char_p, temp, LINE);

  //Since the temporary string was dynamically allocated, we will free it.
  free(temp);
}

/**
* This method modifies the global line string to contain only the string
* after the token.
* @param token_p - A pointer to a string containing the token.
**/
void string_after_token(char *token_p) {
  int i;
  if (strcmp(token_p, "") != 0) {
    line = strstr(line, token_p);
    for (i = 0; i < strlen(token_p); i++)
    line++;
  }
}
