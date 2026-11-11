#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include <fstream>
#include <cstring>

using namespace std;

template <typename T>
class FileManager {
private:
    string filePath;
    const int HEADER_SIZE = sizeof(int);
    void initializeFile();
    void writeRecord(int recordIndex, const T& record);

public:
    FileManager(const string& path);
    
    int getAvailHead();
    void setAvailHead(int availHead);
    T readRecord(int recordIndex);
    int addRecord(const T& record);
    void updateRecord(int recordIndex, const T& record);
    void deleteRecord(int recordIndex);
    
    int getRecordCount();
};

#endif