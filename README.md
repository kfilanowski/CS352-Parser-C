# Author
Kevin Filanowski

# Version
04/08/18 - 1.0.0

# Table of Contents
* [Description](#description)
* [Contents](#contents)
* [Compiling](#compiling)
* [Usage](#usage)
* [Input File](#input-file)

# Description
This program will take a file as input, where the text file contains mathematical sentences. Each sentence is separated on its own line, and ended with a semicolon. It will be sent through a lexical analyzer and a parser to find any errors and evaluate the sentence. The results will be written to another file. It is fairly flexible, ignoring spaces, new lines, and other escape characters. 

# Contents
interpreter.c
  * The main driver of the program. This is the one to call when the program is to be run.
  
tokenizer.c
* The lexical analyzer. This is one of the tests that will be run on the input file.

tokenizer.h
* The header file containing the outline of the constants and functions used in tokenizer.c.

parser.c
* The parser. This parser will analyze the semantics of the sentence and output any errors. If there are no errors, the evaluated total will be output into a file.

parser.h
* The header file containing the outline of the constants and functions used in parser.c.

input.txt
* An example of possible inputs, with a mix and match of sentences that will work and fail to help visualize what the program does. 

output.txt
* An example of the output after running the program and feeding it input.txt. This helps visualize what the program does. 

# Compiling
To compile the program, ensure that the five files, interpreter.c, tokenizer.c, tokenizer.h, parser.c, and parser.h are all in the same directory. Then run the following command to compile it to an executable named interpreter:

```gcc -Wall interpreter.c -o interpreter -lm```

where `-Wall` displays extra warnings if any, and `-o` names the executable.
`-lm` links the math library. There should be no errors or warnings.

# Usage
`interpreter input_file.txt output_file.txt`
or
`./interpreter input_file.txt output_file.txt`

Where input_file.txt is a file containing the sentences and such and output_file.txt will be the result.

# Input File
Each statement should be on its own line in the file, ended with a semicolon (;). 
A line can be a maximum of 100 characters, and a lexeme can be a maximum of 20. Any errors will show up in the output file explaining what is missing or what was expected. Spaces and tabs and certain various escape characters are okay to put between the sentence.
See input.txt for an example.
NOTE: The last line of the input_file.txt MUST be blank!
