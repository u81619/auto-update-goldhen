#include "common.h"

int main(int argc, char** argv) {
    // تهيئة مكتبات النظام
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_SYSTEM_SERVICE);
    sceSysmoduleLoadModule(ORBIS_SYSMODULE_COMMON_DIALOG);

    log_message("Starting Auto Update GoldHEN...");

    int success = 1;

    // 1. إنشاء المجلدات الناقصة
    if (make_dir_recursive(GOLDHEN_DEST_DIR) != 0) {
        log_message("Failed to create directory: %s", GOLDHEN_DEST_DIR);
        success = 0;
    }

    // 2. نسخ payload.bin
    if (success) {
        log_message("Copying payload.bin to /data/...");
        if (copy_file(PAYLOAD_SOURCE, PAYLOAD_DEST) != 0) {
            log_message("Failed to copy payload.bin");
            success = 0;
        }
    }

    // 3. نسخ goldhen.bin
    if (success) {
        log_message("Copying goldhen.bin to GoldHEN payloads...");
        if (copy_file(GOLDHEN_SOURCE, GOLDHEN_DEST) != 0) {
            log_message("Failed to copy goldhen.bin");
            success = 0;
        }
    }

    // إظهار النتيجة للمستخدم
    if (success) {
        log_message("Update completed successfully!");
        show_notification("GoldHEN Updated Successfully!");
    } else {
        log_message("Update failed! Check logs.");
        show_notification("GoldHEN Update Failed!");
    }

    // انتظر قليلاً قبل الخروج ليتمكن المستخدم من رؤية الإشعار
    sleep(5);

    return 0;
}
