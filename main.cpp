#include <iostream>
#include <string>
#include "managers/IndexManager.h"
#include "Repositories/DoctorRepository.h"
#include "Repositories/AppointmentRepository.h"
#include "QueryEngine/engine.hpp"

using namespace std;

void printWelcome() {
    cout << "\n===== Hospital Agenda =====\n";
    cout << "1.  Add New Doctor\n";
    cout << "2.  Add New Appointment\n";
    cout << "3.  Update Doctor Name (Doctor ID)\n";
    cout << "4.  Update Doctor ID [Key Update Test]\n";
    cout << "5. Update Appointment Date (Appointment ID)\n";
    cout << "6. Delete Appointment (Appointment ID)\n";
    cout << "7. Delete Doctor (Doctor ID)\n";
    cout << "8. Print Doctor Info (Doctor ID)\n";
    cout << "9. Print Appointment Info (Appointment ID)\n";
    cout << "10. Write Query\n";
    cout << "11. Exit\n";
    cout << "===========================\n";
    cout << "Choose an option: ";
}

string prompt(const string& message) {
    cout << message;
    string value;
    getline(cin, value);
    return value;
}

void printRecord(const Record& r) {
    for (const auto& [key, value] : r) {
        if (const string* s = get_if<string>(&value))
            cout << key << ": " << *s << "\n";
        else if (const double* d = get_if<double>(&value))
            cout << key << ": " << *d << "\n";
        else if (const bool* b = get_if<bool>(&value))
            cout << key << ": " << (*b ? "TRUE" : "FALSE") << "\n";
        else
            cout << key << ": NULL\n";
    }
}

void addNewDoctor(DoctorRepository& doctorRepo) {
    string id = prompt("Doctor ID: ");
    string name = prompt("Doctor Name: ");
    string specialty = prompt("Doctor Specialty: ");
    string address = prompt("Address: ");
    try {
        doctorRepo.addDoctor(Doctor(id, name, specialty, address));
        cout << "Doctor added successfully.\n";
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}

void addNewAppointment(AppointmentRepository& appointmentRepo) {
    string id = prompt("Appointment ID: ");
    string date = prompt("Appointment Date: ");
    string docId = prompt("Doctor ID: ");
    if (appointmentRepo.add(Appointment(id, date, docId)))
        cout << "Appointment added successfully.\n";
}

void updateDoctorName(DoctorRepository& doctorRepo) {
    string id = prompt("Doctor ID: ");
    string newName = prompt("New Name: ");
    try {
        doctorRepo.updateDoctorName(id, newName);
        cout << "Doctor name updated successfully.\n";
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}

void updateDoctorId(DoctorRepository& doctorRepo) {
    string oldId = prompt("Current Doctor ID: ");
    string newId = prompt("New Doctor ID: ");
    try {
        doctorRepo.updateDoctorId(oldId, newId);
        cout << "Doctor ID updated successfully.\n";
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}

void updateAppointmentDate(AppointmentRepository& appointmentRepo) {
    string id = prompt("Appointment ID: ");
    string newDate = prompt("New Date: ");
    if (appointmentRepo.updateDate(id, newDate))
        cout << "Appointment date updated successfully.\n";
}

void deleteAppointment(AppointmentRepository& appointmentRepo) {
    string id = prompt("Appointment ID: ");
    if (appointmentRepo.remove(id))
        cout << "Appointment deleted successfully.\n";
}

void deleteDoctor(DoctorRepository& doctorRepo) {
    string id = prompt("Doctor ID: ");
    try {
        doctorRepo.deleteDoctor(id);
        cout << "Doctor deleted successfully.\n";
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}

void printDoctorInfo(DoctorRepository& doctorRepo) {
    string id = prompt("Doctor ID: ");
    try {
        int rrn = doctorRepo.findDoctorRRN(id);
        if (rrn == -1) {
            cout << "Doctor not found.\n";
            return;
        }
        doctorRepo.getDoctor(rrn).print();
    } catch (const exception& e) {
        cout << e.what() << "\n";
    }
}

void printAppointmentInfo(AppointmentRepository& appointmentRepo) {
    string id = prompt("Appointment ID: ");
    optional<Appointment> app = appointmentRepo.getById(id);
    if (!app.has_value()) {
        cout << "Appointment not found.\n";
        return;
    }
    app->print();
}

void writeQuery(Engine& engine) {
    cout << "Query (e.g. SELECT FROM Doctor WHERE DoctorSpecialty = 'cardio'): ";
    string query;
    getline(cin, query);
    try {
        Result result = engine.execute(query);
        for (const auto& r : result.records) {
            cout << "----\n";
            printRecord(r);
        }
        if (result.records.empty() && result.affectedRows == 0)
            cout << "No rows affected.\n";
        else
            cout << result.affectedRows << " row(s) affected.\n";
    } catch (const exception& e) {
        cout << "Query error: " << e.what() << "\n";
    }
}

int main() {
    IndexManager indexManager;
    DoctorRepository doctorRepo("data/Doctors.bin", indexManager);
    AppointmentRepository appointmentRepo("data/Appointments.bin", indexManager);
    QueryExecutor executor(doctorRepo, appointmentRepo);
    Engine engine(executor);

    while (true) {
        printWelcome();
        string line;
        if (!getline(cin, line)) break;

        int choice = 0;
        try {
            choice = stoi(line);
        } catch (const exception&) {
            continue;
        }

        if (choice == 1) addNewDoctor(doctorRepo);
        else if (choice == 2) addNewAppointment(appointmentRepo);
        else if (choice == 3) updateDoctorName(doctorRepo);
        else if (choice == 4) updateDoctorId(doctorRepo);
        else if (choice == 5) updateAppointmentDate(appointmentRepo);
        else if (choice == 6) deleteAppointment(appointmentRepo);
        else if (choice == 7) deleteDoctor(doctorRepo);
        else if (choice == 8) printDoctorInfo(doctorRepo);
        else if (choice == 9) printAppointmentInfo(appointmentRepo);
        else if (choice == 10) writeQuery(engine);
        else if (choice == 11) break;
    }
    return 0;
}
