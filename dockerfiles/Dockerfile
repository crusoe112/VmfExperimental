FROM kalilinux/kali-last-release AS deps

# RUN  apt-get update \
#      && DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing \
#        ca-certificates \
#        curl \
#        gdb \
#        git \
#        gnupg \
#        lsb-release \
#        zip

RUN apt-get update

# Install AFL++ dependencies
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing \
    build-essential \
    python3-dev \
    automake \
    cmake \
    git \
    flex \
    bison \
    libglib2.0-dev \
    libpixman-1-dev \
    python3-setuptools \
    cargo \
    libgtk-3-dev \
    ninja-build \
    cpio \
    libcapstone-dev \
    wget \
    curl \
    python3-pip

# try to install llvm 14 and install the distro default if that fails
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing lld-14 llvm-14 llvm-14-dev clang-14 \
    || DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing lld llvm llvm-dev clang

# install relevant gcc-X-plugin-dev and libstdc++-X-dev
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gcc-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-plugin-dev \
    libstdc++-$(gcc --version|head -n1|sed 's/\..*//'|sed 's/.* //')-dev

# # Install VMF dependencies
# RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing \
#     libcurl4-openssl-dev

FROM deps AS aflpp

# Clone and build AFL++
RUN set -ex \
    && cd /usr/local/src \
    && git clone --depth 1 https://github.com/AFLplusplus/AFLplusplus.git \
    && cd AFLplusplus \
    && make all \
    && make install \
    && set +ex
    
# FROM aflpp AS vmf
# ENV  LLVM_CONFIG=llvm-config-16
# # Clone, build, install VMF
# RUN set -ex \
#     && cd /usr/local/src \
#     && git clone https://github.com/draperlaboratory/vadermodularfuzzer.git VaderModularFuzzer\
#     && cd VaderModularFuzzer \
#     && mkdir -p build \
#     && cd build \
#     && cmake .. \
#     && make -j \
#     && make install \
#     && set +ex

# FROM vmf AS vmfexp

# # Copy VmfExperimental
# COPY . /usr/local/src/VmfExperimental
# RUN set -ex \
#     && cd /usr/local/src/VmfExperimental \
#     && mkdir build \
#     && cd build \
#     && cmake -DCMAKE_INSTALL_PREFIX=/usr/local/src/VaderModularFuzzer/build/vmf_install .. \
#     && make -j \
#     && make install \
#     && set +ex

# # Create experimental config file
# RUN set -ex \
#     && cd /usr/local/src/ \
#     && cp VmfExperimental/test/config/experimentalModules.yaml VaderModularFuzzer/build/vmf_install/test/config/experimentalModules.yaml \
#     && set +ex

# # WORKDIR /usr/local/src/VmfExperimental/build
# WORKDIR /usr/local/src/VaderModularFuzzer/build/vmf_install
# ENTRYPOINT [ "./bin/vader", "-c", "test/config/experimentalModules.yaml", "-c", "test/haystackSUT/haystack_stdin.yaml" ]