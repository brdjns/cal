# Cal
Cal is a portable, command-line interface, desk calculator.

Cal supports basic arithmetic using `+`, `-`, `/`, `% ` and `*`.  It also
supports factorials, with the `!` operator, and exponentiation, with the `^`
operator.  Cal supports mathematical functions as rvalues, and variable
assignment and declaration.  Internally, Cal uses double-precision
floating-point arithmetic for most operations, and integer arithmetic
for operations that are defined only for integers.

## Operators
Operator precedence is as follows:
```
-------------------------------------------------------------------------------
 priority       operator        description                      associativity
-------------------------------------------------------------------------------
 1              +a -a           unary plus and minus                      left
                a()             function call
                (a)             bracketed expression
                {a}
                [a]
                a!              factorial
                |a|             absolute value
-------------------------------------------------------------------------------
 2              a!              factorial                                right
                a^b             exponentiation
-------------------------------------------------------------------------------
 3              a*b a/b a%b     multiplication, division, remainder       left

 4              a+b a-b         addition, subtraction
-------------------------------------------------------------------------------
 5              a=b             assignment, initialisation               right
-------------------------------------------------------------------------------
```

## Functions
Cal implements the following mathematical functions:
```
sqrt( expression )    -- returns the square root of expression
```

## Predefined constants
The following constants are predefined:
```
E           2.71828182845904523536    e (Euler's number)
LOG2E       1.44269504088896340736    log2(e)
LOG10E      0.434294481903251827651   log10(e)
LN2         0.693147180559945309417   ln(2)
LN10        2.30258509299404568402    ln(10)
PI          3.14159265358979323846    π
PI_2        1.57079632679489661923    π÷2
PI_4        0.785398163397448309616   π÷4
SQRT2       1.41421356237309504880    √2
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
    statement = 
          expression ";" .

    expression =
          declaration
        | term
        | expression "+" term
        | expression "-" term .

    declaration =
          "let" "=" expression
          "set" "=" expression .

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
        | "|" expression "|"
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
