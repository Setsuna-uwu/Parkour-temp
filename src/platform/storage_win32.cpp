#include "platform/platform.h"

#include <windows.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#pragma pack(push, 1)
typedef struct HighScoreRecord {
    char magic[8];
    int32_t value;
    uint32_t checksum;
    uint32_t version;
} HighScoreRecord;
#pragma pack(pop)

static const char HIGH_SCORE_MAGIC[8] = {'S', 'P', 'H', 'S', '0', '0', '0', '1'};
static const uint32_t HIGH_SCORE_VERSION = 1u;

static uint32_t highscore_checksum(int32_t value)
{
    return ((uint32_t)value * 2654435761u) ^ 0xA5A5C0DEu;
}

static bool executable_path(wchar_t *buffer, size_t capacity, const wchar_t *filename)
{
    DWORD length;
    wchar_t *slash;

    if (buffer == 0 || capacity == 0 || filename == 0) {
        return false;
    }

    length = GetModuleFileNameW(0, buffer, (DWORD)capacity);
    if (length == 0 || (size_t)length >= capacity) {
        return false;
    }

    slash = wcsrchr(buffer, L'\\');
    if (slash == 0) {
        return false;
    }
    slash[1] = L'\0';

    if (wcsncat_s(buffer, capacity, filename, _TRUNCATE) != 0) {
        return false;
    }
    return true;
}

extern "C" bool platform_storage_load_highscore(int *out_value)
{
    wchar_t path[MAX_PATH];
    FILE *file = 0;
    HighScoreRecord record;
    size_t read_count;

    if (out_value == 0) {
        return false;
    }
    *out_value = 0;

    if (!executable_path(path, MAX_PATH, L"highscore.dat")) {
        return false;
    }
    if (_wfopen_s(&file, path, L"rb") != 0 || file == 0) {
        return false;
    }

    memset(&record, 0, sizeof(record));
    read_count = fread(&record, 1, sizeof(record), file);
    fclose(file);

    if (read_count != sizeof(record)) {
        return false;
    }
    if (memcmp(record.magic, HIGH_SCORE_MAGIC, sizeof(HIGH_SCORE_MAGIC)) != 0) {
        return false;
    }
    if (record.version != HIGH_SCORE_VERSION) {
        return false;
    }
    if (record.checksum != highscore_checksum(record.value)) {
        return false;
    }
    if (record.value < 0 || record.value > 100000000) {
        return false;
    }

    *out_value = (int)record.value;
    return true;
}

extern "C" bool platform_storage_save_highscore(int value)
{
    wchar_t path[MAX_PATH];
    wchar_t temp_path[MAX_PATH];
    FILE *file = 0;
    HighScoreRecord record;
    size_t written;
    bool ok;

    if (value < 0) {
        value = 0;
    }
    if (value > 100000000) {
        value = 100000000;
    }

    if (!executable_path(path, MAX_PATH, L"highscore.dat")) {
        return false;
    }
    if (!executable_path(temp_path, MAX_PATH, L"highscore.tmp")) {
        return false;
    }

    memset(&record, 0, sizeof(record));
    memcpy(record.magic, HIGH_SCORE_MAGIC, sizeof(HIGH_SCORE_MAGIC));
    record.value = (int32_t)value;
    record.checksum = highscore_checksum(record.value);
    record.version = HIGH_SCORE_VERSION;

    if (_wfopen_s(&file, temp_path, L"wb") != 0 || file == 0) {
        return false;
    }

    written = fwrite(&record, 1, sizeof(record), file);
    ok = (written == sizeof(record));
    if (ok) {
        ok = (fflush(file) == 0);
    }
    fclose(file);

    if (!ok) {
        DeleteFileW(temp_path);
        return false;
    }

    if (!MoveFileExW(temp_path, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        DeleteFileW(temp_path);
        return false;
    }
    return true;
}
