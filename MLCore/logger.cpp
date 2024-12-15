#include "logger.h"

void Logger::Init(const std::string& fileName) {
    if (logFile.is_open())
        logFile.close();

    std::filesystem::path logFilePath;

#ifdef _WIN32
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    logFilePath = exeDir / fileName;
#else
    // Cross-platform alternative to get the current working directory
    char exePath[1024];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len == -1) 
        THROW_RUNTIME_ERROR("Failed to get executable path");

    exePath[len] = '\0';
    std::filesystem::path exeDir = std::filesystem::path(exePath).parent_path();
    logFilePath = exeDir / fileName;
#endif

    logFile.open(logFilePath, std::ios::app);

    if (!logFile)
        THROW_RUNTIME_ERROR("Failed to open the log file: " + logFilePath.string());


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