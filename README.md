# My Lang
Short about section

## Formal Grammar specification
The language grammar expressed in EBNF:
```
// Program structure && functions
program             -> functionDefinition+ //empty programs considered invalid
functionDefinition  -> "def" IDENTIFIER "(" ")" "{" statement* "}"

// Statements
statement           -> declaration | assignment
declaration         -> "var" IDENTIFIER ["=" expr] ";"
assignment          -> IDENTIFIER "=" expr ";"

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
...