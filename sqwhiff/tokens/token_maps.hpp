#pragma once
#include <string>
#include <unordered_map>

#include "sqwhiff/tokens/token_types.hpp"

const std::unordered_map<char, TokenType> SQF_Token_Chars{
    {';', TokenType::semi},   {',', TokenType::comma},  {'+', TokenType::plus},
    {'-', TokenType::minus},  {'*', TokenType::mul},    {'/', TokenType::div},
    {'^', TokenType::pow},    {'%', TokenType::mod},    {'#', TokenType::hash},
    {'(', TokenType::lparen}, {')', TokenType::rparen}, {'[', TokenType::lsqb},
    {']', TokenType::rsqb},   {'{', TokenType::lcurl},  {'}', TokenType::rcurl},
};

const std::unordered_map<std::string, TokenType> SQF_Token_Keywords{
    {"private", TokenType::private_op},
    {"mod", TokenType::mod},
    {"atan2", TokenType::atan2},
    {"min", TokenType::min},
    {"max", TokenType::max},
    {"not", TokenType::negation},
    {"or", TokenType::disjunction},
    {"and", TokenType::conjunction},
    {"else", TokenType::else_op},
};
