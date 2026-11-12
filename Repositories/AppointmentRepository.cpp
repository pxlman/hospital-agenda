#include "AppointmentRepository.h"
#include <stdexcept>

AppointmentRepository::AppointmentRepository(const string& dataFilePath, IndexManager& idxMgr) 
    : fileManager(dataFilePath), indexManager(idxMgr) {}

bool AppointmentRepository::add(const Appointment& appointment) {
    if (indexManager.getAppointmentRecNoByPk(appointment.getAppointmentID()) != -1) {
        throw std::runtime_error("Appointment ID already exists!");
    }

    if (indexManager.getDoctorRecNoByPk(appointment.getDoctorID()) == -1) {
        throw std::runtime_error("Appointment doesn't have a valid Doctor ID!");
    }

    int rrn = fileManager.addRecord(appointment);
    indexManager.addAppointment(appointment, rrn);
    indexManager.saveIndexes();
    
    return true;
}

bool AppointmentRepository::remove(const string& id) {
    int rrn = indexManager.getAppointmentRecNoByPk(id);
    if (rrn == -1) {
        throw std::runtime_error("Appointment not found!");
    }

    Appointment oldApp = fileManager.readRecord(rrn);

    indexManager.deleteAppointment(oldApp);
    fileManager.deleteRecord(rrn);
    indexManager.saveIndexes();
    
    return true;
}

bool AppointmentRepository::updateDate(const string& id, const string& newDate) {
    int rrn = indexManager.getAppointmentRecNoByPk(id);
    if (rrn == -1) {
        throw std::runtime_error("Appointment not found!");
    }


    Appointment oldApp = fileManager.readRecord(rrn);
    Appointment newApp(oldApp.getAppointmentID(), newDate, oldApp.getDoctorID());

    fileManager.updateRecord(rrn, newApp);

    indexManager.updateAppointment(oldApp, newApp, rrn);
    indexManager.saveIndexes();

    return true;
}

optional<Appointment> AppointmentRepository::getById(const string& id) {
    int rrn = indexManager.getAppointmentRecNoByPk(id);
    if (rrn == -1) {
        return nullopt;
    }
    
    return fileManager.readRecord(rrn);
}

vector<Appointment> AppointmentRepository::getByDoctorId(const string& docId) {
    vector<Appointment> results;
    
    vector<int> rrns = indexManager.getAppointmentRecNoByDoctorId(docId);
    
    for (int rrn : rrns) {
        results.push_back(fileManager.readRecord(rrn));
    }
    
    return results;
}

vector<Appointment> AppointmentRepository::getAll() {
    vector<Appointment> results;
    int totalSlots = fileManager.getRecordCount();
    
    for (int i = 0; i < totalSlots; i++) {
        Appointment app = fileManager.readRecord(i);
        if (!app.getAppointmentID().empty() && app.getAppointmentID()[0] != '*') {
            results.push_back(app);
        }
    }
    return results;
}