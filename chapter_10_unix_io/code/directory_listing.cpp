
// similar to the ls command, but with a different output format

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <string>

int main(int argc, char* argv[]) {
    const char* path = (argc == 2) ? argv[1] : ".";

    DIR* dir = opendir(path);

    if (dir == nullptr) {
        std::cerr << "Failed to open directory\n";
        return 1;
    }

    struct dirent* entry;

    while ((entry = readdir(dir)) != nullptr) {
        std::string full_path = std::string(path) + "/" + entry->d_name;

        std::cout << "Full_path: " << full_path << std::endl;
        
        struct stat info;

        if (stat(full_path.c_str(), &info) == -1) {
            std::cerr << "Failed to get metadata for "
                      << entry->d_name << '\n';
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            std::cout << "[DIR]  ";
        } else if (S_ISREG(info.st_mode)) {
            std::cout << "[FILE] ";
        } else {
            std::cout << "[OTHER]";
        }

        std::cout << entry->d_name << '\n';
    }

    closedir(dir);

    return 0;
}