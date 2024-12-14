#pragma once

#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <filesystem>
#include <windows.h>

/**
 * @class Logger
 * @brief A simple logging utility for writing log messages to a file with support for log types and timestamps.
 *
 * This class provides functionality to initialize a log file, log messages with different severity levels,
 * and manage the format of the log output (e.g., timestamped messages).
 *
 * Usage:
 * - Call `Logger::Init` to initialize the log file before logging messages.
 * - Use `Logger::Log` or the global `Log` function to write log messages to the file.
 */
class Logger {
public:
    enum class Type {
        Info,    ///< Informational messages (default log type).
        Warning, ///< Warning messages indicating potential issues.
        Error,   ///< Error messages indicating serious problems.
        Debug    ///< Debug messages for development purposes.
    };

    /**
     * @brief Logs a message to the file with the specified log type.
     *
     * ---
     *
     * If the logger has not been initialized, it will automatically initialize
     * with the default file name `log_backend.txt`. The log message is written
     * both to the log file and to the terminal.
     *
     * Example log format:
     *
     * [12 Oct. - 14:32:01] [INFO]: This is a log message.
     *
     * @param message: The message to log.
     * @param logType: The log type (e.g., `Type::Info`). Defaults to `Type::Info`.
     */
    static void Init(const std::string& filePath = "log_backend.txt");

    /**
     * @brief Logs a message to the file with the specified log type.
     *
     * ---
     * 
     * Example of a log format:
     * [12 Oct. - 14:32:01] [INFO]: This is a log message.
     *
     * @param message: The message to log.
     * @param logType: The log type (e.g., `Type::Info`). Defaults to `Type::Info`.
     * @throws std::runtime_error: If the logger has not been initialized.
     */
    static void Log(const std::string& message, Type logType = Type::Info);

private:
    /**
     * @brief Static file stream for writing log messages.
     *
     * ---
     * 
     * This file stream is initialized in the `Init` method and used by all logging methods
     * to write messages to the log file.
     */
    static inline std::ofstream logFile;

    /**
     * @brief Converts a `Logger::Type` enum value to its string representation.
     *
     * @param logType: The log type to convert.
     * @return A string representation of the log type.
     */
    static std::string LogTypeToString(Type logType);

    /**
     * @brief Gets the current timestamp in the format `[<day> <month_abbr>. - <hour>:<minute>:<second>]`.
     *
     * ---
     * 
     * Example: "12 Oct. - 14:32:01".
     *
     * @return The formatted timestamp as a string.
     */
    static std::string GetCurrentTimestamp();
};

/**
 * @brief Logs a message to the file with the specified log type. This function defaults to the `Type::Info` log type.
 *
 * ---
 * 
 * Example:
 * 
 * - Log("This is an informational message.");
 * 
 * - Log("This is an error message.", Logger::Type::Error);
 *
 * @param message: The message to log.
 * @param logType: The log type (e.g., `Logger::Type::Info`). Defaults to `Logger::Type::Info`.
 */
inline void Log(const std::string& message, Logger::Type logType = Logger::Type::Info) {
    Logger::Log(message, logType);
}
