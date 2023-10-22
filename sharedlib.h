#ifndef SHAREDLIB_H
#define SHAREDLIB_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum conversion {
  STANDARD,
  EXTENDED
};

char *get_pixel_data_converted(enum conversion conv_type);

#endif
