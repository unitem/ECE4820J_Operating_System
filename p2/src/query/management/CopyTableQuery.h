#ifndef PROJECT_COPYTABLEQUERY_H
#define PROJECT_COPYTABLEQUERY_H

#include "../Query.h"

class CopyTableQuery : public Query {
  static constexpr const char *qname = "COPYTABLE";
  const std::string newTable;

public:
  explicit CopyTableQuery(std::string table, std::string newTable)
      : Query(std::move(table)), newTable(std::move(newTable)) {}

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_COPYTABLEQUERY_H
