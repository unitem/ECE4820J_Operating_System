#ifndef PROJECT_SELECTQUERY_H
#define PROJECT_SELECTQUERY_H

#include <mutex>

#include "../Query.h"

class SelectQuery : public ComplexQuery {
  static constexpr const char *qname = "SELECT";

  RowsResult::RowsType rows = RowsResult::RowsType();
  std::mutex rows_mutex;

public:
  using ComplexQuery::ComplexQuery;

  void execute_thd(void);

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_SELECTQUERY_H
