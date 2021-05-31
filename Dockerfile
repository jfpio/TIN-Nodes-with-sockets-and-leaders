# syntax=docker/dockerfile:1
FROM ubuntu:20.04
COPY . /app
RUN apt-get update && apt-get install -y g++ cmake;
WORKDIR app

RUN mkdir build; \
    cd build; \
    cmake ../; \
    cmake --build .

WORKDIR /app/build

ENTRYPOINT ["./long_live_the_king"]