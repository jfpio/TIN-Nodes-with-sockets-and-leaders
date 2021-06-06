# syntax=docker/dockerfile:1
FROM ubuntu:20.04
COPY . /app
RUN apt-get update && apt-get install -y g++ cmake;
WORKDIR app

RUN mkdir docker-cmake-build; \
    cd docker-cmake-build; \
    cmake -DDOCKER=ON -DBUILD_TESTS=OFF ../; \
    cmake --build .

WORKDIR /app/docker-cmake-build

ENTRYPOINT ["./long_live_the_king"]