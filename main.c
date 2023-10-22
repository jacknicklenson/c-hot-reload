#include "dep/m-array.h"
#include "sharedlib.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#define DMON_IMPL
#include "dep/dmon.h"

typedef char *(*conv_fn)(enum conversion);

static void watch_callback(dmon_watch_id watch_id, dmon_action action,
                           const char *rootdir, const char *filepath,
                           const char *oldfilepath, void *user) {
  conv_fn fn = (conv_fn)user;
  (void)(watch_id);

  void *handle = dlopen("sharedlib/sharedlib.so", RTLD_LAZY);
  fn = (conv_fn)dlsym(handle, "get_pixel_data_converted");

  switch (action) {
  case DMON_ACTION_CREATE:
    printf("CREATE: [%s]%s\n", rootdir, filepath);
    printf("EXECUTING: %s\n", fn(STANDARD));
    dlclose(handle);
    break;
  case DMON_ACTION_DELETE:
    printf("DELETE: [%s]%s\n", rootdir, filepath);
    printf("EXECUTING: %s\n", fn(STANDARD));
    dlclose(handle);
    break;
  case DMON_ACTION_MODIFY:
    printf("MODIFY: [%s]%s\n", rootdir, filepath);
    printf("EXECUTING: %s\n", fn(EXTENDED));
    dlclose(handle);
    break;
  case DMON_ACTION_MOVE:
    printf("MOVE: [%s]%s -> [%s]%s\n", rootdir, oldfilepath, rootdir, filepath);
    dlclose(handle);
    break;
  }
}

struct pixel {
  uint8_t a;
  uint8_t b;
  uint8_t c;
  const char type[3];
};

ARRAY_DEF(vec_pixel, struct pixel, M_POD_OPLIST)
int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  vec_pixel_t f = ARRAY_INIT_VALUE();
  conv_fn fn = 0;
  dmon_init();
  puts("waiting for changes on sharedlib.so ....");
  dmon_watch("/home/cx/github/test_c/sharedlib", watch_callback,
             DMON_WATCHFLAGS_RECURSIVE, (void *)fn);
  for (size_t i = 0; i < 100; i++) {
    vec_pixel_push_back(f, (struct pixel){1, 2, 3, "vi"});
    printf("Size: %zu, Capacity: %zu\n", vec_pixel_size(f),
           vec_pixel_capacity(f));
  }

  {
    vec_pixel_it_t it;
    for (vec_pixel_it(it, f); !vec_pixel_end_p(it); vec_pixel_next(it)) {
      const struct pixel *item = vec_pixel_cref(it);
      printf("struct pixel {\n  uint8_t a = %d;\n  uint8_t b = %d;\n  uint8_t "
             "c = %d;\n  const char type[3] = %s;\n}\n",
             item->a, item->b, item->c, item->type);
    }
  }
  vec_pixel_clear(f);

  getchar();
  dmon_deinit();
  return 0;
}
