#include "member.h"

static const logctx *logger = NULL;

discord_member *member_init(discord_state *state, json_object *data){
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

    /* parse json */

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
