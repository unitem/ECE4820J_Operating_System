#ifndef PROJECT_TRUNCATETABLEQUERY_H
#define PROJECT_TRUNCATETABLEQUERY_H

#include "../Query.h"

class TruncateTableQuery : public Query {
  static constexpr const char *qname = "TRUNCATE";

public:
  explicit TruncateTableQuery(std::string table) : Query(std::move(table)) {}

  QueryResult::Ptr execute() override;

  std::string toString() override;
};

#endif // PROJECT_TRUNCATETABLEQUERY_H
