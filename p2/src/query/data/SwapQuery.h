#ifndef PROJECT_SWAPQUERY_H
#define PROJECT_SWAPQUERY_H

#include "../Query.h"

class SwapQuery : public ComplexQuery {
  static constexpr const char *qname = "SWAP";

  Table::FieldIndex fieldId1 = Table::FieldIndex(),
                    fieldId2 = Table::FieldIndex();

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(Table::SizeType &counter);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_SWAPQUERY_H
