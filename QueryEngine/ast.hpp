#pragma once
#include <variant>
#include <string>
#include <map>
#include <memory>
#include <vector>
using namespace std;

using Value = std::variant<std::string, double, bool, std::monostate>;
using Record = std::map<std::string, Value>;

struct Expr {
  enum Kind { Bin, Ref, Fixed } kind;
  Value value; // For Fixed Kind
  string column; // For Ref Kind 
  string op; // For Bin Kind
  unique_ptr<Expr> left; // For Bin Kind
  unique_ptr<Expr> right; // For Bin Kind
};

struct Query {
  enum Kind { Select, Insert, Update, Delete } kind;
  string table;
  unique_ptr<Expr> where;
  vector<Record> kvs; // keys and values For Insert and Update
};


