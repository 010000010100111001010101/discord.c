#include "channel.h"

static const logctx *logger = NULL;

discord_channel *channel_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] channel_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] channel_init() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    discord_channel *channel = calloc(1, sizeof(*channel));

    if (!channel){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] channel_init() - channel object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    channel->state = state;

    return channel;
}

void channel_free(discord_channel *channel){
    if (!channel){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] channel_free() - channel is NULL\n",
            __FILE__
        );

        return;
    }

    /* free other stuff */

    free(channel);
}
