#pragma once

#include <stdint.h>
#include <jni.h>
#include <vector>
#include <daemon.hpp>

#define MAGISKTMP_ENV  "MAGISKTMP"

#define HIJACK_BIN64   "/system/bin/appwidget"
#define HIJACK_BIN32   "/system/bin/bu"

namespace ZygiskRequest {
enum : int {
    SETUP,
    GET_INFO,
    GET_LOG_PIPE,
    CONNECT_COMPANION,
    GET_MODDIR,
    PASSTHROUGH,
    END
};
}

#if defined(__LP64__)
#define ZLOGD(...) LOGD("zygisk64: " __VA_ARGS__)
#define ZLOGE(...) LOGE("zygisk64: " __VA_ARGS__)
#define ZLOGI(...) LOGI("zygisk64: " __VA_ARGS__)
#define ZLOGW(...) LOGW("zygisk64: " __VA_ARGS__)
#define HIJACK_BIN HIJACK_BIN64
#else
#define ZLOGD(...) LOGD("zygisk32: " __VA_ARGS__)
#define ZLOGE(...) LOGE("zygisk32: " __VA_ARGS__)
#define ZLOGI(...) LOGI("zygisk32: " __VA_ARGS__)
#define ZLOGW(...) LOGW("zygisk32: " __VA_ARGS__)
#define HIJACK_BIN HIJACK_BIN32
#endif

extern void *self_handle;

void hook_functions();
int remote_get_info(int uid, const char *process, uint32_t *flags, std::vector<int> &fds);

inline int zygisk_request(int req) {
    int fd = connect_daemon(MainRequest::ZYGISK);
    if (fd < 0) return fd;
    write_int(fd, req);
    return fd;
}
