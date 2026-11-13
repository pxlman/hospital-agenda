#include "FileManager.h"
#include "../models/Doctor.h"
#include "../models/Appointment.h"
#include <filesystem>

namespace fs = std::filesystem;

template <typename T>
void FileManager<T>::initializeFile() {
    fs::path path(filePath);

    if (!path.parent_path().empty() && !fs::exists(path.parent_path())) {
        fs::create_directories(path.parent_path());
    }

    if (fs::is_directory(path)) {
        throw runtime_error("Path is a directory, expected file: " + filePath);
    }

    ifstream checker(filePath, ios::binary | ios::ate);
    streampos fileSize = checker.is_open() ? checker.tellg() : streampos(0);
    checker.close();

    if (fileSize != 0) return;

    ofstream creator(filePath, ios::binary);
    if (!creator.is_open()) {
        throw runtime_error("Cannot create file: " + filePath);
    }
    int initialAvailHead = -1;
    creator.write(reinterpret_cast<const char*>(&initialAvailHead), sizeof(int));
    creator.close();
}

template <typename T>
FileManager<T>::FileManager(const string& path) : filePath(path) {
    initializeFile();
}

template <typename T>
int FileManager<T>::getAvailHead() {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filePath);
    }

    int availHead = -1;
    file.read(reinterpret_cast<char*>(&availHead), sizeof(int));
    file.close();
    return availHead;
}

template <typename T>
void FileManager<T>::setAvailHead(int availHead) {
    fstream file(filePath, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file: " + filePath);
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char*>(&availHead), sizeof(int));
    file.close();
}

template <typename T>
void FileManager<T>::writeRecord(int recordIndex, const T& record) {
    fstream file(filePath, ios::in | ios::out | ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file for writing: " + filePath);
    }

    int byteOffset = HEADER_SIZE + (recordIndex * sizeof(T));
    file.seekp(byteOffset);
    file.write(reinterpret_cast<const char*>(&record), sizeof(T));
    file.close();
}

template <typename T>
T FileManager<T>::readRecord(int recordIndex) {
    ifstream file(filePath, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Cannot open file for reading: " + filePath);
    }
    T record;

    int byteOffset = HEADER_SIZE + (recordIndex * sizeof(T));
    file.seekg(byteOffset);
    file.read(reinterpret_cast<char*>(&record), sizeof(T));
    file.close();

    return record;
}

template <typename T>
int FileManager<T>::addRecord(const T& record) {
    int availHead = getAvailHead();
    int targetIndex;

    if (availHead != -1) {
        targetIndex = availHead;
        ifstream file(filePath, ios::binary);
        int nextAvail = -1;

        if (file.is_open()) {
            int byteOffset = HEADER_SIZE + (targetIndex * sizeof(T)) + 1;
            file.seekg(byteOffset);
            file.read(reinterpret_cast<char*>(&nextAvail), sizeof(int));
            file.close();
        }

        setAvailHead(nextAvail);
        writeRecord(targetIndex, record);

    } else {
        targetIndex = getRecordCount();
        writeRecord(targetIndex, record);
    }
    
    return targetIndex;
}

template <typename T>
void FileManager<T>::updateRecord(int recordIndex, const T& record) {
    writeRecord(recordIndex, record);
}

template <typename T>
void FileManager<T>::deleteRecord(int recordIndex) {
    fstream file(filePath, ios::in | ios::out | ios::binary);
    if (!file.is_open()) return;

    int currentHead = getAvailHead();
    int byteOffset = HEADER_SIZE + (recordIndex * sizeof(T));
    file.seekp(byteOffset);

    char marker = '*';
    file.write(&marker, 1);
    file.write(reinterpret_cast<const char*>(&currentHead), sizeof(int));
    file.close();
    setAvailHead(recordIndex);
}

template <typename T>
int FileManager<T>::getRecordCount() {
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file.is_open()) return 0;

    streampos fileSize = file.tellg();
    file.close();

    long dataSize = fileSize - static_cast<long>(HEADER_SIZE);
    if (dataSize <= 0) return 0;

    return dataSize / sizeof(T);
}

template class FileManager<Doctor>;
template class FileManager<Appointment>;