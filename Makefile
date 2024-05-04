CC = gcc
CFLAGS = -Wall -Werror -g
LIBS = -lcurl

SRC = crawler.c bag.c hashtable.c set.c url.c pagedir.c curl.c
TARGET = crawler
OBJS = $(patsubst %.c, %.o, $(SRC))

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean