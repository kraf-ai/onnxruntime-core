#include "logger.h"

void Logger::Init(const std::string& fileName) {
    if (logFile.is_open())
        logFile.close();

#ifdef X64
    std::filesystem::path logFilePath;

    std::filesystem::path parentPath = std::filesystem::current_path().parent_path();

    if (!std::filesystem::exists(parentPath)) {
        THROW_RUNTIME_ERROR("Parent path does not exist: " + parentPath.string());
    }
    if (!std::filesystem::is_directory(parentPath)) {
        THROW_RUNTIME_ERROR("Parent path is not a directory: " + parentPath.string());
    }

    logFilePath = parentPath / fileName;

    logFile.open(logFilePath, std::ios::app);

    if (!logFile)
        THROW_RUNTIME_ERROR("Failed to open the log file: " + logFilePath.string());

    std::cout << "Log file path: " << logFilePath.string() << std::endl;
#endif
}


void Logger::Log(const std::string& message, Type logType) {
    if (!logFile) {
        Init("log_mlcore.txt");
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
    oss << std::put_time(&localTime, "%d %b. %H:%M:%S");
    return oss.str();
}