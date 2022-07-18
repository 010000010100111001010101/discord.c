#include "embed.h"

static const logctx *logger = NULL;

/* embed json constructors */
static bool set_embed_json_footer(const discord_embed *embed, json_object *embedobj){
    if (!embed->footer){
        return true;
    }

    json_object *footerobj = json_object_new_object();

    if (!footerobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_footer() - footer object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->footer->text);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_footer() - text object initialization failed\n",
            __FILE__
        );

        json_object_put(footerobj);

        return false;
    }

    json_object_object_add(footerobj, "text", obj);

    obj = json_object_new_string(embed->footer->icon_url);

    json_object_object_add(footerobj, "icon_url", obj);

    json_object_object_add(embedobj, "footer", footerobj);

    return true;
}

static bool set_embed_json_image(const discord_embed *embed, json_object *embedobj){
    if (!embed->image){
        return true;
    }

    json_object *imageobj = json_object_new_object();

    if (!imageobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_image() - image object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->image->url);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_image() - url object initialization failed\n",
            __FILE__
        );

        json_object_put(imageobj);

        return false;
    }

    json_object_object_add(imageobj, "url", obj);

    obj = json_object_new_int(embed->image->height);

    json_object_object_add(imageobj, "height", obj);

    obj = json_object_new_int(embed->image->width);

    json_object_object_add(imageobj, "width", obj);

    json_object_object_add(embedobj, "image", imageobj);

    return true;
}

static bool set_embed_json_thumbnail(const discord_embed *embed, json_object *embedobj){
    if (!embed->thumbnail){
        return true;
    }

    json_object *thumbnailobj = json_object_new_object();

    if (!thumbnailobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_thumbnail() - thumbnail object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->thumbnail->url);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_thumbnail() - url object initialization failed\n",
            __FILE__
        );

        json_object_put(thumbnailobj);

        return false;
    }

    json_object_object_add(thumbnailobj, "url", obj);

    obj = json_object_new_int(embed->thumbnail->height);

    json_object_object_add(thumbnailobj, "height", obj);

    obj = json_object_new_int(embed->thumbnail->width);

    json_object_object_add(thumbnailobj, "width", obj);

    json_object_object_add(embedobj, "thumbnail", thumbnailobj);

    return true;
}

static bool set_embed_json_video(const discord_embed *embed, json_object *embedobj){
    if (!embed->video){
        return true;
    }

    json_object *videoobj = json_object_new_object();

    if (!videoobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_video() - video object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->video->url);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_video() - url object initialization failed\n",
            __FILE__
        );

        json_object_put(videoobj);

        return false;
    }

    json_object_object_add(videoobj, "url", obj);

    obj = json_object_new_int(embed->video->height);

    json_object_object_add(videoobj, "height", obj);

    obj = json_object_new_int(embed->video->width);

    json_object_object_add(videoobj, "width", obj);

    json_object_object_add(embedobj, "video", videoobj);

    return true;
}

static bool set_embed_json_provider(const discord_embed *embed, json_object *embedobj){
    if (!embed->provider){
        return true;
    }

    json_object *providerobj = json_object_new_object();

    if (!providerobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_provider() - provider object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->provider->name);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_provider() - name object initialization failed\n",
            __FILE__
        );

        json_object_put(providerobj);

        return false;
    }

    json_object_object_add(providerobj, "name", obj);

    obj = json_object_new_string(embed->provider->url);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_provider() - url object initialization failed\n",
            __FILE__
        );

        json_object_put(providerobj);

        return false;
    }

    json_object_object_add(providerobj, "url", obj);

    json_object_object_add(embedobj, "provider", providerobj);

    return true;
}

static bool set_embed_json_author(const discord_embed *embed, json_object *embedobj){
    if (!embed->author){
        return true;
    }

    json_object *authorobj = json_object_new_object();

    if (!authorobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_author() - author object initialization failed\n",
            __FILE__
        );

        return false;
    }

    json_object *obj = json_object_new_string(embed->author->name);

    if (!obj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_author() - name object initialization failed\n",
            __FILE__
        );

        json_object_put(authorobj);

        return false;
    }

    json_object_object_add(authorobj, "name", obj);

    obj = json_object_new_string(embed->author->url);

    json_object_object_add(authorobj, "url", obj);

    obj = json_object_new_string(embed->author->icon_url);

    json_object_object_add(authorobj, "icon_url", obj);

    json_object_object_add(embedobj, "author", authorobj);

    return true;
}

