#pragma once
#include <functional>
#include "parser.hpp"
#include "../Repositories/DoctorRepository.h"
#include "../Repositories/AppointmentRepository.h"
#include "ast.hpp"
#include "../services/types.hpp"

using Pred = std::function<bool(const Record&)>;

Record doctorToRecord(const Doctor& d) {
  return {{"DoctorID", d.getDoctorID()}, {"DoctorName", d.getDoctorName()},
          {"DoctorSpecialty", d.getDoctorSpecialty()}, {"Address", d.getAddress()}};
}

Doctor doctorFromRecord(const Record& r) {
  return Doctor(std::get<std::string>(r.at("DoctorID")), std::get<std::string>(r.at("DoctorName")),
                std::get<std::string>(r.at("DoctorSpecialty")), std::get<std::string>(r.at("Address")));
}

Record appointmentToRecord(const Appointment& a) {
  return {{"AppointmentID", a.getAppointmentID()}, {"AppointmentDate", a.getAppointmentDate()},
          {"DoctorID", a.getDoctorID()}};
}

Appointment appointmentFromRecord(const Record& r) {
  return Appointment(std::get<std::string>(r.at("AppointmentID")),
                     std::get<std::string>(r.at("AppointmentDate")),
                     std::get<std::string>(r.at("DoctorID")));
}

class QueryExecutor {
  DoctorRepository& doctorRepo;
  AppointmentRepository& appointmentRepo;

  vector<Record> filterAll(vector<Record> all, const Pred& where, std::optional<size_t> limit) {
    vector<Record> result;
    for (auto& r : all) {
      if (where(r)) {
        result.push_back(std::move(r));
        if (limit && result.size() >= *limit) break;
      }
    }
    return result;
  }

  public:
  QueryExecutor(DoctorRepository& dr, AppointmentRepository& ar)
      : doctorRepo(dr), appointmentRepo(ar) {}

  vector<Record> select(const std::string& table, const Pred& where, std::optional<size_t> limit) {
    if (table == "Doctor") {
      vector<Record> all;
      for (const auto& d : doctorRepo.getAllDoctors()) all.push_back(doctorToRecord(d));
      return filterAll(std::move(all), where, limit);
    }
    if (table == "Appointment") {
      vector<Record> all;
      for (const auto& a : appointmentRepo.getAll()) all.push_back(appointmentToRecord(a));
      return filterAll(std::move(all), where, limit);
    }
    throw std::runtime_error("Unknown table: " + table);
  }

  size_t insert(const std::string& table, const Record& row) {
    if (table == "Doctor") {
      doctorRepo.addDoctor(doctorFromRecord(row));
      return 1;
    }
    if (table == "Appointment") {
      appointmentRepo.add(appointmentFromRecord(row));
      return 1;
    }
    throw std::runtime_error("Unknown table: " + table);
  }

  size_t update(const std::string& table, const Pred& where, const Record& changes) {
    if (table == "Doctor") {
      vector<Doctor> matched;
      for (const auto& d : doctorRepo.getAllDoctors())
        if (where(doctorToRecord(d))) matched.push_back(d);

      size_t count = 0;
      for (const auto& d : matched) {
        Doctor next = d;
        if (changes.count("DoctorName")) next.setDoctorName(std::get<std::string>(changes.at("DoctorName")));
        if (changes.count("DoctorSpecialty")) next.setDoctorSpecialty(std::get<std::string>(changes.at("DoctorSpecialty")));
        if (changes.count("Address")) next.setAddress(std::get<std::string>(changes.at("Address")));
        if (changes.count("DoctorID")) next.setDoctorID(std::get<std::string>(changes.at("DoctorID")));

        if (next.getAddress() != d.getAddress() || next.getDoctorSpecialty() != d.getDoctorSpecialty())
          doctorRepo.updateDoctorNonKey(d.getDoctorID(), next.getAddress(), next.getDoctorSpecialty());
        if (next.getDoctorName() != d.getDoctorName())
          doctorRepo.updateDoctorName(d.getDoctorID(), next.getDoctorName());
        if (next.getDoctorID() != d.getDoctorID())
          doctorRepo.updateDoctorId(d.getDoctorID(), next.getDoctorID());
        count++;
      }
      return count;
    }
    if (table == "Appointment") {
      vector<Appointment> matched;
      for (const auto& a : appointmentRepo.getAll())
        if (where(appointmentToRecord(a))) matched.push_back(a);

      size_t count = 0;
      for (const auto& a : matched) {
        if (changes.count("AppointmentDate"))
          appointmentRepo.updateDate(a.getAppointmentID(), std::get<std::string>(changes.at("AppointmentDate")));
        count++;
      }
      return count;
    }
    throw std::runtime_error("Unknown table: " + table);
  }

  size_t remove(const std::string& table, const Pred& where) {
    if (table == "Doctor") {
      vector<Doctor> matched;
      for (const auto& d : doctorRepo.getAllDoctors())
        if (where(doctorToRecord(d))) matched.push_back(d);

      size_t count = 0;
      for (const auto& d : matched) {
        doctorRepo.deleteDoctor(d.getDoctorID());
        count++;
      }
      return count;
    }
    if (table == "Appointment") {
      vector<Appointment> matched;
      for (const auto& a : appointmentRepo.getAll())
        if (where(appointmentToRecord(a))) matched.push_back(a);

      size_t count = 0;
      for (const auto& a : matched) {
        appointmentRepo.remove(a.getAppointmentID());
        count++;
      }
      return count;
    }
    throw std::runtime_error("Unknown table: " + table);
  }
};
