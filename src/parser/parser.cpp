#include "./parser.h"

#include <deque>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

#include "../ast/all_nodes.h"
#include "../lexer/lexer.h"

// Consume a file's tokens and structure them together into an AST (parse the
// file)
Parser::Parser(Lexer &lexer) : lexer_(lexer) {}

void Parser::eat(TokenType type) {
  if (current_token_.type == type) {
    current_token_ = lexer_.get();
  } else {
    throw SyntaxError(current_token_.line, current_token_.column,
                      current_token_.file,
                      "Unexpected token '" + current_token_.raw +
                          "', expected " + SQF_Token_Descriptors.at(type));
  }
}

// program : statement_list EOF
ast_ptr Parser::program() {
  ast_ptr node = statement_list();
  eat(TokenType::end_of_file);
  return node;
}

// statement_list : statement ((SEMI|COMMA) statement)*
ast_ptr Parser::statement_list() {
  std::vector<ast_ptr> results;
  results.push_back(statement());

  while (current_token_.type == TokenType::semi ||
         current_token_.type == TokenType::comma) {
    eat(current_token_.type);
    results.push_back(statement());
  }

  return ast_ptr(new Compound(std::move(results)));
}

// statement : assignment | expr | empty
ast_ptr Parser::statement() {
  switch (current_token_.type) {
    // Private keyword can modify assignment operation, but could also just be
    // an expression, requires peeking to differentiate
    case TokenType::private_op: {
      if (lexer_.peek(2).type == TokenType::assign &&
          lexer_.peek().type == TokenType::id) {
        return assignment();
      }

      // This is not a private assignment modifier, just regular keyword
      return expr();
    }
    // A variable could be the start of an assignment, but could also just be an
    // expression, requires peeking to differentiate
    case TokenType::id: {
      if (lexer_.peek().type == TokenType::assign) {
        return assignment();
      }
      return expr();
    }
    // If there's an immediate statement delimiter the current statement is
    // empty
    case TokenType::semi:
    case TokenType::comma:
    case TokenType::end_of_file:
    case TokenType::rcurl: {
      return empty();
    }
    default:
      return expr();
  }
}

// assignment : [PRIVATE] variable ASSIGN expr
ast_ptr Parser::assignment() {
  bool isPrivate = false;
  if (current_token_.type == TokenType::private_op) {
    isPrivate = true;
    eat(TokenType::private_op);
  }

  ast_ptr left = variable();
  Token t = current_token_;
  eat(TokenType::assign);

  return ast_ptr(new Assign(isPrivate, std::move(left), t, expr()));
}

// expr: conjunction (DISJUNCTION conjunction)*
ast_ptr Parser::expr() {
  ast_ptr node = conjunction();

  while (current_token_.type == TokenType::disjunction) {
    Token t = current_token_;
    eat(TokenType::disjunction);
    node = ast_ptr(new BinaryOp(std::move(node), t, conjunction()));
  }

  return node;
}

// conjunction : comparison (CONJUNCTION comparison)*
ast_ptr Parser::conjunction() {
  ast_ptr node = comparison();

  while (current_token_.type == TokenType::conjunction) {
    Token t = current_token_;
    eat(TokenType::conjunction);
    node = ast_ptr(new BinaryOp(std::move(node), t, comparison()));
  }

  return node;
}

// comparison : binary_op ((EQL|NEQL|GT|LT|GTEQL|LTEQL|GTGT) binary_op)*
ast_ptr Parser::comparison() {
  ast_ptr node = binary_op();

  while (current_token_.type == TokenType::eql ||
         current_token_.type == TokenType::neql ||
         current_token_.type == TokenType::gt ||
         current_token_.type == TokenType::lt ||
         current_token_.type == TokenType::gteql ||
         current_token_.type == TokenType::lteql ||
         current_token_.type == TokenType::gtgt) {
    Token t = current_token_;
    eat(t.type);
    node = ast_ptr(new BinaryOp(std::move(node), t, binary_op()));
  }

  return node;
}

// binary_op : else_op (KEYWORD else_op)*
ast_ptr Parser::binary_op() {
  ast_ptr node = else_op();

  while (current_token_.type == TokenType::keyword) {
    Token t = current_token_;
    eat(TokenType::keyword);
    node = ast_ptr(new BinaryOp(std::move(node), t, else_op()));
  }

  return node;
}

// else_op : term (ELSE term)*
ast_ptr Parser::else_op() {
  ast_ptr node = term();

  while (current_token_.type == TokenType::else_op) {
    Token t = current_token_;
    eat(TokenType::else_op);
    node = ast_ptr(new BinaryOp(std::move(node), t, term()));
  }

  return node;
}

