//
// Created by liu on 18-10-25.
//

#include "UpdateQuery.h"

#include <mutex>
#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *UpdateQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void UpdateQuery::execute_thd(Table::SizeType &counter) {
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
      if (this->keyValue.empty()) {
        (*it)[this->fieldId] = this->fieldValue;
      } else {
        it->setKey(this->keyValue);
      }
      ++counter;
    }
  }
}

QueryResult::Ptr UpdateQuery::execute() {
  using namespace std;
  if (this->operands.size() != 2)
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  try {
    Table::SizeType counter = 0;
    auto &table = db[this->targetTable];
    if (this->operands[0] == "KEY") {
      this->keyValue = this->operands[1];
    } else {
      this->fieldId = table.getFieldIndex(this->operands[0]);
      this->fieldValue =
          (Table::ValueType)strtol(this->operands[1].c_str(), nullptr, 10);
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
        threads.emplace_back(&UpdateQuery::execute_thd, this, ref(counters[i]));
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

std::string UpdateQuery::toString() {
  return "QUERY = UPDATE " + this->targetTable + "\"";
}
