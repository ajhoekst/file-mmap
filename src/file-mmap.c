#include "file-mmap.h"

// Ensure this code plays nice with C++
#ifdef __cplusplus
// Scope if this is being built in AHIO super project
#ifdef __ahio
namespace ahio
{
#endif
#endif

int map_file(
    struct file_map* map,
    const char* const filename,
    const int mode
)
{
    // Forward declarations
    int status = FILE_MAP_OK;

    // Open the file for mapping
    map->fid = open(
        filename,           // Filename to map and, consequently, from which to read
        O_RDWR | O_CREAT,   // We only need to read to map a file to memory
        S_IRUSR	| S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH
    );
    if ( map->fid < 0 ) // Negative return from open indicates an error
    {
        perror("open");         // Set the error code for upstream handling
        return FILE_MAP_ERROR;  // Return early with error
    }

    // Get the meta information of the file
    struct stat sb;
    if ( fstat( map->fid, &sb ) == -1 ) // -1 return from fstat indicates an error
    {
        // Set the error code for upstream handling
        if ( close( map->fid ) == -1 ) // Attempt to close the file
        {
            perror("fstat/close");
        }
        else
        {
            perror("fstat");
        }
        return FILE_MAP_ERROR; // Return early with error
    }

    // Ensure this file is a regular file
    if ( ( sb.st_mode & S_IFMT ) != S_IFREG )
    {
        // Set the error code for upstream handling
        if ( close( map->fid ) == -1 ) // Attempt to close the file
        {
            perror("stat/mode/close");  
        }
        else
        {
            perror("state/mode");
        }
        return FILE_MAP_ERROR; // Return early with error
    }

    // Ensure the file is not empty so it can be mapped
    if ( sb.st_size == 0 )
    {
        if ( map->length == 0 )
        {
            // Set the error code for upstream handling
            if ( close( map->fid ) == -1 ) // Attempt to close the file
            {
                perror("length/close");  
            }
            else
            {
                perror("length");
            }
            return FILE_MAP_ERROR; // Return early with error
        }
        else // Pad the file with zeros
        {
            if ( ftruncate( map->fid, map->length ) == -1 )
            {
                // Set the error code for upstream handling
                if ( close( map->fid ) == -1 ) // Attempt to close the file
                {
                    perror("length/truncate/close");  
                }
                else
                {
                    perror("length/truncate");
                }
                return FILE_MAP_ERROR; // Return early with error
            }
        }
    }
    else // Save file size as the map size
    {
        map->length = sb.st_size;
    }
    
    // Extract map protections from mode
    int map_prot = 0x00;
    if ( ( mode & FILE_MAP_PROT ) & FILE_MAP_PROT_READ )  map_prot |= PROT_READ;
    if ( ( mode & FILE_MAP_PROT ) & FILE_MAP_PROT_WRITE ) map_prot |= PROT_WRITE;
    if ( map_prot == 0x00 ) // Ensure at least one protection was specified
    {
        // Set the error code for upstream handling
        if ( close( map->fid ) == -1 ) // Attempt to close the file
        {
            perror("prot/close");  
        }
        else
        {
            perror("prot");
        }
        return FILE_MAP_ERROR; // Return early with error
    }

    // Extract map access from mode
    int map_access;
    if ( ( mode & FILE_MAP_ACCESS ) == FILE_MAP_ACCESS_PRIVATE )
    {
        map_access = MAP_PRIVATE;
    }
    else if ( ( mode & FILE_MAP_ACCESS ) == FILE_MAP_ACCESS_SHARED )
    {
        map_access = MAP_SHARED;
    }
    else // Ensure a valid access mode was provided
    {
        // Set the error code for upstream handling
        if ( close( map->fid ) == -1 ) // Attempt to close the file
        {
            perror("access/close");  
        }
        else
        {
            perror("access");
        }
        return FILE_MAP_ERROR; // Return early with error
    }

    // Map file
    map->address = mmap(
        NULL,       // Let the system choose where to allocate memory
        map->length, // Map the entire file
        map_prot,   // Use only the permissions specified
        map_access, // Use only the access specified
        map->fid,    // Reference to the file to map
        0           // Start at the beginning of the file
    );
    if ( map->address == MAP_FAILED )
    {
        // Set the error code for upstream handling
        if ( close( map->fid ) == -1 ) // Attempt to close the file
        {
            perror("mmap/close");  
        }
        else
        {
            perror("mmap");
        }
        return FILE_MAP_ERROR; // Return early with error
    }

    // Return OK or WARN if necessary
    return status;
}
int unmap_file(
    struct file_map* map
)
{
    // Foward declarations
    int status;
    
    // Attempt to unmap file
    status = munmap(
        map->address,
        map->length
    );
    if ( status == -1 )
    {
        perror("munmap");       // Set the error code for upstream handling
        return FILE_MAP_ERROR;  // Return early with error
    }

    // Return OK
    return FILE_MAP_OK;
}

// Ensure this code plays nice with C++
#ifdef __cplusplus
// Scope if this is being built in AHIO super project
#ifdef __ahio
} // end of namespace ahio
#endif
#endif
