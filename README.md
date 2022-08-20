# C--, like C but a bit worse
This project is a compiler for a toy language called c--, as the name suggests it is similar to C but a bit simplified. My plan is to iteratively add new features, but is very unlike that it ever goes from c-- to full c89 (or newer).

**Note: This is strictly a hobby project and is not built to be used for production purposes**

## Formal Grammar specification
The language grammar expressed in EBNF:
```
// Functions
function -> prototype expr
prototype -> int identifier '(' (int identifier)* ')'
extern -> 'extern' prototype

// Expressions
expr -> primary_expr bin_expr
primary_expr -> identifier_expr | number_expr | parenthesis_expr 
number_expr -> NUMBER
parenthesis_expr -> '(' expr ')'
identifier_expr ->  identifier // variable
                    | identifier '(' expr* ')' // func call with comma separated arguments

// Binary expressions
bin_expr -> (bin_op primary_expr)* // left recursive
bin_op -> '+' | '-' | '*' | '<'

```
## Implementation details
### Lexer
...
### Parser
Handwritten recursive decent parser

### Backend
Is currently planned to be written in llvm.

## TODO list:
* Working compiler :-)
* Up-to-date readme
* Re-introduce handwritten parser and lexer