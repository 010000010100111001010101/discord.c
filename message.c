#include "message.h"

#include "log.h"
#include "str.h"

static const logctx *logger = NULL;

static bool construct_message_activity(discord_message *message, json_object *data){
    message->activity = calloc(1, sizeof(*message->activity));

    if (!message->activity){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] construct_message_activity() - activity alloc failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_object_get(data, "type");
    message->activity->type = json_object_get_int(obj);

    obj = json_object_object_get(data, "party_id");
    message->activity->party_id = json_object_get_string(obj);

    return true;
}

static bool construct_message_reference(discord_message *message, json_object *data){
    message->reference = calloc(1, sizeof(*message->reference));

    if (!message->reference){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] construct_message_reference() - reference alloc failed\n",
            __FILE__
        );

        return false;
    }

    bool success = true;

    json_object *obj = json_object_object_get(data, "message_id");

    if (obj){
        const char *objstr = json_object_get_string(obj);

        success = snowflake_from_string(objstr, &message->reference->message_id);

        if (!success){
            return false;
        }
    }

    obj = json_object_object_get(data, "channel_id");

    if (obj){
        const char *objstr = json_object_get_string(obj);

        success = snowflake_from_string(objstr, &message->reference->channel_id);

        if (!success){
            return false;
        }
    }

    obj = json_object_object_get(data, "guild_id");

    if (obj){
        const char *objstr = json_object_get_string(obj);

        success = snowflake_from_string(objstr, &message->reference->guild_id);

        if (!success){
            return false;
        }
    }

    obj = json_object_object_get(data, "fail_if_not_exists");

    if (obj){
        message->reference->fail_if_not_exists = json_object_get_boolean(obj);
    }

    return success;
}

static bool construct_message(discord_message *message){
    bool success = true;

    struct json_object_iterator curr = json_object_iter_begin(message->raw_object);
    struct json_object_iterator end = json_object_iter_end(message->raw_object);

    while (!json_object_iter_equal(&curr, &end)){
        const char *key = json_object_iter_peek_name(&curr);
        json_object *valueobj = json_object_iter_peek_value(&curr);
        json_type type = json_object_get_type(valueobj);

        bool skip = false;

        if (!valueobj || type == json_type_null){
            skip = true;
        }
        else if (type == json_type_array){
            skip = !json_object_array_length(valueobj);
        }

        if (skip){
            json_object_iter_next(&curr);

            continue;
        }

        if (!strcmp(key, "id")){
            const char *objstr = json_object_get_string(valueobj);

            success = snowflake_from_string(objstr, &message->id);
        }
        else if (!strcmp(key, "channel_id")){
            const char *objstr = json_object_get_string(valueobj);

            success = snowflake_from_string(objstr, &message->channel_id);
        }
        else if (!strcmp(key, "guild_id")){
            const char *objstr = json_object_get_string(valueobj);

            success = snowflake_from_string(objstr, &message->guild_id);
        }
        else if (!strcmp(key, "author")){
            message->author = state_set_user(message->state, valueobj);

            success = message->author;
        }
        else if (!strcmp(key, "member")){
            if (message->member){
                member_free(message->member);

                message->member = NULL;
            }

            message->member = member_init(message->state, valueobj);

            success = message->member;
        }
        else if (!strcmp(key, "content")){
            message->content = json_object_get_string(valueobj);
        }
        else if (!strcmp(key, "timestamp")){
            /* i'd like to convert this to a time_t */
            message->timestamp = json_object_get_string(valueobj);
        }
        else if (!strcmp(key, "edited_timestamp")){
            /* same as above */
            message->edited_timestamp = json_object_get_string(valueobj);
        }
        else if (!strcmp(key, "tts")){
            message->tts = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "mention_everyone")){
            message->mention_everyone = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "mentions")){
            // mention.c
        }
        else if (!strcmp(key, "mention_roles")){
            // mention.c
        }
        else if (!strcmp(key, "mention_channels")){
            // mention.c
        }
        else if (!strcmp(key, "attachments")){
            // attachment.c ???
        }
        else if (!strcmp(key, "embeds")){
            message->embeds = embed_list_from_json_array(message->state, valueobj);

            success = message->embeds;
        }
        else if (!strcmp(key, "reactions")){
            // reaction.c ???
        }
        else if (!strcmp(key, "nonce")){
            message->nonce = json_object_get_int(valueobj);
        }
        else if (!strcmp(key, "pinned")){
            message->pinned = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "webhook_id")){
            const char *objstr = json_object_get_string(valueobj);

            success = snowflake_from_string(objstr, &message->webhook_id);
        }
        else if (!strcmp(key, "type")){
            message->type = json_object_get_int(valueobj);
        }
        else if (!strcmp(key, "activity")){
            if (message->activity){
                free(message->activity);

                message->activity = NULL;
            }

            success = construct_message_activity(message, valueobj);
        }
        else if (!strcmp(key, "application")){
            message->application = application_init(message->state, valueobj);

            success = message->application;
        }
        else if (!strcmp(key, "application_id")){
            const char *objstr = json_object_get_string(valueobj);

            success = snowflake_from_string(objstr, &message->application_id);
        }
        else if (!strcmp(key, "message_reference")){
            if (message->reference){
                free(message->reference);

                message->reference = NULL;
            }

            success = construct_message_reference(message, valueobj);
        }
        else if (!strcmp(key, "flags")){
            message->flags = json_object_get_int(valueobj);
        }
        else if (!strcmp(key, "referenced_message")){
            message->referenced_message = state_set_message(
                message->state,
                valueobj,
                false
            );

            success = message->referenced_message;
        }
        else if (!strcmp(key, "interaction")){
            // interaction.c ???
        }
        else if (!strcmp(key, "thread")){
            // state_set_channel(message->state, data, false); --- BOOKMARK ---
        }
        else if (!strcmp(key, "components")){
            // component.c ???
        }
        else if (!strcmp(key, "sticker_items")){
            // sticker.c
        }

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] construct_message() - failed to set %s with value: %s\n",
                __FILE__,
                key,
                json_object_to_json_string(valueobj)
            );

            break;
        }

        json_object_iter_next(&curr);
    }

    return success;
}

