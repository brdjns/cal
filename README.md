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
sqrt( expression )    # return the square root of expression
```

## Reserved words
```
let         # initialise
set         # assign
sqrt()      # square root
exit        # exit cal
quit        # exit cal
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

## Usage
All statements are terminated by `;`.  The grouping operators `[]`, `{}`, 
and `()` all mean the same thing and exist as a way to easily distinguish
complex expression nesting.  Expressions can span multiple lines.  An
expression will not be evaluated until the statement terminator `;` and
`RET` are typed.
```
> let x = 2.5;                    # initialise x with 2.5
2.5

> set x = 8.3;                    # assign 8.3 to x
8.3

> x;                              # print x's value
8.3

> 2 * (x + x) / 5;                # compute an expression
2

> 8 * [45 / {5 - (4 + 2)
+ 8} + 0.2];                      # compute a nested expression
53.02857142857143202        

> -2;                             # unary operators work as expected
-2

> +2;
2

> |-x|                            # the absolute value of -x
2.5

> 2 / 0;                          # division by zero is undefined
error: division by zero

> 5 % 0;
error: modulo division by zero

> let y = sqrt(x);                # initialise y with the square root of x
1.581138830084189761

> 8!;                             # compute the factorial of 8
40320

> -8!;                            # can't take negative factorials
error: domain error

exit                              # quits cal; note: quit is a synonym
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
