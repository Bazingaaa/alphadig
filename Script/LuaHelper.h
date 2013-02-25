
#ifndef SCRIPT_MANAGER_H__
#define SCRIPT_MANAGER_H__

#include <string>

#include "cocoa/CCGeometry.h"

struct lua_State;

namespace AlphaDig
{

using namespace cocos2d;

class LuaHelper
{
public:

	static void s_executeFile( const char *pFileName );

	static std::string s_getStringVar( const char *pVarName );
	static CCRect s_getRectVar( const char *pVarName );
	static double s_getNumberVar( const char *pVarName );

	static bool s_protectedCallFunc( int nNumParams, int nNumRets );

	static lua_State* s_getLuaState();
};

}

#endif

