#include "IndexManager.h"


using namespace std;
using namespace IndexEntry;

IndexManager::IndexManager() {}

void IndexManager::initLinks(int maxDoctors, int maxAppointments) {
    doctorNameLink.assign(maxDoctors, -1);
    doctorSpecialtyLink.assign(maxDoctors, -1);
    doctorIdLink.assign(maxAppointments, -1);
}

void writeFixedString(ofstream& out, const string& str, int size) {
    char* buffer = new char[size];
    memset(buffer, 0, size);
    strncpy(buffer, str.c_str(), size - 1);
    out.write(buffer, size);
    delete[] buffer;
}

string readFixedString(ifstream& in, int size) {
    char* buffer = new char[size + 1];
    in.read(buffer, size);
    buffer[size] = '\0';
    string result(buffer);
    delete[] buffer;
    return result;
}

int IndexManager::binarySearchString(const vector<DoctorPkIndex>& arr, const string& target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid].doctorId == target) return mid;
        if (arr[mid].doctorId < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

int IndexManager::binarySearchString(const vector<AppointmentPkIndex>& arr, const string& target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid].appointmentId == target) return mid;
        if (arr[mid].appointmentId < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

int IndexManager::binarySearchString(const vector<DoctorNameIndex>& arr, const string& target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid].name == target) return mid;
        if (arr[mid].name < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

int IndexManager::binarySearchString(const vector<DoctorSpecialtyIndex>& arr, const string& target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid].specialty == target) return mid;
        if (arr[mid].specialty < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}

int IndexManager::binarySearchString(const vector<DoctorIdIndex>& arr, const string& target) {
    int l = 0, r = arr.size() - 1;
    while (l <= r) {
        int mid = l + (r - l) / 2;
        if (arr[mid].doctorId == target) return mid;
        if (arr[mid].doctorId < target) l = mid + 1;
        else r = mid - 1;
    }
    return -1;
}


// INDEXES
void IndexManager::saveIndexes() {
    ofstream docPkOut("../data/Indexes/DoctorPK.bin", ios::binary | ios::trunc);
    for (const auto& entry : doctorPkIndex) {
        docPkOut.write(reinterpret_cast<const char*>(&entry.rrn), sizeof(int));
        writeFixedString(docPkOut, entry.doctorId, 15);
    }
    docPkOut.close();

    ofstream appPkOut("../data/Indexes/AppointmentPK.bin", ios::binary | ios::trunc);
    for (const auto& entry : appointmentPkIndex) {
        appPkOut.write(reinterpret_cast<const char*>(&entry.rrn), sizeof(int));
        writeFixedString(appPkOut, entry.appointmentId, 15);
    }
    appPkOut.close();

    ofstream nameOut("../data/Indexes/DoctorName.bin", ios::binary | ios::trunc);
    for (const auto& entry : doctorNameIndex) {
        nameOut.write(reinterpret_cast<const char*>(&entry.headRRN), sizeof(int));
        writeFixedString(nameOut, entry.name, 30);
    }
    nameOut.close();

    ofstream specOut("../data/Indexes/DoctorSpecialty.bin", ios::binary | ios::trunc);
    for (const auto& entry : doctorSpecialtyIndex) {
        specOut.write(reinterpret_cast<const char*>(&entry.headRRN), sizeof(int));
        writeFixedString(specOut, entry.specialty, 20);
    }
    specOut.close();

    ofstream docIdOut("../data/Indexes/DoctorID.bin", ios::binary | ios::trunc);
    for (const auto& entry : doctorIdIndex) {
        docIdOut.write(reinterpret_cast<const char*>(&entry.headRRN), sizeof(int));
        writeFixedString(docIdOut, entry.doctorId, 15);
    }
    docIdOut.close();

    ofstream nameLinkOut("../data/Indexes/DoctorNameLinks.bin", ios::binary | ios::trunc);
    for (int rrn : doctorNameLink) nameLinkOut.write(reinterpret_cast<const char*>(&rrn), sizeof(int));
    nameLinkOut.close();

    ofstream specLinkOut("../data/Indexes/DoctorSpecialtyLinks.bin", ios::binary | ios::trunc);
    for (int rrn : doctorSpecialtyLink) specLinkOut.write(reinterpret_cast<const char*>(&rrn), sizeof(int));
    specLinkOut.close();

    ofstream docIdLinkOut("../data/Indexes/DoctorIDLinks.bin", ios::binary | ios::trunc);
    for (int rrn : doctorIdLink) docIdLinkOut.write(reinterpret_cast<const char*>(&rrn), sizeof(int));
    docIdLinkOut.close();
}

