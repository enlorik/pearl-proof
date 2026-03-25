FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and runtime dependencies
RUN apt-get update && apt-get install -y \
    ca-certificates \
    cmake \
    curl \
    g++ \
    gnupg \
    make \
    git \
    pkg-config \
    uuid-dev \
    openjdk-17-jre-headless \
    && rm -rf /var/lib/apt/lists/*

# Install Node.js 20 LTS via NodeSource
RUN curl -fsSL https://deb.nodesource.com/setup_20.x | bash - && \
    apt-get install -y nodejs && \
    rm -rf /var/lib/apt/lists/*

# Build and install ANTLR4 C++ runtime from source (version 4.10)
# Install headers to /usr/include/antlr4-runtime and lib to /usr/lib/x86_64-linux-gnu
# to match the paths hardcoded in CMakeLists.txt
RUN git clone --depth 1 --branch 4.10 https://github.com/antlr/antlr4.git /tmp/antlr4 && \
    cmake -S /tmp/antlr4/runtime/Cpp -B /tmp/antlr4/build \
        -DANTLR4_INSTALL=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_INSTALL_LIBDIR=/usr/lib/x86_64-linux-gnu \
        -DCMAKE_INSTALL_INCLUDEDIR=/usr/include \
        -DPKG_CONFIG_EXECUTABLE=/usr/bin/pkg-config && \
    cmake --build /tmp/antlr4/build --parallel $(nproc) && \
    cmake --install /tmp/antlr4/build && \
    rm -rf /tmp/antlr4

# Copy repo source
WORKDIR /src
COPY . .

# Build PearlProof
RUN cmake -B build && cmake --build build

# Install to /app
RUN mkdir -p /app && cp build/pearlproof /app/pearlproof

# Install Node.js server dependencies
RUN npm install --prefix server

# Expose port
EXPOSE 3000

# Start the API server
CMD ["node", "server/index.js"]
