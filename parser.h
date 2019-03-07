#ifndef PARSER_H
#define PARSER_H
#define ERROR -999999
/*
 * Author:  William Kreahling and Mark Holliday and Kevin Filanowski
 * Purpose: Function Prototypes for parser.c
 * Date:    Modified 9-26-08, 3-25-15, 04-08-18
 */
int bexpr (char *);
int expr  (char *);
int term  (char *);
int ttail (char *, int);
int stmt  (char *);
int stail (char *, int);
int factor(char *);
int ftail (char *, int);
int expp  (char *);

void add_sub_tok(char *token);
void mul_div_tok(char *token);
void compare_tok(char *token);
void expon_tok  (char *token);
int num         (char *token);
int is_number   (char *token);

#endif
