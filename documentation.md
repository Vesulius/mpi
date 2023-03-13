Vesa Louhelainen 13.3.2023

Project github: https://github.com/Vesulius/mpi

# Instructions

The interpreter is compiled using **g++** compiler (version 11.3.0). There are two settings for the compilation: normal and debug mode.

**Default compilation:**
``` g++ ./*.cpp -o mpi ```
This should produce regular executable binary file named **mpi**. 

**Debug compilation:**
``` g++ ./*.cpp -D DEBUG -o mpi ```
This should produce regular executable binary file named **mpi**.  Debug mode prints additional info about the interpreter such as tokens and abstract syntax tree when run.

**RUN:**  
``` ./mpi YOUR_MINIPASCAL_SOURCE_FILE ```


# Documentation

The project is written in c++ using some of the standard library features.

## Achictecture

The interpreter is made up of three parts: scanner, parser, and runner. Running these is handled by main class.

Scanner reads the source and attempts to find tokens. Scanner is implemented as a class .

Parser requests tokens from scanner and creates abstract syntax tree nodes based on them. Parser is implemented using the visitor pattern, where for each token encountered, specific function is called depending on the context.

Runner accepts abstract syntax tree input and runs it. Runner is also responsible for creation and curation of symbol table and scoping. Runner is implemented using visitor pattern, where each node has their own function. Each node then calls the specific function for all of their children.

There is one additional printVisitor part that is responsible for printing the abstract syntax tree. It is only run for the debugger mode.


## Language definition

Language definition follows quite closely to the minipascal language definition.

**Variables** are one of three types: string, int or bool. All variables have value when initialized. Strings are empty string "", ints are 0, bools are true.

**Scopes**: The language supports scope up to depth of five. The first scope is "zero" scope and scope is moved one up every time an if- or for statement is encountered. Each scope has their own symbol table. If variable is declared inside a scope, it will be assigned to the current symbol table. If variable is used but not found in the current scope, all scopes below the current scope are searched.

```
var i : int := 3;
if true do
	var i : int := 1;
	if true do
	    print i;
	    i := i + 1;
	end if;
	print i;
end if;
print i;

/*
prints:
1
2
3
*/
```

When current scope is exited, the scope is cleared:

```
var i : int := 1;
if true do
	var i : int := 2;
end if;
if true do
	print i; // i is found from the global scope, not from the previous if definition
end if;

/*
prints:
1
*/
```

**For loops** are always in specified range. Ranges can be defined with variables:

```
var i : int;
for i in 0..2 do
	print i;
end for;

/*
prints:
0
1
2
*/

var a : int := -2;
var i : int;
for i in a..(a+2) do
	print i;
end for;

/*
prints:
-2
-2
0
*/
```

**Comments** can be single line **//** or multiline /* \*/. Multiline comments can be nested and single line comments can appear inside multiline comments.

```
/* comment /*
comment
// very much a comment
*/*/
```

**Negative numbers** can be created by adding **-** before any int literal value. This implementation is still quite clunky as braces are not required.

```
var i : int := -1 + 1;   // i = 0
var i : int := 1 -(-1);  // i = 0
var i : int := 1 - -1;   // i = 0
```

#### Context-free grammar:

Items written **bold** are terminals.

program → statemetnt-list **endfile**
statement-list → statement statement-list | **null**
statement→ **id :=** expression **;** | **read** **id** **;** | **write** expression **;** | **declare id declare-type type ;** 
					 **if** expression **do** statement-list **end if** **;** | **for id in** expression **..** expression **do** statement-list **end for** **;**
expression → term term-tail | **!** term term-tail
term-tail → add-op term term-tail | **null**
term → factor factor-tail
factor-tail → mult-op factor factor-tail | **null**
factor → **(** expr **)** | **id** | **literal**
add-op → **+** | **-** | **&** | **=** | **<** | **>**
mult-op → **\*** | **/**


#### Abstact syntax tree

Abstract syntax tree follows closely from the context-free grammar. All nonterminal symbols are their own nodes that have both the terminals and nonterminals as children. Definition of each node can be seen from node.h file. If running in debug mode the entire AST will be printed to console.

#### Error handling

The interpreter has several error messages that differ depending on when the error is encountered. Scanner and parser will continue if error is encountered but the runner is only run if no error is found.

**Scanner** will print a syntax error if in case of bad keyword is encountered. There are only few keywords that can cause a syntax error. Mostly mishandled comments or string literals missing closing tags.

**Parser** catches most of the errors. Most commonly missing keyword. If error is encountered parser will print parsing error and skips to the next found statement.

**Runner** is responsible of type checking. If error is found a runtimer error is printed and execution is immediately stopped.


#### Workhours

Hard to count accurately as most of this project was learning to use c++ but here is some estimates:

18.2.2022: 1h initilizing project
19.2.2022: 3h basic scanner
19.2.2022: 2h c++ stuff
20.2.2022: 2h parsing
24.2.2022: 3h still parsing
28.2.2022: 5h expressions and printvisitor
2.3.2022: 2h added brackets and some stuff
4.3.2022: 4h added read and print and moved scanner to class
5.3.2022: 2h added simple typechecker
6.3.2022: 1h var declare node
8.3.2022: 3h spitted header to multiple files and first symbol table
9.3.2022: 1h general fixes
10.3.2022: 4h first runner version and read node
11.3.2022: 5h if and fore statements and more operators
12.3.2022: 8h scopes lots of general fixes
13.3.2022: 8h added debug mode and more fixes

Total: 53h


