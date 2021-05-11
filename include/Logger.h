/**
 * @file Logger.h
 * @author Kamil Jabłonowski
 * @brief contains definition of class Logger
 * @date 10-05-2021
 */

#ifndef TIN_LONG_LIVE_THE_KING_LOGGER_H
#define TIN_LONG_LIVE_THE_KING_LOGGER_H
#include <mutex>
#include <experimental/filesystem>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <chrono>

/// some strange errors when using std::experimental::filesystem::path, so temporarily using std::string as path_type
//using path_type = std::experimental::filesystem::path;

using path_type = std::string;
/**
 * @brief class Logger - singleton that prints log info to specified logging file. Implements simple monitor design pattern
 */
class Logger
{
    /// path to out file
    path_type log_file_;
    /// mutex used for synchronisation
    mutable std::mutex mutex_;
    Logger() = default;

public:
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief method sets logging file path
     * @param path
     */
    void setLogFile(const std::string& path);

    /**
     * @brief only way to get access to Logger object. usage: Logger::getInstance()
     * @return Logger& instance
     */
    static Logger& getInstance();

    /**
     * @brief methods used for printing log messages to file
     * @param msg
     */
    void log(const std::string& msg) const;
    void log(std::stringstream& msg) const;
};

#endif //TIN_LONG_LIVE_THE_KING_LOGGER_H
