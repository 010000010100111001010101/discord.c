PROG = discordc
SRCS = main.c \
       discord.c state.c snowflake.c http.c gateway.c \
       member.c message.c embed.c \
       user.c channel.c \
       c-utils/json_utils.c c-utils/log.c c-utils/str.c c-utils/list.c c-utils/map.c c-utils/hashers/spooky.c
OBJS = $(SRCS:.c=.o)
IGNORE = -Wno-implicit-fallthrough -Wno-pointer-to-int-cast
DEBUGFLAGS = -Og -ggdb -DDEBUG -fsanitize=address

CFLAGS = -std=c18 -pedantic -Wall -Wextra -Werror $(IGNORE) $(DEBUGFLAGS)
INCLUDES = -I/usr/local/include -I/usr/include -I./c-utils

LDFLAGS = -L/usr/local/lib -L/usr/lib -L.
LDLIBS = -lpthread -lcurl -ljson-c -lwebsockets

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROG) $(OBJS) $(LDFLAGS) $(LDLIBS)

lib$(PROG).so: $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o lib$(PROG).so -fPIC -shared $(SRCS) $(LDFLAGS) $(LDLIBS) 

.PHONY: clean
clean:
	rm -rf $(PROG) $(OBJS) *.o *.so *.core vgcore.*
