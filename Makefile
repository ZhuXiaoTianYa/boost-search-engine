cc=g++

main:parser.cc
	${cc} -o $@ $^ -std=c++11 -lboost_system -lboost_filesystem

.PHONY:clean
clean:
	rm -f main