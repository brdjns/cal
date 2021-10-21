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
    // @param[in] ch a kind.
    Token(char ch) : kind{ch}, value{0} {}

    // @brief Construct a token from a character and value.
    // @param[in] ch a kind.
    // @param[in] val a value.
    Token(char ch, double val) : kind{ch}, value{val} {}

    // @brief Construct a token from a character and name.
    // @param[in] ch a kind.
    // @param[in] n an identifier.
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
    // @param[in] t a token.
    void putback(Token t)
    {
        buffer = t;
        full = true;
    }

    // @brief Discard characters up to and including a c.
    // @param[in] c character to ignore.
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
    bar = '|',

    // arithmetic operators
    star = '*',
    slash = '/',
    percent = '%',
    plus = '+',
    minus = '-',
    bang = '!',
    caret = '^',

    // assignment
    equals = '=',

    // general keywords
    let = 'L',
    set = 'S',
    quit = 'Q',
    number = '#',
    ident = '@',

    // function keywords
    f_sqrt = 'R',

    // non-printing
    eof = '\0',

    // other
    print = ';',
    dot = '.',
};

// Keywords.
const std::string kw_decl = "let";
const std::string kw_assn = "set";
const std::string kw_quit = "quit";
const std::string kw_exit = "exit";
const std::string kw_sqrt = "sqrt";

// @brief Fetch a token from the standard input.
// @pre An ASCII character.
// @returns A token.
// @throws std::runtime_error if token is not alphanumeric or an operator.
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
    case bar:
    case star:
    case slash:
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
    default: // identifiers
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
            if (str == kw_assn) {
                return Token{set};
            }
            if (str == kw_quit || str == kw_exit) {
                return Token{quit};
            }
            if (str == kw_sqrt) {
                return Token{f_sqrt};
            }
            return Token{ident, str};
        }
        cal::error("unrecognized token");
    }
    return Token{'?'}; // never reached
}

// @brief Discard characters up to and including a c.
// @param c a character to ignore.
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
    bool is_const;    // true if variable is a constant

    // @brief Construct a variable with a name and value.
    // @param[in] id a variable identifier.
    // @param[in] v a variable value.
    Variable(std::string id, double v) : name{id}, value{v} {}

    // @brief Construct a variable with a name and value.
    // @param[in] id a variable identifier.
    // @param[in] v a variable value.
    // @param[in] b true if b is a constant.
    Variable(std::string id, double v, bool b)
        : name{id}, value{v}, is_const{false}
    {}
};

// @class Symbol_table
// @brief A symbol table type.
class Symbol_table {
public:
    std::vector<Variable> var_table; // table of variables

    // @brief Retrieve a variable's value.
    // @param[in] var a variable identifier.
    // @throws std::runtime_error if the variable is undefined.
    // @return the variable's value.
    double get(std::string var);

    // @brief Assign a new value to a variable.
    // @param[in] var a variable identifier.
    // @param[in] val a value.
    // @throws std::runtime_error if the variable is undefined.
    void set(std::string var, double val);

    // @brief Determine if the specified variable is declared.
    // @param[in] var the variable identifier to be tested.
    // @returns true if the variable is declared; false otherwise.
    bool is_declared(std::string var);

    // @brief Add a variable to the symbol table.
    // @param[in] var a variable identifier.
    // @param[in] val a value.
    double declare(std::string var, double val);

    // @brief Construct a symbol table.
    Symbol_table() {}
};

Symbol_table names;

// Retrieve a variable's value.
double Symbol_table::get(std::string var)
{
    for (size_t i = 0; i < names.var_table.size(); ++i) {
        if (names.var_table[i].name == var) {
            return names.var_table[i].value;
        }
    }
    cal::error(var, " is undefined");
    return 1;
}

// Assign a new value to a variable.
void Symbol_table::set(std::string var, double val)
{
    for (Variable& i : names.var_table) {
        if (i.name == var) {
            i.value = val;
            return;
        }
    }
    cal::error(var, " is undefined");
}

