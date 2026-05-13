#include "scripting/ScriptEngine.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#ifdef __cplusplus
}
#endif

#include <utility>

namespace redclone::scripting
{
ScriptEngine::ScriptEngine()
    : m_LuaState(luaL_newstate())
{
    if (m_LuaState != nullptr)
    {
        luaL_openlibs(m_LuaState);
    }
}

ScriptEngine::~ScriptEngine()
{
    if (m_LuaState != nullptr)
    {
        lua_close(m_LuaState);
    }
}

ScriptEngine::ScriptEngine(ScriptEngine&& other) noexcept
    : m_LuaState(other.m_LuaState)
    , m_LastError(std::move(other.m_LastError))
{
    other.m_LuaState = nullptr;
}

ScriptEngine& ScriptEngine::operator=(ScriptEngine&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    if (m_LuaState != nullptr)
    {
        lua_close(m_LuaState);
    }

    m_LuaState = other.m_LuaState;
    m_LastError = std::move(other.m_LastError);
    other.m_LuaState = nullptr;
    return *this;
}

bool ScriptEngine::runString(const std::string_view script)
{
    m_LastError.clear();
    if (m_LuaState == nullptr)
    {
        m_LastError = "Lua state is not initialized";
        return false;
    }

    const int loadResult = luaL_loadbuffer(m_LuaState, script.data(), script.size(), "RedCloneScript");
    if (loadResult != LUA_OK)
    {
        const char* error = lua_tostring(m_LuaState, -1);
        m_LastError = (error != nullptr) ? error : "Unknown Lua load error";
        lua_pop(m_LuaState, 1);
        return false;
    }

    const int callResult = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
    if (callResult != LUA_OK)
    {
        const char* error = lua_tostring(m_LuaState, -1);
        m_LastError = (error != nullptr) ? error : "Unknown Lua runtime error";
        lua_pop(m_LuaState, 1);
        return false;
    }

    lua_settop(m_LuaState, 0);
    return true;
}

const std::string& ScriptEngine::getLastError() const
{
    return m_LastError;
}
}
