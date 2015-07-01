#include <curl/curl.h>
#include <glib.h>
#include <glib/gstdio.h>

#include "global.h"

char * 
upload (gchar * path);

static size_t
getResponse(void * data, size_t size, size_t nmemb, void * dest);

