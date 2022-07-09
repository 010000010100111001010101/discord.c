#include "message.h"

#include "log.h"
#include "str.h"

static const logctx *logger;

static bool set_message_id(discord_message *message, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "id")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_message_id() - failed to get id from data object\n",
            __FILE__
        );

        return false;
    }

    bool success = snowflake_from_string(objstr, &message->id);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_id() - snowflake_from_string call failed\n",
            __FILE__
        );
    }

    return success;
}

static bool set_message_channel_id(discord_message *message, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "channel_id")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_message_channel_id() - failed to get channel_id from data object\n",
            __FILE__
        );

        return false;
    }

    bool success = snowflake_from_string(objstr, &message->channel_id);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_channel_id() - snowflake_from_string call failed\n",
            __FILE__
        );
    }

    return success;
}

static bool set_message_author(discord_message *message, json_object *data){
    json_object *authorobj = json_object_object_get(data, "author");
    message->author = state_set_user(message->state, authorobj);

    if (!message->author){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_author() - state_set_user call failed\n",
            __FILE__
        );

        return false;
    }

    return true;
}

static bool set_message_guild_data(discord_message *message, json_object *data){
    json_object *guildidobj = json_object_object_get(data, "guild_id");
    json_object *memberobj = json_object_object_get(data, "member");

    if (!guildidobj && !memberobj){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] set_message_guild_data() - no guild information to set\n",
            __FILE__
        );

        return true;
    }

    bool success = snowflake_from_string(
        json_object_get_string(guildidobj),
        &message->guild_id
    );

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_guild_data() - snowflake_from_string call for guild_id failed\n",
            __FILE__
        );

        return false;
    }

    message->member = member_init(message->state, data);

    if (!message->member){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_guild_data() - member_init call failed\n",
            __FILE__
        );

        return false;
    }

    return true;
}

static bool set_message_content(discord_message *message, json_object *data){
    const char *contentstr = json_object_get_string(
        json_object_object_get(data, "content")
    );

    if (!contentstr){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_message_content() - failed to get content object from data\n",
            __FILE__
        );

        return false;
    }

    string_copy(contentstr, message->content, sizeof(message->content));

    return true;
}

static bool set_message_timestamp(discord_message *message, json_object *data){
    json_object *createdobj = json_object_object_get(data, "timestamp");
    json_object *editedobj = json_object_object_get(data, "edited_timestamp");

    /*
     * ISO8601 format so this needs work BOOKMARK
     */
    message->timestamp = createdobj ? 0 : 0;
    message->edited_timestamp = editedobj ? 0 : 0;

    return true;
}

discord_message *message_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    discord_message *message = calloc(1, sizeof(*message));

    if (!message){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - message object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    message->state = state;

    if (!set_message_id(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_id call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    if (!set_message_channel_id(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_channel_id call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    if (!set_message_author(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_author call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    if (!set_message_guild_data(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_guild_data call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    if (!set_message_content(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_content call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    if (!set_message_timestamp(message, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_init() - set_message_timestamp call failed\n",
            __FILE__
        );

        message_free(message);

        return NULL;
    }

    //const char *objstr = NULL;

    message->tts = json_object_get_boolean(json_object_object_get(data, "tts"));
    message->mention_everyone = json_object_get_boolean(json_object_object_get(data, "mention_everyone"));

    return message;
}

bool message_delete(const discord_message *message, const char *reason){
    if (!message){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_delete() - message is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_http_response *res = http_delete_message(
        message->state->http,
        reason,
        message->channel_id,
        message->id
    );

    if (!res){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_delete() - http_delete_message call failed\n",
            __FILE__
        );

        return false;
    }

    bool success = res->status == 204;

    http_response_free(res);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_delete() - API request failed: %s\n",
            __FILE__,
            json_object_to_json_string(res->data)
        );
    }

    return success;
}

void message_free(void *messageptr){
    discord_message *message = messageptr;

    if (!message){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_free() - message is NULL\n",
            __FILE__
        );

        return;
    }

    member_free(message->member);

    free(message);
}

/* BOOKMARK
 * this needs to be fixed up
 * maybe separate reply?
 */
json_object *message_reply_to_json(const discord_message_reply *reply){
    if (!reply){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_reply_to_json() - reply is NULL\n",
            __FILE__
        );

        return NULL;
    }
    else if (!reply->content && !reply->embeds && !reply->sticker_ids){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_reply_to_json() - must set one of content, file, embeds, sticker_ids\n",
            __FILE__
        );

        return NULL;
    }

    json_object *message = json_object_new_object();

    if (!message){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_reply_to_json() - message object initialization failed\n",
            __FILE__
        );

        return NULL;
    }

    json_object *obj = json_object_new_string(reply->content);

    if (obj){
        json_object_object_add(message, "content", obj);
    }

    obj = json_object_new_boolean(reply->tts);

    json_object_object_add(message, "tts", obj);

    obj = list_to_json_array(reply->embeds);

    if (obj){
        json_object_object_add(message, "embeds", obj);
    }

    if (reply->allowed_mentions){
        json_object *allowedmentions = json_object_new_object();

        if (!allowedmentions){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] message_reply_to_json() - allowed_mentions object initialization failed\n",
                __FILE__
            );

            json_object_put(message);

            return NULL;
        }

        obj = list_to_json_array(reply->allowed_mentions->parse);

        if (obj){
            json_object_object_add(allowedmentions, "parse", obj);
        }

        obj = list_to_json_array(reply->allowed_mentions->roles);

        if (obj){
            json_object_object_add(allowedmentions, "roles", obj);
        }

        obj = list_to_json_array(reply->allowed_mentions->users);

        if (obj){
            json_object_object_add(allowedmentions, "users", obj);
        }

        obj = json_object_new_boolean(reply->allowed_mentions->replied_user);

        if (obj){
            json_object_object_add(allowedmentions, "replied_user", obj);
        }

        json_object_object_add(message, "allowed_mentions", allowedmentions);
    }

    /*
    if (reply->message_reference){
        json_object *messagereference = json_object_new_object();

        if (!messagereference){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] message_reply_to_json() - message_reference object initialization failed\n",
                __FILE__
            );

            json_object_put(message);

            return NULL;
        }

        obj = 
    }
    */

    obj = list_to_json_array(reply->components);

    if (obj){
        json_object_object_add(message, "components", obj);
    }

    obj = list_to_json_array(reply->sticker_ids);

    if (obj){
        json_object_object_add(message, "sticker_ids", obj);
    }

    if (reply->payload_json){
        json_object_object_add(message, "payload_json", obj);
    }

    obj = list_to_json_array(reply->attachments);

    if (obj){
        json_object_object_add(message, "attachments", obj);
    }

    obj = json_object_new_int(reply->flags);

    json_object_object_add(message, "flags", obj);

    return message;
}