void IndexManager::loadIndexes() {
    doctorPkIndex.clear();
    appointmentPkIndex.clear();
    doctorNameIndex.clear();
    doctorSpecialtyIndex.clear();
    doctorIdIndex.clear();
    doctorNameLink.clear();
    doctorSpecialtyLink.clear();
    doctorIdLink.clear();

    ifstream docPkIn("../data/Indexes/DoctorPK.bin", ios::binary);
    if (docPkIn.is_open()) {
        while (docPkIn.peek() != EOF) {
            DoctorPkIndex entry;
            docPkIn.read(reinterpret_cast<char*>(&entry.rrn), sizeof(int));
            entry.doctorId = readFixedString(docPkIn, 15);
            doctorPkIndex.push_back(entry);
        }
        docPkIn.close();
    }

    ifstream appPkIn("../data/Indexes/AppointmentPK.bin", ios::binary);
    if (appPkIn.is_open()) {
        while (appPkIn.peek() != EOF) {
            AppointmentPkIndex entry;
            appPkIn.read(reinterpret_cast<char*>(&entry.rrn), sizeof(int));
            entry.appointmentId = readFixedString(appPkIn, 15);
            appointmentPkIndex.push_back(entry);
        }
        appPkIn.close();
    }

    ifstream nameIn("../data/Indexes/DoctorName.bin", ios::binary);
    if (nameIn.is_open()) {
        while (nameIn.peek() != EOF) {
            DoctorNameIndex entry;
            nameIn.read(reinterpret_cast<char*>(&entry.headRRN), sizeof(int));
            entry.name = readFixedString(nameIn, 30);
            doctorNameIndex.push_back(entry);
        }
        nameIn.close();
    }

    ifstream specIn("../data/Indexes/DoctorSpecialty.bin", ios::binary);
    if (specIn.is_open()) {
        while (specIn.peek() != EOF) {
            DoctorSpecialtyIndex entry;
            specIn.read(reinterpret_cast<char*>(&entry.headRRN), sizeof(int));
            entry.specialty = readFixedString(specIn, 20);
            doctorSpecialtyIndex.push_back(entry);
        }
        specIn.close();
    }

    ifstream docIdIn("../data/Indexes/DoctorID.bin", ios::binary);
    if (docIdIn.is_open()) {
        while (docIdIn.peek() != EOF) {
            DoctorIdIndex entry;
            docIdIn.read(reinterpret_cast<char*>(&entry.headRRN), sizeof(int));
            entry.doctorId = readFixedString(docIdIn, 15);
            doctorIdIndex.push_back(entry);
        }
        docIdIn.close();
    }

    ifstream nameLinkIn("../data/Indexes/DoctorNameLinks.bin", ios::binary);
    if (nameLinkIn.is_open()) {
        int rrn;
        while (nameLinkIn.read(reinterpret_cast<char*>(&rrn), sizeof(int))) {
            doctorNameLink.push_back(rrn);
        }
        nameLinkIn.close();
    }

    ifstream specLinkIn("../data/Indexes/DoctorSpecialtyLinks.bin", ios::binary);
    if (specLinkIn.is_open()) {
        int rrn;
        while (specLinkIn.read(reinterpret_cast<char*>(&rrn), sizeof(int))) {
            doctorSpecialtyLink.push_back(rrn);
        }
        specLinkIn.close();
    }

    ifstream docIdLinkIn("../data/Indexes/DoctorIDLinks.bin", ios::binary);
    if (docIdLinkIn.is_open()) {
        int rrn;
        while (docIdLinkIn.read(reinterpret_cast<char*>(&rrn), sizeof(int))) {
            doctorIdLink.push_back(rrn);
        }
        docIdLinkIn.close();
    }
}


// DOCTORS

