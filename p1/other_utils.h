#ifndef OTHER_UTILS_H_
#define OTHER_UTILS_H_

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "argslist.h"
#include "basic_io.h"
#include "parse.h"
#include "mumsh.h"

void mypwd(void);
// self implementation of pwd

int mycd(DynamicStringArray *args);
// self implementation of cd

#endif
