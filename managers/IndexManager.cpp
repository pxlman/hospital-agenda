#include "IndexManager.h"
#include <algorithm>

using namespace std;
using namespace IndexEntry;

IndexManager::IndexManager() {}

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

// Binary Searches
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

    // --- SAVE LINK NODES ---
    ofstream nameLinkOut("../data/Indexes/DoctorNameLinks.bin", ios::binary | ios::trunc);
    for (const auto& node : doctorNameLink) {
        writeFixedString(nameLinkOut, node.primaryKey, 15);
        nameLinkOut.write(reinterpret_cast<const char*>(&node.nextNodeIndex), sizeof(int));
    }
    nameLinkOut.close();

    ofstream specLinkOut("../data/Indexes/DoctorSpecialtyLinks.bin", ios::binary | ios::trunc);
    for (const auto& node : doctorSpecialtyLink) {
        writeFixedString(specLinkOut, node.primaryKey, 15);
        specLinkOut.write(reinterpret_cast<const char*>(&node.nextNodeIndex), sizeof(int));
    }
    specLinkOut.close();

    ofstream docIdLinkOut("../data/Indexes/DoctorIDLinks.bin", ios::binary | ios::trunc);
    for (const auto& node : doctorIdLink) {
        writeFixedString(docIdLinkOut, node.primaryKey, 15);
        docIdLinkOut.write(reinterpret_cast<const char*>(&node.nextNodeIndex), sizeof(int));
    }
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

    // --- LOAD LINK NODES ---
    ifstream nameLinkIn("../data/Indexes/DoctorNameLinks.bin", ios::binary);
    if (nameLinkIn.is_open()) {
        while (nameLinkIn.peek() != EOF) {
            LinkNode node;
            node.primaryKey = readFixedString(nameLinkIn, 15);
            nameLinkIn.read(reinterpret_cast<char*>(&node.nextNodeIndex), sizeof(int));
            doctorNameLink.push_back(node);
        }
        nameLinkIn.close();
    }

    ifstream specLinkIn("../data/Indexes/DoctorSpecialtyLinks.bin", ios::binary);
    if (specLinkIn.is_open()) {
        while (specLinkIn.peek() != EOF) {
            LinkNode node;
            node.primaryKey = readFixedString(specLinkIn, 15);
            specLinkIn.read(reinterpret_cast<char*>(&node.nextNodeIndex), sizeof(int));
            doctorSpecialtyLink.push_back(node);
        }
        specLinkIn.close();
    }

    ifstream docIdLinkIn("../data/Indexes/DoctorIDLinks.bin", ios::binary);
    if (docIdLinkIn.is_open()) {
        while (docIdLinkIn.peek() != EOF) {
            LinkNode node;
            node.primaryKey = readFixedString(docIdLinkIn, 15);
            docIdLinkIn.read(reinterpret_cast<char*>(&node.nextNodeIndex), sizeof(int));
            doctorIdLink.push_back(node);
        }
        docIdLinkIn.close();
    }
}


// DOCTORS
void IndexManager::addDoctor(const Doctor& doctor, int rrn) {
    if (getDoctorRecNoByPk(doctor.getDoctorID()) != -1) return;

    // 1. PK Insert (Binary Search)
    DoctorPkIndex pkEntry {rrn, doctor.getDoctorID()};
    auto it = lower_bound(doctorPkIndex.begin(), doctorPkIndex.end(), doctor.getDoctorID(), 
        [](const DoctorPkIndex& entry, const string& id) { return entry.doctorId < id; });
    doctorPkIndex.insert(it, pkEntry);

    // 2. Name SK Insert
    int nameIdx = binarySearchString(doctorNameIndex, doctor.getDoctorName());
    if (nameIdx != -1) {
        int oldHeadNode = doctorNameIndex[nameIdx].headRRN; 
        doctorNameLink.push_back({doctor.getDoctorID(), oldHeadNode});
        doctorNameIndex[nameIdx].headRRN = doctorNameLink.size() - 1; 
    } else {
        doctorNameLink.push_back({doctor.getDoctorID(), -1});
        DoctorNameIndex newNameEntry {(int)doctorNameLink.size() - 1, doctor.getDoctorName()};
        auto itName = lower_bound(doctorNameIndex.begin(), doctorNameIndex.end(), doctor.getDoctorName(),
            [](const DoctorNameIndex& entry, const string& val) { return entry.name < val; });
        doctorNameIndex.insert(itName, newNameEntry);
    }

    // 3. Specialty SK Insert
    int specIdx = binarySearchString(doctorSpecialtyIndex, doctor.getDoctorSpecialty());
    if (specIdx != -1) {
        int oldHeadNode = doctorSpecialtyIndex[specIdx].headRRN;
        doctorSpecialtyLink.push_back({doctor.getDoctorID(), oldHeadNode});
        doctorSpecialtyIndex[specIdx].headRRN = doctorSpecialtyLink.size() - 1;
    } else {
        doctorSpecialtyLink.push_back({doctor.getDoctorID(), -1});
        DoctorSpecialtyIndex newSpecEntry {(int)doctorSpecialtyLink.size() - 1, doctor.getDoctorSpecialty()};
        auto itSpec = lower_bound(doctorSpecialtyIndex.begin(), doctorSpecialtyIndex.end(), doctor.getDoctorSpecialty(),
            [](const DoctorSpecialtyIndex& entry, const string& val) { return entry.specialty < val; });
        doctorSpecialtyIndex.insert(itSpec, newSpecEntry);
    }
}

