discord.c
=========
Discord library written in C that depends on libcurl, libwebsockets, and json-c.

Currently supports:
    - HTTP API requests to *all* endpoints
    - gateway connection with event callbacks
    - rate limit handling for both the HTTP API and the gateway connection
    - reconnect logic (read notes)
    - cache of gateway and HTTP API data (in progress)

Planned support:
    - voice support (after most of the API is covered)
    - abstraction of HTTP API endpoints (in progress) but ``http_request`` is available anyway
    - abstraction of gateway event callbacks and data
 
Not currently planned to support:
    - interactions/slash commands as I don't use them and there's a lot of work left to support the rest of the API
    - sharding since I don't have a bot in a large amount of servers

The gateway connection is pretty barebones right now but is still extensible. The callback typedef:

``bool (*discord_gateway_event)(void *clientptr, const void *data)``

gets a ``void *`` containing a ``discord *`` object and the immutable ``json_object *`` data. The data pointer will eventually not be a ``json_object *`` but a generic pointer to whatever object (ex: ``const discord_message *`` for ``on_message_create``) is relevant in the event.

NOTES
-----
- Current test environment is limited so the library very likely has memory leaks and bugs
- Makefile is set up to compile as a program rather than a library (this is for testing only since the library is very new)
- Reconnect logic has been improved but will still attempt to infinitely reconnect if an error does not occur. This will be fixed up eventually.
