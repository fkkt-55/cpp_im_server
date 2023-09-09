//
// Created by Fkkt on 2023/9/8.
//

#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace fkkt {

class logger {
   public:
    enum log_level {
        D,  // Debug
        I,  // Info
        W,  // Warning
        E   // Error
    };

    /**
     * Construct logger
     *
     * @param log_file_name log file name
     * @param default_console_log_level log level, see log_level
     * @param mode mode, default is append
     */
    explicit logger(const std::string& log_file_name = "app",
                    log_level default_console_log_level = D,
                    const std::ios_base::openmode& mode = std::ios_base::app);

    ~logger();

    /**
     * Set level of console, which will be print
     *
     * @param level log level, see log_level
     */
    void set_console_log_level(log_level level);

    template <typename... Args>
    void d(Args... args) {
        std::ostringstream oss;
        append(oss, args...);
        write(D, oss.str());
    }

    template <typename... Args>
    void i(Args... args) {
        std::ostringstream oss;
        append(oss, args...);
        write(I, oss.str());
    }

    template <typename... Args>
    void w(Args... args) {
        std::ostringstream oss;
        append(oss, args...);
        write(W, oss.str());
    }

    template <typename... Args>
    void e(Args... args) {
        std::ostringstream oss;
        append(oss, args...);
        write(E, oss.str());
    }

   private:
    std::ofstream log_file;
    log_level console_log_level;

    static std::string get_log_level(log_level level);

    static std::string get_current_time_stamp();

    void write(log_level level, const std::string& message);

    void append(std::ostringstream& oss) {}

    template <typename T, typename... Args>
    void append(std::ostringstream& oss, const T& first, Args... args) {
        oss << first;
        append(oss, args...);
    }
};
}  // namespace fkkt