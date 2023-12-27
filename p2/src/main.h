#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include <mutex>
#include <string>

#include "db/Table.h"

extern struct Args_t {
  std::string listen;
  long threads = 0;
} parsedArgs;

extern Table::SizeType size_thd;
extern Table::Iterator table_it;
extern Table::Iterator table_end;
extern std::mutex table_it_mutex;

#endif // PROJECT_MAIN_H
