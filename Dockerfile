FROM ubuntu:xenial

# Deps
RUN apt-get update; apt-get install -y \
  # Some basic dev/build tools
  build-essential nasm bison flex curl gdb \
  # Used by grub-mkrescue to create isos
  xorriso \
  # Incidentally required by grub-mkrescue when EFI platforms are installed
  libfreetype6-dev ttf-unifont mtools \
  # Needed by edk2/ovmf
  uuid-dev python acpica-tools

WORKDIR /root

# Grub
RUN curl -O ftp://ftp.gnu.org/gnu/grub/grub-2.02.tar.gz && tar -xzvf grub-2.02.tar.gz
RUN cd grub-2.02 && ./configure --target=i386 --with-platform=pc --enable-grub-mkfont && make install && make clean
RUN cd grub-2.02 && ./configure --target=i386 --with-platform=efi --enable-grub-mkfont && make install && make clean
RUN cd grub-2.02 && ./configure --target=x86_64 --with-platform=efi --enable-grub-mkfont && make install
RUN grub-mkfont -o /usr/local/share/grub/unicode.pf2 /usr/share/fonts/truetype/unifont/unifont.ttf

# OVMF
# Note that multiple `-a` args to support both x64 and ia32 seem to be supported,
# but the x64 image seems to just clobber the ia32 in-place
RUN curl -L -o edk2.tar.gz https://github.com/tianocore/edk2/archive/vUDK2017.tar.gz \
  && tar -xzf edk2.tar.gz && cd edk2-vUDK2017 && OvmfPkg/build.sh -a IA32 -b RELEASE

# 32-bit stuff for testing
RUN apt-get update; apt-get install -y libc6-dev-i386

# for tests
RUN apt-get update; apt-get install -y ctags

RUN mkdir -p /opt/kos
WORKDIR /opt/kos