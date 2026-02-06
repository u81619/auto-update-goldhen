#ifndef COMMON_H
#define COMMON_H

#include <orbis/libkernel.h>
#include <orbis/SystemMsgDialog.h>
#include <orbis/CommonDialog.h>
#include <orbis/Sysmodule.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PAYLOAD_SOURCE "/app0/assets/payload.bin"
#define GOLDHEN_SOURCE "/app0/assets/goldhen.bin"

#define PAYLOAD_DEST "/data/payload.bin"
#define GOLDHEN_DEST_DIR "/data/GoldHEN/payloads"
#define GOLDHEN_DEST "/data/GoldHEN/payloads/goldhen.bin"

void show_notification(const char* message);
int copy_file(const char* src, const char* dst);
int make_dir_recursive(const char* path);
void log_message(const char* format, ...);

#endif
