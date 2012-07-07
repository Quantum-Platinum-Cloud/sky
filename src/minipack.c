#include "minipack.h"
#include "dbg.h"
#include "stdlib.h"

//==============================================================================
//
// Functions
//
//==============================================================================

//======================================
// Bstring
//======================================

// Reads a MessagePack serialized raw byte stream in as a bstring.
//
// file - The file stream to read from.
// ret  - A pointer to where the bstring should be returned.
//
// Returns 0 if successful, otherwise returns -1.
int sky_minipack_fread_bstring(FILE *file, bstring *ret)
{
    size_t sz;
    
    // Checkpoint stream.
    long pos = ftell(file);
    
    // Read string length.
    uint32_t length = minipack_fread_raw(file, &sz);
    check(sz != 0, "Unable to read raw byte element");

    // Initialize buffer.
    char *buffer = malloc(length+1); check_mem(buffer);
    buffer[length] = 0;
    
    // Read into buffer.
    sz = fread(buffer, sizeof(char*), length, file);
    check(sz != length, "Expected %d bytes, received %ld bytes", length, sz);

    // Create bstring from buffer and return.
    bstring str = blk2bstr(buffer, length); check_mem(str);
    *ret = str;
    
    // Clean up.
    free(buffer);

    return 0;
    
error:
    fseek(file, pos, SEEK_SET);
    if(str) bdestroy(str);
    str = NULL;
    *ret = NULL;
    return -1;
}

// Writes a bstring to a file as a MessagePack formatted raw bytes element.
//
// file - The file stream to write to.
// str  - The bstring to write.
// 
// Returns 0 if successful, otherwise returns -1.
int sky_minipack_fwrite_bstring(FILE *file, bstring str)
{
    // Write header.
    size_t sz;
    int rc = minipack_fwrite_raw(file, blength(str), &sz);
    check(rc == 0, "Unable to write raw bytes header");

    // Write raw bytes.
    if(str) {
        sz = fwrite(bdata(str), sizeof(char*), blength(str), file);
        check(sz == (size_t)blength(str), "Attempted to write %d bytes, only wrote %ld bytes", blength(str), sz);
    }
    
    return 0;

error:
    return -1; 
}
