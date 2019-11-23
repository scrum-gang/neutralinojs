FROM andreiungur/ecse437:cmake-base

WORKDIR /neutralinojs
COPY . /neutralinojs
CMD cmake .

# Sets up base docker image
# FROM ubuntu:bionic # Used to build base image
#RUN apt-get update
#RUN apt-get install -y cmake
#RUN apt-get install -y pkg-config
#RUN apt-get install -y build-essential
#RUN apt-get install -y build-essential libgtk-3-dev
#RUN apt-get install -y webkitgtk-3.0
#RUN cmake --version

