// cal.cc: cal, a portable desk calculator.
// SPDX-FileCopyrightText: © 2021 Bradley M. Jones <brdjns@gmx.us>
// SPDX-License-Identifier: MIT

#include "cal.h"
#include "functions.h"

// @class Token
// @brief A token class.
// @details Represents a token that has a kind and a value.
class Token {
public:
    char kind;        // a token kind
    double value;     // a token value
    std::string name; // an identifier name

    // @brief Construct a token from a character.
    // @param[in] ch a kind
    Token(char ch) : kind{ch}, value{0} {}

    // @brief Construct a token from a character and value.
    // @param[in] ch a kind
    // @param[in] val a value
    Token(char ch, double val) : kind{ch}, value{val} {}

    // @brief Construct a token from a character and name.
    // @param[in] ch a kind
    // @param[in] n an identifier
    Token(char ch, std::string id) : kind{ch}, name{id} {}
};

// @class Token_stream
// @brief A token stream class.
// @details Converts characters into tokens.
class Token_stream {
public:
    // @brief Construct a stream of tokens that reads from the standard input.
    Token_stream() : full{false}, buffer{0} {}

    // @brief Fetch a token from the standard input.
    Token get();

    // @brief Put a token back into the token stream.
    // @param t A token
    // @pre There is no token in the buffer.
    // @post The buffer is full.
    void putback(Token t)
    {
        buffer = t;
        full = true;
    }

    // @brief Discard characters up to and including a c.
    // @param ch character to ignore
    void ignore(char c);

private:
    bool full;    // True when the token buffer is full
    Token buffer; // A buffer of tokens
};

// Recognised scanner symbols.
enum Symbol {
    // lists
    comma = ',',

    // bracketing
    lparen = '(',
    rparen = ')',
    lbrace = '{',
    rbrace = '}',
    lbrack = '[',
    rbrack = ']',

    // arithmetic operators
    star = '*',
    solidus = '/',
    percent = '%',
    plus = '+',
    minus = '-',
    bang = '!',
    caret = '^',

    // assignment
    equals = '=',

    // keywords
    let = 'L',
    quit = 'Q',
    number = '#',
    ident = '@',
    f_sqrt = 'S',
    f_abs = 'A',

    // non-printing
    eof = '\0',

    // other
    print = ';',
    dot = '.',
};

// Keywords.
const std::string kw_decl = "let";
const std::string kw_quit = "quit";
const std::string kw_sqrt = "sqrt";
const std::string kw_abs = "abs";

// @brief Fetch a token from the standard input.
// @pre An ASCII character.
// @post Return a token.
// @throws runtime_error if token is not alphanumeric or an operator.
Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }

    char ch{};
    std::cin >> ch;

    switch (ch) {
    case print:
    case lparen:
    case rparen:
    case lbrace:
    case rbrace:
    case lbrack:
    case rbrack:
    case star:
    case solidus:
    case percent:
    case plus:
    case minus:
    case bang:
    case equals:
    case caret:
    case comma:
        return Token{ch};
    case dot:
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        std::cin.unget();
        double value{};
        std::cin >> value;
        return Token{number, value};
    }
    case eof: // ^Z
        return Token{quit};
    default: // idenitifers
        if (std::isalpha(ch)) {
            std::string str;
            str += ch;
            while (std::cin.get(ch) &&
                   (std::isalpha(ch) || ch == '_' || std::isdigit(ch))) {
                str += ch;
            }
            std::cin.putback(ch);
            if (str == kw_decl) {
                return Token{let};
            }
            if (str == kw_quit) {
                return Token{quit};
            }
            if (str == kw_sqrt) {
                return Token{f_sqrt};
            }
            if (str == kw_abs) {
                return Token{f_abs};
            }
            return Token{ident, str};
        }
        cal::error("invalid token");
    }
    return Token{'?'}; // never reached
}

// @brief Discard characters up to and including a c.
// @param c a character to ignore
void Token_stream::ignore(char c)
{
    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    char ch{};
    while (std::cin >> ch) {
        if (ch == c) {
            return;
        }
    }
}

// @class Variable
// @brief A variable type.
class Variable {
public:
    std::string name; // a variable identifier
    double value;     // a variable value

    // @brief Construct a variable with a name and value.
    // @param[in] id a variable identifier
    // @param[in] v a variable value
    Variable(std::string id, double v) : name{id}, value{v} {}
};

// A symbol table of variables.
std::vector<Variable> symtab;

// Retrieve a variable's value.
double get_value(std::string variable)
{
    for (Variable& i : symtab) {
        if (i.name == variable) {
            return i.value;
        }
    }
    cal::error("get: undefined identifier ", variable);
    return 1;
}

