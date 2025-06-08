# Use the official debian image as the base image
FROM debian:12

# Set the working directory inside the container
# WORKDIR /app

# Install Boost dependencies
RUN apt-get update && \
    apt-get install -y build-essential libboost-all-dev libssl-dev cmake git

RUN git clone https://github.com/libcpr/cpr.git

RUN cd cpr && mkdir build && cd build && \
    cmake .. -DCPR_USE_SYSTEM_CURL=OFF && \
    cmake --build . --parallel && \
    cmake --install .

# Set the entry point for the container
CMD ["bash"]