void IndexManager::deleteDoctor(const Doctor& doctor) {
    int rrn = getDoctorRecNoByPk(doctor.getDoctorID());
    if (rrn == -1) return;

    int pkIdx = binarySearchString(doctorPkIndex, doctor.getDoctorID());
    if (pkIdx != -1) doctorPkIndex.erase(doctorPkIndex.begin() + pkIdx);

    // Helper to unlink by Primary Key
    auto unlinkFromList = [&](auto& indexVec, auto& linkVec, const string& key) {
        int idx = binarySearchString(indexVec, key);
        if (idx == -1) return;
        
        int currNode = indexVec[idx].headRRN;
        int prevNode = -1;
        
        while (currNode != -1) {
            if (linkVec[currNode].primaryKey == doctor.getDoctorID()) {
                if (prevNode == -1) {
                    indexVec[idx].headRRN = linkVec[currNode].nextNodeIndex; 
                } else {
                    linkVec[prevNode].nextNodeIndex = linkVec[currNode].nextNodeIndex; 
                }
                break;
            }
            prevNode = currNode;
            currNode = linkVec[currNode].nextNodeIndex;
        }
        
        if (indexVec[idx].headRRN == -1) {
            indexVec.erase(indexVec.begin() + idx); 
        }
    };

    unlinkFromList(doctorNameIndex, doctorNameLink, doctor.getDoctorName());
    unlinkFromList(doctorSpecialtyIndex, doctorSpecialtyLink, doctor.getDoctorSpecialty());
}

