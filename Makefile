CHECKDIR = /usr/bin/sqlite3

CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp iBot.cpp Connector.cpp SQLLogging.cpp ConfLoader.cpp MsgParser.cpp -lsqlite3
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=retroBot

all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *o $(EXECUTABLE)


install:
	@if [ ! -f $(CHECKDIR) ]; \
  then \
  mkdir tmp; \
  cd tmp; \
  wget http://www.sqlite.org/2013/sqlite-autoconf-3071700.tar.gz; \
  tar xfvz sqlite-autoconfig-3071700.tar.gz; \
  ./sqlite-autoconfig-307100/configure; \
  sudo make install; \
  cd..;\
  sudo rm tmp;\
  echo "install SQLITE3 in $(CHECKDIR)"; \
	else \
  echo "SQLITE3 is already install";\
 fi
