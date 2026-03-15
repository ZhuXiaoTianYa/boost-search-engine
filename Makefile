PARSER=parser
DUG=debug
HTTP_SERVER=http_server
cc=g++
.PHONY:all
all:$(PARSER) $(DUG) $(HTTP_SERVER)

$(PARSER):parser.cc
	$(cc) -o $@ $^ -std=c++11 -lboost_system -lboost_filesystem
$(DUG):debug.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp -lboost_system -lboost_filesystem
$(HTTP_SERVER):http_server.cc
	$(cc) -o $@ $^ -std=c++11 -ljsoncpp -lboost_system -lboost_filesystem

.PHONY:clean
clean:
	rm -f $(PARSER) $(DUG) $(HTTP_SERVER)