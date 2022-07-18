#ifndef EMBED_H
#define EMBED_H

#include "state.h"

#define DISCORD_EMBED_MAX_URL_LENGTH 256

typedef struct discord_embed_footer {
    char text[2049];
    char icon_url[DISCORD_EMBED_MAX_URL_LENGTH];
} discord_embed_footer;

typedef struct discord_embed_image {
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
    int height;
    int width;
} discord_embed_image;

typedef struct discord_embed_thumbnail {
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
    int height;
    int width;
} discord_embed_thumbnail;

typedef struct discord_embed_video {
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
    int height;
    int width;
} discord_embed_video;

typedef struct discord_embed_provider {
    char name[257];
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
} discord_embed_provider;

typedef struct discord_embed_author {
    char name[257];
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
    char icon_url[DISCORD_EMBED_MAX_URL_LENGTH];
} discord_embed_author;

typedef struct discord_embed_field {
    char name[257];
    char value[1025];
    bool display_inline;
} discord_embed_field;

typedef struct discord_embed {
    discord_state *state;

    char title[257];
    char *description;
    char url[DISCORD_EMBED_MAX_URL_LENGTH];
    char timestamp[33];
    int color;
    discord_embed_footer *footer;
    discord_embed_image *image;
    discord_embed_thumbnail *thumbnail;
    discord_embed_video *video;
    discord_embed_provider *provider;
    discord_embed_author *author;
    list *fields;
} discord_embed;

list *embed_list_from_json_array(discord_state *, json_object *);
discord_embed *embed_init(discord_state *, json_object *);

size_t embed_get_length(const discord_embed *);
json_object *embed_to_json(const discord_embed *);

bool embed_set_title(discord_embed *, const char *);
bool embed_set_description(discord_embed *, const char *);
bool embed_set_url(discord_embed *, const char *);
bool embed_set_timestamp(discord_embed *, const char *);
bool embed_set_color(discord_embed *, int);

bool embed_set_footer(discord_embed *, const char *, const char *);
bool embed_set_image(discord_embed *, const char *, int, int);
bool embed_set_thumbnail(discord_embed *, const char *, int, int);
bool embed_set_video(discord_embed *, const char *, int, int);
bool embed_set_provider(discord_embed *, const char *, const char *);
bool embed_set_author(discord_embed *, const char *, const char *, const char *);

bool embed_add_field(discord_embed *, const char *, const char *, bool);
bool embed_remove_field(discord_embed *, size_t);

void embed_free(void *);

#endif
