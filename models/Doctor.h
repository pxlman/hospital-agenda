#ifndef DOCTOR_H
#define DOCTOR_H

#include <bits/stdc++.h>
using namespace std;

struct Doctor {
    char DoctorID[15];
    char DoctorName[30];
    char DoctorSpecialty[20];
    char Address[30];
    Doctor();
    Doctor(const string& id, const string& name, const string& specialty, const string& address);

    void setDoctorID(const string& id);
    void setDoctorName(const string& name);
    void setDoctorSpecialty(const string& specialty);
    void setAddress(const string& address);

    string getDoctorID() const;
    string getDoctorName() const;
    string getDoctorSpecialty() const;
    string getAddress() const;

    bool isDeleted() const;
    void print() const;
};

#endif