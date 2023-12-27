#ifndef PROJECT_MAXQUERY_H
#define PROJECT_MAXQUERY_H

#include "../Query.h"

class MaxQuery : public ComplexQuery {
  static constexpr const char *qname = "MAX";

  size_t fieldNum = 0;
  std::vector<Table::FieldIndex> fieldIds = std::vector<Table::FieldIndex>();

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(std::vector<Table::ValueType> &maxs);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_MAXQUERY_H
