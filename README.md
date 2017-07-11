# print_backtrace
use libunwind lib to trace

1. Download and install libunwind
http://download.savannah.nongnu.org/releases/libunwind/
./config
make && make install

2. one blog to show more information and useful reference.
http://blog.bigpixel.ro/2010/09/stack-unwinding-stack-trace-with-gcc/

3. how to build it
g++ test.cc -L. -lunwind -ldl -o test
./test



