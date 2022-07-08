#include "state.h"

#include "json_utils.h"

static const logctx *logger = NULL;

discord_state *state_init(const char *token, const discord_state_options *opts){
    if (!token){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - token is NULL -- refusing to initialize\n",
            __FILE__
        );

        return NULL;
    }

    discord_state *state = calloc(1, sizeof(*state));

    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - state object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    state->token = token;

    if (opts){
        logger = opts->log;

        state->log = opts->log;
        state->intent = opts->intent;

        if (opts->presence){
            bool success = state_set_gateway_presence(
                state,
                &opts->presence->since,
                opts->presence->activities,
                opts->presence->status,
                &opts->presence->afk
            );

            if (!success){
                log_write(
                    logger,
                    LOG_ERROR,
                    "[%s] state_init() - state_set_gateway_presence call failed\n",
                    __FILE__
                );

                state_free(state);

                return NULL;
            }
        }
    }

    state->http = http_init(state->token, NULL);

    if (!state->http){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - http initialization failed\n",
            __FILE__
        );

        state_free(state);

        return NULL;
    }

    state->users = map_init();

    if (!state->users){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - users cache map initialization failed\n",
            __FILE__
        );

        state_free(state);

        return NULL;
    }

    state->channels = map_init();

    if (!state->channels){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - channels cache map initialization failed\n",
            __FILE__
        );

        state_free(state);

        return NULL;
    }

    state->guilds = map_init();

    if (!state->guilds){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - guilds cache map initialization failed\n",
            __FILE__
        );

        state_free(state);

        return NULL;
    }

    state->messages = map_init();

    if (!state->messages){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_init() - messages cache map initialization failed\n",
            __FILE__
        );

        state_free(state);

        return NULL;
    }

    return state;
}

json_object *state_get_gateway_presence(discord_state *state){
    return state->presence;
}

const char *state_get_gateway_presence_string(discord_state *state){
    return state->presence ? json_object_to_json_string(state->presence) : "null";
}

bool state_set_gateway_presence(discord_state *state, const time_t *since, const list *activities, const char *status, const bool *afk){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] state_set_gateway_presence() - state is NULL\n",
            __FILE__
        );

        return false;
    }

    log_write(
        logger,
        LOG_DEBUG,
        "[%s] state_set_gateway_presence() - updating gateway presence state\n",
        __FILE__
    );

    json_object *presenceobj = state->presence ? state->presence : json_object_new_object();

    if (!presenceobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_gateway_presence() - presence object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = NULL;

    if (since){
        obj = json_object_new_int64(*since);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] state_set_gateway_presence() - since object initialization failed\n",
                __FILE__
            );

            json_object_put(presenceobj);

            return false;
        }
    }
    else {
        obj = json_object_object_get(presenceobj, "since");
    }

    json_object_object_add(presenceobj, "since", obj);

    if (activities){
        obj = list_to_json_array(activities);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] state_set_gateway_presence() - activities object initialization failed\n",
                __FILE__
            );

            json_object_put(presenceobj);

            return false;
        }
    }
    else {
        obj = json_object_object_get(presenceobj, "activities");
    }

    json_object_object_add(presenceobj, "activities", obj);

    if (status){
        obj = json_object_new_string(status);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] state_set_gateway_presence() - status object initialization failed\n",
                __FILE__
            );

            json_object_put(presenceobj);

            return false;
        }
    }
    else {
        obj = json_object_object_get(presenceobj, "status");
    }

    json_object_object_add(presenceobj, "status", obj);

    if (afk){
        obj = json_object_new_boolean(*afk);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] state_set_gateway_presence() - afk object initialization failed\n",
                __FILE__
            );

            json_object_put(presenceobj);

            return false;
        }
    }
    else {
        obj = json_object_object_get(presenceobj, "afk");
    }

    json_object_object_add(presenceobj, "afk", obj);

    state->presence = presenceobj;

    return true;
}

const discord_user *state_set_user(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_user() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }
    else if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_user() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    const char *idstr = json_object_get_string(
        json_object_object_get(data, "id")
    );

    if (!idstr){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_user() - failed to get id from user object\n",
            __FILE__
        );

        return NULL;
    }

    snowflake id = 0;
    bool success = snowflake_from_string(idstr, &id);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_user() - snowflake_from_string call failed\n",
            __FILE__
        );

        return NULL;
    }

    const discord_user *cached = state_get_user(state, id);

    if (cached){
        return cached;
    }

    discord_user *user = user_init(state, data);

    if (!user){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_set_user() - user_init call failed\n",
            __FILE__
        );

        return NULL;
    }

    map_item k = {0};
    k.type = M_TYPE_GENERIC;
    k.size = sizeof(user->id);
    k.data_copy = &user->id;

    map_item v = {0};
    v.type = M_TYPE_GENERIC;
    v.size = sizeof(*user);
    v.data = user;
    v.generic_free = user_free;

    success = map_set(state->users, &k, &v);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - map_set call for user cache failed\n",
            __FILE__
        );

        user_free(user);

        return NULL;
    }

    return user;
}

const discord_user *state_get_user(discord_state *state, snowflake id){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] state_get_user() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    size_t idsize = sizeof(id);

    if (!map_contains(state->users, idsize, &id)){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] state_get_user() - %" PRIu64 " not found in state->users map\n",
            __FILE__,
            id
        );

        return NULL;
    }

    return map_get_generic(state->users, idsize, &id);
}

void state_free(discord_state *state){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] state_free() - state is NULL\n",
            __FILE__
        );

        return;
    }

    map_free(state->users);
    map_free(state->channels);
    map_free(state->guilds);
    map_free(state->messages);

    http_free(state->http);

    user_free(state->user);
    json_object_put(state->presence);

    free(state);
}
