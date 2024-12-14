#include "logger.h"

void Logger::Init(const std::string& fileName) {
    if (logFile.is_open())
        logFile.close();

    // Get the path of the executable
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);

    // Get the directory of the executable
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();

    // Set the log file path relative to the executable directory
    std::filesystem::path logFilePath = exeDir / fileName;

    logFile.open(logFilePath, std::ios::app);

    if (!logFile)
        throw std::runtime_error("Failed to open the log file: " + logFilePath.string());

    std::cout << "Log file path: " << logFilePath.string() << std::endl;
}

void Logger::Log(const std::string& message, Type logType) {
    if (!logFile) {
        Init("log_backend.txt");
    }

    std::string timestamp = GetCurrentTimestamp();
    std::string logTypeStr = LogTypeToString(logType);
    std::string formattedMessage = "[" + timestamp + "] [" + logTypeStr + "]: " + message;

    logFile << formattedMessage << std::endl;
    std::cout << formattedMessage << std::endl;
}

std::string Logger::LogTypeToString(Type logType) {
    switch (logType) {
    case Type::Info:
        return "INFO";
    case Type::Warning:
        return "WARNING";
    case Type::Error:
        return "ERROR";
    case Type::Debug:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

std::string Logger::GetCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm localTime;

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%d %b. - %H:%M:%S");
    return oss.str();
}