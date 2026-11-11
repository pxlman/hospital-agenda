#include "FileManager.h"
#include <bits/stdc++.h>

using namespace std;

template <typename T>
class FileManager {
private:
    string filePath;
    const int HEADER_SIZE = sizeof(int);

    // Ensures the file exists and initializes the 4-byte avail list header if empty
    void initializeFile() {
        ifstream checker(filePath, ios::binary);
        if (!checker.is_open()) {
            // File doesn't exist, create it and write initial avail list head (-1)
            ofstream creator(filePath, ios::binary);
            int initialAvailHead = -1;
            creator.write(reinterpret_cast<const char*>(&initialAvailHead), sizeof(int));
            creator.close();
        }
        checker.close();
    }

public:
    FileManager(const string& path) : filePath(path) {
        initializeFile();
    }

    // Read the 4-byte header integer pointing to the head of the AVAIL LIST[cite: 1, 3]
    int getAvailHead() {
        ifstream file(filePath, ios::binary);
        if (!file.is_open()) return -1;

        int availHead = -1;
        file.read(reinterpret_cast<char*>(&availHead), sizeof(int));
        file.close();
        return availHead;
    }

    // Update the 4-byte header integer pointing to the head of the AVAIL LIST[cite: 1, 3]
    void setAvailHead(int availHead) {
        fstream file(filePath, ios::in | ios::out | ios::binary);
        if (!file.is_open()) return;

        file.seekp(0);
        file.write(reinterpret_cast<const char*>(&availHead), sizeof(int));
        file.close();
    }

    // Write a record at a specific byte offset (accounts for the 4-byte header shift)[cite: 1, 3]
    void writeRecord(int recordIndex, const T& record) {
        fstream file(filePath, ios::in | ios::out | ios::binary);
        if (!file.is_open()) return;

        // Calculate exact byte position: Header offset (4 bytes) + (Record Index * Record Size)
        int byteOffset = HEADER_SIZE + (recordIndex * sizeof(T));
        
        file.seekp(byteOffset);
        file.write(reinterpret_cast<const char*>(&record), sizeof(T));
        file.close();
    }

    // Read a record from a specific byte offset[cite: 1, 3]
    T readRecord(int recordIndex) {
        ifstream file(filePath, ios::binary);
        T record;

        int byteOffset = HEADER_SIZE + (recordIndex * sizeof(T));
        file.seekg(byteOffset);
        file.read(reinterpret_cast<char*>(&record), sizeof(T));
        file.close();

        return record;
    }

    // Get total number of active/deleted records currently in the file (excluding header)
    int getRecordCount() {
        ifstream file(filePath, ios::binary | ios::ate);
        if (!file.is_open()) return 0;

        streampos fileSize = file.tellg();
        file.close();

        long dataSize = fileSize - static_cast<long>(HEADER_SIZE);
        if (dataSize <= 0) return 0;

        return dataSize / sizeof(T);
    }
};