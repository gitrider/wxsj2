/*
** $Id: lzio.h,v 1.1 2006/03/17 08:47:36 chaojin Exp $
** Buffered streams
** See Copyright Notice in lua.h
*/


#ifndef lzio_h
#define lzio_h

#include "lua.h"


#define EOZ	(-1)			/* end of stream */

typedef struct Zio ZIO;


#define char2int(c)	cast(int, cast(lua_WChar, (c)))

#define zgetc(z)  (((z)->n--)>0 ?  char2int(*(z)->p++) : luaZ_fill(z))

#define zname(z)	((z)->name)

void luaZ_init (ZIO *z, lua_Chunkreader reader, void *data, const char *name);
size_t luaZ_read (ZIO* z, void* b, size_t n);	/* read next n bytes */
int luaZ_lookahead (ZIO *z);



typedef struct Mbuffer {
  char *buffer;
  size_t buffsize;
} Mbuffer;


char *luaZ_openspace (lua_State *L, Mbuffer *buff, size_t n);

#define luaZ_initbuffer(L, buff) ((buff)->buffer = NULL, (buff)->buffsize = 0)

#define luaZ_sizebuffer(buff)	((buff)->buffsize)
#define luaZ_buffer(buff)	((buff)->buffer)

#define luaZ_resizebuffer(L, buff, size) \
	(luaM_reallocvector(L, (buff)->buffer, (buff)->buffsize, size, char), \
	(buff)->buffsize = size)

#define luaZ_freebuffer(L, buff)	luaZ_resizebuffer(L, buff, 0)


/* --------- Private Part ------------------ */

struct Zio {
  size_t n;			/* bytes still unread */
  const lua_WChar *p;		/* current position in buffer */
  lua_Chunkreader reader;
  void* data;			/* additional data */
  const char *name;
};


int luaZ_fill (ZIO *z);

#endif
