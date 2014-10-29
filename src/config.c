#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include "config.h"
#include "memory.h"

static void config_load(struct config * config);
static void json_print(json_t * json);

struct config * config_new(const char * file)
{
  struct stat s;
  if (file[0] != '\0' && stat(file, &s) == -1 && errno == ENOENT)
    return NULL;

  struct config * config = mem_salloc(struct config);
  config->file = mem_strdup(file);
  config->json = NULL;
  config->structure = NULL;
  config_isChanged(config);
  return config;
}

void config_free(struct config * config)
{
  json_decref(config->structure);
  json_decref(config->json);
  mem_free(config->file);
  mem_free(config);
}

int config_isChanged(struct config * config)
{
  struct stat s;
  stat(config->file, &s);

  if (config->size == s.st_size &&
      config->mtime == s.st_mtime &&
      config->ctime == s.st_ctime)
    return 0;

  config_load(config);

  config->size = s.st_size;
  config->mtime = s.st_mtime;
  config->ctime = s.st_ctime;
  return 1;
}

json_t * config_getStructure(struct config * config)
{
  config_isChanged(config);
  return config->structure;
}

static void config_load(struct config * config)
{
  json_t * tmp, * tmp2, * tmp3;
  char buf[128] = "";

  if (config->json != NULL)
  {
    json_decref(config->structure);
    json_decref(config->json);
  }

  config->json = json_load_file(config->file, 0, NULL);

  if (config->json == NULL || !json_is_object(config->json))
    config->json = json_object();

  config->structure = json_object_get(config->json, "structure");

  if (config->structure == NULL || !json_is_object(config->structure))
  {
    config->structure = json_object();
    json_object_set_new(config->json, "structure", config->structure);
  }

  tmp = json_object_get(config->structure, "screens");

  if (tmp == NULL || !json_is_array(tmp))
  {
    tmp = json_array();
    json_object_set_new(config->structure, "screens", tmp);
  }

  tmp2 = json_array_get(tmp, 0);

  if (tmp2 == NULL || !json_is_object(tmp2))
  {
    tmp2 = json_object();
    json_array_append_new(tmp, tmp2);
  }

  if (json_object_get(tmp2, "left") == NULL && json_object_get(tmp2, "center") == NULL && json_object_get(tmp2, "right") == NULL)
  {
    tmp3 = json_object();
    snprintf(buf, sizeof(buf), "Please add a config to './wildbar.conf' or '/etc/wildbar.conf'!");
    json_object_set_new(tmp3, "text", json_string(buf));
    json_object_set_new(tmp2, "center", tmp3);
  }

  json_print(config->json);
  fprintf(stderr, "\n");
}

#define _(...) fprintf(stderr, __VA_ARGS__)
static void json_print(json_t * json)
{
  const char * key;
  size_t index = 0;
  json_t * value;

  if (json_is_object(json))
  {
    _("{");
    json_object_foreach(json, key, value)
    {
      if (index > 0)
        _(", ");

      _("'%s' : ", key);
      json_print(value);
      index++;
    }
    _("}");
  }
  else if (json_is_array(json))
  {
    _("[");
    json_array_foreach(json, index, value)
    {
      if (index > 0)
        _(", ");

      json_print(value);
    }
    _("]");
  }
  else if (json_is_boolean(json))
    _("%s", json_is_true(json) ? "true" : "false");
  else if (json_is_integer(json))
    _("%f", json_number_value(json));
  else if (json_is_string(json))
    _("'%s'", json_string_value(json));
  else if (json_is_null(json))
    _("NULL");
}
#undef _
