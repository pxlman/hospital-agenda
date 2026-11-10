#include "IndexManager.h"


template <typename T>
T* IndexManager::binarySearch(const vector<T> &arr, const T &target){
    int l = 0, r = arr.size() - 1;

    while (l <= r){
        int mid = (l + r) / 2;

        if (arr[mid] < target){
            l = mid + 1;
        }
        else if (arr[mid] > target){
            r = mid - 1;
        }
        else{
            return &arr[mid];
        }
    }

    return nullptr;
}


IndexManager::IndexManager(){

}

// doctor

void IndexManager::addDoctor(){

}

void IndexManager::deleteDoctor(){

}

void IndexManager::updateDoctor(){

}

// appointment

void IndexManager::addAppointment(){

}

void IndexManager::deleteAppointment(){

}

void IndexManager::updateAppointment(){

}

// pk search

int IndexManager::getDoctorRecNoByPk(){

}

int IndexManager::getAppointmentRecNoByPk(){

}

// sk search

vector<DoctorNameIndex> IndexManager::getDoctorRecNoByName(){

}

vector<DoctorSpecialtyIndex> IndexManager::getDoctorRecNoBySpecialty(){

}

vector<DoctorIdIndex> IndexManager::getAppointmentRecNoByDoctorId(){

}