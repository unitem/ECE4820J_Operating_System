#include "SelectQuery.h"

#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *SelectQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void SelectQuery::execute_thd(void) {
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
      { // critical region
        const std::lock_guard<std::mutex> guard(this->rows_mutex);
        rows.emplace(it->key(), it);
      }
    }
  }
}

QueryResult::Ptr SelectQuery::execute() {
  using namespace std;
  if (this->operands.empty()) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                       "No operand (? operands)."_f %
                                           operands.size());
  }
  Database &db = Database::getInstance();
  try {
    Table &table = db[this->targetTable];
    if (this->operands[0] != "KEY") {
      return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                         "Invalid first operand \"" +
                                             this->operands[0] + "\".");
    }
    /*
    // check duplicates
    const set<string> operands_set(operands.begin(), operands.end());
    if (operands_set.size() != operands.size()) {
      return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                         "Duplicated fields.");
    }
    */
    auto result = initCondition(table);
    std::vector<Table::FieldIndex> fieldIds;
    if (result.second) {
      // record field ids for printing output
      for (auto it = operands.begin() + 1; it != operands.end(); ++it) {
        fieldIds.push_back(table.getFieldIndex(*it));
      }

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
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads.emplace_back(&SelectQuery::execute_thd, this);
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
      }
    }
    return make_unique<RowsResult>(
        fieldIds, make_unique<RowsResult::RowsType>(this->rows));
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  } catch (const IllFormedQueryCondition &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable, e.what());
  } catch (const invalid_argument &e) {
    // Cannot convert operand to string
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unknown error '?'"_f % e.what());
  } catch (const exception &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Unkonwn error '?'."_f % e.what());
  }
}

std::string SelectQuery::toString() {
  return "QUERY = SELECT \"" + this->targetTable + "\"";
}
