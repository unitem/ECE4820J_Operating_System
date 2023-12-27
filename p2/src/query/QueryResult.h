//
// Created by liu on 18-10-25.
//

#ifndef PROJECT_QUERYRESULT_H
#define PROJECT_QUERYRESULT_H

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../db/Table.h"
#include "../utils/formatter.h"

class QueryResult {
public:
  typedef std::unique_ptr<QueryResult> Ptr;

  virtual bool success() = 0;

  virtual bool display() = 0;

  virtual ~QueryResult() = default;

  friend std::ostream &operator<<(std::ostream &os, const QueryResult &table);

protected:
  virtual std::ostream &output(std::ostream &os) const = 0;
};

class FailedQueryResult : public QueryResult {
  const std::string data;

public:
  bool success() override { return false; }
};

class SucceededQueryResult : public QueryResult {
public:
  bool success() override { return true; }
};

class NullQueryResult : public SucceededQueryResult {
public:
  bool display() override { return false; }

protected:
  std::ostream &output(std::ostream &os) const override { return os << "\n"; }
};

class ErrorMsgResult : public FailedQueryResult {
  std::string msg;

public:
  bool display() override { return false; }

  ErrorMsgResult(const char *qname, const std::string &msg) {
    this->msg = R"(Query "?" failed : ?)"_f % qname % msg;
  }

  ErrorMsgResult(const char *qname, const std::string &table,
                 const std::string &msg) {
    this->msg = R"(Query "?" failed in Table "?" : ?)"_f % qname % table % msg;
  }

protected:
  std::ostream &output(std::ostream &os) const override {
    return os << msg << "\n";
  }
};

class SuccessMsgResult : public SucceededQueryResult {
  std::string msg;

public:
  bool display() override { return false; }

  explicit SuccessMsgResult(const int number) {
    this->msg = R"(ANSWER = "?".)"_f % number;
  }

  explicit SuccessMsgResult(const char *qname) {
    this->msg = R"(Query "?" success.)"_f % qname;
  }

  SuccessMsgResult(const char *qname, const std::string &msg) {
    this->msg = R"(Query "?" success : ?)"_f % qname % msg;
  }

  SuccessMsgResult(const char *qname, const std::string &table,
                   const std::string &msg) {
    this->msg = R"(Query "?" success in Table "?" : ?)"_f % qname % table % msg;
  }

protected:
  std::ostream &output(std::ostream &os) const override {
    return os << msg << "\n";
  }
};

class AnswerResult : public SucceededQueryResult {
  std::string msg;

public:
  bool display() override { return true; }

  explicit AnswerResult(std::vector<int> &&answer) {
    this->msg = R"a(ANSWER = ( ?))a"_f % answer;
  }

  explicit AnswerResult(int answer) {
    this->msg = R"(ANSWER = "?".)"_f % answer;
  }

  std::ostream &output(std::ostream &os) const override {
    return os << this->msg << "\n";
  }
};

class RecordCountResult : public SucceededQueryResult {
  const int affectedRows;

public:
  bool display() override { return true; }

  explicit RecordCountResult(int count) : affectedRows(count) {}

protected:
  std::ostream &output(std::ostream &os) const override {
    return os << "Affected ? rows."_f % affectedRows << "\n";
  }
};

class RowsResult : public SucceededQueryResult {
public:
  using RowsType = std::map<Table::KeyType, Table::Iterator>;

private:
  const std::vector<Table::FieldIndex> fieldIds;
  std::unique_ptr<RowsType> rows;

public:
  bool display() override { return true; }

  explicit RowsResult(const std::vector<Table::FieldIndex> &fieldIds,
                      std::unique_ptr<RowsType> rows)
      : fieldIds(std::move(fieldIds)), rows(std::move(rows)) {}

protected:
  std::ostream &output(std::ostream &os) const override {
    for (const auto &pair : *rows) {
      const Table::KeyType key = pair.first;
      Table::Iterator it = pair.second;
      os << "( " << key;
      for (auto id_it = fieldIds.begin(); id_it != fieldIds.end(); ++id_it) {
        os << ' ' << (*it)[*id_it];
      }
      os << " )\n";
    }
    return os;
  }
};

#endif // PROJECT_QUERYRESULT_H
