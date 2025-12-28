# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
LDFLAGS = 

# Directories
SENDER_DIR = sender
RECEIVER_DIR = receiver
BUILD_DIR = build

# Source files
SENDER_SRCS = $(SENDER_DIR)/clientsim.cpp
RECEIVER_SRCS = $(RECEIVER_DIR)/serversim.cpp

# Object files (in build directory)
SENDER_OBJS = $(BUILD_DIR)/clientsim.o
RECEIVER_OBJS = $(BUILD_DIR)/serversim.o

# Executables
CLIENT = $(SENDER_DIR)/client
SERVER = $(RECEIVER_DIR)/server

# Default target - build both client and server
all: $(BUILD_DIR) $(CLIENT) $(SERVER)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build client executable
$(CLIENT): $(SENDER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ Client built successfully: $(CLIENT)"

# Build server executable
$(SERVER): $(RECEIVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ Server built successfully: $(SERVER)"

# Compile sender/clientsim.cpp
$(BUILD_DIR)/clientsim.o: $(SENDER_DIR)/clientsim.cpp $(SENDER_DIR)/pktstruct.cpp
	$(CXX) $(CXXFLAGS) -c $(SENDER_DIR)/clientsim.cpp -o $@

# Compile receiver/serversim.cpp
$(BUILD_DIR)/serversim.o: $(RECEIVER_DIR)/serversim.cpp
	$(CXX) $(CXXFLAGS) -c $(RECEIVER_DIR)/serversim.cpp -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(CLIENT) $(SERVER)
	@echo "✓ Cleaned build artifacts"

# Run client
run-client: $(CLIENT)
	./$(CLIENT)

# Run server
run-server: $(SERVER)
	./$(SERVER)

# Rebuild everything from scratch
rebuild: clean all

# Show help
help:
	@echo "Netsim Makefile - Available targets:"
	@echo "  make           - Build both client and server"
	@echo "  make client    - Build only client"
	@echo "  make server    - Build only server"
	@echo "  make clean     - Remove all build artifacts"
	@echo "  make rebuild   - Clean and rebuild everything"
	@echo "  make run-client - Build and run client"
	@echo "  make run-server - Build and run server"
	@echo ""
	@echo "Executables:"
	@echo "  Client: $(CLIENT)"
	@echo "  Server: $(SERVER)"

# Phony targets (not actual files)
.PHONY: all clean run-client run-server rebuild help

# Individual targets for convenience
client: $(CLIENT)
server: $(SERVER)
