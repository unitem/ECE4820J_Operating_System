#ifndef _SUBQUERY_H
#define _SUBQUERY_H

#include "../../db/Table.h"
#include "../Query.h"

class SubQuery : public ComplexQuery {
  static constexpr const char *qname = "SUB";
  std::vector<Table::FieldIndex> fieldId;

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(Table::SizeType &counter);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif
