CXX = g++
CXXFLAGS = -std=c++20 -O2

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_DIR = code
SHADER_DIR = shaders
BUILD_DIR = build

TARGET = $(BUILD_DIR)/VulkanTesting

SOURCES = $(SRC_DIR)/main.cpp \
		  $(SRC_DIR)/Assets.cpp \
		  $(SRC_DIR)/HelloTriangleApplication.cpp \
		  $(SRC_DIR)/QueueFamilyIndices.cpp

VERT_SHADER = $(SHADER_DIR)/shader.vert
FRAG_SHADER = $(SHADER_DIR)/shader.frag

VERT_SPV = $(BUILD_DIR)/shader.vert.spv
FRAG_SPV = $(BUILD_DIR)/shader.frag.spv

# ======= RULES =======

all: $(BUILD_DIR) shaders $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

shaders: $(VERT_SPV) $(FRAG_SPV)

$(VERT_SPV): $(VERT_SHADER)
	glslc $< -o $@

$(FRAG_SPV): $(FRAG_SHADER)
	glslc $< -o $@

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: all
	cd $(BUILD_DIR) && ./VulkanTesting

clean:
	rm -rf $(BUILD_DIR)