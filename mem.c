#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "logging.h"

static lua_State *L = NULL;

// to create a global table that emulate the memory
int mem_init() 
{
	L = luaL_newstate();	//open lua
	luaL_openlibs(L);	//open the standard libs
	
	char buff[128];
	int error;
	strcpy(buff, "mem={}\n");

	error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
		lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "%s", lua_tostring(L, -1));
		lua_pop(L, 1);  /* pop error message from the stack */
	}

	// push the table mem to the lua virtual stack
	lua_getglobal(L, "mem");
	if (!lua_istable(L, -1))
		luaL_error(L, "’mem’ is not a table");

	return 0;
}


int mem_fini()
{
	lua_close(L);
}

 /* assume that table is on the stack top */
static uint32_t getfield (lua_State *L, uint32_t key) {
	uint32_t val;
	lua_pushinteger(L, key);
	lua_gettable(L, -2);	/* get mem[key] */
	if (!lua_isnumber(L, -1))
		luaL_error(L, "invalid val");
	val = (uint32_t)lua_tonumber(L, -1);
	lua_pop(L, 1);		/* remove the val */
	return val;
}

 /* assume that table is on the stack top */
static void setfield (lua_State *L, uint32_t key, uint32_t val) {
	LOG_D("key=0x%x, val=0x%x", key, val);

	lua_pushinteger(L, key);
	lua_pushinteger(L, val);
	
	lua_settable(L, -3);	/* get mem[key] */
}

uint32_t  mem_read(uint32_t addr)
{
	uint32_t val = getfield(L, addr);
	LOG_D("addr=0x%d, val=0x%x", addr, val);
	
	return val;
}

int mem_write(uint32_t addr, uint32_t val)
{
	LOG_D("addr=0x%x, val=0x%x", addr, val);

	setfield(L, addr, val);
	return 0;
}


// int main(int argc, char * argv[])
// {
// 	mem_init();
// 	return 0;
// }
