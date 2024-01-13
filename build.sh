CFLAGS="-Wall -Wextra -Wno-unused-parameter -mwindows"
CLIBS="-lraylib -lopengl32 -lgdi32 -lwinmm"
CSRCS="main.c resultlist.c app.c"

gcc $CFLAGS -o mf $CSRCS $CLIBS
