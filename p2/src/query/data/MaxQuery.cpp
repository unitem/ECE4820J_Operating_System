#include "MaxQuery.h"

#include <mutex>
#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *MaxQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void MaxQuery::execute_thd(std::vector<Table::ValueType> &maxs) {
  Table::Iterator begin, end;

  { // critical region
    const std::lock_guard<std::mutex> guard(table_it_mutex);
    if (table_it == table_end) {
      return;
    }
    begin = table_it;
    table_it += static_cast<int>(size_thd);
    if (table_it < table_end) {
      end = table_it;
    } else {
      end = table_end;
      table_it = table_end;
    }
  }

  for (auto &it = begin; it != end; ++it) {
    if (this->evalCondition(*it)) {
      if (maxs.empty()) {
        for (auto fieldId : this->fieldIds) {
          maxs.emplace_back((*it)[fieldId]);
        }
      } else {
        for (size_t i = 0; i < this->fieldNum; ++i) {
          const Table::ValueType val = (*it)[this->fieldIds[i]];
          if (val > maxs[i]) {
            maxs[i] = val;
          }
        }
      }
    }
  }
}

QueryResult::Ptr MaxQuery::execute() {
  using namespace std;
  if (this->operands.empty())
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  if (any_of(operands.begin(), operands.end(),
             [](const string &str) { return str == "KEY"; })) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                       "Cannot compare KEY!");
  }
  Database &db = Database::getInstance();
  try {
    auto &table = db[this->targetTable];
    this->fieldNum = this->operands.size();
    this->fieldIds.reserve(this->fieldNum);
    for (auto &field : this->operands) {
      this->fieldIds.emplace_back(table.getFieldIndex(field));
    }
    vector<Table::ValueType> maxs;
    auto result = initCondition(table);
    if (result.second) {
      // initialize table iterators
      table_it = table.begin();
      table_end = table.end();

      // create threads
      const unsigned int max_threads =
          parsedArgs.threads > 0 ? static_cast<unsigned int>(parsedArgs.threads)
                                 : std::thread::hardware_concurrency();
      size_thd = table.size() / max_threads;
      size_thd += table.size() % max_threads != 0;
      vector<thread> threads;
      vector<vector<Table::ValueType>> maxs_thd;
      maxs_thd.reserve(static_cast<unsigned long>(max_threads));
      for (unsigned int i = 0; i < max_threads; ++i) {
        maxs_thd.emplace_back(vector<Table::ValueType>());
        threads.emplace_back(&MaxQuery::execute_thd, this, ref(maxs_thd[i]));
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
        if (!maxs_thd[i].empty()) {
          if (maxs.empty()) {
            maxs = std::move(maxs_thd[i]);
          } else {
            for (size_t j = 0; j < this->fieldNum; ++j) {
              const Table::ValueType val = maxs_thd[i][j];
              if (val > maxs[j]) {
                maxs[j] = val;
              }
            }
          }
        }
      }
    }
    if (maxs.empty()) {
      return make_unique<SuccessMsgResult>(qname, this->targetTable);
    }
    return make_unique<AnswerResult>(std::move(maxs));
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unkonwn error '?'."_f % e.what());
  }
}

std::string MaxQuery::toString() {
  return "QUERY = MAX \"" + this->targetTable + "\"";
}