// Bind a value to a variable.
void set_value(std::string variable, double value)
{
    for (Variable& i : symtab) {
        if (i.name == variable) {
            i.value = value;
            return;
        }
    }
    cal::error("set: undefined identifier ", variable);
}

// Determine if the specified variable is declared.
bool is_declared(std::string variable)
{
    for (Variable& i : symtab) {
        if (i.name == variable) {
            return true;
        }
    }
    return false;
}

double expression();
Token_stream ts;

// Construct a factor.
double factor()
{
    Token t{ts.get()};

    switch (t.kind) {
    case lparen:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rparen) {
            cal::error("closing ')' missing");
        }
        return temp;
    }
    case lbrace:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrace) {
            cal::error("closing '}' missing");
        }
        return temp;
    }
    case lbrack:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrack) {
            cal::error("closing ']' missing");
        }
        return temp;
    }
    case f_sqrt: // sqrt
    {
        t = ts.get();
        if (t.kind != lparen) {
            cal::error("opening '(' missing for sqrt");
        }
        double temp{expression()};
        if (temp < 0) {
            cal::error("domain error");
        }
        t = ts.get();
        if (t.kind != rparen) {
            cal::error("closing ')' missing for sqrt");
        }
        return std::sqrt(temp);
    }
    case f_abs: // abs
    {
        t = ts.get();
        if (t.kind != lparen) {
            cal::error("opening '(' missing for abs");
        }
        double temp{expression()};
        t = ts.get();
        if (t.kind != rparen) {
            cal::error("closing ')' missing for abs");
        }
        return std::abs(temp);
    }
    // -NUM.
    case minus:
        return -factor();
    // +NUM.
    case plus:
        return factor();
    case number:
        return t.value;
    case ident:
        return get_value(t.name);
    default:
        cal::error("factor expected");
    }

    return 0; // never reached
}

// Construct a postfix expression.
double power_expression()
{
    double left{factor()};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case bang:
        {
            int temp = cal::narrow_cast<int>(left);
            if (temp < 0) {
                cal::error("factorial of a negative number");
            }
            return left = cal::factorial(temp);
            break;
        }
        case caret:
        {
            double temp{factor()};
            return left = std::pow(left, temp);
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Construct a term.
double term()
{
    double left{power_expression()};

    while (true) {
        Token t{ts.get()};
        switch (t.kind) {
        case star:
            left *= power_expression();
            break;
        case solidus:
        {
            double temp{power_expression()};
            if (temp == 0) {
                cal::error("division by zero");
            }
            left /= temp;
            break;
        }
        case percent:
        {
            double temp{power_expression()};
            if (temp == 0) {
                cal::error("modulo division by zero");
            }
            left = std::fmod(left, temp);
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Construct an expression.
double expression()
{
    double left{term()};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case plus:
            left += term();
            t = ts.get();
            break;
        case minus:
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Add a variable to the symbol table.
double define_name(std::string variable, double value)
{
    if (is_declared(variable)) {
        cal::error("variable declared twice");
    }
    symtab.push_back(Variable{variable, value});
    return value;
}

// Declare a variable.
double declaration()
{
    Token t{ts.get()};
    if (t.kind != ident) {
        cal::error("identifier expected in declaration");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != equals) {
        cal::error("'=' missing in declaration");
    }

    double value{expression()};
    define_name(name, value);
    return value;
}

// Turn a declaration into a statement.
double statement()
{
    Token t{ts.get()};
    switch (t.kind) {
    case let:
        return declaration();
    default:
        ts.putback(t);
        return expression();
    }
}

// Clean up remaining tokens during an exception.
void clean_up_mess()
{
    ts.ignore(print);
}

// Compute an expression.
void calculate()
{
    // Get the greatest available precision from a double.
    std::cout.precision(std::numeric_limits<double>::max_digits10 + 2);

    const std::string prompt = "> ";

    while (true) try {
            std::cout << prompt;
            Token t = ts.get();
            while (t.kind == print) { // discard all 'print' tokens
                t = ts.get();
            }
            if (t.kind == quit) {
                return;
            }
            ts.putback(t);
            std::cout << statement() << '\n';
        }
        catch (std::runtime_error& e) {
            std::cerr << "error: " << e.what() << '\n';
            clean_up_mess();
        }
}

int main()
try {
    // Predefined constants.
    define_name("PI", 3.1415926535);
    define_name("E", 2.7182818284);

    calculate();
    return EXIT_SUCCESS;
}
catch (std::exception& e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
}
catch (...) {
    std::cerr << "exception\n";
    return EXIT_FAILURE;
}
