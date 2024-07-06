FROM archlinux:latest

ENV TZ=Asia/Taipei

RUN pacman -Sy --noconfirm base-devel cmake ninja clang gdb

RUN curl -LSs https://kazusa.me/dotfiles | sh

USER kazusa

ENTRYPOINT ["sleep", "infinity"]
