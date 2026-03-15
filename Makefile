PARSER=parser
DUG=debug
cc=g++
.PHONY:all
all:$(PARSER) $(DUG)

$(PARSER):parser.cc
	$(cc) -o $@ $^ -std=c++11 -lboost_system -lboost_filesystem
$(DUG):debug.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp -lboost_system -lboost_filesystem

.PHONY:clean
clean:
	rm -f $(PARSER) $(DUG)