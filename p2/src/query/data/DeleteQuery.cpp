#include "DeleteQuery.h"

#include <mutex>
#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *DeleteQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void DeleteQuery::execute_thd(std::vector<Table::KeyType> &keys) {
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
      keys.emplace_back(it->key());
    }
  }
}

QueryResult::Ptr DeleteQuery::execute() {
  using namespace std;
  if (this->operands.size() != 0) {
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());
  }
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
      vector<vector<Table::KeyType>> keys;
      keys.reserve(static_cast<unsigned long>(max_threads));
      for (unsigned int i = 0; i < max_threads; ++i) {
        keys.emplace_back(vector<Table::KeyType>());
        threads.emplace_back(&DeleteQuery::execute_thd, this, ref(keys[i]));
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
        counter += keys[i].size();
      }
      for (const auto &keys_thd : keys) {
        for (const auto &key : keys_thd) {
          auto last = table.end() - 1;
          auto it = table.find(key);
          if (it != last) {
            last->eraseKey();
            it->setKey(last->key());
            it->copyData(*last);
            table.pop_back();
          } else {
            it->eraseKey();
            table.pop_back();
          }
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

std::string DeleteQuery::toString() {
  return "QUERY = DELETE \"" + this->targetTable + "\"";
}
