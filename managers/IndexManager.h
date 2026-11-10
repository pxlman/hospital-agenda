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

    vector<AppointmentPkIndex> appointmentPkIndex;
    vector<DoctorIdIndex> doctorIdIndex;
    vector<DoctorPkIndex> doctorPkIndex;
    vector<DoctorNameIndex> doctorNameIndex;
    vector<DoctorSpecialtyIndex> doctorSpecialtyIndex;

    template <typename T> 
    T* binarySearch(const vector<T> &arr, const T &target);


    public:

        IndexManager();

        void addDoctor();
        void deleteDoctor();
        void updateDoctor();

        void addAppointment();
        void deleteAppointment();
        void updateAppointment();

        int getDoctorRecNoByPk();
        int getAppointmentRecNoByPk();

        vector<DoctorNameIndex> getDoctorRecNoByName();
        vector<DoctorSpecialtyIndex> getDoctorRecNoBySpecialty();
        vector<DoctorIdIndex> getAppointmentRecNoByDoctorId();
};