static bool set_embed_json_fields(const discord_embed *embed, json_object *embedobj){
    if (!embed->fields){
        return true;
    }

    json_object *fieldsobj = json_object_new_array();

    if (!fieldsobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] set_embed_json_fields() - fields object initialization failed\n",
            __FILE__
        );

        return false;
    }

    for (size_t index = 0; index < list_get_length(embed->fields); ++index){
        const discord_embed_field *field = list_get_generic(embed->fields, index);

        if (!strlen(field->value)){
            continue;
        }

        json_object *fieldobj = json_object_new_object();

        if (!fieldobj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_embed_json_fields() - field object initialization failed\n",
                __FILE__
            );

            json_object_put(fieldsobj);

            return false;
        }

        json_object *obj = json_object_new_string(field->name);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_embed_json_fields() - name object initialization failed\n",
                __FILE__
            );

            json_object_put(fieldobj);
            json_object_put(fieldsobj);

            return false;
        }

        json_object_object_add(fieldobj, "name", obj);

        obj = json_object_new_string(field->value);

        if (!obj){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] set_embed_json_fields() - value object initialization failed\n",
                __FILE__
            );

            json_object_put(fieldobj);
            json_object_put(fieldsobj);

            return false;
        }

        json_object_object_add(fieldobj, "value", obj);

        obj = json_object_new_boolean(field->display_inline);

        json_object_object_add(fieldobj, "inline", obj);

        json_object_array_add(fieldsobj, fieldobj);
    }

    json_object_object_add(embedobj, "fields", fieldsobj);

    return true;
}

