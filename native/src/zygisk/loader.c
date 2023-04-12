#include <dlfcn.h>
#include <android/dlext.h>

#if defined(__LP64__)
#define SECOND_STAGE_PATH "/system/bin/magisk64"
#else
#define SECOND_STAGE_PATH "/system/bin/magisk32"
#endif

static void *handle = NULL;

__attribute__((constructor, used))
static void zygisk_loader(void) {
    android_dlextinfo info = {
            .flags = ANDROID_DLEXT_FORCE_LOAD
    };
    handle = android_dlopen_ext(SECOND_STAGE_PATH, RTLD_LAZY, &info);
    if (handle) {
        void (*entry)(void*) = dlsym(handle, "zygisk_inject_entry");
        if (entry) {
            entry(handle);
        }
    }
}

__attribute__((destructor, used))
static void zygisk_unloader(void) {
    if (handle) {
        void (*entry)(void) = dlsym(RTLD_DEFAULT, "unload_first_stage");
        if (entry) {
            entry();
        }
    }
}