void IndexManager::addDoctor(const Doctor& doctor, int rrn) {
    if (getDoctorRecNoByPk(doctor.getDoctorID()) != -1) return;

    DoctorPkIndex pkEntry {rrn, doctor.getDoctorID()};
    auto it = doctorPkIndex.begin();
    while (it != doctorPkIndex.end() && it->doctorId < doctor.getDoctorID()) ++it;
    doctorPkIndex.insert(it, pkEntry);

    int nameIdx = binarySearchString(doctorNameIndex, doctor.getDoctorName());
    if (nameIdx != -1) {
        doctorNameLink[rrn] = doctorNameIndex[nameIdx].headRRN; 
        doctorNameIndex[nameIdx].headRRN = rrn; 
    } else {
        DoctorNameIndex newNameEntry {rrn, doctor.getDoctorName()};
        auto itName = doctorNameIndex.begin();
        while (itName != doctorNameIndex.end() && itName->name < doctor.getDoctorName()) ++itName;
        doctorNameIndex.insert(itName, newNameEntry);
        doctorNameLink[rrn] = -1; 
    }

    int specIdx = binarySearchString(doctorSpecialtyIndex, doctor.getDoctorSpecialty());
    if (specIdx != -1) {
        doctorSpecialtyLink[rrn] = doctorSpecialtyIndex[specIdx].headRRN;
        doctorSpecialtyIndex[specIdx].headRRN = rrn;
    } else {
        DoctorSpecialtyIndex newSpecEntry {rrn, doctor.getDoctorSpecialty()};
        auto itSpec = doctorSpecialtyIndex.begin();
        while (itSpec != doctorSpecialtyIndex.end() && itSpec->specialty < doctor.getDoctorSpecialty()) ++itSpec;
        doctorSpecialtyIndex.insert(itSpec, newSpecEntry);
        doctorSpecialtyLink[rrn] = -1;
    }
}

void IndexManager::deleteDoctor(const Doctor& doctor) {
    int rrn = getDoctorRecNoByPk(doctor.getDoctorID());
    if (rrn == -1) return;

    int pkIdx = binarySearchString(doctorPkIndex, doctor.getDoctorID());
    if (pkIdx != -1) doctorPkIndex.erase(doctorPkIndex.begin() + pkIdx);

    int nameIdx = binarySearchString(doctorNameIndex, doctor.getDoctorName());
    if (nameIdx != -1) {
        int head = doctorNameIndex[nameIdx].headRRN;
        if (head == rrn) { 
            doctorNameIndex[nameIdx].headRRN = doctorNameLink[rrn];
        } else {
            int curr = head;
            while (curr != -1 && doctorNameLink[curr] != rrn) {
                curr = doctorNameLink[curr];
            }
            if (curr != -1) {
                doctorNameLink[curr] = doctorNameLink[rrn];
            }
        }
        if (doctorNameIndex[nameIdx].headRRN == -1) {
            doctorNameIndex.erase(doctorNameIndex.begin() + nameIdx);
        }
    }

    int specIdx = binarySearchString(doctorSpecialtyIndex, doctor.getDoctorSpecialty());
    if (specIdx != -1) {
        int head = doctorSpecialtyIndex[specIdx].headRRN;
        if (head == rrn) {
            doctorSpecialtyIndex[specIdx].headRRN = doctorSpecialtyLink[rrn];
        } else {
            int curr = head;
            while (curr != -1 && doctorSpecialtyLink[curr] != rrn) {
                curr = doctorSpecialtyLink[curr];
            }
            if (curr != -1) doctorSpecialtyLink[curr] = doctorSpecialtyLink[rrn];
        }
        if (doctorSpecialtyIndex[specIdx].headRRN == -1) {
            doctorSpecialtyIndex.erase(doctorSpecialtyIndex.begin() + specIdx);
        }
    }
}


