#include "DuplicateQuery.h"

#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *DuplicateQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void DuplicateQuery::execute_thd(
    Table &table, Table::SizeType &counter,
    std::unordered_map<Table::KeyType, std::vector<Table::ValueType>> &rows) {
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
      const std::string keyCopy = it->key() + "_copy";
      if (table[keyCopy] == nullptr) {
        ++counter;
        rows.emplace(keyCopy, it->getData());
      }
    }
  }
}

QueryResult::Ptr DuplicateQuery::execute() {
  using namespace ::std;
  if (!this->operands.empty())
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  Database &db = Database::getInstance();
  try {
    Table::SizeType counter = 0;
    Table &table = db[this->targetTable];
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
      vector<unordered_map<Table::KeyType, vector<Table::ValueType>>> rows;
      counters.reserve(static_cast<unsigned long>(max_threads));
      rows.reserve(static_cast<unsigned long>(max_threads));
      for (unsigned int i = 0; i < max_threads; ++i) {
        counters.emplace_back(Table::SizeType(0));
        rows.emplace_back(
            unordered_map<Table::KeyType, vector<Table::ValueType>>());
        threads.emplace_back(&DuplicateQuery::execute_thd, this, ref(table),
                             ref(counters[i]), ref(rows[i]));
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
        counter += counters[i];
        for (auto &[key, data] : rows[i]) {
          table.insertByIndex(key, std::move(data));
        }
      }
    }
    return make_unique<RecordCountResult>(counter);
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

std::string DuplicateQuery::toString() {
  return "QUERY = DUPLICATE \"" + this->targetTable + "\"";
}
