#pragma once

#include <string>
#include <string_view>

struct lua_State;

namespace redclone::scripting
{
class ScriptEngine
{
public:
    ScriptEngine();
    ~ScriptEngine();

    ScriptEngine(const ScriptEngine&) = delete;
    ScriptEngine& operator=(const ScriptEngine&) = delete;

    ScriptEngine(ScriptEngine&& other) noexcept;
    ScriptEngine& operator=(ScriptEngine&& other) noexcept;

    bool runString(std::string_view script);
    [[nodiscard]] const std::string& getLastError() const;

private:
    lua_State* m_LuaState;
    std::string m_LastError;
};
}
