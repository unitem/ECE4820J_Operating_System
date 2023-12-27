#!/bin/bash

check_code() {
  # compile
  if [ ! -d build ]; then
    mkdir build
  fi
  cd build
  echo "Start compiling..."
  echo
  if cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../src; then
    if cmake --build . -- -j8; then
      echo
      echo "Congratulations, all files compiled without any error!"
      echo
    else
      exit 1
    fi
  else
    exit 1
  fi
  cd ..

  # clang-tidy
  echo "Start clang-tidy test..."
  echo
  run-clang-tidy -quiet -load=/usr/local/lib/libcodequality.so -p build
  echo
  echo "clang-tidy test done."
  echo

  # cppling
  echo "Start cpplint test..."
  echo
  cpplint --linelength=120 --filter=-legal,-readability/casting,-whitespace,-runtime/int,-runtime/printf,-runtime/reference,-runtime/threadsafe_fn,-readability/todo,-build/c++11,-build/include_subdir,-build/namespaces,-build/header_guard,-build/include_what_you_use --recursive src/
  echo
  echo "cpplint test done."
  echo

  # cppcheck
  echo "Start cppcheck test..."
  echo
  cppcheck --enable=all --disable=missingInclude,unusedFunction src/
  echo
  echo "cppcheck test done."
}

test_code() {
  ln -s /opt/lemondb/db db
  mkdir out
  cd build
  echo "Start running test..."
  echo
  time ./lemondb --listen ${ORIG_HOME}/test/test.query >../out/my.out
  echo
  echo "Program exits. Check results..."
  echo
  diff ${ORIG_HOME}/test/answer.out ../out/my.out
}

case $1 in
check_code)
  check_code
  ;;
test_code)
  test_code
  ;;
esac
