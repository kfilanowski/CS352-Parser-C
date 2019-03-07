/**
 * Interpreter.c
 * This program reads from an input file, sends each line through a
 * lexical analyzer and a parser, and prints the results of the errors or the
 * complete evaluation to an output file.
 * NOTE: The terms 'token' and 'lexeme' are used interchangeably in this
 *       program.
 *
 * USAGE: interpreter input_file.txt output_file.txt
 *
 * @author Kevin Filanowski
 * @version April 8, 2018
 */

 /** Required Libraries **/
#include "tokenizer.h"
#include "tokenizer.c"
#include "parser.h"
#include "parser.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/** Global variable from tokenizer.c. Contains each line in the file. **/
extern char * line;

/**
 * The main function of the program. Interpreter.c
 * Takes command line input of a text file and sends each sentence
 * through a lexical analyzer and a parser to check for errors and evaluate
 * the total. It then prints the results to another file.
 * USAGE: interpreter input_file.txt output_file.txt
 */
int main(int argc, char* argv[]) {
  char temp_holder[LINE];/* Holds input_line temporarily                     */
  char input_line[LINE]; /* Line of input, fixed size                        */
  FILE *in_file  = NULL; /* input file pointer                               */
  FILE *out_file = NULL; /* output file pointer                              */
  int lexical_error;     /* FALSE if no lexical errors found, TRUE otherwise */
  int value;             /* end total value of the statement per line        */

  if (argc != 3) {
    printf("Usage: tokenizer inputFile outputFile\n");
    exit(1);
  }

  in_file = fopen(argv[1], "r");
  if (in_file == NULL) {
    fprintf(stderr, "ERROR: could not open %s for reading\n", argv[1]);
    exit(1);
  }

  out_file = fopen(argv[2], "w");
  if (out_file == NULL) {
    fprintf(stderr, "ERROR: could not open %s for writing\n", argv[2]);
    exit(1);
  }

  while (fgets(input_line, LINE, in_file) != NULL) {
    //If the input_line is empty, then skip it.
    strncpy(temp_holder, input_line, LINE);
    squeeze_together(input_line);
    if (!strncmp(input_line, "", LINE))
      continue;

    //Line is not empty, continue.
    strncpy(input_line, temp_holder, LINE);
    line = input_line;

    //Write line to file.
    fprintf(out_file, "%s", line);

    //Find and print lexical errors in the current line.
    lexical_error = tokenizer(out_file);

    line = input_line;

    //Look for syntaxtical errors.
    if (lexical_error == FALSE) {
      value = bexpr(line);

      //Print if there were no errors!
      if (value != -999999)
        fprintf(out_file, "Syntax OK\nValue is %d\n\n", value);
      else {
        if (strlen(line) > 2)
          fprintf(out_file, "===> '%s' expected\nSyntax Error\n\n", line);
        else
          fprintf(out_file, "===> '%c' expected\nSyntax Error\n\n", *line);
      }
    }
  }
  fclose(out_file);
  fclose(in_file);
  return 0;
}
