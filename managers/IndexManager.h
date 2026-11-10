#pragma once

#include <vector>
#include "../models/Doctor.h"
#include "../models/Appointment.h"
#include "../models/DoctorPkIndex.hpp"
#include "../models/DoctorIdIndex.hpp"
#include "../models/DoctorNameIndex.hpp"
#include "../models/AppointmentPkIndex.hpp"
#include "../models/DoctorSpecialtyIndex.hpp"


using namespace std;
using namespace IndexEntry;

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
    vector<int> doctorIdLink;
    vector<int> doctorNameLink;
    vector<int> doctorSpecialtyLink;


    int binarySearchString(const vector<DoctorPkIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorNameIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorSpecialtyIndex>& arr, const string& target);
    int binarySearchString(const vector<AppointmentPkIndex>& arr, const string& target);
    int binarySearchString(const vector<DoctorIdIndex>& arr, const string& target);


    public:

        IndexManager();

        void initLinks(int maxDoctors, int maxAppointments);

        // doctor
        void addDoctor(const Doctor& doctor, int rrn);
        void deleteDoctor(const Doctor& doctor);
        void updateDoctor(const string& id, const Doctor& newDoctor);

        // appointment
        void addAppointment(const Appointment& appointment, int rrn);
        void deleteAppointment(const Appointment& appointment);
        void updateAppointment(const string& id, const Appointment& newAppointment);

        // PK
        int getDoctorRecNoByPk(const string& id);
        int getAppointmentRecNoByPk(const string& id);

        // SK
        vector<int> getDoctorRecNoByName(const string& name);
        vector<int> getDoctorRecNoBySpecialty(const string& specialty);
        vector<int> getAppointmentRecNoByDoctorId(const string& doctorId);
};
