#ifndef DOCTOR_REPOSITORY_H
#define DOCTOR_REPOSITORY_H

#include <string>
#include "../models/Doctor.h"
#include "../managers/FileManager.h"
#include "../managers/IndexManager.h"

using namespace std;

class DoctorRepository {
private:
    FileManager<Doctor> fileManager;
    IndexManager& indexManager;
    string dataFilePath;

public:
    DoctorRepository(const string& path, IndexManager& idxMgr);

    bool addDoctor(const Doctor& doctor);
    bool deleteDoctor(const string& doctorId);
    bool updateDoctorName(const string& doctorId, const string& newName);
    bool updateDoctorId(const string& oldDoctorId, const string& newDoctorId);
    bool updateDoctorNonKey(const string& doctorId, const string& newAddress, const string& newSpecialty);
    
    int findDoctorRRN(const string& doctorId);
    Doctor getDoctor(int rrn);
    
    void loadAllIndexes();
    void saveAllIndexes();

    vector<Doctor> getAllDoctors();
};

#endif
