#!/bin/bash
BUILD_DIR="build"
EXECUTABLE_NAME="Horrified"

install_fmt() {
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        if grep -q -i ubuntu /etc/os-release; then
            echo "🔧 Installing fmt package for Ubuntu..."
            sudo apt update
            sudo apt install -y libfmt-dev
        elif grep -q -i arch /etc/os-release; then
            echo "🔧 Installing fmt package for Arch Linux..."
            sudo pacman -S --noconfirm fmt
        else
            echo "❌ Unsupported Linux distribution. Please install fmt manually."
            exit 1
        fi
    else
        echo "❌ This script only supports Linux systems."
        exit 1
    fi
}

install_fmt

if [ -d "$BUILD_DIR" ]; then
    echo "📁 Build directory exists."
else
    echo "📁 Creating build directory..."
    echo "🔧 Running cmake..."
    cmake -B "$BUILD_DIR" .
fi

cmake --build "$BUILD_DIR"
cd "$BUILD_DIR"

if [ $? -eq 0 ]; then
    echo "✅ Build successful. Running..."
    echo "==============================="
    ./$EXECUTABLE_NAME
else
    echo "❌ Build failed."
fi
