#include "src/parser/parser.h"
#include "src/lexer/lexer.h"
#include "src/ast/all_nodes.h"
#include "src/errors/error_messages.h"
#include <memory>
#include <utility>
#include <vector>
#include <ostream>
#include <deque>
#include <exception>

// Consume a file's tokens and structure them together into an AST (parse the file)
Parser::Parser(Lexer &lexer) : lexer_(lexer) {}

void Parser::error(Token t, ErrorType type)
{
    Error e;
    e.token = t;
    e.type = type;
    errors_.push_back(e);
    throw std::runtime_error(ErrorMessages.at(e.type));
}

void Parser::eat(TokenType type)
{
    if (current_token_.type == type)
    {
        // Pull from the buffer first if any peek has occured
        if (!peek_buffer_.empty())
        {
            current_token_ = peek_buffer_.front();
            peek_buffer_.pop_front();
        }
        else
        {
            current_token_ = lexer_.nextToken();
        }
    }
    else
    {
        // TODO report the expected token
        error(current_token_, ErrorType::unexpected_token);
    }
}

// Allows looking ahead to future tokens as required due to grammar structures
Token Parser::peek(int peek_by = 1)
{
    while (peek_buffer_.size() < peek_by)
    {
        peek_buffer_.push_back(lexer_.nextToken());
    }

    // Convert peek request to 0-indexed
    return peek_buffer_.at(peek_by - 1);
}

// program : statement_list EOF
std::unique_ptr<AST> Parser::program()
{
    std::unique_ptr<AST> node = statement_list();
    eat(TokenType::end_of_file);
    return node;
}

// statement_list : statement ((SEMI|COMMA) statement)*
std::unique_ptr<AST> Parser::statement_list()
{
    std::vector<std::unique_ptr<AST>> results;
    results.push_back(statement());

    while (
        current_token_.type == TokenType::semi
        || current_token_.type == TokenType::comma
    )
    {
        eat(current_token_.type);
        results.push_back(statement());
    }

    return std::unique_ptr<AST>(new Compound(std::move(results)));
}

// statement : assignment | expr | empty
std::unique_ptr<AST> Parser::statement()
{
    switch (current_token_.type)
    {
    // Private keyword can modify assignment operation, but could also just be an expression, requires peeking to differentiate
    case TokenType::private_op:
    {
        if (peek(2).type == TokenType::assign && peek().type == TokenType::id) {
            return assignment();
        }

        // This is not a private assignment modifier, just regular keyword
        return expr();
    }
    // A variable could be the start of an assignment, but could also just be an expression, requires peeking to differentiate
    case TokenType::id:
    {
        if (peek().type == TokenType::assign)
        {
            return assignment();
        }
        return expr();
    }
    // If there's an immediate statement delimiter the current statement is empty
    case TokenType::semi:
    case TokenType::comma:
    case TokenType::end_of_file:
    {
        return empty();
    }
    default:
        return expr();
    }
}

// assignment : [PRIVATE] variable ASSIGN expr
std::unique_ptr<AST> Parser::assignment()
{
    bool isPrivate = false;
    if (current_token_.type == TokenType::private_op) {
        isPrivate = true;
        eat(TokenType::private_op);
    }

    std::unique_ptr<AST> left = variable();
    Token t = current_token_;
    eat(TokenType::assign);

    return std::unique_ptr<AST>(new Assign(isPrivate, std::move(left), t, expr()));
}

// expr: conjunction (DISJUNCTION conjunction)*
std::unique_ptr<AST> Parser::expr()
{
    std::unique_ptr<AST> node = conjunction();

    while (current_token_.type == TokenType::disjunction)
    {
        Token t = current_token_;
        eat(TokenType::disjunction);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, conjunction()));
    }

    return node;
}

// conjunction : comparison (CONJUNCTION comparison)*
std::unique_ptr<AST> Parser::conjunction()
{
    std::unique_ptr<AST> node = comparison();

    while (current_token_.type == TokenType::conjunction)
    {
        Token t = current_token_;
        eat(TokenType::conjunction);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, comparison()));
    }

    return node;
}

// comparison : binary_op ((EQL|NEQL|GT|LT|GTEQL|LTEQL|GTGT) binary_op)*
std::unique_ptr<AST> Parser::comparison()
{
    std::unique_ptr<AST> node = binary_op();

    while (
        current_token_.type == TokenType::eql
        || current_token_.type == TokenType::neql
        || current_token_.type == TokenType::gt
        || current_token_.type == TokenType::lt
        || current_token_.type == TokenType::gteql
        || current_token_.type == TokenType::lteql
        || current_token_.type == TokenType::gtgt
    ) {
        Token t = current_token_;
        eat(t.type);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, binary_op()));
    }

    return node;
}

// binary_op : else_op (KEYWORD else_op)*
std::unique_ptr<AST> Parser::binary_op()
{
    std::unique_ptr<AST> node = else_op();

    while (current_token_.type == TokenType::keyword)
    {
        Token t = current_token_;
        eat(TokenType::keyword);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, else_op()));
    }

    return node;
}

