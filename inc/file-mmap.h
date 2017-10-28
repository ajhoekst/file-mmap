#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Masks for easy program control
#define FILE_MAP_PROT_READ      0x01
#define FILE_MAP_PROT_WRITE     0x02
#define FILE_MAP_PROT           0x03
#define FILE_MAP_ACCESS_SHARED  0x10
#define FILE_MAP_ACCESS_PRIVATE 0x20
#define FILE_MAP_ACCESS         0x30

// Presets to reduce the includes for calling code
#define FILE_MAP_R_SHARED     FILE_MAP_PROT_READ    \
                            | FILE_MAP_ACCESS_SHARED
#define FILE_MAP_W_SHARED     FILE_MAP_PROT_WRITE   \
                            | FILE_MAP_ACCESS_SHARED
#define FILE_MAP_RW_SHARED    FILE_MAP_PROT_READ    \
                            | FILE_MAP_PROT_WRITE   \
                            | FILE_MAP_ACCESS_SHARED
#define FILE_MAP_R_PRIVATE    FILE_MAP_PROT_READ    \
                            | FILE_MAP_ACCESS_PRIVATE
#define FILE_MAP_W_PRIVATE    FILE_MAP_PROT_WRITE   \
                            | FILE_MAP_ACCESS_PRIVATE
#define FILE_MAP_RW_PRIVATE   FILE_MAP_PROT_READ    \
                            | FILE_MAP_PROT_WRITE   \
                            | FILE_MAP_ACCESS_PRIVATE

// Status codes
#define FILE_MAP_OK      0
#define FILE_MAP_WARN   -1
#define FILE_MAP_ERROR  -2

// Ensure this code plays nice with C++
#ifdef __cplusplus
// Scope if this is being built in AHIO super project
#ifdef __ahio
namespace ahio
{
#endif
extern "C" {
#endif

struct file_map
{
    void* address;
    size_t length;
    int fid;
};

int map_file(
    struct file_map* map,
    const char* const filename,
    const int mode
);

int unmap_file(
    struct file_map* map
);

// Ensure this code plays nice with C++
#ifdef __cplusplus
// Scope if this is being built in AHIO super project
#ifdef __ahio
} // end of namespace ahio
#endif
} // end of extern C
#endif
