#include "user.h"

#include "log.h"
#include "str.h"

static const logctx *logger = NULL;

static bool set_user_id(discord_user *user, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "id")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_user_id() - failed to get id from data object\n",
            __FILE__
        );

        return false;
    }

    bool success = snowflake_from_string(objstr, &user->id);

    if (!success){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_user_id() - snowflake_from_string call failed\n",
            __FILE__
        );
    }

    return success;
}

static bool set_user_name(discord_user *user, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "username")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_user_name() - failed to get username from data object\n",
            __FILE__
        );

        return false;
    }

    return string_copy(objstr, user->username, sizeof(user->username));
}

static bool set_user_discriminator(discord_user *user, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "discriminator")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_user_discriminator() - failed to get discriminator from data object\n",
            __FILE__
        );

        return false;
    }

    return string_copy(objstr, user->discriminator, sizeof(user->discriminator));
}

static bool set_user_avatar(discord_user *user, json_object *data){
    const char *objstr = json_object_get_string(
        json_object_object_get(data, "avatar")
    );

    if (!objstr){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] set_user_avatar() - failed to get avatar from data object\n",
            __FILE__
        );

        return false;
    }

    return string_copy(objstr, user->avatar, sizeof(user->avatar));
}

discord_user *user_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    if (!data){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    discord_user *user = calloc(1, sizeof(*user));

    if (!user){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - user object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    user->state = state;

    if (!set_user_id(user, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - set_user_id call failed\n",
            __FILE__
        );

        user_free(user);

        return NULL;
    }

    if (!set_user_name(user, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - set_user_name call failed\n",
            __FILE__
        );

        user_free(user);

        return NULL;
    }

    if (!set_user_discriminator(user, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - set_user_discriminator call failed\n",
            __FILE__
        );

        user_free(user);

        return NULL;
    }

    if (!set_user_avatar(user, data)){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] user_init() - set_user_avatar call failed\n",
            __FILE__
        );

        user_free(user);

        return NULL;
    }

    const char *objstr = NULL;

    user->bot = json_object_get_boolean(json_object_object_get(data, "bot"));
    user->system = json_object_get_boolean(json_object_object_get(data, "system"));
    user->mfa_enabled = json_object_get_boolean(json_object_object_get(data, "mfa_enabled"));

    objstr = json_object_get_string(json_object_object_get(data, "banner"));

    if (objstr){
        string_copy(objstr, user->banner, sizeof(user->banner));
    }

    user->accent_color = json_object_get_int(json_object_object_get(data, "accent_color"));

    objstr = json_object_get_string(json_object_object_get(data, "locale"));

    if (objstr){
        string_copy(objstr, user->locale, sizeof(user->locale));
    }

    user->verified = json_object_get_boolean(json_object_object_get(data, "verified"));

    objstr = json_object_get_string(json_object_object_get(data, "email"));

    if (objstr){
        string_copy(objstr, user->email, sizeof(user->email));
    }

    user->flags = json_object_get_int(json_object_object_get(data, "flags"));
    user->premium_type = json_object_get_int(json_object_object_get(data, "premium_type"));
    user->public_flags = json_object_get_int(json_object_object_get(data, "public_flags"));

    return user;
}

time_t user_get_creation_time(const discord_user *user){
    return snowflake_get_creation_time(user->id);
}

void user_free(void *userptr){
    discord_user *user = userptr;

    if (!user){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] user_free() - user is NULL\n",
            __FILE__
        );

        return;
    }

    free(user);
}
