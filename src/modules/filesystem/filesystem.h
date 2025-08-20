#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <LittleFS.h>
#include <vector>

std::vector<String> GetFileList(const String& directry_path, const std::vector<String>& extensions);

#endif