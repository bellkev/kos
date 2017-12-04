FROM ubuntu:xenial

RUN apt-get update; apt-get install -y build-essential nasm xorriso bison flex curl gdb
WORKDIR /root
# RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz && cd grub-2.02 && ./configure --with-platform=efi --enable-grub-mkfont && make install
RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz
RUN apt-get update; apt-get install -y libfreetype6-dev ttf-unifont # Needed for grub font support
RUN cd grub-2.02 && ./configure --target=i386 --with-platform=pc --enable-grub-mkfont && make install && make clean
RUN cd grub-2.02 && ./configure --target=i386 --with-platform=efi --enable-grub-mkfont && make install && make clean
RUN cd grub-2.02 && ./configure --target=x86_64 --with-platform=efi --enable-grub-mkfont && make install
RUN grub-mkfont -o /usr/local/share/grub/unicode.pf2 /usr/share/fonts/truetype/unifont/unifont.ttf

# OVMF and its deps:
RUN apt-get update; apt-get install -y uuid-dev python acpica-tools
# Note that multiple `-a` args to support both x64 and ia32 seem to be supported,
# but the x64 image seems to just clobber the ia32 in-place
RUN curl -L -o edk2.tar.gz https://github.com/tianocore/edk2/archive/vUDK2017.tar.gz && tar -xzf edk2.tar.gz && cd edk2-vUDK2017 && OvmfPkg/build.sh -a IA32 -b RELEASE

# For grub-mkrescue
RUN apt-get update; apt-get install -y mtools

RUN mkdir -p /opt/kos
WORKDIR /opt/kos