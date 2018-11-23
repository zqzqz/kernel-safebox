CC = gcc
XX = g++
CFLAGS = -Wall -O -g -Wwrite-strings
LIBS = -lreadline -lncurses -lcrypto++
TARGET = ./client

%.o: %.c
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

%.o:%.cpp
	$(XX) $(CFLAGS) $(LIBS) -c $< -o $@

SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))

$(TARGET) : $(OBJS)
	$(XX) $(OBJS) $(CFLAGS) $(LIBS) -o $(TARGET)
	chmod a+x $(TARGET)

clean:
	rm -rf *.o client