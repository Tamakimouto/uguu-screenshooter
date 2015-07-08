#include "uploader.h"

/* Set up container for URL */
typedef struct response {
  char * string;
  size_t length;
} container;

/**
 * Function: getResponse 
 *
 * To be passed into curlOPT as the write function.
 * Copies the return data from the curl call to the URL container.
 */
static size_t
getResponse(void * data, size_t size, size_t nmemb, void * dest) {
  size_t sz = size * nmemb;
  container * url = (container *) dest;
  
  url->string = realloc(url->string, url->length + sz + 1);
  if (url->string == NULL)
    exit(EXIT_FAILURE);
  
  memcpy(&(url->string[url->length]), data, sz);
  url->length += sz;
  url->string[url->length] = 0;

  return sz;

}//getResponse

char * 
upload (gchar * path) {

  CURL * curl;
  CURLcode res;
  FILE * image;

  container url;
  url.string = malloc(1);
  url.length = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  image = fopen(path, "r");
  if (!image) {
    exit(EXIT_FAILURE);
  }

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt (curl, CURLOPT_URL,
		      "http://uguu.se/api.php?d=upload");
    
    curl_easy_setopt (curl, CURLOPT_READDATA, image);
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, getResponse);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, (void *)&url);

    res = curl_easy_perform(curl);
  }

  if (res != CURLE_OK){
    printf("Curl Failed \n");
    free(url.string);
    exit(EXIT_FAILURE);
  } else {
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return url.string;
  }

}//upload