/* embed constructors */
static bool construct_embed(discord_embed *embed, json_object *data){
    bool success = true;

    struct json_object_iterator curr = json_object_iter_begin(data);
    struct json_object_iterator end = json_object_iter_end(data);

    while (!json_object_iter_equal(&curr, &end)){
        const char *key = json_object_iter_peek_name(&curr);
        json_object *valueobj = json_object_iter_peek_value(&curr);
        json_type type = json_object_get_type(valueobj);

        bool skip = false;

        if (!valueobj || type == json_type_null){
            skip = true;
        }
        else if (type == json_type_array){
            skip = !json_object_array_length(valueobj);
        }

        if (skip){
            json_object_iter_next(&curr);

            continue;
        }

        if (!strcmp(key, "title")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                embed->title,
                sizeof(embed->title)
            );
        }
        else if (!strcmp(key, "description")){
            if (embed->description){
                free(embed->description);

                embed->description = NULL;
            }

            const char *objstr = json_object_get_string(valueobj);
            embed->description = string_duplicate(objstr);

            success = embed->description;
        }
        else if (!strcmp(key, "url")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                embed->url,
                sizeof(embed->url)
            );
        }
        else if (!strcmp(key, "timestamp")){
            const char *objstr = json_object_get_string(valueobj);

            success = string_copy(
                objstr,
                embed->timestamp,
                sizeof(embed->timestamp)
            );
        }
        else if (!strcmp(key, "color")){
            embed->color = json_object_get_int(valueobj);
        }
        else if (!strcmp(key, "footer")){
            json_object *textobj = json_object_object_get(valueobj, "text");
            json_object *iconurlobj = json_object_object_get(valueobj, "icon_url");

            success = embed_set_footer(
                embed,
                json_object_get_string(textobj),
                json_object_get_string(iconurlobj)
            );
        }
        else if (!strcmp(key, "image")){
            json_object *urlobj = json_object_object_get(valueobj, "url");
            json_object *heightobj = json_object_object_get(valueobj, "height");
            json_object *widthobj = json_object_object_get(valueobj, "width");

            success = embed_set_image(
                embed,
                json_object_get_string(urlobj),
                json_object_get_int(heightobj),
                json_object_get_int(widthobj)
            );
        }
        else if (!strcmp(key, "thumbnail")){
            json_object *urlobj = json_object_object_get(valueobj, "url");
            json_object *heightobj = json_object_object_get(valueobj, "height");
            json_object *widthobj = json_object_object_get(valueobj, "width");

            success = embed_set_thumbnail(
                embed,
                json_object_get_string(urlobj),
                json_object_get_int(heightobj),
                json_object_get_int(widthobj)
            );
        }
        else if (!strcmp(key, "video")){
            json_object *urlobj = json_object_object_get(valueobj, "url");
            json_object *heightobj = json_object_object_get(valueobj, "height");
            json_object *widthobj = json_object_object_get(valueobj, "width");

            success = embed_set_video(
                embed,
                json_object_get_string(urlobj),
                json_object_get_int(heightobj),
                json_object_get_int(widthobj)
            );
        }
        else if (!strcmp(key, "provider")){
            json_object *nameobj = json_object_object_get(valueobj, "name");
            json_object *urlobj = json_object_object_get(valueobj, "url");

            success = embed_set_provider(
                embed,
                json_object_get_string(nameobj),
                json_object_get_string(urlobj)
            );
        }
        else if (!strcmp(key, "author")){
            json_object *nameobj = json_object_object_get(valueobj, "name");
            json_object *urlobj = json_object_object_get(valueobj, "url");
            json_object *iconurlobj = json_object_object_get(valueobj, "icon_url");

            success = embed_set_author(
                embed,
                json_object_get_string(nameobj),
                json_object_get_string(urlobj),
                json_object_get_string(iconurlobj)
            );
        }
        else if (!strcmp(key, "fields")){
            for (size_t index = 0; index < json_object_array_length(valueobj); ++index){
                json_object *fieldobj = json_object_array_get_idx(valueobj, index);

                json_object *nameobj = json_object_object_get(fieldobj, "name");
                json_object *valueobj = json_object_object_get(fieldobj, "value");
                json_object *inlineobj = json_object_object_get(fieldobj, "inline");

                success = embed_add_field(
                    embed,
                    json_object_get_string(nameobj),
                    json_object_get_string(valueobj),
                    json_object_get_boolean(inlineobj)
                );

                if (!success){
                    log_write(
                        logger,
                        LOG_ERROR,
                        "[%s] construct_embed() - embed_add_field call failed\n",
                        __FILE__
                    );

                    break;
                }
            }

            if (!success){
                break;
            }
        }

        if (!success){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] construct_embed() - failed to set %s with value: %s\n",
                __FILE__,
                key,
                json_object_to_json_string(valueobj)
            );

            break;
        }

        json_object_iter_next(&curr);
    }

    return success;
}

list *embed_list_from_json_array(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_list_from_json_array() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }
    else if (!data){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_list_from_json_array() - data is NULL\n",
            __FILE__
        );

        return NULL;
    }

    json_type type = json_object_get_type(data);
    size_t length = json_object_array_length(data);

    if (type != json_type_array){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_list_from_json_array() - data is type %d -- not a json array\n",
            __FILE__,
            type
        );

        return NULL;
    }
    else if (!length){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] embed_list_from_json_array() - json array is empty\n",
            __FILE__
        );

        return NULL;
    }

    list *embeds = list_init();

    if (!embeds){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_list_from_json_array() - embeds initialization failed\n",
            __FILE__
        );

        return NULL;
    }

    for (size_t index = 0; index < json_object_array_length(data); ++index){
        json_object *valueobj = json_object_array_get_idx(data, index);
        type = json_object_get_type(valueobj);

        if (type != json_type_object){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_list_from_json_array() - object (type: %d) is not a json object: %s\n",
                __FILE__,
                type,
                json_object_to_json_string(valueobj)
            );

            list_free(embeds);

            return NULL;
        }

        discord_embed *embed = embed_init(state, valueobj);

        if (!embed){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_list_from_json_array() - embed initialization failed\n",
                __FILE__
            );

            list_free(embeds);

            return NULL;
        }

        list_item item = {0};
        item.type = L_TYPE_GENERIC;
        item.size = sizeof(*embed);
        item.data = embed;
        item.generic_free = embed_free;

        if (!list_append(embeds, &item)){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_list_from_json_array() - list_append call failed\n",
                __FILE__
            );

            embed_free(embed);
            list_free(embeds);

            return NULL;
        }
    }

    return embeds;
}

