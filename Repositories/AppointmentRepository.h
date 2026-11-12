#pragma once

#include <string>
#include <vector>
#include <optional>
#include "../managers/FileManager.h"
#include "../managers/IndexManager.h"
#include "../models/Appointment.h"

using namespace std;

class AppointmentRepository {
private:
    FileManager<Appointment> fileManager;
    IndexManager& indexManager;

public:

    AppointmentRepository(const string& dataFilePath, IndexManager& idxMgr);

    bool add(const Appointment& appointment);
    bool remove(const string& id);
    bool updateDate(const string& id, const string& newDate);
    optional<Appointment> getById(const string& id);
    vector<Appointment> getByDoctorId(const string& docId);
    
    vector<Appointment> getAll();
};