
// similar to a simple file copy utility, similar to the cp command

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }
    
    int src = open(argv[1], O_RDONLY);
    
    if (src == -1) {
        fprintf(stderr, "Failed to open source file\n");
        return 1;
    }
    
    int dst = open(argv[2],
    O_WRONLY | O_CREAT | O_TRUNC,
    0644);
    
    if (dst == -1) {
        fprintf(stderr, "Failed to open destination file\n");
        close(src);
        return 1;
    }
    
    char buffer[4096];
    
    while (1) {
        ssize_t bytes_read = read(src, buffer, sizeof(buffer));
        
        if (bytes_read == 0) {
            // EOF
            break;
        }
        
        if (bytes_read == -1) {
            fprintf(stderr, "Read error\n");
            close(src);
            close(dst);
            return 1;
        }
        
        ssize_t total_written = 0;
        
        while (total_written < bytes_read) {
            ssize_t bytes_written =
            write(dst,
            buffer + total_written,
            bytes_read - total_written);
            
            if (bytes_written == -1) {
                fprintf(stderr, "Write error\n");
                close(src);
                close(dst);
                return 1;
            }
            
            total_written += bytes_written;
        }
    }
    
    close(src);
    close(dst);
    
    return 0;
}