// Determine if the specified variable is declared.
bool Symbol_table::is_declared(std::string var)
{
    for (Variable& i : names.var_table) {
        if (i.name == var) {
            return true;
        }
    }
    return false;
}

// Add a variable to the symbol table.
double Symbol_table::declare(std::string var, double val)
{
    if (names.is_declared(var)) {
        cal::error(var, " is defined");
    }
    names.var_table.push_back(Variable{var, val});
    return val;
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
            cal::error("')' missing in expression");
        }
        return temp;
    }
    case lbrace:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrace) {
            cal::error("'}' missing in expression");
        }
        return temp;
    }
    case lbrack:
    {
        double temp{expression()};
        t = ts.get();
        if (t.kind != rbrack) {
            cal::error("']' missing in expression");
        }
        return temp;
    }
    case f_sqrt: // sqrt(a)
    {
        t = ts.get();
        if (t.kind != lparen) {
            cal::error("'(' missing in expression");
        }
        double temp{expression()};
        if (temp < 0) {
            cal::error("domain error");
        }
        t = ts.get();
        if (t.kind != rparen) {
            cal::error("')' missing in expression");
        }
        return std::sqrt(temp);
    }
    case bar: // |a|
    {
        double temp = expression();
        t = ts.get();
        if (t.kind != bar) {
            cal::error("'|' missing in expression");
        }
        return std::abs(temp);
    }
    case minus: // -a
        return -factor();
    case plus: // +a
        return factor();
    case number: // [.0-9]
        return t.value;
    case ident: // [a-zA-Z_]
        return names.get(t.name);
    default:
        cal::error("factor expected");
    }

    return 0; // never reached
}

// Construct a power expression.
double power_expression()
{
    double left{factor()};
    Token t{ts.get()};

    while (true) {
        switch (t.kind) {
        case bang: // a!
        {
            int temp = cal::narrow_cast<int>(left);
            if (temp < 0) {
                cal::error("domain error");
            }
            return left = cal::factorial(temp);
            break;
        }
        case caret: // a^b
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
        case star: // a*b
            left *= power_expression();
            break;
        case slash: // a/b
        {
            double temp{power_expression()};
            if (temp == 0) {
                cal::error("division by zero");
            }
            left /= temp;
            break;
        }
        case percent: // a%b is defined for floats
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
        case plus: // a+b
            left += term();
            t = ts.get();
            break;
        case minus: // a-b
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}

// Declare a variable.
double declaration()
{
    Token t{ts.get()};
    if (t.kind != ident) {
        cal::error("identifier missing in declaration");
    }
    std::string name{t.name};

    Token t2{ts.get()};
    if (t2.kind != equals) {
        cal::error("'=' missing in declaration of ", name);
    }

    double value{expression()};
    names.declare(name, value);
    return value;
}

// Deal with assignments.
double assignment()
{
    Token t{ts.get()};
    if (t.kind != ident) {
        cal::error("identifier missing in assignment");
    }
    std::string name{t.name};

    Token t2 = ts.get();
    if (t2.kind != '=') {
        cal::error("'=' missing in assignment of ", name);
    }
    double value{expression()};
    names.set(name, value);
    return value;
}

// Turn a declaration into a statement.
double statement()
{
    Token t{ts.get()};
    switch (t.kind) {
    case let: // declaration
        return declaration();
    case set: // assignment
        return assignment();
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
    names.declare("E", 2.71828182845904523536);       // e
    names.declare("LOG2E", 1.44269504088896340736);   // log2(e)
    names.declare("LOG10E", 0.434294481903251827651); // log10(e)
    names.declare("LN2", 0.693147180559945309417);    // ln(2)
    names.declare("LN10", 2.30258509299404568402);    // ln(10)
    names.declare("PI", 3.14159265358979323846);      // pi
    names.declare("PI_2", 1.57079632679489661923);    // pi/2
    names.declare("PI_4", 0.785398163397448309616);   // pi/4
    names.declare("SQRT2", 1.41421356237309504880);   // sqrt(2)

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
