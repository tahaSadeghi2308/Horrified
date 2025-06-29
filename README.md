# 🎲 Horrified

**Horrified** is a C++ console-based board game developed as part of an advanced programming course.  
Inspired by classic horror themes, the game offers a strategic and immersive experience. Players navigate a town under siege, confronting monsters, collecting items, and completing objectives before terror overwhelms the city.

---

## 🧩 Game Overview

In *Horrified*, you play as a hero facing legendary monsters in a haunted town.  
Your mission is to defeat these monsters through cooperation, careful planning, and strategic use of in-game mechanics like collecting items, activating locations, and interacting with tombs and villagers.

Key Features:
- Classic monsters (e.g., Dracula, Frankenstein, Invisible Man)
- Custom ASCII interface using the [tabulate](https://github.com/p-ranav/tabulate) library
- Terror Level system affecting difficulty
- Hero action system with multiple turn options
- Modular architecture using object-oriented C++

---

## 📁 Project Structure

```
Horrified/
├── src/                # C++ source code
│   ├── controller/     # Game logic (input handling, actions)
│   ├── model/          # Data models (Monsters, Items, Places)
│   └── view/           # Terminal-based UI rendering
├── include/            # Header files
├── data/               # Game data files (e.g., place/monster config)
├── assets/             # Optional art or external resources
├── main.cc             # Game entry point
├── CMakeLists.txt      # Build system configuration
├── install.sh          # Shell script for building the game
├── LICENSE             # License info (GPLv3)
└── TODO.md             # Planned improvements and open issues
```

---

## 🚀 Getting Started

### Prerequisites

Make sure you have the following installed:

- C++17 compatible compiler (`g++`, `clang++`, etc.)
- [CMake](https://cmake.org/) (version 3.10+)
- A Unix-like terminal (Linux or WSL recommended)

### Build and Run

```bash
git clone https://github.com/tahaSadeghi2308/Horrified.git
cd Horrified
./install.sh
./horrified
```

---

## 🚧 Known Issues & Planned Features

See [`TODO.md`](TODO.md) for the current issue list.

---

## 🤝 Contributing

Contributions are welcome!

- Fork the repository
- Submit a pull request
- Report bugs or ideas in [Issues](https://github.com/tahaSadeghi2308/Horrified/issues)

---

## 📄 License

This project is licensed under the **GNU General Public License v3.0**.  
See the [LICENSE](LICENSE) file for more details.

---

## 🙌 Credits

Created by [@tahaSadeghi2308](https://github.com/tahaSadeghi2308) and [@imShahriar-klvd](https://github.com/imShahriar-klvd)
Icons by [Twemoji](https://twemoji.twitter.com/) and Unicode Consortium

---

> "Face your fears. Fight together. Save the town."
