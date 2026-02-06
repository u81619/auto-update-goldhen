#include "common.h"
#include <stdarg.h>

void log_message(const char* format, ...) {
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("[AutoUpdateGH] %s\n", buffer);
    
    // يمكن أيضاً الكتابة إلى ملف log في /data/
    FILE* f = fopen("/data/goldhen_update.log", "a");
    if (f) {
        fprintf(f, "%s\n", buffer);
        fclose(f);
    }
}

void show_notification(const char* message) {
    // استخدام إشعارات النظام البسيطة
    // في بيئة GoldHEN، يمكن استخدام وظائف Notification
    // هنا نستخدم MessageBox كبديل آمن أو وظيفة Notification إذا كانت متوفرة
    OrbisSystemMsgDialogConfiguration config;
    memset(&config, 0, sizeof(config));
    config.size = sizeof(config);
    
    sceSystemMsgDialogInitialize();
    sceSystemMsgDialogOpen(message);
}

int make_dir_recursive(const char* path) {
    char tmp[256];
    char* p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO);
            *p = '/';
        }
    }
    return mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO);
}

int copy_file(const char* src, const char* dst) {
    int src_fd = open(src, O_RDONLY);
    if (src_fd < 0) return -1;

    // فتح الملف للوجهة مع خيار الاستبدال الإجباري
    int dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
    if (dst_fd < 0) {
        close(src_fd);
        return -1;
    }

    char buffer[8192];
    ssize_t nread;
    while ((nread = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dst_fd, buffer, nread) != nread) {
            close(src_fd);
            close(dst_fd);
            return -1;
        }
    }

    close(src_fd);
    close(dst_fd);
    return 0;
}
