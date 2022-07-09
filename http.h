#ifndef HTTP_H
#define HTTP_H

#include "map.h"

#include "snowflake.h"
#include "state.h"

#include <json-c/json.h>

typedef enum http_method {
    HTTP_GET,
    HTTP_DELETE,
    HTTP_PATCH,
    HTTP_POST,
    HTTP_PUT
} http_method;

typedef struct discord_http_request_options {
    json_object *data;
    const char *reason;
} discord_http_request_options;

typedef struct discord_http_response {
    long status;
    map *headers;
    json_object *data;
} discord_http_response;

typedef struct discord_http_options {
    const logctx *log;
} discord_http_options;

typedef struct discord_http {
    const char *token;

    bool globalratelimit;
    map *buckets;
} discord_http;

discord_http *http_init(const char *, const discord_http_options *);

discord_http_response *http_request(discord_http *, http_method, const char *, const discord_http_request_options *);

/*
 * API calls by type
 */

/* General */
discord_http_response *http_get_gateway(discord_http *);
discord_http_response *http_get_bot_gateway(discord_http *);

/* User */
discord_http_response *http_get_current_user(discord_http *);
discord_http_response *http_get_user(discord_http *, snowflake);
discord_http_response *http_modify_current_user(discord_http *, const char *, const char *);
discord_http_response *http_get_current_user_guilds(discord_http *, snowflake, snowflake, int);
discord_http_response *http_get_current_user_guild_member(discord_http *, snowflake);
discord_http_response *http_leave_guild(discord_http *, snowflake);
discord_http_response *http_create_dm(discord_http *, snowflake);
//discord_http_response *http_create_group_dm(discord_http *, const json_object *);
discord_http_response *http_get_user_connections(discord_http *);

/* Channel */
discord_http_response *http_trigger_typing_indicator(discord_http *, snowflake);
discord_http_response *http_follow_news_channel(discord_http *, snowflake, json_object *);

discord_http_response *http_get_channel(discord_http *, snowflake);
discord_http_response *http_edit_channel(discord_http *, snowflake, json_object *, const char *);
discord_http_response *http_delete_channel(discord_http *, snowflake, const char *);

discord_http_response *http_edit_channel_permissions(discord_http *, snowflake, snowflake, json_object *, const char *);
discord_http_response *http_delete_channel_permission(discord_http *, snowflake, snowflake, const char *);

discord_http_response *http_get_channel_invites(discord_http *, snowflake);
discord_http_response *http_create_channel_invite(discord_http *, snowflake, json_object *, const char *);

discord_http_response *http_get_pinned_messages(discord_http *, snowflake);
discord_http_response *http_pin_message(discord_http *, snowflake, snowflake, const char *);
discord_http_response *http_unpin_message(discord_http *, snowflake, snowflake, const char *);

/* Message */
discord_http_response *http_get_channel_messages(discord_http *, snowflake, json_object *);
discord_http_response *http_get_channel_message(discord_http *, snowflake, snowflake);

discord_http_response *http_crosspost_message(discord_http *, snowflake, snowflake);

discord_http_response *http_create_message(discord_http *, snowflake, json_object *);
discord_http_response *http_edit_message(discord_http *, snowflake, snowflake, json_object *);
discord_http_response *http_delete_message(discord_http *, snowflake, snowflake, const char *);
discord_http_response *http_bulk_delete_messages(discord_http *, snowflake, json_object *, const char *);

discord_http_response *http_get_reactions(discord_http *, snowflake, snowflake, const char *);
discord_http_response *http_create_reaction(discord_http *, snowflake, snowflake, const char *);
discord_http_response *http_delete_own_reaction(discord_http *, snowflake, snowflake, const char *);
discord_http_response *http_delete_user_reaction(discord_http *, snowflake, snowflake, snowflake, const char *);
discord_http_response *http_delete_all_reactions(discord_http *, snowflake, snowflake);
discord_http_response *http_delete_all_reactions_for_emoji(discord_http *, snowflake, snowflake, const char *);

void http_response_free(discord_http_response *);
void http_free(discord_http *);

#endif
