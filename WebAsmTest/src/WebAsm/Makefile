# Compiler
EMCC = emcc

# Source files
SRC = main.cpp AniSrc/Converge/Render.cpp AniSrc/Shapes/Shapes.cpp AniSrc/SpaceS/SpaceS.cpp

# Output file
OUT = sdl_app.mjs

# Compiler flags
CFLAGS = -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -s MODULARIZE=1 -s EXPORT_ES6=1 -s EXPORT_NAME='SDLModule'

# Default target
all: $(OUT)

# Build target
$(OUT): $(SRC)
	$(EMCC) $(SRC) -o $(OUT) $(CFLAGS)

# Clean target
clean:
	rm -f $(OUT)

.PHONY: all clean