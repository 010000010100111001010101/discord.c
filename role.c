#include "role.h"

#include "log.h"

static const logctx *logger = NULL;

discord_role *role_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] role_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    discord_role *role = calloc(1, sizeof(*role));

    if (!role){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] role_init() - role alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    role->state = state;
    role->raw_object = json_object_get(data);

    return role;
}

void role_free(void *roleptr){
    discord_role *role = roleptr;

    if (!role){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] role_free() - role is NULL\n",
            __FILE__
        );

        return;
    }

    json_object_put(role->raw_object);

    free(role);
}
