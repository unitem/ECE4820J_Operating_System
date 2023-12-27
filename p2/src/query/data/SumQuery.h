#ifndef _SUMQUERY_H
#define _SUMQUERY_H

#include "../../db/Table.h"
#include "../Query.h"

class SumQuery : public ComplexQuery {
  static constexpr const char *qname = "SUM";

  std::vector<Table::FieldIndex> fieldId = std::vector<Table::FieldIndex>();
  std::vector<Table::ValueType> fieldSum = std::vector<Table::ValueType>();

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(std::vector<Table::ValueType> &sums);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif
