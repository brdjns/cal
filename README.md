# Cal
Cal is a portable, command-line interface, desk calculator.

Cal supports basic arithmetic using `+`, `-`, `/`, `% ` and `*`.  It also
supports factorials, with the `!` operator, and exponentiation, with the `^`
operator.  Cal supports mathematical functions as rvalues.

## Functions
```
sqrt( expression )    -- takes the square root of a number
```

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
          power-expression
        | term "*" factor
        | term "/" factor
        | term "%" factor .

    power-expression = 
          factor
        | power-expression "!" .
        | power-expression "^" factor .

    factor =
          number
        | "-" number
        | "+" number
        | "(" expression ")"
        | "[" expression "]"
        | "{" expression "}"
        | "sqrt" "(" expression ")" .

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
