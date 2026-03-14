PARSER=parser
SSERVER=seacher_server
cc=g++
.PHONY:all
all:$(PARSER) $(SSERVER)

$(PARSER):parser.cc
	$(cc) -o $@ $^ -std=c++11 -lboost_system -lboost_filesystem
$(SSERVER):server.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp -lboost_system -lboost_filesystem

.PHONY:clean
clean:
	rm -f parser seacher_server