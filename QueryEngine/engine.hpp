#pragma once
#include "executor.hpp"

struct Result {
  std::vector<Record> records;
  size_t affectedRows;
};

class Engine {
  QueryExecutor& repo;

  static bool truthy(const Value& val) {
    auto b = std::get_if<bool>(&val);
    return b && *b;
  }

  static Value evaluateExpr(const Expr& e, const Record& row) {
    switch (e.kind) {
      case Expr::Fixed: return e.value;
      case Expr::Ref: {
                        auto it = row.find(e.column);
                        return it == row.end() ? Value{monostate{}} : it->second;   // missing column = NULL
                      }
      case Expr::Bin: break;
    }

    Value l = evaluateExpr(*e.left, row), r = evaluateExpr(*e.right, row);
    if (e.op == "AND") return truthy(l) && truthy(r);
    if (e.op == "OR")  return truthy(l) || truthy(r);

    // comparisons
    if (holds_alternative<monostate>(l) || holds_alternative<monostate>(r)) return false;  // NULL never matches
    if (l.index() != r.index()) throw runtime_error("type mismatch in '" + e.op + "'");
    if (e.op == "=")  return l == r;
    if (e.op == "!=") return l != r;
    if (e.op == "<")  return l < r;
    if (e.op == ">")  return l > r;
    if (e.op == "<=") return l <= r;
    if (e.op == ">=") return l >= r;
    throw runtime_error("unknown operator " + e.op);
  }
  static Pred compile(const Expr* where) {
    if (!where) return [](const Record&) { return true; };   // no WHERE: match all
    return [where](const Record& r) { return truthy(evaluateExpr(*where, r)); };
  }
  public:
  Engine(QueryExecutor& r) : repo(r) {}

  Result execute(const string& query) {
    Query q = Parser(query).parse();
    Pred pred = compile(q.where.get());

    Record kvs;
    for (const auto& kv : q.kvs) {
      for (const auto& [key, value] : kv) {
        kvs[key] = value;
      }
    }

    Result result;
    switch (q.kind) {
      case Query::Select:
        result.records = repo.select(q.table, pred, std::nullopt);
        result.affectedRows = result.records.size();
        return result;
      case Query::Insert:
        result.affectedRows = repo.insert(q.table, kvs);
        return result;
      case Query::Update:
        result.affectedRows = repo.update(q.table, pred, kvs);
        return result;
      case Query::Delete:
        result.affectedRows = repo.remove(q.table, pred);
        return result;
    }
    throw runtime_error("unknown query kind");
  }
};
