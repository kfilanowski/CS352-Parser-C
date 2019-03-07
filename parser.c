/**
 * parser.c - recursive descent parser for a simple expression language.
 *            Most of the functions in this file model a Non-terminal in the
 *            grammar listed below
 * NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
 *       program.
 * Author: William Kreahling and Mark Holliday and Kevin Filanowski
 * Date:   Modified 9-29-08 and 3-25-15 and 04-08-18
 */

 /** Required Libraries **/
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "parser.h"
#include "tokenizer.h"

/** Global variable containing each line of the input file. **/
extern char * line;

/**
 * <bexpr> ::= <expr> ;
 * <expr> ::=  <term> <ttail>
 * <ttail> ::=  <add_sub_tok> <term> <ttail> | e
 * <term> ::=  <stmt> <stail>
 * <stail> ::=  <mult_div_tok> <stmt> <stail> | e
 * <stmt> ::=  <factor> <ftail>
 * <ftail> ::=  <compare_tok> <factor> <ftail> | e
 * <factor> ::=  <expp> ^ <factor> | <expp>
 * <expp> ::=  ( <expr> ) | <num>
 * <add_sub_tok> ::=  + | -
 * <mul_div_tok> ::=  * | /
 * <compare_tok> ::=  < | > | <= | >= | ! = | ==
 * <num> ::=  {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
 */

/**
 * <bexpr> -> <expr>
 * Begins and ends the parse tree.
 * @param {char *} line_to_parse - A line from the input file to be parsed.
 * @return {int} - The total computed value, or ERROR.
 */
int bexpr(char *line_to_parse) {
  char token[TSIZE];
  int result;

  get_token(token);
  result = expr(token);

/** Added support for checking invalid left parenthesis **/
  if (!strncmp(token, ")", 1)) {
    strncpy(line, "(", LINE);
    return ERROR;
  } else if (strncmp(token, ";", 1) != 0) {
    strncpy(line, ";", LINE);
    return ERROR;
  } else
    return result;
}

/**
 * <expr> -> <term> <ttail>
 * The start of a new expression. Calls term and ttail.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} - The total computed value, or ERROR.
 */
int expr(char *token) {
  int subtotal = term(token);

  //Return ERROR if subtotal is not a number, otherwise return a call to ttail.
  if (subtotal == ERROR)
    return subtotal;
  else
    return ttail(token, subtotal);
}

/**
 * <ttail> -> <add_sub_tok> <term> <ttail> | e
 * Checks and evaluates addition and subtraction.
 * @param {char *} token - The current lexeme in the sentence.
 * @param {int} subtotal - The current subtotal.
 * @return {int}
 */
int ttail(char *token, int subtotal) {
  int term_value;

  //Check if the token is an add operator.
  if (!strncmp(token, "+", 1)) {
    add_sub_tok(token);
    term_value = term(token);

    //If term_value is not a number, return an error. Otherwise return ttail.
    if (term_value == ERROR)
      return term_value;
    else
      return ttail(token, (subtotal + term_value));
      //Check if the token is a subtract operator.
  } else if(!strncmp(token, "-", 1)) {
      add_sub_tok(token);
      term_value = term(token);

      //If term_value is not a number, return an error. Otherwise return ttail.
      if (term_value == ERROR)
        return term_value;
      else
        return ttail(token, (subtotal - term_value));
  } else
    return subtotal;
}

/**
 * <term> -> <stmt> <stail>
 * Grabs the current int literal if there is one, with possible calculations
 * that have been made to it.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} - A singular value used in the calculation, or an error.
 */
int term(char *token) {
  int val = stmt(token);

  if (val == ERROR)
    return val;
  else
    return stail(token, val);
  }

/**
 * <stail> -> <mult_div_tok> <stmt> <stail> | e
 * Searches for multiply or divide tokens and applies them.
 * @param {char *} token - The current lexeme in the sentence.
 * @param {int} subtotal - The current subtotal.
 * @return {int} - The subtotal with possible operations completed on it.
 */
int stail(char *token, int subtotal) {
  int term_value;

  //Searches for the multiplication operator.
  if (!strncmp(token, "*", 1)) {
     mul_div_tok(token);
     term_value = term(token);

     //If term_value is not a number, return an error. Otherwise return stail.
     if (term_value == ERROR)
       return term_value;
     else
        return stail(token, (subtotal * term_value));
  } else if(!strncmp(token, "/", 1)) {
     mul_div_tok(token);
     term_value = term(token);

     //If term_value is not a number, return an error. Otherwise return stail.
     if (term_value == ERROR)
       return term_value;
     else
       return stail(token, (subtotal / term_value));
  } else
     return subtotal;
}

/**
 * <stmt> -> <factor> <ftail>
 * A statement, gets a value and returns it to ftail.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} - A value with any compare operators completed upon it.
 */
