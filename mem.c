#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int init_mem() 
{
	lua_State *L = luaL_newstate(); //open lua
	luaL_openlibs(L);		//open the standard libs
	
	char buff[256];
	int error;

	while (fgets(buff, sizeof(buff), stdin) != NULL) {
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
			lua_pcall(L, 0, 0, 0);
		if (error) {
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);  /* pop error message from the stack */
		}
	}
	lua_close(L);
	return 0;
}

int main(int argc, char * argv[])
{
	init_mem();
	return 0;
}
