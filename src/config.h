#ifndef CONFIGA_H_
#define CONFIGA_H_

#include <jansson.h>
#include <sys/types.h>

struct config
{
  char * file;
  off_t size;
  time_t mtime;
  time_t ctime;
  json_t * json;

  json_t * structure;
};

struct config * config_new(const char * file);
void config_free(struct config * config);

int config_isChanged(struct config * config);
json_t * config_getStructure(struct config * config);

#endif // CONFIG_H_
