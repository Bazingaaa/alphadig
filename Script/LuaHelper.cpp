
#include "Script/LuaHelper.h"

#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "cocoa/CCString.h"
#include "cocoa/CCGeometry.h"


namespace AlphaDig
{

using namespace cocos2d;

void LuaHelper::s_executeFile( const char *pFileName )
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	std::string strDirPath( "scripts" );
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile((strDirPath + "/" + pFileName).c_str());
    if (pstrFileContent)
    {
    	pLuaEngine->executeString(pstrFileContent->getCString());
    }
#else
    std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath((strDirPath + "/" + pFileName).c_str());
    pLuaEngine->addSearchPath(path.substr(0, path.find_last_of("/") - dirPath.length()).c_str());
    pLuaEngine->executeScriptFile(path.c_str());
#endif
}

std::string LuaHelper::s_getStringVar( const char *pVarName )
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	lua_State *pLuaState = pLuaEngine->getLuaState();

	lua_getglobal( pLuaState, pVarName );

	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getStringVar, the var not exist" );

	std::string strRet = lua_tostring( pLuaState,  -1 );
	lua_pop( pLuaState, 1 );

	return strRet;
}

CCRect LuaHelper::s_getRectVar( const char *pVarName )
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	lua_State *pLuaState = pLuaEngine->getLuaState();

	lua_getglobal( pLuaState, pVarName );

	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getRectVar, the var not exist" );

	CCRect rect;
	// a rect in lua is a table with 4 number elements
	lua_pushnumber( pLuaState, 1 );
	lua_gettable( pLuaState, -2 );
	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getRectVar" );
	rect.origin.x = lua_tonumber( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	lua_pushnumber( pLuaState, 2 );
	lua_gettable( pLuaState, -2 );
	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getRectVar"  );
	rect.origin.y = lua_tonumber( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	lua_pushnumber( pLuaState, 3 );
	lua_gettable( pLuaState, -2 );
	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getRectVar" );
	rect.size.width = lua_tonumber( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	lua_pushnumber( pLuaState, 4 );
	lua_gettable( pLuaState, -2 );
	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getRectVar" );
	rect.size.height = lua_tonumber( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	lua_pop( pLuaState, 1 );

	return rect;

}

double LuaHelper::s_getNumberVar( const char *pVarName )
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	lua_State *pLuaState = pLuaEngine->getLuaState();

	lua_getglobal( pLuaState, pVarName );

	CCAssert( !lua_isnil( pLuaState, -1 ), "error in LuaHelper::s_getNumberVar, the var not exist" );

	double dRet = lua_tonumber( pLuaState,  -1 );
	lua_pop( pLuaState, 1 );

	return dRet;
}

bool LuaHelper::s_protectedCallFunc( int nParams, int nRets )
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	lua_State *pLuaState = pLuaEngine->getLuaState();

	if( lua_pcall( pLuaState, nParams, nRets, 0 ) != 0 )
	{
		const char *pError = lua_tostring( pLuaState, -1 );
		CCLog( pError );
		return false;
	}

	return true;

}

lua_State* LuaHelper::s_getLuaState()
{
	CCLuaEngine *pLuaEngine = static_cast< CCLuaEngine* > ( CCScriptEngineManager::sharedManager()->getScriptEngine() );
	lua_State *pLuaState = pLuaEngine->getLuaState();

	return pLuaState;

}

}

