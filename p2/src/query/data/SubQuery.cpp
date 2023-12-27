#include "SubQuery.h"

#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *SubQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void SubQuery::execute_thd(Table::SizeType &counter) {
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
      auto dest = this->fieldId.back();
      auto itId = this->fieldId.begin();
      auto answer = (*it)[*itId];
      for (++itId; itId != this->fieldId.end() - 1; ++itId) {
        answer -= (*it)[*itId];
      }
      (*it)[dest] = answer;
      ++counter;
    }
  }
}

std::string SubQuery::toString() {
  return "QUERY = SUB, Table = \"" + this->targetTable + "\"";
}

QueryResult::Ptr SubQuery::execute() {
  using namespace std;
  if (this->operands.size() < 2)
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());

  Database &db = Database::getInstance();
  try {
    Table::SizeType counter = 0;
    auto &table = db[this->targetTable];
    for (const auto &operand : this->operands) {
      if (operand == "KEY") {
        return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                           "Cannot sub KEY.");
      } else {
        this->fieldId.emplace_back(table.getFieldIndex(operand));
      }
    }
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
      vector<Table::SizeType> counters;
      counters.reserve(static_cast<unsigned long>(max_threads));
      for (unsigned int i = 0; i < max_threads; ++i) {
        counters.emplace_back(Table::SizeType(0));
        threads.emplace_back(&SubQuery::execute_thd, this, ref(counters[i]));
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
        counter += counters[i];
      }
    }
    return make_unique<RecordCountResult>(counter);
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