void IndexManager::updateDoctor(const Doctor& oldDoctor, const Doctor& newDoctor, int rrn) {
    if (oldDoctor.getDoctorID() != newDoctor.getDoctorID()) {
        int pkIdx = binarySearchString(doctorPkIndex, oldDoctor.getDoctorID());
        if (pkIdx != -1) {
            doctorPkIndex.erase(doctorPkIndex.begin() + pkIdx);
        }

        DoctorPkIndex newPkEntry {rrn, newDoctor.getDoctorID()};
        auto it = doctorPkIndex.begin();
        while (it != doctorPkIndex.end() && it->doctorId < newDoctor.getDoctorID()) {
            ++it;
        }
        doctorPkIndex.insert(it, newPkEntry);
    }

    if (oldDoctor.getDoctorName() != newDoctor.getDoctorName()) {
        int oldNameIdx = binarySearchString(doctorNameIndex, oldDoctor.getDoctorName());
        if (oldNameIdx != -1) {
            int head = doctorNameIndex[oldNameIdx].headRRN;
            if (head == rrn) {
                doctorNameIndex[oldNameIdx].headRRN = doctorNameLink[rrn];
            } else {
                int curr = head;
                while (curr != -1 && doctorNameLink[curr] != rrn) {
                    curr = doctorNameLink[curr];
                }
                if (curr != -1) {
                    doctorNameLink[curr] = doctorNameLink[rrn];
                }
            }
            if (doctorNameIndex[oldNameIdx].headRRN == -1) {
                doctorNameIndex.erase(doctorNameIndex.begin() + oldNameIdx);
            }
        }

        int newNameIdx = binarySearchString(doctorNameIndex, newDoctor.getDoctorName());
        if (newNameIdx != -1) {
            doctorNameLink[rrn] = doctorNameIndex[newNameIdx].headRRN;
            doctorNameIndex[newNameIdx].headRRN = rrn;
        } else {
            DoctorNameIndex newEntry {rrn, newDoctor.getDoctorName()};
            auto itName = doctorNameIndex.begin();
            while (itName != doctorNameIndex.end() && itName->name < newDoctor.getDoctorName()) {
                ++itName;
            }
            doctorNameIndex.insert(itName, newEntry);
            doctorNameLink[rrn] = -1;
        }
    }

    if (oldDoctor.getDoctorSpecialty() != newDoctor.getDoctorSpecialty()) {
        int oldSpecIdx = binarySearchString(doctorSpecialtyIndex, oldDoctor.getDoctorSpecialty());
        if (oldSpecIdx != -1) {
            int head = doctorSpecialtyIndex[oldSpecIdx].headRRN;
            if (head == rrn) {
                doctorSpecialtyIndex[oldSpecIdx].headRRN = doctorSpecialtyLink[rrn];
            } else {
                int curr = head;
                while (curr != -1 && doctorSpecialtyLink[curr] != rrn) {
                    curr = doctorSpecialtyLink[curr];
                }
                if (curr != -1) {
                    doctorSpecialtyLink[curr] = doctorSpecialtyLink[rrn];
                }
            }
            if (doctorSpecialtyIndex[oldSpecIdx].headRRN == -1) {
                doctorSpecialtyIndex.erase(doctorSpecialtyIndex.begin() + oldSpecIdx);
            }
        }

        int newSpecIdx = binarySearchString(doctorSpecialtyIndex, newDoctor.getDoctorSpecialty());
        if (newSpecIdx != -1) {
            doctorSpecialtyLink[rrn] = doctorSpecialtyIndex[newSpecIdx].headRRN;
            doctorSpecialtyIndex[newSpecIdx].headRRN = rrn;
        } else {
            DoctorSpecialtyIndex newEntry {rrn, newDoctor.getDoctorSpecialty()};
            auto itSpec = doctorSpecialtyIndex.begin();
            while (itSpec != doctorSpecialtyIndex.end() && itSpec->specialty < newDoctor.getDoctorSpecialty()) {
                ++itSpec;
            }
            doctorSpecialtyIndex.insert(itSpec, newEntry);
            doctorSpecialtyLink[rrn] = -1;
        }
    }
}

int IndexManager::getDoctorRecNoByPk(const string& id) {
    int idx = binarySearchString(doctorPkIndex, id);
    return (idx != -1) ? doctorPkIndex[idx].rrn : -1;
}

vector<int> IndexManager::getDoctorRecNoByName(const string& name) {
    vector<int> results;
    int idx = binarySearchString(doctorNameIndex, name);
    if (idx == -1) return results;

    int curr = doctorNameIndex[idx].headRRN;
    while (curr != -1) {
        results.push_back(curr); 
        curr = doctorNameLink[curr]; 
    }
    return results;
}

vector<int> IndexManager::getDoctorRecNoBySpecialty(const string& specialty) {
    vector<int> results;
    int idx = binarySearchString(doctorSpecialtyIndex, specialty);
    if (idx == -1) return results;

    int curr = doctorSpecialtyIndex[idx].headRRN;
    while (curr != -1) {
        results.push_back(curr);
        curr = doctorSpecialtyLink[curr];
    }
    return results;
}


// APPOINTMENTS

void IndexManager::addAppointment(const Appointment& appointment, int rrn) {
    if (getAppointmentRecNoByPk(appointment.getAppointmentID()) != -1) return;

    AppointmentPkIndex pkEntry {rrn, appointment.getAppointmentID()};
    auto it = appointmentPkIndex.begin();
    while (it != appointmentPkIndex.end() && it->appointmentId < appointment.getAppointmentID()) ++it;
    appointmentPkIndex.insert(it, pkEntry);

    int docIdx = binarySearchString(doctorIdIndex, appointment.getDoctorID());
    if (docIdx != -1) {
        doctorIdLink[rrn] = doctorIdIndex[docIdx].headRRN;
        doctorIdIndex[docIdx].headRRN = rrn;
    } else {
        DoctorIdIndex newDocEntry {rrn, appointment.getDoctorID()};
        auto itDoc = doctorIdIndex.begin();
        while (itDoc != doctorIdIndex.end() && itDoc->doctorId < appointment.getDoctorID()) ++itDoc;
        doctorIdIndex.insert(itDoc, newDocEntry);
        doctorIdLink[rrn] = -1;
    }
}

