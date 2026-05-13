#include "scripting/ScriptEngine.h"

#include <iostream>

int runScriptEngineTests()
{
    redclone::scripting::ScriptEngine scriptEngine;

    if (!scriptEngine.runString("return 1 + 1"))
    {
        std::cerr << "Expected valid Lua script to execute successfully: " << scriptEngine.getLastError() << "\n";
        return 1;
    }

    if (!scriptEngine.getLastError().empty())
    {
        std::cerr << "Expected successful run to clear last error\n";
        return 1;
    }

    if (scriptEngine.runString("this is not valid lua"))
    {
        std::cerr << "Expected invalid Lua script to fail\n";
        return 1;
    }

    if (scriptEngine.getLastError().empty())
    {
        std::cerr << "Expected invalid Lua script to provide an error message\n";
        return 1;
    }

    return 0;
}