discord_embed *embed_init(discord_state *state, json_object *data){
    if (!state){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_init() - state is NULL\n",
            __FILE__
        );

        return NULL;
    }

    logger = state->log;

    discord_embed *embed = calloc(1, sizeof(*embed));

    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_init() - embed object alloc failed\n",
            __FILE__
        );

        return NULL;
    }

    embed->state = state;

    if (data){
        if (!construct_embed(embed, data)){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_init() - construct_embed call failed\n",
                __FILE__
            );

            embed_free(embed);

            return NULL;
        }
    }

    return embed;
}

size_t embed_get_length(const discord_embed *embed){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_get_length() - embed is NULL\n",
            __FILE__
        );

        return 0;
    }

    size_t length = strlen(embed->title) + strlen(embed->description);

    if (embed->footer){
        length += strlen(embed->footer->text);
    }

    if (embed->author){
        length += strlen(embed->author->name);
    }

    if (embed->fields){
        for (size_t index = 0; index < list_get_length(embed->fields); ++index){
            const discord_embed_field *field = list_get_generic(embed->fields, index);

            length += strlen(field->name) + strlen(field->value);
        }
    }

    return length;
}

json_object *embed_to_json(const discord_embed *embed){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_to_json() - embed is NULL\n",
            __FILE__
        );

        return NULL;
    }

    json_object *embedobj = json_object_new_object();

    if (!embedobj){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_to_json() - embed object initialization failed\n",
            __FILE__
        );

        return NULL;
    }

    json_object_object_add(
        embedobj,
        "title",
        json_object_new_string(embed->title)
    );

    if (embed->description){
        json_object_object_add(
            embedobj,
            "description",
            json_object_new_string(embed->description)
        );
    }

    json_object_object_add(
        embedobj,
        "url",
        json_object_new_string(embed->url)
    );

    json_object_object_add(
        embedobj,
        "timestamp",
        json_object_new_string(embed->timestamp)
    );

    json_object_object_add(
        embedobj,
        "color",
        json_object_new_int(embed->color)
    );

    if (!set_embed_json_footer(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_image(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_thumbnail(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_video(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_provider(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_author(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    if (!set_embed_json_fields(embed, embedobj)){
        json_object_put(embedobj);

        return NULL;
    }

    return embedobj;
}

bool embed_set_title(discord_embed *embed, const char *title){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_title() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    return string_copy(title, embed->title, sizeof(embed->title));
}

bool embed_set_description(discord_embed *embed, const char *description){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_description() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    return string_copy(description, embed->description, sizeof(embed->description));
}

bool embed_set_url(discord_embed *embed, const char *url){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_url() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    return string_copy(url, embed->url, sizeof(embed->url));
}

bool embed_set_timestamp(discord_embed *embed, const char *timestamp){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_timestamp() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    return string_copy(timestamp, embed->timestamp, sizeof(embed->timestamp));
}

bool embed_set_color(discord_embed *embed, int color){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_color() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    embed->color = color;

    return true;
}

bool embed_set_footer(discord_embed *embed, const char *text, const char *iconurl){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_footer() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_footer *footer = NULL;

    if (text || iconurl){
        footer = calloc(1, sizeof(*footer));

        if (!footer){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_footer() - footer object alloc failed\n",
                __FILE__
            );

            return false;
        }

        if (text){
            string_copy(text, footer->text, sizeof(footer->text));
        }

        if (iconurl){
            string_copy(iconurl, footer->icon_url, sizeof(footer->icon_url));
        }
    }

    embed->footer = footer;

    return true;
}

bool embed_set_image(discord_embed *embed, const char *url, int height, int width){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_image() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_image *image = NULL;

    if (url){
        image = calloc(1, sizeof(*image));

        if (!image){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_image() - image object alloc failed\n",
                __FILE__
            );

            return false;
        }

        string_copy(url, image->url, sizeof(image->url));

        image->height = height;
        image->width = width;
    }

    embed->image = image;

    return true;
}

bool embed_set_thumbnail(discord_embed *embed, const char *url, int height, int width){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_thumbnail() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_thumbnail *thumbnail = NULL;

    if (url){
        thumbnail = calloc(1, sizeof(*thumbnail));

        if (!thumbnail){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_thumbnail() - thumbnail object alloc failed\n",
                __FILE__
            );

            return false;
        }

        string_copy(url, thumbnail->url, sizeof(thumbnail->url));

        thumbnail->height = height;
        thumbnail->width = width;
    }

    embed->thumbnail = thumbnail;

    return true;
}

bool embed_set_video(discord_embed *embed, const char *url, int height, int width){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_video() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_video *video = NULL;

    if (url){
        video = calloc(1, sizeof(*video));

        if (!video){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_video() - video object alloc failed\n",
                __FILE__
            );

            return false;
        }

        string_copy(url, video->url, sizeof(video->url));

        video->height = height;
        video->width = width;
    }

    embed->video = video;

    return true;
}

bool embed_set_provider(discord_embed *embed, const char *name, const char *url){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_set_provider() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_provider *provider = NULL;

    if (name || url){
        provider = calloc(1, sizeof(*provider));

        if (!provider){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_provider() - provider object alloc failed\n",
                __FILE__
            );

            return false;
        }

        if (name){
            string_copy(name, provider->name, sizeof(provider->name));
        }

        if (url){
            string_copy(url, provider->url, sizeof(provider->url));
        }
    }

    embed->provider = provider;

    return true;
}

bool embed_set_author(discord_embed *embed, const char *name, const char *url, const char *iconurl){
    if (!embed){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_set_author() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    discord_embed_author *author = NULL;

    if (name){
        author = calloc(1, sizeof(*author));

        if (!author){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_set_author() - author alloc failed\n",
                __FILE__
            );

            return false;
        }

        string_copy(name, author->name, sizeof(author->name));

        if (url){
            string_copy(url, author->url, sizeof(author->url));
        }

        else if (iconurl){
            string_copy(iconurl, author->icon_url, sizeof(author->icon_url));
        }
    }
    else if (url || iconurl){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_set_author() - name argument is required\n",
            __FILE__
        );

        return false;
    }

    embed->author = author;

    return true;
}

