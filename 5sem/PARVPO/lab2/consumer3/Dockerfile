# Use the official debian image as the base image
FROM base_cpr:latest

# Copy the consumer.cpp, crow_all.h, and json.hpp files to the working directory
COPY consumer.cpp .
COPY crow_all.h .
COPY json.hpp .
COPY CMakeLists.txt .

# Compile the consumer.cpp file
#RUN cd /app
RUN mkdir build && cd build && cmake .. -DCMAKE_PREFIX_PATH="/cpr" && make

# Set the working directory inside the container
#WORKDIR /app

# Set the entry point for the container
CMD ["./build/consumer"]
