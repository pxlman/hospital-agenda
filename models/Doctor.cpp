#include "Doctor.h"
#include <cstring>

namespace {
template <size_t N>
void copyText(char (&destination)[N], const string& value) {
    strncpy(destination, value.c_str(), N - 1);
    destination[N - 1] = '\0';
}
}

Doctor::Doctor(const string& id, const string& name, const string& specialty, const string& address) {
    setDoctorID(id);
    setDoctorName(name);
    setDoctorSpecialty(specialty);
    setAddress(address);
}

void Doctor::setDoctorID(const string& id) {
    copyText(DoctorID, id);
}

void Doctor::setDoctorName(const string& name) {
    copyText(DoctorName, name);
}

void Doctor::setDoctorSpecialty(const string& specialty) {
    copyText(DoctorSpecialty, specialty);
}

void Doctor::setAddress(const string& address) {
    copyText(Address, address);
}

string Doctor::getDoctorID() const {
    return DoctorID;
}

string Doctor::getDoctorName() const {
    return DoctorName;
}

string Doctor::getDoctorSpecialty() const {
    return DoctorSpecialty;
}

string Doctor::getAddress() const {
    return Address;
}