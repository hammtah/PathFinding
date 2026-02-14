FROM alpine:3.20 AS builder
LABEL authors="taha"

#ENTRYPOINT ["top", "-b"]
# Prevent prompts during installation
#ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apk update
RUN apk add --no-cache \
    build-base \
    cmake \
    python3 \
    py3-pip \
    git \
    linux-headers
# Virtual environement for conan
RUN python3 -m venv /opt/venv
# No need to activate the env for each cmd, by making this the path for all following cmds
ENV PATH="/opt/venv/bin:$PATH"
# Install Conan
RUN pip3 install --upgrade pip && pip3 install conan
# Create a conan profile
RUN conan profile detect --force
WORKDIR /app
# dependcy files
COPY conanfile.txt .
# install conan dependencies
RUN conan install . --output-folder=build --build=missing -s build_type=Release
# Copy source code
COPY . .
# Build the project
WORKDIR /app/build
# Configure cmake with conan toolchain
RUN cmake --version
RUN ls -R
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
# Build
RUN cmake --build .

FROM alpine:3.20
## Run
WORKDIR /app
RUN apk add --no-cache libstdc++ gcompat
COPY --from=builder /app/build/Path_Finding .
RUN chmod +x ./Path_Finding
EXPOSE 18080
CMD ["./Path_Finding"]