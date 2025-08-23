FROM archlinux/archlinux

RUN pacman -Syu --noconfirm && \
    pacman -S base-devel --noconfirm && \
    pacman -S cmake --noconfirm && \
    pacman -S raylib --noconfirm && \
    pacman -S fmt --noconfirm &&\
    pacman -S mesa libx11 libxrandr libxinerama libxcursor libxfixes libxdamage --noconfirm

RUN mkdir -p /home/app

COPY . /home/app

WORKDIR /home/app

RUN cmake -S . -B bulid

WORKDIR /home/app/bulid

RUN make

CMD [ "./Horrified" ]