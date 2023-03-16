FROM ubuntu

RUN  apt update  \
    &&  apt install -y cmake -y build-essential -y net-tools -y gcc


# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Generate the build files
RUN cmake .

# Compile the application
RUN cmake --build .

# Run the application when the container launches
#CMD ["./SimpleWeb"]
