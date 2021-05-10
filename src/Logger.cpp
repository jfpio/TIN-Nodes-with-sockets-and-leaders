//
// Created by kamil on 10.05.2021.
//
#include "../include/Logger.h"
void Logger::setLogFile(const std::string& path)
{
    log_file_ = path;
    std::ofstream out;
    std::lock_guard guard(mutex_);
    out.open(log_file_, std::ofstream::out | std::ofstream::trunc);
    out.close();
}

void Logger::log(const std::string& msg) const
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream log_msg;
    log_msg << "[" << std::put_time(std::localtime(&now), "%T") << "]" << ": " << msg;

    std::ofstream out;
    std::lock_guard guard(mutex_);
    out.open(log_file_, std::ios_base::app);
    out << log_msg.rdbuf() << std::endl;
    out.close();
}

void Logger::log(std::stringstream& msg) const
{
    log(msg.str());
    msg.flush();
    msg.str(std::string(""));
}

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}