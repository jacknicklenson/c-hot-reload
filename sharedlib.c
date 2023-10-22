#include "sharedlib.h"

char *xstrdup(const char *s) {
    size_t size = strlen(s) + 1;
    char *p = malloc(size);
    if (p) {
        memcpy(p, s, size);
    }
    return p;
}

char *get_pixel_data_converted(enum conversion conv_type) {
  switch (conv_type) {
    case STANDARD:
      return xstrdup("STANDARD!");
    case EXTENDED:
      return xstrdup("EXTENDED!");
    default:
      perror("WRONG TYPE PASSED!");
      return NULL;
  }
}

