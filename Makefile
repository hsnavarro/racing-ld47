CXX = g++

CXXFLAGS = -Wall \
				-Wshadow \
				-Wextra \
				-std=c++17 \
				-lsfml-graphics \
				-lsfml-window \
		    -lsfml-system \
				-lsfml-audio \
				-lsfml-network

OUT_DIR = build

OUT_EXE = racing-ld47

CPP_FILES = $(wildcard ./src/*.cpp)

BUILD_EXISTS = $(wildcard ./${OUT_DIR})

build: create_dir compile

compile:
	@${CXX} ${CXXFLAGS} ${CPP_FILES} -o ${OUT_DIR}/${OUT_EXE}

create_dir:
	@mkdir -p ${OUT_DIR}

execute: 
	@./${OUT_DIR}/${OUT_EXE}

ifeq (${BUILD_EXISTS}, ) 
run_dependencies: build execute
else 
run_dependencies: execute
endif

run: run_dependencies

build-and-run: build execute

clean:
	@rm -rf ${OUT_DIR}