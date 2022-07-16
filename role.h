#ifndef ROLE_H
#define ROLE_H

#include "snowflake.h"
#include "state.h"

typedef struct discord_role {
    discord_state *state;
    json_object *raw_object;

    snowflake id;
    const char *name;
} discord_role;

#endif
