FROM ubuntu:latest

RUN apt-get update \
  && apt-get -y install build-essential cmake

COPY . /app

WORKDIR app/build/
RUN cmake -S ../ -B .; \
    make