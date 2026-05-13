#include "core/Logger.h"

int runLoggerTests()
{
    REDCLONE_LOG_TRACE("logger trace test");
    REDCLONE_LOG_DEBUG("logger debug test");
    REDCLONE_LOG_INFO("logger info test");
    REDCLONE_LOG_WARNING("logger warning test");
    REDCLONE_LOG_ERROR("logger error test");
    return 0;
}
