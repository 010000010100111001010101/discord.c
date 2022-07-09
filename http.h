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
discord_http_response *http_get_channel(discord_http *, snowflake);

/* Message */
discord_http_response *http_create_message(discord_http *, snowflake, json_object *);
discord_http_response *http_edit_message(discord_http *, snowflake, snowflake, json_object *);
discord_http_response *http_delete_message(discord_http *, const char *, snowflake, snowflake);
discord_http_response *http_bulk_delete_messages(discord_http *, const char *, snowflake, json_object *);

void http_response_free(discord_http_response *);
void http_free(discord_http *);

#endif
