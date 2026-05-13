#pragma once

#include <string_view>

namespace redclone::core
{
enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warning,
    Error
};

class Logger
{
  public:
    static void log(LogLevel level, std::string_view file, int line, std::string_view message);
};

#if defined(REDCLONE_DEBUG_LOGS) && REDCLONE_DEBUG_LOGS
#define REDCLONE_LOG_TRACE(message) ::redclone::core::Logger::log(::redclone::core::LogLevel::Trace, __FILE__, __LINE__, (message))
#define REDCLONE_LOG_DEBUG(message) ::redclone::core::Logger::log(::redclone::core::LogLevel::Debug, __FILE__, __LINE__, (message))
#else
#define REDCLONE_LOG_TRACE(message) ((void)0)
#define REDCLONE_LOG_DEBUG(message) ((void)0)
#endif

#define REDCLONE_LOG_INFO(message) ::redclone::core::Logger::log(::redclone::core::LogLevel::Info, __FILE__, __LINE__, (message))
#define REDCLONE_LOG_WARNING(message) ::redclone::core::Logger::log(::redclone::core::LogLevel::Warning, __FILE__, __LINE__, (message))
#define REDCLONE_LOG_ERROR(message) ::redclone::core::Logger::log(::redclone::core::LogLevel::Error, __FILE__, __LINE__, (message))
} // namespace redclone::core
