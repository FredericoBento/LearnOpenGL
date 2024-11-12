PROJECT_NAME=LearnOpenGL
.PHONY: build
	
all: build

build:
	@mkdir -p build
	cd build && make

rebuild: clean
	@mkdir -p build
	cd build && cmake .. && make

clean:
	@rm -rf build

run: build
	./build/${PROJECT_NAME}
