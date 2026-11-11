#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <bits/stdc++.h>
using namespace std;
struct Appointment {
    char AppointmentID[15];
    char AppointmentDate[30];
    char DoctorID[15];
    Appointment();
    Appointment(const string& id, const string& date, const string& doc_id);

    void setAppointmentID(const string& id);
    void setAppointmentDate(const string& date);
    void setDoctorID(const string& doc_id);

    string getAppointmentID() const;
    string getAppointmentDate() const;
    string getDoctorID() const;

    bool isDeleted() const;
    void print() const;
};

#endif