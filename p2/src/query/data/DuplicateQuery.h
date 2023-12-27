#ifndef PROJECT_DUPLICATEQUERY_H
#define PROJECT_DUPLICATEQUERY_H

#include "../Query.h"

class DuplicateQuery : public ComplexQuery {
  static constexpr const char *qname = "DUPLICATE";

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(
      Table &table, Table::SizeType &counter,
      std::unordered_map<Table::KeyType, std::vector<Table::ValueType>> &rows);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_DUPLICATEQUERY_H
