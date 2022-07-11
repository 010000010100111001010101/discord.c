#ifndef MEMBER_H
#define MEMBER_H

//#include "roles.h"
#include "state.h"
#include "user.h"

#include <json-c/json.h>

typedef struct discord_member {
    discord_state *state;

    const discord_user *user;
    char nick[34];
    char avatar[64];
    list *roles;
    time_t joined_at;
    time_t premium_since;
    bool deaf;
    bool mute;
    bool pending;
    char permissions[128];
    bool communication_disabled_until;
} discord_member;

discord_member *member_init(discord_state *, json_object *);

void member_free(void *);

#endif
