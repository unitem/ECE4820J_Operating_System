#include "CopyTableQuery.h"

#include "../../db/Database.h"

constexpr const char *CopyTableQuery::qname;

QueryResult::Ptr CopyTableQuery::execute() {
  using namespace std;
  Database &db = Database::getInstance();
  try {
    db.testDuplicate(this->newTable);

    Table const &table = db[this->targetTable];
    auto tableCopy = make_unique<Table>(this->newTable, table);
    db.registerTable(std::move(tableCopy));

    return make_unique<SuccessMsgResult>(qname, this->targetTable + " -> " +
                                                    this->newTable);
  } catch (const DuplicatedTableName &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "Name already exists."s);
  } catch (const TableNameNotFound &e) {
    return make_unique<ErrorMsgResult>(qname, this->targetTable,
                                       "No such table."s);
  }
}

std::string CopyTableQuery::toString() {
  return "QUERY = COPYTABLE, Table = \"" + targetTable + "\", NewTable = \"" +
         newTable + "\"";
}
