# Use the official debian image as the base image
FROM base_cpr:latest

# Copy the consumer.cpp, crow_all.h, and json.hpp files to the working directory
COPY compute.cpp .
COPY crow_all.h .
COPY json.hpp .
COPY CMakeLists.txt .

RUN mkdir build && cd build && cmake .. -DCMAKE_PREFIX_PATH="/cpr" && make

#CMD ["cd /app"]

# Set the entry point for the container
CMD ./build/compute $MSize $ServersNum
