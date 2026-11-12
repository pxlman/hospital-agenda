#pragma once

#include <vector>
#include <fstream>
#include <cstring>
#include "../models/Doctor.h"
#include "../models/Appointment.h"
#include "../dtos/DoctorPkIndex.hpp"
#include "../dtos/DoctorIdIndex.hpp"
#include "../dtos/DoctorNameIndex.hpp"
#include "../dtos/AppointmentPkIndex.hpp"
#include "../dtos/DoctorSpecialtyIndex.hpp"


using namespace std;
using namespace IndexEntry;


struct LinkNode {
    string primaryKey;
    int nextNodeIndex;
};

class IndexManager {
    private:

    // PK index
    vector<AppointmentPkIndex> appointmentPkIndex;
    vector<DoctorPkIndex> doctorPkIndex;

    // SK index
    vector<DoctorIdIndex> doctorIdIndex;
    vector<DoctorNameIndex> doctorNameIndex;
    vector<DoctorSpecialtyIndex> doctorSpecialtyIndex;

    // SK link index
    vector<LinkNode> doctorIdLink;
    vector<LinkNode> doctorNameLink;
    vector<LinkNode> doctorSpecialtyLink;


    int binarySearchString(const vector<DoctorPkIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorNameIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorSpecialtyIndex>& arr, const string& target);
    int binarySearchString(const vector<AppointmentPkIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorIdIndex>& arr, const string& target);


    public:

        IndexManager();

        int getDoctorNameLinkSize() const { return doctorNameLink.size(); }
        int getDoctorIdLinkSize() const { return doctorIdLink.size(); }

        // file functions
        void saveIndexes();
        void loadIndexes();

        // doctor
        void addDoctor(const Doctor& doctor, int rrn);
        void deleteDoctor(const Doctor& doctor);
        void updateDoctor(const Doctor& oldDoctor, const Doctor& newDoctor, int rrn);

        // appointment
        void addAppointment(const Appointment& appointment, int rrn);
        void deleteAppointment(const Appointment& appointment);
        void updateAppointment(const Appointment& oldApp, const Appointment& newApp, int rrn);

        // PK
        int getDoctorRecNoByPk(const string& id);
        int getAppointmentRecNoByPk(const string& id);

        // SK
        vector<int> getDoctorRecNoByName(const string& name);
        vector<int> getDoctorRecNoBySpecialty(const string& specialty);
        vector<int> getAppointmentRecNoByDoctorId(const string& doctorId);
};
