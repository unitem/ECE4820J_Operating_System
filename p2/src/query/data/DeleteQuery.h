#ifndef PROJECT_DELETEQUERY_H
#define PROJECT_DELETEQUERY_H

#include "../Query.h"

class DeleteQuery : public ComplexQuery {
  static constexpr const char *qname = "DELETE";

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(std::vector<Table::KeyType> &keys);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_DELETEQUERY_H