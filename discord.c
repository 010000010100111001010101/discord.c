#include "discord.h"

/*
 * logger is global
 * all extern logger vars are const
 */
const logctx *logger = NULL;

discord *discord_init(const char *token, const discord_options *opts){
    logger = opts ? opts->log : NULL;

    if (!token){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_init() - a bot token is required\n",
            __FILE__
        );

        return NULL;
    }

    discord *client = calloc(1, sizeof(*client));

    if (!client){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_init() - calloc for client failed\n",
            __FILE__
        );

        return NULL;
    }

    discord_state_options sopts = {0};
    discord_gateway_options gopts = {0};

    if (opts){
        sopts.log = opts->log;
        sopts.intent = opts->intent;
        sopts.presence = opts->presence;

        gopts.compress = opts->compress;
        gopts.large_threshold = opts->large_threshold;
        gopts.events = opts->events;
    }

    client->state = state_init(token, &sopts);

    if (!client->state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_init() - state_init call failed\n",
            __FILE__
        );

        discord_free(client);

        return NULL;
    }

    client->state->event_context = client;
    client->state->user_pointer = (void *)&client->user;

    client->gateway = gateway_init(client->state, &gopts);

    if (!client->gateway){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_init() - gateway initialization failed\n",
            __FILE__
        );

        discord_free(client);

        return NULL;
    }

    return client;
}

bool discord_connect_gateway(discord *client){
    if (!client){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_connect_gateway() - client is NULL\n",
            __FILE__
        );

        return false;
    }

    if (!gateway_connect(client->gateway)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_connect_gateway() - gateway_connect call failed\n",
            __FILE__
        );

        return false;
    }

    return gateway_run_loop(client->gateway);
}

void discord_disconnect_gateway(discord *client){
    if (!client){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_disconnect_gateway() - client is NULL\n",
            __FILE__
        );

        return;
    }

    gateway_disconnect(client->gateway);
}

bool discord_set_presence(discord *client, const discord_gateway_presence *presence){
    if (!client){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_set_presence() - client is NULL\n",
            __FILE__
        );

        return false;
    }

    bool success = false;

    if (presence){
        success = state_set_gateway_presence(
            client->state,
            &presence->since,
            presence->activities,
            presence->status,
            &presence->afk
        );
    }
    else {
        success = state_set_gateway_presence(
            client->state,
            NULL,
            NULL,
            NULL,
            NULL
        );
    }

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_set_presence() - state_set_gateway_presence call failed\n",
            __FILE__
        );

        return false;
    }

    success = gateway_send(
        client->gateway,
        GATEWAY_OP_PRESENCE_UPDATE,
        state_get_gateway_presence(client->state)
    );

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_set_presence() - gateway_send call failed\n",
            __FILE__
        );
    }

    return success;
}

bool discord_send_message(discord *client, snowflake channelid, const discord_message_reply *message){
    if (!client){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_send_message() - client is NULL\n",
            __FILE__
        );

        return false;
    }

    json_object *data = message_reply_to_json(message);

    if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_send_message() - message_reply_to_json call failed\n",
            __FILE__
        );

        return false;
    }

    discord_http_response *res = http_create_message(client->state->http, channelid, data);

    json_object_put(data);

    bool success = true;

    if (!res){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] discord_send_message() - http_create_message call failed\n",
            __FILE__
        );

        success = false;
    }
    else if (res->status != 200){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] discord_send_message() - request failed: %s\n",
            __FILE__,
            json_object_to_json_string(res->data)
        );
    }

    http_response_free(res);

    return success;
}

void discord_free(discord *client){
    if (!client){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] discord_free() - client is NULL\n",
            __FILE__
        );

        return;
    }

    gateway_free(client->gateway);
    state_free(client->state);

    free(client);
}