// term : factor ((PLUS|MINUS|MIN|MAX) factor)*
ast_ptr Parser::term() {
  ast_ptr node = factor();

  while (current_token_.type == TokenType::plus ||
         current_token_.type == TokenType::minus ||
         current_token_.type == TokenType::min ||
         current_token_.type == TokenType::max) {
    Token t = current_token_;
    eat(t.type);
    node = ast_ptr(new BinaryOp(std::move(node), t, factor()));
  }

  return node;
}

// factor : power ((MUL|DIV|MOD|ATAN2) power)*
ast_ptr Parser::factor() {
  ast_ptr node = power();

  while (current_token_.type == TokenType::div ||
         current_token_.type == TokenType::mul ||
         current_token_.type == TokenType::mod ||
         current_token_.type == TokenType::atan2) {
    Token t = current_token_;
    eat(t.type);
    node = ast_ptr(new BinaryOp(std::move(node), t, power()));
  }

  return node;
}

// power : hash_select (POW hash_select)*
ast_ptr Parser::power() {
  ast_ptr node = hash_select();

  while (current_token_.type == TokenType::pow) {
    Token t = current_token_;
    eat(TokenType::pow);
    node = ast_ptr(new BinaryOp(std::move(node), t, hash_select()));
  }

  return node;
}

// hash_select : unary_op (HASH unary_op)*
ast_ptr Parser::hash_select() {
  ast_ptr node = unary_op();

  while (current_token_.type == TokenType::hash) {
    Token t = current_token_;
    eat(TokenType::hash);
    node = ast_ptr(new BinaryOp(std::move(node), t, unary_op()));
  }

  return node;
}

// unary_op : (PLUS|MINUS|NEGATION|KEYWORD) factor | nullary_op
ast_ptr Parser::unary_op() {
  if (current_token_.type == TokenType::private_op) {
    // If a private op wasn't consumed in an assignment it's just a regular
    // command
    current_token_.type = TokenType::keyword;
  }

  Token t = current_token_;
  switch (t.type) {
    case TokenType::plus:
    case TokenType::minus:
    case TokenType::negation:
    case TokenType::keyword: {
      eat(t.type);
      return ast_ptr(new UnaryOp(t, unary_op()));
    }
    default:
      return nullary_op();
  }
}

// nullary_op : NULLARY | LPAREN expr RPAREN | atom
ast_ptr Parser::nullary_op() {
  Token t = current_token_;
  switch (t.type) {
    case TokenType::nullary: {
      eat(TokenType::nullary);
      return ast_ptr(new NullaryOp(t));
    }
    case TokenType::lparen: {
      eat(TokenType::lparen);
      ast_ptr node = expr();
      eat(TokenType::rparen);
      return node;
    }
    default:
      return atom();
  }
}

// atom : STR_LITERAL | HEX_LITERAL | DEC_LITERAL | array | code | variable
ast_ptr Parser::atom() {
  Token t = current_token_;
  switch (t.type) {
    case TokenType::dec_literal:
    case TokenType::hex_literal: {
      eat(t.type);
      return ast_ptr(new Number(t));
    }
    case TokenType::str_literal: {
      eat(t.type);
      return ast_ptr(new StringLiteral(t));
    }
    case TokenType::lsqb: {
      return array();
    }
    case TokenType::lcurl: {
      return code();
    }
    default:
      return variable();
  }
}

ast_ptr Parser::array() {
  eat(TokenType::lsqb);

  std::vector<ast_ptr> expressions;

  // Array can be empty
  if (current_token_.type != TokenType::rsqb) {
    expressions.push_back(expr());

    while (current_token_.type == TokenType::comma) {
      eat(TokenType::comma);
      expressions.push_back(expr());
    }
  }

  eat(TokenType::rsqb);

  return ast_ptr(new Array(ast_ptr(new Compound(std::move(expressions)))));
}

ast_ptr Parser::code() {
  eat(TokenType::lcurl);
  ast_ptr content = statement_list();
  eat(TokenType::rcurl);

  return ast_ptr(new Code(std::move(content)));
}

ast_ptr Parser::variable() {
  ast_ptr node = ast_ptr(new Variable(current_token_));
  eat(TokenType::id);
  return node;
}

ast_ptr Parser::empty() { return ast_ptr(new NoOp()); }

// Parses the file and returns the root node of the AST (may throw errors)
ast_ptr Parser::parse() {
  // Load the initial token for parsing
  current_token_ = lexer_.get();
  ast_ptr node = program();

  return node;
};
