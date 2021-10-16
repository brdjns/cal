# Cal
Cal is a portable, command-line interface, desk calculator.

Cal supports basic arithmetic using `+`, `-`, `/`, `% ` and `*`.  It also
supports factorials, with the `!` operator.

## Building
Cal uses CMake as its build system. To build:
```
    cmake -S . -B build
    cmake --build build
```
## Grammar
The Wirth syntax notation grammar is as follows:
```
    declaration =
          "let" "=" expression .

    expression =
          declaration
        | term
        | expression "+" term
        | expression "-" term .

    term = 
          postfix-expression
        | term "*" factor
        | term "/" factor
        | term "%" factor .

    postfix-expression = 
          factor
        | postfix-expression "!" .
        | postfix-expression "^" factor .

    factor =
          number
        | "-" number
        | "+" number
        | "(" expression ")"
        | "[" expression "]"
        | "{" expression "}" .

    number =
          floating-point-literal .

    floating-point-literal =
          real
        | real exponent
        | integer exponent .

    exponent =
          "e" "+" integer
        | "e" "-" integer
        | "e" integer
        | "E" "+" integer
        | "E" "-" integer
        | "E" integer .
    
    integer =
          digit
        | digit integer .

    real =
          "." integer
        | integer "."
        | integer "." integer .
```
