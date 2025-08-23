# Horrified (C++/raylib)

A single-player, raylib-powered reimplementation inspired by the board game "Horrified." Navigate the city, guide villagers, collect items, and defeat classic monsters through a turn-based Hero Phase and Monster Phase loop.

## Requirments
1. raylib : [https://www.raylib.com/](https://www.raylib.com/)
2. CMake

## How to use ??

### Manual Running 
first you have to clone this project.
than you have to download this [zip](https://drive.google.com/file/d/1yBVMTstVI49yq-ELa4JqEd6WH-z0iAc_/view?usp=drive_link) file and extract it into the root of project directory

than base on your OS you can compile and build the project but :
> make sure your executable file should be in build directory !!! otherwise project does not work correctly 

### 🐋 Run with Docker
> this way is just for unix base OS not windows !!
First make sure you have installed docker based on your OS<br>
Than you have to install X11 xhost utility on your os for example on : 

Arch base :
```bash
sudo pacman -Syu xorg-xhost
```

Debian / Ubuntu:
```bash
sudo apt update && sudo apt install x11-xserver-utils
```

than run this command on your terminal : `xhost +local:root`

Now you can run project with docker:
```bash
docker build -t Horrified .
docker run --rm -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix Horrified
```
At the end of the game and after the the game run this : `xhost -local:root`


## Collaborators
- 👤 [**Taha Sadeghi**](https://codeberg.org/Taha_Sadeghi) -> 40312358025
- 👤 [**Shahriar Kolivand**](https://codeberg.org/imShahriar-klvd) ->40312358035

> Have fun :D !!