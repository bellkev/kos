FROM ubuntu:xenial

RUN apt-get update; apt-get install -y build-essential nasm xorriso automake autogen bison flex curl
WORKDIR /root
RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz && cd grub-2.02 && ./configure && make install
RUN apt-get install -y genisoimage