// else_op : term (ELSE term)*
std::unique_ptr<AST> Parser::else_op()
{
    std::unique_ptr<AST> node = term();

    while (current_token_.type == TokenType::else_op)
    {
        Token t = current_token_;
        eat(TokenType::else_op);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, term()));
    }

    return node;
}

// term : factor ((PLUS|MINUS|MIN|MAX) factor)*
std::unique_ptr<AST> Parser::term()
{
    std::unique_ptr<AST> node = factor();

    while (
        current_token_.type == TokenType::plus
        || current_token_.type == TokenType::minus
        || current_token_.type == TokenType::min
        || current_token_.type == TokenType::max
    ) {
        Token t = current_token_;
        eat(t.type);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, factor()));
    }

    return node;
}

// factor : power ((MUL|DIV|MOD|ATAN2) power)*
std::unique_ptr<AST> Parser::factor()
{
    std::unique_ptr<AST> node = power();

    while (
        current_token_.type == TokenType::div
        || current_token_.type == TokenType::mul
        || current_token_.type == TokenType::mod
        || current_token_.type == TokenType::atan2
    ) {
        Token t = current_token_;
        eat(t.type);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, power()));
    }

    return node;
}

// power : hash_select (POW hash_select)*
std::unique_ptr<AST> Parser::power()
{
    std::unique_ptr<AST> node = hash_select();

    while (current_token_.type == TokenType::pow)
    {
        Token t = current_token_;
        eat(TokenType::pow);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, hash_select()));
    }

    return node;
}

// hash_select : unary_op (HASH unary_op)*
std::unique_ptr<AST> Parser::hash_select()
{
    std::unique_ptr<AST> node = unary_op();

    while (current_token_.type == TokenType::hash)
    {
        Token t = current_token_;
        eat(TokenType::hash);
        node = std::unique_ptr<AST>(new BinaryOp(std::move(node), t, unary_op()));
    }

    return node;
}

// unary_op : (PLUS|MINUS|NEGATION|KEYWORD) factor | nullary_op
std::unique_ptr<AST> Parser::unary_op()
{
    if (current_token_.type == TokenType::private_op)
    {
        // If a private op wasn't consumed in an assignment it's just a regular command
        current_token_.type = TokenType::keyword;
    }

    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::plus:
    case TokenType::minus:
    case TokenType::negation:
    case TokenType::keyword:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new UnaryOp(t, unary_op()));
    }
    default:
        return nullary_op();
    }
}

// nullary_op : NULLARY | LPAREN expr RPAREN | atom
std::unique_ptr<AST> Parser::nullary_op()
{
    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::nullary:
    {
        eat(TokenType::nullary);
        return std::unique_ptr<AST>(new NullaryOp(t));
    }
    case TokenType::lparen:
    {
        eat(TokenType::lparen);
        std::unique_ptr<AST> node = expr();
        eat(TokenType::rparen);
        return node;
    }
    default:
        return atom();
    }
}

// atom : STR_LITERAL | HEX_LITERAL | DEC_LITERAL | array | code | variable
std::unique_ptr<AST> Parser::atom()
{
    Token t = current_token_;
    switch (t.type)
    {
    case TokenType::dec_literal:
    case TokenType::hex_literal:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new Number(t));
    }
    case TokenType::str_literal:
    {
        eat(t.type);
        return std::unique_ptr<AST>(new StringLiteral(t));
    }
    case TokenType::lsqb:
    {
        return array();
    }
    case TokenType::lcurl:
    {
        return code();
    }
    default:
        return variable();
    }
}

std::unique_ptr<AST> Parser::array()
{
    eat(TokenType::lsqb);

    std::vector<std::unique_ptr<AST>> expressions;

    // Array can be empty
    if (current_token_.type != TokenType::rsqb)
    {
        expressions.push_back(expr());

        while (current_token_.type == TokenType::comma)
        {
            eat(TokenType::comma);
            expressions.push_back(expr());
        }
    }

    eat(TokenType::rsqb);

    return std::unique_ptr<AST>(new Array(
        std::unique_ptr<AST>(new Compound(std::move(expressions)))
    ));
}

std::unique_ptr<AST> Parser::code()
{
    eat(TokenType::lcurl);
    std::unique_ptr<AST> node = std::unique_ptr<AST>(new Code(statement_list()));
    eat(TokenType::rcurl);
    return node;
}

std::unique_ptr<AST> Parser::variable()
{
    std::unique_ptr<AST> node = std::unique_ptr<AST>(new Variable(current_token_));
    eat(TokenType::id);
    return node;
}

std::unique_ptr<AST> Parser::empty(){
    return std::unique_ptr<AST>(new NoOp());
}

// Parses the file and returns the root node of the AST
std::unique_ptr<AST> Parser::parse()
{
    std::unique_ptr<AST> node = nullptr;

    // Lexer or parser may fail due to bad syntax
    try
    {
        // Load the initial token for parsing
        current_token_ = lexer_.nextToken();
        node = program();
    }
    catch(const std::runtime_error& e)
    {
        // Throw up for whoever's using parser to handle
        throw;
    }

    return node;
};

void Parser::logErrors(std::ostream &out)
{
    lexer_.logErrors(out);
    for (auto &&e : errors_)
    {
        out << e.token.line << ":" << e.token.column << " " << ErrorMessages.at(e.type);
    }
}