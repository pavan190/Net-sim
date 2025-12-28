# Netsim Makefile - Usage Guide

## Quick Start

```bash
cd /home/pavanub/OT20/Netsim

# Build everything
make

# Run server (in one terminal)
make run-server

# Run client (in another terminal)  
make run-client

# Clean build
make clean
```

## What the Makefile Does

### Current Structure:
```
Netsim/
├── Makefile              ← Main build file
├── build/                ← Generated .o files (created automatically)
├── sender/
│   ├── clientsim.cpp    → Compiled to build/clientsim.o
│   ├── pktstruct.cpp    → Included by clientsim.cpp
│   └── client           ← Final executable (generated)
└── receiver/
    ├── serversim.cpp    → Compiled to build/serversim.o
    └── server           ← Final executable (generated)
```

## How to Add New Files

### 1. Adding a New Source File to Client

**Example: Add `protocol.cpp`**

```bash
# Create the file
touch sender/protocol.cpp
```

**Update Makefile:**
```makefile
# Add to SENDER_SRCS
SENDER_SRCS = $(SENDER_DIR)/clientsim.cpp $(SENDER_DIR)/protocol.cpp

# Add object file
SENDER_OBJS = $(BUILD_DIR)/clientsim.o $(BUILD_DIR)/protocol.o

# Add compilation rule
$(BUILD_DIR)/protocol.o: $(SENDER_DIR)/protocol.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Update clientsim dependency if protocol.cpp is included
$(BUILD_DIR)/clientsim.o: $(SENDER_DIR)/clientsim.cpp $(SENDER_DIR)/pktstruct.cpp $(SENDER_DIR)/protocol.cpp
	$(CXX) $(CXXFLAGS) -c $(SENDER_DIR)/clientsim.cpp -o $@
```

### 2. Adding a New Source File to Server

**Example: Add `database.cpp`**

```bash
touch receiver/database.cpp
```

**Update Makefile:**
```makefile
# Add to RECEIVER_SRCS
RECEIVER_SRCS = $(RECEIVER_DIR)/serversim.cpp $(RECEIVER_DIR)/database.cpp

# Add object file
RECEIVER_OBJS = $(BUILD_DIR)/serversim.o $(BUILD_DIR)/database.o

# Add compilation rule
$(BUILD_DIR)/database.o: $(RECEIVER_DIR)/database.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
```

### 3. Adding Header Files

**Example: Add `pktstruct.h`**

```bash
# Create header file
touch sender/pktstruct.h
```

**Update Makefile - Add as dependency:**
```makefile
# Update clientsim rule to depend on header
$(BUILD_DIR)/clientsim.o: $(SENDER_DIR)/clientsim.cpp $(SENDER_DIR)/pktstruct.h
	$(CXX) $(CXXFLAGS) -c $(SENDER_DIR)/clientsim.cpp -o $@
```

### 4. Adding a Common/Shared Directory

**Example: Create `common/` with shared utilities**

```bash
mkdir common
touch common/utils.cpp
touch common/utils.h
```

**Update Makefile:**
```makefile
# Add common directory
COMMON_DIR = common

# Add include path
CXXFLAGS = -Wall -Wextra -std=c++11 -g -I$(COMMON_DIR)

# Add common sources/objects
COMMON_SRCS = $(COMMON_DIR)/utils.cpp
COMMON_OBJS = $(BUILD_DIR)/utils.o

# Update executables to link with common objects
$(CLIENT): $(SENDER_OBJS) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER): $(RECEIVER_OBJS) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Add compilation rule for common
$(BUILD_DIR)/utils.o: $(COMMON_DIR)/utils.cpp $(COMMON_DIR)/utils.h
	$(CXX) $(CXXFLAGS) -c $< -o $@
```

## Step-by-Step: Adding Files

### Pattern to Follow:

1. **Create the file(s)**
   ```bash
   touch path/to/newfile.cpp
   ```

2. **Add source to variables** (top of Makefile)
   ```makefile
   SENDER_SRCS = ... $(SENDER_DIR)/newfile.cpp
   ```

3. **Add object file**
   ```makefile
   SENDER_OBJS = ... $(BUILD_DIR)/newfile.o
   ```

4. **Add compilation rule**
   ```makefile
   $(BUILD_DIR)/newfile.o: $(SENDER_DIR)/newfile.cpp dependencies...
   	$(CXX) $(CXXFLAGS) -c $< -o $@
   ```

5. **Update dependencies** of files that include it
   ```makefile
   $(BUILD_DIR)/clientsim.o: ... $(SENDER_DIR)/newfile.cpp
   ```

6. **Test**
   ```bash
   make clean
   make
   ```

## Common Makefile Variables Explained

- `CXX = g++` - C++ compiler
- `CXXFLAGS` - Compilation flags
  - `-Wall -Wextra` - Enable all warnings
  - `-std=c++11` - Use C++11 standard
  - `-g` - Include debug symbols
  - `-I<dir>` - Add include directory
- `LDFLAGS` - Linker flags (for libraries like `-lpthread`)
- `$@` - Target name (left side of `:`)
- `$<` - First dependency
- `$^` - All dependencies

## Adding External Libraries

**Example: Add pthread**

```makefile
LDFLAGS = -lpthread
```

**Example: Add multiple libraries**

```makefile
LDFLAGS = -lpthread -lm -lssl -lcrypto
```

## Available Make Commands

```bash
make              # Build both client and server
make client       # Build only client
make server       # Build only server
make clean        # Remove all build artifacts
make rebuild      # Clean and rebuild
make run-client   # Build and run client
make run-server   # Build and run server
make help         # Show help message
```

## Troubleshooting

### Error: "No rule to make target"
- Check file paths in Makefile match actual file locations
- Ensure all dependencies are listed correctly

### Error: "Undefined reference"
- Add missing source files to *_SRCS variables
- Add missing libraries to LDFLAGS

### Build doesn't update after changes
```bash
make clean && make
```

## Best Practices

1. **Always clean before important builds**: `make clean && make`
2. **Test incrementally**: Add one file at a time
3. **Keep .o files in build/**: Don't clutter source directories
4. **Use variables**: Easier to maintain than hardcoded paths
5. **Add dependencies correctly**: Rebuild when headers change
