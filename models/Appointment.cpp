#include "Appointment.h"
#include <cstring>

namespace {
template <size_t N>
void copyText(char (&destination)[N], const string& value) {
    strncpy(destination, value.c_str(), N - 1);
    destination[N - 1] = '\0';
}
}

Appointment::Appointment() {
    memset(AppointmentID, ' ', sizeof(AppointmentID));
    memset(AppointmentDate, ' ', sizeof(AppointmentDate));
    memset(DoctorID, ' ', sizeof(DoctorID));
}

Appointment::Appointment(const string& id, const string& date, const string& doc_id) {
    setAppointmentID(id);
    setAppointmentDate(date);
    setDoctorID(doc_id);
}

void Appointment::setAppointmentID(const string& id) {
    copyText(AppointmentID, id);
}

void Appointment::setAppointmentDate(const string& date) {
    copyText(AppointmentDate, date);
}

void Appointment::setDoctorID(const string& doc_id) {
    copyText(DoctorID, doc_id);
}

string Appointment::getAppointmentID() const {
    return AppointmentID;
}

string Appointment::getAppointmentDate() const {
    return AppointmentDate;
}

string Appointment::getDoctorID() const {
    return DoctorID;
}

bool Appointment::isDeleted() const {
    return AppointmentID[0] == '*';
}

void Appointment::print() const {
    cout    << "Appointment ID: " << getAppointmentID() 
            << " | Date: " << getAppointmentDate() 
            << " | Doctor ID: " << getDoctorID() << "\n";
}