void IndexManager::updateDoctor(const Doctor& oldDoctor, const Doctor& newDoctor, int rrn) {
    // 1. Update PK
    if (oldDoctor.getDoctorID() != newDoctor.getDoctorID()) {
        int pkIdx = binarySearchString(doctorPkIndex, oldDoctor.getDoctorID());
        if (pkIdx != -1) doctorPkIndex.erase(doctorPkIndex.begin() + pkIdx);

        DoctorPkIndex newPkEntry {rrn, newDoctor.getDoctorID()};
        auto it = lower_bound(doctorPkIndex.begin(), doctorPkIndex.end(), newDoctor.getDoctorID(),
            [](const DoctorPkIndex& entry, const string& id) { return entry.doctorId < id; });
        doctorPkIndex.insert(it, newPkEntry);
    }

    // 2. Update Name
    if (oldDoctor.getDoctorName() != newDoctor.getDoctorName()) {
        // Unlink Old
        int oldNameIdx = binarySearchString(doctorNameIndex, oldDoctor.getDoctorName());
        if (oldNameIdx != -1) {
            int currNode = doctorNameIndex[oldNameIdx].headRRN;
            int prevNode = -1;
            while (currNode != -1) {
                if (doctorNameLink[currNode].primaryKey == oldDoctor.getDoctorID()) {
                    if (prevNode == -1) doctorNameIndex[oldNameIdx].headRRN = doctorNameLink[currNode].nextNodeIndex;
                    else doctorNameLink[prevNode].nextNodeIndex = doctorNameLink[currNode].nextNodeIndex;
                    break;
                }
                prevNode = currNode;
                currNode = doctorNameLink[currNode].nextNodeIndex;
            }
            if (doctorNameIndex[oldNameIdx].headRRN == -1) doctorNameIndex.erase(doctorNameIndex.begin() + oldNameIdx);
        }

        // Link New
        int newNameIdx = binarySearchString(doctorNameIndex, newDoctor.getDoctorName());
        if (newNameIdx != -1) {
            int oldHeadNode = doctorNameIndex[newNameIdx].headRRN;
            doctorNameLink.push_back({newDoctor.getDoctorID(), oldHeadNode});
            doctorNameIndex[newNameIdx].headRRN = doctorNameLink.size() - 1;
        } else {
            doctorNameLink.push_back({newDoctor.getDoctorID(), -1});
            DoctorNameIndex newEntry {(int)doctorNameLink.size() - 1, newDoctor.getDoctorName()};
            auto itName = lower_bound(doctorNameIndex.begin(), doctorNameIndex.end(), newDoctor.getDoctorName(),
                [](const DoctorNameIndex& entry, const string& val) { return entry.name < val; });
            doctorNameIndex.insert(itName, newEntry);
        }
    }

    // 3. Update Specialty
    if (oldDoctor.getDoctorSpecialty() != newDoctor.getDoctorSpecialty()) {
        // Unlink Old
        int oldSpecIdx = binarySearchString(doctorSpecialtyIndex, oldDoctor.getDoctorSpecialty());
        if (oldSpecIdx != -1) {
            int currNode = doctorSpecialtyIndex[oldSpecIdx].headRRN;
            int prevNode = -1;
            while (currNode != -1) {
                if (doctorSpecialtyLink[currNode].primaryKey == oldDoctor.getDoctorID()) {
                    if (prevNode == -1) doctorSpecialtyIndex[oldSpecIdx].headRRN = doctorSpecialtyLink[currNode].nextNodeIndex;
                    else doctorSpecialtyLink[prevNode].nextNodeIndex = doctorSpecialtyLink[currNode].nextNodeIndex;
                    break;
                }
                prevNode = currNode;
                currNode = doctorSpecialtyLink[currNode].nextNodeIndex;
            }
            if (doctorSpecialtyIndex[oldSpecIdx].headRRN == -1) doctorSpecialtyIndex.erase(doctorSpecialtyIndex.begin() + oldSpecIdx);
        }

        // Link New
        int newSpecIdx = binarySearchString(doctorSpecialtyIndex, newDoctor.getDoctorSpecialty());
        if (newSpecIdx != -1) {
            int oldHeadNode = doctorSpecialtyIndex[newSpecIdx].headRRN;
            doctorSpecialtyLink.push_back({newDoctor.getDoctorID(), oldHeadNode});
            doctorSpecialtyIndex[newSpecIdx].headRRN = doctorSpecialtyLink.size() - 1;
        } else {
            doctorSpecialtyLink.push_back({newDoctor.getDoctorID(), -1});
            DoctorSpecialtyIndex newEntry {(int)doctorSpecialtyLink.size() - 1, newDoctor.getDoctorSpecialty()};
            auto itSpec = lower_bound(doctorSpecialtyIndex.begin(), doctorSpecialtyIndex.end(), newDoctor.getDoctorSpecialty(),
                [](const DoctorSpecialtyIndex& entry, const string& val) { return entry.specialty < val; });
            doctorSpecialtyIndex.insert(itSpec, newEntry);
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

    int currNode = doctorNameIndex[idx].headRRN;
    while (currNode != -1) {
        string pk = doctorNameLink[currNode].primaryKey;
        int rrn = getDoctorRecNoByPk(pk); // Bound to PK!
        if (rrn != -1) results.push_back(rrn);
        currNode = doctorNameLink[currNode].nextNodeIndex; 
    }
    return results;
}

vector<int> IndexManager::getDoctorRecNoBySpecialty(const string& specialty) {
    vector<int> results;
    int idx = binarySearchString(doctorSpecialtyIndex, specialty);
    if (idx == -1) return results;

    int currNode = doctorSpecialtyIndex[idx].headRRN;
    while (currNode != -1) {
        string pk = doctorSpecialtyLink[currNode].primaryKey;
        int rrn = getDoctorRecNoByPk(pk);
        if (rrn != -1) results.push_back(rrn);
        currNode = doctorSpecialtyLink[currNode].nextNodeIndex;
    }
    return results;
}


// APPOINTMENTS
void IndexManager::addAppointment(const Appointment& appointment, int rrn) {
    if (getAppointmentRecNoByPk(appointment.getAppointmentID()) != -1) return;

    // 1. PK Insert
    AppointmentPkIndex pkEntry {rrn, appointment.getAppointmentID()};
    auto it = lower_bound(appointmentPkIndex.begin(), appointmentPkIndex.end(), appointment.getAppointmentID(), 
        [](const AppointmentPkIndex& entry, const string& id) { return entry.appointmentId < id; });
    appointmentPkIndex.insert(it, pkEntry);

    // 2. Doctor ID SK Insert
    int docIdx = binarySearchString(doctorIdIndex, appointment.getDoctorID());
    if (docIdx != -1) {
        int oldHeadNode = doctorIdIndex[docIdx].headRRN;
        doctorIdLink.push_back({appointment.getAppointmentID(), oldHeadNode});
        doctorIdIndex[docIdx].headRRN = doctorIdLink.size() - 1;
    } else {
        doctorIdLink.push_back({appointment.getAppointmentID(), -1});
        DoctorIdIndex newDocEntry {(int)doctorIdLink.size() - 1, appointment.getDoctorID()};
        auto itDoc = lower_bound(doctorIdIndex.begin(), doctorIdIndex.end(), appointment.getDoctorID(),
            [](const DoctorIdIndex& entry, const string& val) { return entry.doctorId < val; });
        doctorIdIndex.insert(itDoc, newDocEntry);
    }
}

void IndexManager::deleteAppointment(const Appointment& appointment) {
    int rrn = getAppointmentRecNoByPk(appointment.getAppointmentID());
    if (rrn == -1) return;

    int pkIdx = binarySearchString(appointmentPkIndex, appointment.getAppointmentID());
    if (pkIdx != -1) appointmentPkIndex.erase(appointmentPkIndex.begin() + pkIdx);

    int docIdx = binarySearchString(doctorIdIndex, appointment.getDoctorID());
    if (docIdx != -1) {
        int currNode = doctorIdIndex[docIdx].headRRN;
        int prevNode = -1;
        
        while (currNode != -1) {
            if (doctorIdLink[currNode].primaryKey == appointment.getAppointmentID()) {
                if (prevNode == -1) doctorIdIndex[docIdx].headRRN = doctorIdLink[currNode].nextNodeIndex;
                else doctorIdLink[prevNode].nextNodeIndex = doctorIdLink[currNode].nextNodeIndex;
                break;
            }
            prevNode = currNode;
            currNode = doctorIdLink[currNode].nextNodeIndex;
        }
        if (doctorIdIndex[docIdx].headRRN == -1) {
            doctorIdIndex.erase(doctorIdIndex.begin() + docIdx);
        }
    }
}

void IndexManager::updateAppointment(const Appointment& oldApp, const Appointment& newApp, int rrn) {
    // 1. Update PK
    if (oldApp.getAppointmentID() != newApp.getAppointmentID()) {
        int pkIdx = binarySearchString(appointmentPkIndex, oldApp.getAppointmentID());
        if (pkIdx != -1) appointmentPkIndex.erase(appointmentPkIndex.begin() + pkIdx);

        AppointmentPkIndex newPkEntry {rrn, newApp.getAppointmentID()};
        auto it = lower_bound(appointmentPkIndex.begin(), appointmentPkIndex.end(), newApp.getAppointmentID(),
            [](const AppointmentPkIndex& entry, const string& id) { return entry.appointmentId < id; });
        appointmentPkIndex.insert(it, newPkEntry);
    }

    // 2. Update Doctor ID (Foreign Key)
    if (oldApp.getDoctorID() != newApp.getDoctorID()) {
        // Unlink old
        int oldDocIdx = binarySearchString(doctorIdIndex, oldApp.getDoctorID());
        if (oldDocIdx != -1) {
            int currNode = doctorIdIndex[oldDocIdx].headRRN;
            int prevNode = -1;
            while (currNode != -1) {
                if (doctorIdLink[currNode].primaryKey == oldApp.getAppointmentID()) {
                    if (prevNode == -1) doctorIdIndex[oldDocIdx].headRRN = doctorIdLink[currNode].nextNodeIndex;
                    else doctorIdLink[prevNode].nextNodeIndex = doctorIdLink[currNode].nextNodeIndex;
                    break;
                }
                prevNode = currNode;
                currNode = doctorIdLink[currNode].nextNodeIndex;
            }
            if (doctorIdIndex[oldDocIdx].headRRN == -1) doctorIdIndex.erase(doctorIdIndex.begin() + oldDocIdx);
        }
        
        // Link new
        int newDocIdx = binarySearchString(doctorIdIndex, newApp.getDoctorID());
        if (newDocIdx != -1) {
            int oldHeadNode = doctorIdIndex[newDocIdx].headRRN;
            doctorIdLink.push_back({newApp.getAppointmentID(), oldHeadNode});
            doctorIdIndex[newDocIdx].headRRN = doctorIdLink.size() - 1;
        } else {
            doctorIdLink.push_back({newApp.getAppointmentID(), -1});
            DoctorIdIndex newEntry {(int)doctorIdLink.size() - 1, newApp.getDoctorID()};
            auto itDoc = lower_bound(doctorIdIndex.begin(), doctorIdIndex.end(), newApp.getDoctorID(),
                [](const DoctorIdIndex& entry, const string& val) { return entry.doctorId < val; });
            doctorIdIndex.insert(itDoc, newEntry);
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

    int currNode = doctorIdIndex[idx].headRRN;
    while (currNode != -1) {
        string pk = doctorIdLink[currNode].primaryKey;
        int rrn = getAppointmentRecNoByPk(pk); // Bound to PK!
        if (rrn != -1) results.push_back(rrn);
        currNode = doctorIdLink[currNode].nextNodeIndex;
    }
    return results;
}