bool embed_add_field(discord_embed *embed, const char *name, const char *value, bool displayinline){
    if (!embed){
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_add_field() - embed is NULL\n",
            __FILE__
        );

        return false;
    }

    if (name && value){
        if (!embed->fields){
            list *tmp = list_init();

            if (!tmp){
                log_write(
                    logger,
                    LOG_ERROR,
                    "[%s] embed_add_field() - fields list initialization failed\n",
                    __FILE__
                );

                return false;
            }

            embed->fields = tmp;
        }

        discord_embed_field *field = malloc(sizeof(*field));

        if (!field){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_add_field() - field object alloc failed\n",
                __FILE__
            );

            return false;
        }

        string_copy(name, field->name, sizeof(field->name));
        string_copy(value, field->value, sizeof(field->value));

        field->display_inline = displayinline;

        list_item item = {0};
        item.type = L_TYPE_GENERIC;
        item.size = sizeof(*field);
        item.data = field;

        if (!list_append(embed->fields, &item)){
            log_write(
                logger,
                LOG_ERROR,
                "[%s] embed_add_field() - list_append call failed\n",
                __FILE__
            );

            free(field);

            return false;
        }
    }
    else {
        log_write(
            logger,
            LOG_ERROR,
            "[%s] embed_add_field() - name and value arguments are required\n",
            __FILE__
        );

        return false;
    }

    return true;
}

bool embed_remove_field(discord_embed *embed, size_t pos){
    if (!embed){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_remove_field() - embed is NULL\n",
            __FILE__
        );

        return false;
    }
    else if (pos >= list_get_length(embed->fields)){
        log_write(
            logger,
            LOG_WARNING,
            "[%s] embed_remove_field() - position is out of bounds\n",
            __FILE__
        );

        return false;
    }

    list_remove(embed->fields, pos);

    return true;
}

void embed_free(void *embedptr){
    discord_embed *embed = embedptr;

    if (!embed){
        log_write(
            logger,
            LOG_DEBUG,
            "[%s] embed_free() - embed is NULL\n",
            __FILE__
        );

        return;
    }

    free(embed->footer);
    free(embed->image);
    free(embed->thumbnail);
    free(embed->video);
    free(embed->provider);
    free(embed->author);
    list_free(embed->fields);

    free(embed->description);
    free(embed);
}
