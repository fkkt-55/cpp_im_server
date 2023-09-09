//
// Created by Fkkt on 2023/9/8.
//
#include "logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <mutex>

namespace fkkt {
    logger::logger(const std::string &log_file_name,
                   log_level default_console_log_level,
                   const std::ios_base::openmode &mode) : console_log_level(default_console_log_level) {
        if (log_file_name.rfind(".log") != log_file_name.length() - 4) {
            log_file.open(log_file_name + ".log", mode);
        } else {
            log_file.open(log_file_name, mode);
        }
        if (!log_file.is_open()) {
            throw std::runtime_error("Failed to open log file: " + log_file_name);
        }
        std::ios::sync_with_stdio(false);
        set_console_log_level(W);
    }

    logger::~logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
        std::ios::sync_with_stdio(true);
    }

    void logger::set_console_log_level(log_level level) {
        console_log_level = level;
    }

    std::string logger::get_log_level(log_level level) {
        switch (level) {
            case D:
                return "DEBUG";
            case I:
                return "INFO";
            case W:
                return "WARNING";
            case E:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    std::string logger::get_current_time_stamp() {
        auto currentTime = std::chrono::system_clock::now();
        auto currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
        auto timeInfo = *std::localtime(&currentTimeT);
        std::stringstream ss;
        ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    void logger::write(log_level level, const std::string &message) {
        std::lock_guard<std::mutex> lock(std::mutex);
        std::string logMessage = get_current_time_stamp() + " [" + get_log_level(level) + "] " + message + "\n";
        if (level >= console_log_level) {
            std::cout << logMessage;
        }
        if (log_file.is_open()) {
            log_file << logMessage;
        }
    }

}
