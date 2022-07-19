#ifndef GUILD_H
#define GUILD_H

#include "state.h"

typedef enum discord_guild_verification_level {
    GUILD_VERIFICATION_NONE = 0,
    GUILD_VERIFICATION_LOW = 1,
    GUILD_VERIFICATION_MEDIUM = 2,
    GUILD_VERIFICATION_HIGH = 3,
    GUILD_VERIFICATION_VERY_HIGH = 4
} discord_guild_verification_level;

typedef enum discord_guild_notification_level {
    GUILD_NOTIFICATION_ALL_MESSAGES = 0,
    GUILD_NOTIFICATION_ONLY_MENTIONS = 1
} discord_guild_notification_level;

typedef enum discord_guild_filter_level {
    GUILD_FILTER_DISABLED = 0,
    GUILD_FILTER_MEMBERS_WITHOUT_ROLES = 1,
    GUILD_FILTER_ALL_MEMBERS = 2
} discord_guild_filter_level;

typedef enum discord_guild_nsfw_level {
    GUILD_NSFW_DEFAULT = 0,
    GUILD_NSFW_EXPLICIT = 1,
    GUILD_NSFW_SAFE = 2,
    GUILD_NSFW_AGE_RESTRICTED = 3
} discord_guild_nsfw_level;

typedef struct discord_guild {
    discord_state *state;
    json_object *raw_object;

    snowflake id;
    const char *name;
    const char *icon;
    const char *icon_hash;
    const char *splash;
    const char *discovery_splash;
    bool owner;
    snowflake owner_id;
    const char *permissions;
    const char *region;
    snowflake afk_channel_id;
    int afk_timeout;
    bool widget_enabled;
    snowflake widget_channel_id;
    discord_guild_verification_level verification_level;
    discord_guild_notification_level default_message_notifications;
    discord_guild_filter_level explicit_content_filter;
    map *roles;
    list *emojis;
    
} discord_guild;

bool test(void);

#endif
