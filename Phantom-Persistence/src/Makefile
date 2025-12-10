# Makefile for Phantom-Persistence Educational Malware Implant
# Compiles implant.c with necessary libraries and flags

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -std=c11

# Libraries to link
LIBS = -lcurl

# Target binary name
TARGET = phantom-implant

# Source file
SRC = implant.c

# Default target - builds the implant
all: $(TARGET)

# Compile the implant
$(TARGET): $(SRC)
	@echo "[+] Compiling Phantom-Persistence implant..."
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)
	@echo "[+] Build complete: $(TARGET)"
	@echo "[+] Binary size: $$(du -h $(TARGET) | cut -f1)"

# Debug build with symbols
debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)
	@echo "[+] Debug build complete with symbols"

# Stripped build (smaller, harder to analyze)
stripped: $(TARGET)
	@echo "[+] Stripping binary..."
	strip $(TARGET)
	@echo "[+] Stripped size: $$(du -h $(TARGET) | cut -f1)"

# Clean build artifacts
clean:
	@echo "[+] Cleaning build artifacts..."
	rm -f $(TARGET)
	@echo "[+] Clean complete"

# Install dependencies (Debian/Ubuntu)
install-deps:
	@echo "[+] Installing required dependencies..."
	sudo apt-get update
	sudo apt-get install -y libcurl4-openssl-dev build-essential
	@echo "[+] Dependencies installed"

# Check if required libraries are available
check-deps:
	@echo "[+] Checking for required libraries..."
	@pkg-config --exists libcurl && echo "✓ libcurl found" || echo "✗ libcurl NOT found - run 'make install-deps'"

# Display help
help:
	@echo "Phantom-Persistence Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make              - Build the implant (default)"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make stripped     - Build and strip binary"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make install-deps - Install required libraries"
	@echo "  make check-deps   - Check if dependencies are installed"
	@echo "  make help         - Show this help message"

.PHONY: all debug stripped clean install-deps check-deps help