int stmt(char *token) {
  int value = factor(token);
  return ftail(token, value);
}

/**
 * <ftail> -> <compare_tok> <factor> <ftail> | e
 * Searches for and completes any compare tokens. Then calls factor and ftail.
 * @param {char *} token - The current lexeme in the sentence.
 * @para {int} subtotal - The value obtained by factor in the function stmt.
 * @return {int} - A value with any compare operators completed upon it.
 */
int ftail(char *token, int subtotal) {
  int val;

  //Check for less than operator
  if (!strncmp(token, "<", 1)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal < val);
  }

  //Check for greater than operator
  if (!strncmp(token, ">", 1)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal > val);
  }

  //Check for less than or equal to operator
  if (!strncmp(token, "<=", 2)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal <= val);
  }

  //Check for greater than or equal to operator
  if (!strncmp(token, ">=", 2)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal >= val);
  }

  //Check for not equal to operator
  if (!strncmp(token, "!=", 2)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal != val);
  }

  //Check for equal to operator
  if (!strncmp(token, "==", 2)) {
    get_token(token);
    val = factor(token);

    if (val == ERROR)
      return val;
    else
      return ftail(token, subtotal == val);

  }
  return subtotal;
}

/**
 * <factor> -> <expp> ^ <factor> | <expp>
 * With left factoring applied, we have:
 * <factor> -> <expp> { ^ <factor> }
 * This method calls expp, and searches for exponentials to apply.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} - The subtotal with possible exponential operators applied.
 */
int factor(char *token) {
  int factor_value;
  int subtotal = expp(token);

  //If subtotal is not a number, then error. Otherwise, check for exponents.
  if (subtotal == ERROR)
    return subtotal;
  else {
    expon_tok(token);

    //Check for an exponent.
    if (!strncmp(token, "^", 1)) {
      get_token(token);
      factor_value = factor(token);

      if (factor_value == ERROR)
        return factor_value;
      else
        return pow(subtotal,factor_value);

    } else
      return subtotal;
  }
  return expp(token);
}

/**
 * <expp> -> ( <expr> ) | <num>
 * Deals with parenthesis operations, or returns a number.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} - A subtotal with another expression acted upon it, or just
 * an int literal.
 */
int expp(char *token) {
  int value;

  //Check for left parenthesis
  if (!strncmp(token, "(", 1)) {
    get_token(token);
    value = expr(token);

    //Check for the following right parenthesis
    if (!strncmp(token, ")", 1))
      return value;
    else {
      //If no right parenthesis was found, throw an error.
      strncpy(line, ")", 1);
      return ERROR;
    }

    //Check for right parenthesis without a left parenthesis to match it.
  } else if (!strncmp(token, ")", 1)) {
    strncpy(line, "(", 1);
    return ERROR;
  }
  return num(token);
}

/**
 * <add_sub_tok> -> + | -
 * Iterates to the next token when an add or subtract token is found.
 * @param {char *} token - The current lexeme in the sentence.
 */
void add_sub_tok(char *token) {
  get_token(token);
}

/**
 * <mul_div_tok> -> * | /
 * Iterates to the next token when a multiply or divide token is found.
 * @param {char *} token - The current lexeme in the sentence.
 */
void mul_div_tok(char *token) {
  get_token(token);
}

/**
 * <compare_tok> -> < | > | <= | >= | ! = | ==
 * Iterates to the next token when a relational operator is found.
 * @param {char *} token - The current lexeme in the sentence.
 */
void compare_tok(char *token) {
  get_token(token);
}

/**
* <num> -> {0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9}+
* Returns a number if it is valid, error otherwise.
* @param {char *} token - The current lexeme in the sentence.
* @return {int} - An int_literal lexeme.
*/
int num(char *token) {
  if (is_number(token))
    return atoi(token);
  else {
    //We expected an int_literal here, so this will be the error.
    strncpy(line, INT_LITERAL, LINE);
    strncpy(token, ";", 1);
    return ERROR;
  }
}

/**
 * <expon_tok> -> ^
 * Iterates to the next token when an exponential token is found.
 * @param {char *} token - The current lexeme in the sentence.
 */
void expon_tok(char *token) {
  get_token(token);
}

/**
 * This helper method checks the true nature of a number to ensure it is
 *  actually a number and not an error.
 * @param {char *} token - The current lexeme in the sentence.
 * @return {int} the comparison boolean logic result.
 */
int is_number(char *token) {
  int cmp1, cmp2;

  //Since the atoi() function returns 0 when the token is anything but an
  //integer, we don't know if it is actually the integer 0 or an error. Hence
  //we must provide extra checks on this.
  cmp1 = atoi(token);

  if (cmp1 != 0)
    return TRUE;
  else {
    cmp2 = strncmp(token, "0", 1);
    return cmp1 == cmp2;
  }
}
