CXX = g++
CXXFLAGS = -std=c++20 -O2
CPPFLAGS = -I../components -I../components/stb-master

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

SRC_DIR = code
SHADER_DIR = shaders
BUILD_DIR = build

TARGET = $(BUILD_DIR)/VulkanTesting
TEXTURE_DIR = textures
TEXTURE_OUTPUT_DIR = $(BUILD_DIR)/assets/textures
TEXTURES = $(wildcard $(TEXTURE_DIR)/*)
TEXTURE_TARGETS = $(addprefix $(TEXTURE_OUTPUT_DIR)/,$(notdir $(TEXTURES)))

SOURCES = $(SRC_DIR)/main.cpp \
		  $(SRC_DIR)/Assets.cpp \
		  $(SRC_DIR)/HelloTriangleApplication.cpp \
		  $(SRC_DIR)/QueueFamilyIndices.cpp

VERT_SHADER = $(SHADER_DIR)/shader.vert
FRAG_SHADER = $(SHADER_DIR)/shader.frag

VERT_SPV = $(BUILD_DIR)/assets/shaders/vert.spv
FRAG_SPV = $(BUILD_DIR)/assets/shaders/frag.spv

# ======= RULES =======

all: $(BUILD_DIR) shaders textures $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/assets/shaders
	mkdir -p $(BUILD_DIR)/assets/textures

shaders: $(VERT_SPV) $(FRAG_SPV)

textures: $(TEXTURE_TARGETS)

$(TEXTURE_OUTPUT_DIR)/%: $(TEXTURE_DIR)/% | $(TEXTURE_OUTPUT_DIR)
	cp $< $@

$(TEXTURE_OUTPUT_DIR):
	mkdir -p $@

$(VERT_SPV): $(VERT_SHADER)
	glslc $< -o $@

$(FRAG_SPV): $(FRAG_SHADER)
	glslc $< -o $@

$(TARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: all
	cd $(BUILD_DIR) && ./VulkanTesting

clean:
	rm -rf $(BUILD_DIR)