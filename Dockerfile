FROM ubuntu:xenial

RUN apt-get update; apt-get install -y build-essential nasm xorriso bison flex curl gdb
WORKDIR /root
# RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz && cd grub-2.02 && ./configure --with-platform=efi --enable-grub-mkfont && make install
RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz && cd grub-2.02 && ./configure --target=i386 --with-platform=efi && make install

# OVMF and its deps:
RUN apt-get install -y uuid-dev python acpica-tools
RUN curl -L -o edk2.tar.gz https://github.com/tianocore/edk2/archive/vUDK2017.tar.gz && tar -xzf edk2.tar.gz && cd edk2-vUDK2017 && OvmfPkg/build.sh -a IA32 -b RELEASE

# Sometimes grub complains if this is missing
# RUN apt-get install -y libfreetype6-dev ttf-unifont
# RUN grub-mkfont -o /usr/local/share/grub/unicode.pf2 /usr/share/fonts/truetype/unifont/unifont.ttf

RUN mkdir -p /opt/kos
WORKDIR /opt/kos