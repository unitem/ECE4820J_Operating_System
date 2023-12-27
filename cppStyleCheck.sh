#!/usr/bin/bash
cpplint --linelength=120 --filter=-legal,-readability/casting,-whitespace,-runtime/printf,-runtime/threadsafe_fn,-readability/todo,-build/include_subdir,-build/header_guard *.h *.c
# if there is no output other than the following, you pass the test:
# Done processing [File Name].c
# Done processing [File Name].h
