FROM kalilinux/kali-last-release AS aflpp
# Install AFL++ dependencies
RUN apt-get update
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

# Clone AFL++
RUN set -ex \
    && cd /usr/local/src \
    && git clone --depth 1 https://github.com/AFLplusplus/AFLplusplus.git \
    && set +ex

# Build AFL++
RUN set -ex \
    && cd /usr/local/src/AFLplusplus \
    && make all \
    && make install \
    && set +ex 
    
FROM aflpp AS vmf
# Install VMF dependencies
RUN DEBIAN_FRONTEND="noninteractive" apt-get install -y --no-install-recommends --fix-missing \
    libcurl4-openssl-dev \
    curl \
    # asan
    libclang-rt-$(clang --version | head -n1 | sed 's/.*clang version //' | sed 's/\..*//')-dev \
    libgcc-$(gcc --version | head -n1 | sed 's/\..*//' | sed 's/.* //')-dev

# Clone VMF
RUN set -ex \
    && cd /usr/local/src \
    && git clone https://github.com/draperlaboratory/vadermodularfuzzer.git VaderModularFuzzer\
    && set +ex

# Build VMF (with patches for GCC 15+ compatibility)
RUN set -ex \
    && cd /usr/local/src/VaderModularFuzzer \
    # patch to enable ASAN
    && sed -i 's/#add_compile_options(-fsanitize=address)/add_compile_options(-fsanitize=address)/' CMakeLists.txt \
    && sed -i 's/#add_link_options(-fsanitize=address)/add_link_options(-fsanitize=address)/' CMakeLists.txt \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && sed -i '/#include <cmath>/a #include <cstdint>' build_artifacts/deps/json11/json11-1.0.0/json11.cpp \
    && sed -i '/#include "yaml-cpp\/null.h"/a #include <cstdint>' build_artifacts/deps/yaml-cpp/yaml-cpp-0.8.0/src/emitterutils.cpp \
    && sed -i '/#include "ControllerModulePattern.hpp"/a #include <cstdint>' ../vmf/src/modules/common/controller/BalancedController.hpp \
    && make -j \
    && make install \
    && set +ex

FROM vmf AS vmfexp

# Copy and Build VmfExperimental
COPY . /usr/local/src/VmfExperimental
RUN set -ex \
    && cd /usr/local/src/VmfExperimental \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX=/usr/local/src/VaderModularFuzzer/build/vmf_install .. \
    && make -j \
    && make install \
    && set +ex

# Create experimental config file
RUN set -ex \
    && cd /usr/local/src/ \
    && cp VmfExperimental/test/config/experimentalModules.yaml VaderModularFuzzer/build/vmf_install/test/config/experimentalModules.yaml \
    && set +ex

# Create a runtime entrypoint script that fixes core dump handling before launching vader.
# /proc/sys/kernel/core_pattern is a live kernel interface that cannot be written during
# docker build. It requires --privileged (or --sysctl kernel.core_pattern=core) at runtime.
# The script warns but continues if the write fails so the image works in both cases.
RUN printf '#!/bin/sh\nif ! echo core >/proc/sys/kernel/core_pattern 2>/dev/null; then\n    echo "WARNING: could not set core_pattern (run with --privileged to fix crash/timeout misdetection)" >&2\nfi\nexec "$@"\n' > /usr/local/bin/docker-entrypoint.sh \
    && chmod +x /usr/local/bin/docker-entrypoint.sh

# WORKDIR /usr/local/src/VmfExperimental/build
# ENTRYPOINT [ "ctest", "--output-on-failure" ]
WORKDIR /usr/local/src/VaderModularFuzzer/build/vmf_install
ENTRYPOINT [ "/usr/local/bin/docker-entrypoint.sh", "./bin/vader", "-c", "test/config/experimentalModules.yaml", "-c", "test/haystackSUT/haystack_stdin.yaml" ]