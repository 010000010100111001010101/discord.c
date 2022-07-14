#include "member.h"

#include "str.h"

static const logctx *logger = NULL;

static bool construct_member(discord_member *member, json_object *data){
    bool success = true;

    struct json_object_iterator curr = json_object_iter_begin(data);
    struct json_object_iterator end = json_object_iter_end(data);

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

        if (!strcmp(key, "user")){
            member->user = state_set_user(
                member->state,
                valueobj
            );

            success = member->user;
        }
        else if (!strcmp(key, "nick")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->nick,
                sizeof(member->nick)
            );
        }
        else if (!strcmp(key, "avatar")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->avatar,
                sizeof(member->avatar)
            );
        }
        else if (!strcmp(key, "roles")){
            if (member->roles){
                list_empty(member->roles);
            }
            else {
                member->roles = list_init();

                if (!member->roles){
                    log_write(
                        logger,
                        LOG_ERROR,
                        "[%s] construct_member() - roles initialization failed\n",
                        __FILE__
                    );

                    success = false;

                    break;
                }
            }

            for (size_t index = 0; index < json_object_array_length(valueobj); ++index){
                json_object *obj = json_object_array_get_idx(valueobj, index);
                const char *idstr = json_object_get_string(obj);

                snowflake id = 0;

                success = snowflake_from_string(idstr, &id);

                if (!success){
                    log_write(
                        logger,
                        LOG_WARNING,
                        "[%s] construct_member() - snowflake_from_string call failed\n",
                        __FILE__
                    );

                    break;
                }

                list_item item = {0};
                item.type = L_TYPE_GENERIC;
                item.size = sizeof(id);
                item.data_copy = &id;

                success = list_append(member->roles, &item);

                if (!success){
                    log_write(
                        logger,
                        LOG_ERROR,
                        "[%s] construct_member() - list_append call failed\n",
                        __FILE__
                    );

                    break;
                }
            }
        }
        else if (!strcmp(key, "joined_at")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->joined_at,
                sizeof(member->joined_at)
            );
        }
        else if (!strcmp(key, "premium_since")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->premium_since,
                sizeof(member->premium_since)
            );
        }
        else if (!strcmp(key, "deaf")){
            member->deaf = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "mute")){
            member->mute = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "pending")){
            member->pending = json_object_get_boolean(valueobj);
        }
        else if (!strcmp(key, "permissions")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->permissions,
                sizeof(member->permissions)
            );
        }
        else if (!strcmp(key, "communication_disabled_until")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                member->communication_disabled_until,
                sizeof(member->communication_disabled_until)
            );
        }

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] construct_member() - failed to set %s with value: %s\n",
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

discord_member *member_init(discord_state *state, const discord_user *user, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] member_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] member_init() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    discord_member *member = calloc(1, sizeof(*member));

    if (!member){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] member_init() - member object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    member->state = state;
    member->user = user;

    if (!construct_member(member, data)){
        member_free(member);

        return NULL;
    }

    return member;
}

void member_free(void *memberptr){
    discord_member *member = memberptr;

    if (!member){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] member_free() - member is NULL\n",
            __FILE__
        );

        return;
    }

    list_free(member->roles);

    free(member);
}