void IndexManager::deleteAppointment(const Appointment& appointment) {
    int rrn = getAppointmentRecNoByPk(appointment.getAppointmentID());
    if (rrn == -1) return;

    int pkIdx = binarySearchString(appointmentPkIndex, appointment.getAppointmentID());
    if (pkIdx != -1) appointmentPkIndex.erase(appointmentPkIndex.begin() + pkIdx);

    int docIdx = binarySearchString(doctorIdIndex, appointment.getDoctorID());
    if (docIdx != -1) {
        int head = doctorIdIndex[docIdx].headRRN;
        if (head == rrn) {
            doctorIdIndex[docIdx].headRRN = doctorIdLink[rrn];
        } else {
            int curr = head;
            while (curr != -1 && doctorIdLink[curr] != rrn) {
                curr = doctorIdLink[curr];
            }
            if (curr != -1) doctorIdLink[curr] = doctorIdLink[rrn];
        }
        if (doctorIdIndex[docIdx].headRRN == -1) {
            doctorIdIndex.erase(doctorIdIndex.begin() + docIdx);
        }
    }
}


void IndexManager::updateAppointment(const Appointment& oldApp, const Appointment& newApp, int rrn) {
    if (oldApp.getAppointmentID() != newApp.getAppointmentID()) {
        int pkIdx = binarySearchString(appointmentPkIndex, oldApp.getAppointmentID());
        if (pkIdx != -1) {
            appointmentPkIndex.erase(appointmentPkIndex.begin() + pkIdx);
        }

        AppointmentPkIndex newPkEntry {rrn, newApp.getAppointmentID()};
        auto it = appointmentPkIndex.begin();
        while (it != appointmentPkIndex.end() && it->appointmentId < newApp.getAppointmentID()) {
            ++it;
        }
        appointmentPkIndex.insert(it, newPkEntry);
    }

    if (oldApp.getDoctorID() != newApp.getDoctorID()) {
        int oldDocIdx = binarySearchString(doctorIdIndex, oldApp.getDoctorID());
        if (oldDocIdx != -1) {
            int head = doctorIdIndex[oldDocIdx].headRRN;
            if (head == rrn) {
                doctorIdIndex[oldDocIdx].headRRN = doctorIdLink[rrn];
            } else {
                int curr = head;
                while (curr != -1 && doctorIdLink[curr] != rrn) {
                    curr = doctorIdLink[curr];
                }
                if (curr != -1) {
                    doctorIdLink[curr] = doctorIdLink[rrn];
                }
            }
            if (doctorIdIndex[oldDocIdx].headRRN == -1) {
                doctorIdIndex.erase(doctorIdIndex.begin() + oldDocIdx);
            }
        }

        int newDocIdx = binarySearchString(doctorIdIndex, newApp.getDoctorID());
        if (newDocIdx != -1) {
            doctorIdLink[rrn] = doctorIdIndex[newDocIdx].headRRN;
            doctorIdIndex[newDocIdx].headRRN = rrn;
        } else {
            DoctorIdIndex newEntry {rrn, newApp.getDoctorID()};
            auto itDoc = doctorIdIndex.begin();
            while (itDoc != doctorIdIndex.end() && itDoc->doctorId < newApp.getDoctorID()) {
                ++itDoc;
            }
            doctorIdIndex.insert(itDoc, newEntry);
            doctorIdLink[rrn] = -1;
        }
    }
}

int IndexManager::getAppointmentRecNoByPk(const string& id) {
    int idx = binarySearchString(appointmentPkIndex, id);
    return (idx != -1) ? appointmentPkIndex[idx].rrn : -1;
}

vector<int> IndexManager::getAppointmentRecNoByDoctorId(const string& doctorId) {
    vector<int> results;
    int idx = binarySearchString(doctorIdIndex, doctorId);
    if (idx == -1) return results;

    int curr = doctorIdIndex[idx].headRRN;
    while (curr != -1) {
        results.push_back(curr);
        curr = doctorIdLink[curr];
    }
    return results;
}