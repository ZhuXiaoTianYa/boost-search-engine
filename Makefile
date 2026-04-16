PARSER=parser
DUG=debug
HTTP_SERVER=http_server
cc=g++

SRCDIR=src/

.PHONY:all
all:$(PARSER) $(DUG) $(HTTP_SERVER)

$(PARSER):$(SRCDIR)parser.cc
	$(cc) -o $@ $^ -std=c++11 -lboost_system -lboost_filesystem
$(DUG):$(SRCDIR)debug.cc
	$(cc) -o $@ $^ -std=c++11 /lib/x86_64-linux-gnu/libjsoncpp.so.25 -lboost_system -lboost_filesystem
$(HTTP_SERVER):$(SRCDIR)http_server.cc
	$(cc) -o $@ $^ -std=c++11 /lib/x86_64-linux-gnu/libjsoncpp.so.25 -lboost_system -lboost_filesystem 	

.PHONY:clean
clean:
	rm -f $(PARSER) $(DUG) $(HTTP_SERVER) nohup.out