

CFLAGS = -std=c++20 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi 
binFolder = bin
sourceShaders = src/shaders
# create list of all spv files and set as dependency
vertSources = $(shell find $(sourceShaders) -type f -name "*.vert")
vertObjFiles = $(patsubst $(sourceShaders)/%.vert, bin/%.vert.spv, $(vertSources))
fragSources = $(shell find $(sourceShaders) -type f -name "*.frag")
fragObjFiles = $(patsubst $(sourceShaders)/%.frag, bin/%.frag.spv, $(fragSources))




TARGET = bin/neuton
#$(TARGET): $(vertObjFiles) $(fragObjFiles)
$(TARGET): src/*.cpp src/*.hpp
	g++ $(CFLAGS) -o $(TARGET) src/*.cpp $(LDFLAGS)

# make shader targets
# $(binFolder)/%.spv: $(sourceShaders)/%
# 	glslc $< -o $@

.PHONY: test clean

test: bin/neuton
	./bin/neuton

clean:
	rm -f ./bin/neuton
	rm -f ./bin/*.spv
