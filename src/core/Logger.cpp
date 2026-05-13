#include "core/Logger.h"

#include <iostream>

namespace redclone::core
{
namespace
{
std::string_view toString(const LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
        return "trace";
    case LogLevel::Debug:
        return "debug";
    case LogLevel::Info:
        return "info";
    case LogLevel::Warning:
        return "warning";
    case LogLevel::Error:
        return "error";
    }
    return "unknown";
}
} // namespace

void Logger::log(const LogLevel level, const std::string_view file, const int line, const std::string_view message)
{
    std::ostream& out = (level == LogLevel::Warning || level == LogLevel::Error) ? std::cerr : std::cout;
    out << "[" << toString(level) << "] " << file << ":" << line << " - " << message << "\n";
    out.flush();
}
} // namespace redclone::core
