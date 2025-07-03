# 🎲 Horrified 👻

> A thrilling C++ terminal board game adventure! 

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++17"/>
  <img src="https://img.shields.io/badge/License-GPLv3-blue.svg" alt="GPLv3"/>
  <img src="https://img.shields.io/badge/Status-Playable-brightgreen.svg" alt="Status: Playable"/>
</p>

---

## 📑 Table of Contents

1. [✨ Features](#-features)
2. [⚡ Installation](#-installation)
3. [🕹️ Usage](#-usage)
4. [📦 Dependencies](#-dependencies)
5. [📝 License](#-license)
6. [🤝 Collaborators](#-collaborators)
7. [🚀 Contributing](#-contributing)
8. [🙏 Acknowledgements](#-acknowledgements)

---

## ✨ Features

- 🦸‍♂️ Play as classic heroes to defeat monsters like Dracula and the Invisible Man.
- 🧑‍🌾 Move, guide, and protect villagers across a dynamic map.
- 🧳 Collect items, perks, and solve clues to win.
- 🧩 Modular, extensible C++ codebase with clear OOP design.
- 💻 Interactive terminal UI.
- 🖼️ Includes a detailed UML diagram for architecture overview.

---

## ⚡ Installation

### Prerequisites

- 🖥️ C++ compiler (C++17 or newer)
- 🛠️ CMake (version 3.22+)
- 📦 [fmt library](https://github.com/fmtlib/fmt) (auto-installed for Ubuntu/Arch)
- 🐧 Linux (Ubuntu or Arch recommended)

### Quick Start

1. **Clone the repository:**
   ```sh
   git clone <repo-url>
   cd horr
   ```

2. **Run the installation script:**
   ```sh
   ./install.sh
   ```
   This script will:
   - 🚀 Install the `fmt` library (on Ubuntu/Arch)
   - 🏗️ Build the project using CMake
   - 🎮 Run the game

3. **Manual Build (if needed):**
   ```sh
   sudo apt install libfmt-dev   # Ubuntu
   # or
   sudo pacman -S fmt           # Arch

   mkdir build
   cd build
   cmake ..
   make
   ./Horrified
   ```

---

## 🕹️ Usage

After building, simply run the executable:

```sh
./build/Horrified
```

Follow the on-screen instructions to play the game. Good luck, hero! 🦸‍♀️🦸‍♂️


---

## 📦 Dependencies

- **fmt**: Fast, safe C++ formatting library ([MIT License](https://github.com/fmtlib/fmt))
- **CMake**: Build system generator
- **Threads**: For multi-threading support (standard on most systems)

All other dependencies are standard C++ libraries.

---

## 📝 License

This project is licensed under the [GNU General Public License v3.0 (GPLv3)](LICENSE).

---

## 🤝 Collaborators

- 👤 [**Taha Sadeghi**](https://codeberg.org/Taha_Sadeghi) -> 40312358025
- 👤 [**Shahriar Kolivand**](https://codeberg.org/imShahriar-klvd) ->40312358035

---

## 🚀 Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, improvements, or new features.

---

## 🙏 Acknowledgements

- 🎲 Inspired by the "Horrified" board game.
- 🧑‍💻 Uses [fmt](https://github.com/fmtlib/fmt) for formatting.
- 📊 Table rendering and some utility code based on [tabulate](https://github.com/p-ranav/tabulate) (MIT License).

---

> **Enjoy the game! But be careful monsters are around us 👾**
