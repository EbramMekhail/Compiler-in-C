A lexical analyzer, parser, and interpreter are all components of a compiler, which is a program that translates source code written in a high-level programming language into machine code that can be executed by a computer.

A lexical analyzer, also known as a lexer or tokenizer, reads the source code and breaks it down into smaller units called tokens. These tokens represent the basic building blocks of the language, such as keywords, variables, and operators. The lexer ignores whitespace and comments, and it may also perform some basic transformations on the tokens, such as converting all keywords to uppercase.

A parser analyzes the sequence of tokens produced by the lexer and checks to see if it follows the grammatical rules of the language. The parser builds a tree-like structure called a parse tree or syntax tree that represents the structure of the source code. The parse tree can be used to check for errors in the source code, such as missing or mismatched parentheses or undeclared variables.

An interpreter is a program that executes the source code directly, rather than compiling it into machine code. The interpreter reads the source code, parses it, and then executes the resulting instructions one at a time. This allows the interpreter to provide immediate feedback on the source code, but it is generally slower than compiling the code into machine code and running it.

In summary, a lexical analyzer breaks down the source code into tokens, a parser checks the structure of the source code, and an interpreter executes the source code. Together, these components form the heart of a compiler, which translates source code into machine code that can be run on a computer.

To run the program you first run the make-file by typing `make` in your terminal then to write a test case do the following in your terminal `make run filename=test.txt`. For example `make run filename=Test_Cases/cleanprog`.
Finally to clean up after looking at the program run `make clean`.

In addition in the Test_cases directory you can see the inputs as well as the correct outputs.
