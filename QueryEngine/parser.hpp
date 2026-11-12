#pragma once
#include "ast.hpp"
#include <algorithm>
#include <regex>
#include <stdexcept>

// Regex for tokenizing the query string

#define tokenRegex regex(R"((\s+|[(),;=<>!]+|'.*?'))")
#define doubleRegex regex(R"(\d+(\.\d+)?)")
#define stringRegex regex(R"('.*')")
#define valueRegex regex(R"(\d+(\.\d+)?|TRUE|FALSE|NULL|'.*')")
#define columnRegex regex(R"([a-zA-Z_][a-zA-Z0-9_]*)")
#define operatorRegex regex(R"(=|!=|<|>|<=|>=|AND|OR)")
#define actionRegex regex(R"(SELECT|INSERT|UPDATE|DELETE)")
#define queryRegex regex(R"((SELECT|INSERT|UPDATE|DELETE)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*(.*))")

class Parser {
  vector<string> tokens;
  size_t i = 0;

  string toUpper(const string& str) {
    string upperStr = str;
    transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
    return upperStr;
  }

  string peek() {
    if (i < tokens.size()) {
      return toUpper(tokens[i]);
    }
    return "";
  }

  string eat(const string& expected = "") {
    if (i < tokens.size() && (peek() == expected || expected.empty())) {
      return tokens[i++];
    }
    throw runtime_error("Expected '" + ((expected.empty())? "token" : expected) + "' but found '" + peek() + "'");
  }

  bool accept(const string& expected) {
    if (peek() == expected) {
      eat();
      return true;
    }
    return false;
  }

  Value parseValue() {
    string token = eat();
    string upperToken = toUpper(token);
    if (regex_match(token, doubleRegex)) {
      return stod(token);
    } else if (upperToken == "TRUE") {
      return true;
    } else if (upperToken == "FALSE") {
      return false;
    } else if (upperToken == "NULL") {
      return std::monostate{};
    } else if (regex_match(token, stringRegex)) return token.substr(1, token.size()-2); // treat as string
    
    throw runtime_error("Invalid value: " + token);
  }

  unique_ptr<Expr> parsePrimary(){
    if (accept("(")) {
      auto expr = parseExpression();
      eat(")");
      return expr;
    } else if (regex_match(peek(), valueRegex)) {
      return make_unique<Expr>(Expr{Expr::Fixed, parseValue()});
    } else {
      string column = eat();
      if (!regex_match(column, columnRegex)) throw runtime_error("Bad column: " + column);
      return make_unique<Expr>(Expr{Expr::Ref, {}, column});
    }
  }

  template <class Next>
    unique_ptr<Expr> binary(vector<string> ops, Next next) {
      auto l = next();
      while (find(ops.begin(), ops.end(), peek()) != ops.end()) {
        auto e = std::make_unique<Expr>();
        e->kind = Expr::Bin; e->op = toUpper(eat());
        e->left = std::move(l); e->right = next();
        l = std::move(e);
      }
      return l;
    }

  unique_ptr<Expr> cmp()     { return binary({"=", "!=", "<", ">", "<=", ">="}, [&] { return parsePrimary(); }); }
  unique_ptr<Expr> andExpr() { return binary({"AND"}, [&] { return cmp(); }); }
  unique_ptr<Expr> orExpr()  { return binary({"OR"},  [&] { return andExpr(); }); }
  unique_ptr<Expr> parseExpression() { return orExpr(); }

  void where(Query& q) { if (accept("WHERE")) q.where = orExpr(); }

  public:
  explicit Parser(const string& query) {
    static const regex re(R"(\s+|!=|<=|>=|[(),;=<>]|'[^']*')");
    sregex_token_iterator it(query.begin(), query.end(), re, {-1, 0});
    sregex_token_iterator end;
    for (; it != end; ++it) {
      string token = it->str();
      if (!token.empty() && !all_of(token.begin(), token.end(), ::isspace)) {
        tokens.push_back(token);
      }
    }
  }

  Query parse() {
    Query q;
    string action = toUpper(eat());
    if (action == "SELECT") {
      q.kind = Query::Select;
      eat("FROM");
      q.table = eat();
      where(q);
    } else if (action == "INSERT") {
      q.kind = Query::Insert;
      eat("INTO");
      q.table = eat();
      eat("(");
      while (true) {
        string column = eat();
        Value value = parseValue();
        q.kvs.push_back({{column, value}});
        if (accept(")")) break;
        eat(",");
      }
    } else if (action == "UPDATE") {
      q.kind = Query::Update;
      q.table = eat();
      eat("SET");
      while (true) {
        string column = eat();
        eat("=");
        Value value = parseValue();
        q.kvs.push_back({{column, value}});
        if (accept("WHERE")) {
          q.where = parseExpression();
          break;
        }
        if (!accept(",")) break;
      }
    } else if (action == "DELETE") {
      q.kind = Query::Delete;
      eat("FROM");
      q.table = eat();
      where(q);
    } else {
      throw runtime_error("Unknown action: " + action);
    }

    accept(";");
    if (i < tokens.size()) throw runtime_error("Unexpected '" + tokens[i] + "'");
    return q;
  }
};

