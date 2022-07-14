#ifndef MEMBER_H
#define MEMBER_H

//#include "role.h"
#include "state.h"
#include "user.h"

#include <json-c/json.h>

typedef struct discord_member {
    discord_state *state;

    const discord_user *user;
    char nick[34];
    char avatar[64];
    list *roles;
    char joined_at[33];
    char premium_since[33];
    bool deaf;
    bool mute;
    bool pending;
    char permissions[128];
    char communication_disabled_until[33];
} discord_member;

discord_member *member_init(discord_state *, const discord_user *, json_object *);

void member_free(void *);

#endif
