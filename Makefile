PROG = discordc
SRCS = $(wildcard *.c) $(wildcard c-utils/*.c) c-utils/hashers/spooky.c
OBJS = $(SRCS:.c=.o)

EXTRAS = -Wshadow -Wundef -Wpointer-arith -Wfloat-equal -Wcast-align \
         -Wstrict-prototypes -Wwrite-strings -Waggregate-return \
         -Wswitch-default -Wunreachable-code -Wformat=2

IGNORE = -Wno-implicit-fallthrough -Wno-pointer-to-int-cast \
         -Wno-format-nonliteral

DEBUGFLAGS = -Og -ggdb -DDEBUG -fsanitize=address,undefined

CFLAGS = -std=c18 -pedantic -Wall -Wextra -Werror $(EXTRAS) $(IGNORE) $(DEBUGFLAGS)
INCLUDES = -I/usr/local/include -I/usr/include -I. -I./c-utils

LDFLAGS = -L/usr/local/lib -L/usr/lib -L.
LDLIBS = -lpthread -lcurl -ljson-c -lwebsockets

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ -fPIC

lib$(PROG).so: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o lib$(PROG).so $(OBJS) $(LDFLAGS) -shared $(LDLIBS) 

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROG) $(OBJS) $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -rf $(PROG) $(OBJS) *.o *.so *.core vgcore.*
