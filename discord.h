#ifndef DISCORD_H
#define DISCORD_H

#include "gateway.h"
#include "state.h"

typedef struct discord_options {
    const logctx *log;

    discord_gateway_intents intent;
    const discord_gateway_presence *presence;

    /* passthrough state options */
    size_t max_messages;

    /* passthrough gateway options */
    bool compress;
    int large_threshold;
    const discord_gateway_events *events;
} discord_options;

typedef struct discord {
    discord_state *state;

    discord_gateway *gateway;
    const discord_user *user;
} discord;

discord *discord_init(const char *, const discord_options *);

bool discord_connect_gateway(discord *);
void discord_disconnect_gateway(discord *);

bool discord_set_presence(discord *, const discord_gateway_presence *);

bool discord_send_message(discord *, snowflake, const discord_message_reply *);

void discord_free(discord *);

#endif
