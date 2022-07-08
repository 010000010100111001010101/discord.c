discord.c
=========
Discord library written in C that depends on libcurl, libwebsockets, and json-c.

Currently supports:
    - HTTP API requests to *all* endpoints
    - gateway connection with event callbacks (using the default libwebsockets event loop)
    - rate limit handling for both the HTTP API and the gateway connection
    - reconnect logic (read notes)
    - cache of gateway and HTTP API data (in progress -- currently pretty useless)

Planned support:
    - voice support (after most of the API is covered)
    - abstraction of HTTP API endpoints (in progress) but ``http_request`` is available anyway
 
Not currently planned to support:
    - interactions/slash commands as I can't use them
    - sharding since I don't have a bot in a large amount of servers and can't reliably test new code

NOTES
-----
- Current test environment is limited so the library *does have* memory leaks and bugs. I'm fixing them as I find them.
- Makefile is set up to compile as a program rather than a library (this is for testing only since the library is very new)
- Reconnect logic is stable but will try to infinitely reconnect unless an error is hit. This is low priority since the idea is to keep the bot running but it will be "fixed" eventually
- The HTTP API can be used without ever connecting to the gateway. This is because I sometimes need to send messages from the terminal without eating memory with a gateway connection.
- A lot of the code is probably written horrendously but it functions and it holds (and resumes) a connection reliably which is what I need. This is inspired heavily by discord.py but C is a much different beast.

Example
=======
.. code :: c

    #include "discord.h"
    #include <stdio.h>

    static bool on_ready(void *clientptr, const void *data){
        discord *client = clientptr;
        const discord_user *user = data;

        printf("%s#%s has logged in\n", user->username, user->discriminator);

        return true;
    }

    static const discord_gateway_events callbacks[] = {
        {"READY", on_ready},

        {NULL, NULL}
    }

    int main(void){
        discord_gateway_presence presence = {0};
        presence.status = "dnd";

        discord_options opts = {0};
        opts.presence = &presence;
        opts.intent = INTENT_ALL;
        opts.events = callbacks;

        discord *bot = discord_init("token", &opts);

        discord_connect_gateway(bot); /* blocks */

        discord_free(bot);
    }
