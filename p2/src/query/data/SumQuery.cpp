#include "SumQuery.h"

#include <algorithm>
#include <mutex>
#include <thread>

#include "../../db/Database.h"
#include "../../main.h"

constexpr const char *SumQuery::qname;

extern struct Args_t parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

void SumQuery::execute_thd(std::vector<Table::ValueType> &sums) {
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
      for (size_t i = 0; i < this->fieldId.size(); ++i) {
        sums[i] += (*it)[this->fieldId[i]];
      }
    }
  }
}

QueryResult::Ptr SumQuery::execute() {
  using namespace std;
  if (this->operands.empty())
    return make_unique<ErrorMsgResult>(
        qname, this->targetTable.c_str(),
        "Invalid number of operands (? operands)."_f % operands.size());

  Database &db = Database::getInstance();
  try {
    auto &table = db[this->targetTable];
    for (const auto &operand : this->operands) {
      if (operand == "KEY") {
        return make_unique<ErrorMsgResult>(qname, this->targetTable.c_str(),
                                           "Cannot sum KEY.");
      } else {
        this->fieldId.emplace_back(table.getFieldIndex(operand));
      }
    }
    const auto num = this->fieldId.size();
    this->fieldSum = vector<Table::ValueType>(num, 0);
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
      vector<vector<Table::ValueType>> sums;
      sums.reserve(static_cast<unsigned long>(max_threads));
      for (unsigned int i = 0; i < max_threads; ++i) {
        sums.emplace_back(vector<Table::ValueType>(num, 0));
        threads.emplace_back(&SumQuery::execute_thd, this, ref(sums[i]));
      }

      // collecte threads
      for (unsigned int i = 0; i < max_threads; ++i) {
        threads[i].join();
        for (size_t j = 0; j < num; ++j) {
          this->fieldSum[j] += sums[i][j];
        }
      }
    }
    return make_unique<AnswerResult>(std::move(this->fieldSum));
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

std::string SumQuery::toString() {
  return "QUERY = SUM, Table = \"" + this->targetTable + "\"";
}
