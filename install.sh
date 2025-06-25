#!/bin/bash
BUILD_DIR="build"
EXECUTABLE_NAME="Horrified"

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