discord_message *message_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    if (!data){
        log_write(
            logger,
            LOG_WARNING,
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
            "[%s] message_init() - message alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    message->state = state;
    message->raw_object = json_object_get(data);

    if (!construct_message(message)){
        message_free(message);

        message = NULL;
    }

    return message;
}

bool message_update(discord_message *message, json_object *data){
    if (!message){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_update() - message is NULL\n",
            __FILE__
        );

        return false;
    }
    else if (!data){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_update() - data is NULL\n",
            __FILE__
        );

        return false;
    }

    if (!json_merge_objects(data, message->raw_object)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_update() - json_merge_objects call failed\n",
            __FILE__
        );

        return false;
    }

    return construct_message(message);
}

/* API calls */
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
        message->channel_id,
        message->id,
        reason
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

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_delete() - API request failed: %s\n",
            __FILE__,
            json_object_to_json_string(res->data)
        );
    }

    http_response_free(res);

    return success;
}

void message_free(void *messageptr){
    discord_message *message = messageptr;

    if (!message){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] message_free() - message is NULL\n",
            __FILE__
        );

        return;
    }

    json_object_put(message->raw_object);

    /* --- BOOKMARK --- add guild object, guild will have ownership of member */
    member_free(message->member);

    list_free(message->mentions);
    list_free(message->mention_roles);
    list_free(message->mention_channels);
    list_free(message->attachments);
    list_free(message->embeds);
    list_free(message->reactions);
    list_free(message->components);
    list_free(message->sticker_items);

    application_free(message->application);

    free(message->activity);
    free(message->reference);
    free(message);
}

/* message reply functions */

static bool set_reply_json_content(json_object *replyobj, const char *content){
    if (!content){
        return true;
    }

    json_object *obj = json_object_new_string(content);
    bool success = !json_object_object_add(replyobj, "content", obj);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_reply_json_content() - json_object_object_add call failed\n",
            __FILE__
        );
    }

    return success;
}

static bool set_reply_json_misc(json_object *replyobj, bool tts, int flags){
    json_object *obj = json_object_new_boolean(tts);
    bool success = !json_object_object_add(replyobj, "tts", obj);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_reply_json_misc() - json_object_object_add call failed for tts\n",
            __FILE__
        );

        return false;
    }

    obj = json_object_new_int(flags);
    success = !json_object_object_add(replyobj, "flags", obj);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_reply_json_misc() - json_object_object_add call failed for flags\n",
            __FILE__
        );
    }

    return success;
}

static bool set_reply_json_embeds(json_object *replyobj, const discord_embed *embed, const list *embeds){
    if (!embed && !embed){
        return true;
    }

    json_object *embedsobj = json_object_new_array();

    if (!embedsobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_reply_json_embeds() - embeds object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = NULL;
    bool success = false;

    if (embed){
        obj = embed_to_json(embed);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_reply_json_embeds() - embed_to_json call failed for embed\n",
                __FILE__
            );

            json_object_put(embedsobj);

            return false;
        }

        success = !json_object_array_add(embedsobj, obj);

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_reply_json_embeds() - json_object_array_add call failed for embed\n",
                __FILE__
            );

            json_object_put(embedsobj);

            return false;
        }
    }

    for (size_t index = 0; index < list_get_length(embeds); ++index){
        const discord_embed *embed = list_get_generic(embeds, index);
        obj = embed_to_json(embed);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_reply_json_embeds() - embed_to_json call failed\n",
                __FILE__
            );

            json_object_put(embedsobj);

            success = false;

            break;
        }

        success = json_object_array_add(embedsobj, obj);

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_reply_json_embeds() - json_object_array_add call failed\n",
                __FILE__
            );

            break;
        }
    }

    if (success){
        success = !json_object_object_add(replyobj, "embeds", embedsobj);

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_reply_json_embeds() - json_object_object_add call failed\n",
                __FILE__
            );

            json_object_put(embedsobj);
        }
    }

    return success;
}

/* --- BOOKMARK --- convert this to the constructor format */
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
    else if (!reply->content && !reply->embed && !reply->embeds && !reply->sticker_ids){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] message_reply_to_json() - one of content, file, embeds, sticker_ids required\n",
            __FILE__
        );

        return NULL;
    }

    json_object *replyobj = json_object_new_object();

    if (!replyobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] message_reply_to_json() - reply object initialization failed\n",
            __FILE__
        );

        return NULL;
    }

    if (!set_reply_json_content(replyobj, reply->content)){
        json_object_put(replyobj);

        return NULL;
    }

    if (!set_reply_json_misc(replyobj, reply->tts, reply->flags)){
        json_object_put(replyobj);

        return NULL;
    }

    if (!set_reply_json_embeds(replyobj, reply->embed, reply->embeds)){
        json_object_put(replyobj);

        return NULL;
    }

    /* support the rest */

    return replyobj;
}
