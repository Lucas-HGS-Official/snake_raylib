TARGET = bin/game
SRC = $(wildcard code/src/*.c)
OBJ = $(patsubst code/src/%.c, code/obj/%.o, $(SRC))
WEB_RAYLIB = code/libs/static_libs/web
LIB_SRC = $(wildcard code/libs/src/*.c)
LIB_OBJ = $(patsubst code/libs/src/%.c, code/libs/obj/%.o, $(LIB_SRC))

###################################
# Build Everything
###################################
default: from_scratch

from_scratch: clean lib_build build
###################################

###################################
# Build Game
###################################
build: $(TARGET)

$(TARGET): $(OBJ)
	gcc -o $@ $? -Wall -std=c99 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

code/obj/%.o : code/src/%.c
	gcc -c $< -o $@ -Icode/include -Icode/libs/include

clean:
	rm -f code/obj/*.o
	rm -f $(TARGET)*
###################################

###################################
# Build Libs
###################################
lib_build: lib_clean $(LIB_OBJ)

code/libs/obj/%.o : code/libs/src/%.c
	gcc -c $< -o $@ -Icode/libs/include

lib_copy:
	cp code/libs/obj/*.o code/obj/

lib_clean:
	rm -f code/libs/obj/*.o
###################################

###################################
# Build Web
###################################
update_emcc:
	~/emsdk/emsdk install latest

activate_emcc:
	~/emsdk/emsdk activate latest

activate_emcc_env:
	source ~/emsdk/emsdk_env.sh

web_build:
	emcc -o $(TARGET).html code/src/main.c -Os -Wall ./$(WEB_RAYLIB)/libraylib.a -Icode/include -Icode/libs/include -I/home/lhgs/raylib/src -L$(WEB_RAYLIB) -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s STACK_SIZE=20MB -s ASSERTIONS=1 -s EXPORTED_RUNTIME_METHODS=ccall --profiling -DPLATFORM_WEB
# --preload-file resources

###################################

###################################
# Run Game
###################################
run:
	./$(TARGET)

web_run:
	emrun $(TARGET).html