#include "DoctorRepository.h"
#include "../models/Appointment.h"
#include "../managers/FileManager.h"
#include <iostream>
#include <stdexcept>
#include <vector>

DoctorRepository::DoctorRepository(const string& path) 
    : fileManager(path), dataFilePath(path) {
    indexManager.loadIndexes();
}

int DoctorRepository::findDoctorRRN(const string& doctorId) {
    return indexManager.getDoctorRecNoByPk(doctorId);
}

bool DoctorRepository::addDoctor(const Doctor& doctor) {
    if (doctor.getDoctorID().length() > 15) {
        throw invalid_argument("Error: Doctor ID exceeds maximum allowed length of 15 characters.");
    }
    if (doctor.getDoctorName().length() > 30) {
        throw invalid_argument("Error: Doctor Name exceeds maximum allowed length of 30 characters.");
    }
    if (doctor.getDoctorSpecialty().length() > 20) {
        throw invalid_argument("Error: Doctor Specialty exceeds maximum allowed length of 20 characters.");
    }
    if (doctor.getAddress().length() > 30) {
        throw invalid_argument("Error: Address exceeds maximum allowed length.");
    }

    if (findDoctorRRN(doctor.getDoctorID()) != -1) {
        throw runtime_error("Error: Doctor with ID " + doctor.getDoctorID() + " already exists!");
    }

    int rrn = fileManager.addRecord(doctor);
    indexManager.addDoctor(doctor, rrn);
    indexManager.saveIndexes();

    return true;
}

bool DoctorRepository::deleteDoctor(const string& doctorId) {
    int rrn = findDoctorRRN(doctorId);
    if (rrn == -1) {
        throw runtime_error("Error: Doctor with ID " + doctorId + " does not exist!");
    }

    Doctor doctor = fileManager.readRecord(rrn);

    FileManager<Appointment> appointmentFileManager("../data/Appointments.bin");
    vector<int> relatedAppointmentRRNs = indexManager.getAppointmentRecNoByDoctorId(doctorId);

    for (int appRrn : relatedAppointmentRRNs) {
        Appointment appointment = appointmentFileManager.readRecord(appRrn);
        appointmentFileManager.deleteRecord(appRrn);
        indexManager.deleteAppointment(appointment);
    }

    fileManager.deleteRecord(rrn);
    indexManager.deleteDoctor(doctor);
    indexManager.saveIndexes();

    return true;
}

bool DoctorRepository::updateDoctorName(const string& doctorId, const string& newName) {
    if (newName.length() > 30) {
        throw invalid_argument("Error: New Doctor Name exceeds maximum allowed length of 30 characters.");
    }

    int rrn = findDoctorRRN(doctorId);
    if (rrn == -1) {
        throw runtime_error("Error: Doctor with ID " + doctorId + " does not exist!");
    }

    Doctor oldDoctor = fileManager.readRecord(rrn);
    Doctor newDoctor = oldDoctor;
    newDoctor.setDoctorName(newName);

    fileManager.updateRecord(rrn, newDoctor);
    indexManager.updateDoctor(oldDoctor, newDoctor, rrn);
    indexManager.saveIndexes();
    return true;
}

bool DoctorRepository::updateDoctorId(const string& oldDoctorId, const string& newDoctorId) {
    if (newDoctorId.length() > 15) {
        throw invalid_argument("Error: New Doctor ID exceeds maximum allowed length of 15 characters.");
    }

    int rrn = findDoctorRRN(oldDoctorId);
    if (rrn == -1) {
        throw runtime_error("Error: Doctor with ID " + oldDoctorId + " does not exist!");
    }

    if (findDoctorRRN(newDoctorId) != -1) {
        throw runtime_error("Error: Target Doctor ID " + newDoctorId + " already exists!");
    }

    Doctor oldDoctor = fileManager.readRecord(rrn);
    Doctor newDoctor = oldDoctor;
    newDoctor.setDoctorID(newDoctorId);

    fileManager.updateRecord(rrn, newDoctor);
    indexManager.updateDoctor(oldDoctor, newDoctor, rrn);

    FileManager<Appointment> appointmentFileManager("../data/Appointments.bin");
    vector<int> relatedAppointmentRRNs = indexManager.getAppointmentRecNoByDoctorId(oldDoctorId);

    for (int appRrn : relatedAppointmentRRNs) {
        Appointment oldApp = appointmentFileManager.readRecord(appRrn);
        Appointment newApp = oldApp;
        
        newApp.setDoctorID(newDoctorId);
        appointmentFileManager.updateRecord(appRrn, newApp);
        indexManager.updateAppointment(oldApp, newApp, appRrn);
    }

    indexManager.saveIndexes();
    return true;
}

bool DoctorRepository::updateDoctorNonKey(const string& doctorId, const string& newAddress, const string& newSpecialty) {
    if (!newSpecialty.empty() && newSpecialty.length() > 20) {
        throw invalid_argument("Error: Specialty exceeds maximum allowed length of 20 characters.");
    }
    if (!newAddress.empty() && newAddress.length() > 30) {
        throw invalid_argument("Error: Address exceeds maximum allowed length.");
    }

    int rrn = findDoctorRRN(doctorId);
    if (rrn == -1) {
        throw runtime_error("Warning: Doctor with ID " + doctorId + " does not exist for update!");
    }

    Doctor oldDoctor = fileManager.readRecord(rrn);
    Doctor newDoctor = oldDoctor;

    if (!newAddress.empty()) newDoctor.setAddress(newAddress);
    if (!newSpecialty.empty()) newDoctor.setDoctorSpecialty(newSpecialty);

    fileManager.updateRecord(rrn, newDoctor);
    indexManager.updateDoctor(oldDoctor, newDoctor, rrn);
    indexManager.saveIndexes();

    return true;
}

Doctor DoctorRepository::getDoctor(int rrn) {
    int recordCount = fileManager.getRecordCount();
    if (rrn < 0 || rrn >= recordCount) {
        throw out_of_range("Error: RRN out of valid file boundaries.");
    }
    return fileManager.readRecord(rrn);
}

void DoctorRepository::loadAllIndexes() {
    indexManager.loadIndexes();
}

void DoctorRepository::saveAllIndexes() {
    indexManager.saveIndexes();
}

vector<Doctor> DoctorRepository::getAllDoctors() {
    vector<Doctor> doctors;
    int recordCount = fileManager.getRecordCount();
    for (int i = 0; i < recordCount; ++i) {
        Doctor doctor = fileManager.readRecord(i);
        if (!doctor.isDeleted()) {
            doctors.push_back(doctor);
        }
    }
    return doctors;
}
