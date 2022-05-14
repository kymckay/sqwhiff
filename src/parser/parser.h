#pragma once

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "./syntax_error.h"

class Parser {
  // Reference member, no need to copy the supplied lexer
  Lexer &lexer_;
  Token current_token_;

  void eat(TokenType);

  // Member functions for AST nodes
  // Dynamic allocation is needed for AST interface types

  ast_ptr program();
  ast_ptr statement_list();
  ast_ptr statement();
  ast_ptr assignment();
  ast_ptr expr();
  ast_ptr conjunction();
  ast_ptr comparison();
  ast_ptr binary_op();
  ast_ptr else_op();
  ast_ptr term();
  ast_ptr factor();
  ast_ptr power();
  ast_ptr hash_select();
  ast_ptr unary_op();
  ast_ptr nullary_op();
  ast_ptr atom();
  ast_ptr array();
  ast_ptr code();
  ast_ptr variable();
  ast_ptr empty();

 public:
  Parser(Lexer &);
  ast_ptr parse();
};
