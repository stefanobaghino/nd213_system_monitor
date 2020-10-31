.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* include/* test/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

.PHONY: debug
debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

.PHONY: clean
clean:
	rm -rf build
	rm -rf build_test

.PHONY: test
test: build
	mkdir -p build_test
	g++ -std=c++17 -I ./include/ -c ./test/test.cpp -o ./build_test/test.o
	g++ -std=c++17 -o ./build_test/test ./build_test/*.o \
	  ./build/CMakeFiles/monitor.dir/src/format.cpp.o \
	  ./build/CMakeFiles/monitor.dir/src/linux_parser.cpp.o \
	  ./build/CMakeFiles/monitor.dir/src/processor.cpp.o \
	  ./build/CMakeFiles/monitor.dir/src/process.cpp.o
	./build_test/test
