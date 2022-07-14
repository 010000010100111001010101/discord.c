#ifndef ROLE_H
#define ROLE_H

#include "snowflake.h"

typedef struct discord_role {
    snowflake id;
    char name[101];
} discord_role;

#endif
