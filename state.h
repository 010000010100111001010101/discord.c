#ifndef STATE_H
#define STATE_H

#include "log.h"
#include "map.h"

#include "http.h"
#include "user.h"

#include <json-c/json.h>

#define DISCORD_LIBRARY_NAME "discord.c"
#define DISCORD_LIBRARY_VERSION "null"
#define DISCORD_LIBRARY_OS "Linux"

#define DISCORD_API "https://discord.com/api/v10"
#define DISCORD_CDN "https://cdn.discordapp.com"
#define DISCORD_USER_AGENT ("DiscordBot (" DISCORD_LIBRARY_NAME ", " DISCORD_LIBRARY_VERSION ")")

#define DISCORD_GATEWAY_VERSION 9
#define DISCORD_GATEWAY_PORT 443
#define DISCORD_GATEWAY_ENCODING "json"
#define DISCORD_GATEWAY_IDENTIFY_LIMIT 1000
#define DISCORD_GATEWAY_HEARTBEAT_JITTER 0.5
#define DISCORD_GATEWAY_RATE_LIMIT_INTERVAL 60
#define DISCORD_GATEWAY_RATE_LIMIT_COUNT 110
#define DISCORD_GATEWAY_LWS_LOG_LEVEL (LLL_ERR | LLL_WARN | LLL_INFO | LLL_DEBUG)

typedef struct discord_http discord_http;
typedef struct discord_user discord_user;

typedef enum discord_gateway_intents {
    INTENT_GUILDS = 1,
    INTENT_GUILD_MEMBERS = 2,
    INTENT_GUILD_BANS = 4,
    INTENT_GUILD_EMOJIS_AND_STICKERS = 8,
    INTENT_GUILD_INTEGRATIONS = 16,
    INTENT_GUILD_WEBHOOKS = 32,
    INTENT_GUILD_INVITES = 64,
    INTENT_GUILD_VOICE_STATES = 128,
    INTENT_GUILD_PRESENCES = 256,
    INTENT_GUILD_MESSAGES = 512,
    INTENT_GUILD_MESSAGE_REACTIONS = 1024,
    INTENT_GUILD_MESSAGE_TYPING = 2048,
    INTENT_DIRECT_MESSAGES = 4096,
    INTENT_DIRECT_MESSAGE_REACTIONS = 8192,
    INTENT_DIRECT_MESSAGE_TYPING = 16384,
    INTENT_MESSAGE_CONTENT = 32768,
    INTENT_GUILD_SCHEDULED_EVENTS = 65536,

    INTENT_ALL = 131071
} discord_gateway_intents;

typedef struct discord_gateway_presence {
    time_t since;
    const list *activities;
    const char *status;
    bool afk;
} discord_gateway_presence;

typedef struct discord_state_options {
    const logctx *log;
    discord_gateway_intents intent;
    const discord_gateway_presence *presence;
} discord_state_options;

typedef struct discord_state {
    const char *token;

    const logctx *log;
    discord_gateway_intents intent;
    void *event_context;
    void **user_pointer;

    json_object *presence;
    discord_user *user;
    discord_http *http;

    map *users;
    map *channels;
    map *guilds;
    map *messages;
} discord_state;

discord_state *state_init(const char *, const discord_state_options *);

json_object *state_get_gateway_presence(discord_state *);
const char *state_get_gateway_presence_string(discord_state *);
bool state_set_gateway_presence(discord_state *, const time_t *, const list *, const char *, const bool *);

const discord_user *state_set_user(discord_state *, json_object *);
const discord_user *state_get_user(discord_state *, snowflake);

void state_free(discord_state *);

#endif
