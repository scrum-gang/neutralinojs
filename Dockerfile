FROM andreiungur/ecse437:cmake-base2

WORKDIR /neutralinojs
COPY . /neutralinojs
RUN cmake -DCMAKE_C_COMPILER=/usr/bin/gcc-8 -DCMAKE_CXX_COMPILER=/usr/bin/g++-8 .
CMD make

# Sets up base docker image
#FROM ubuntu:bionic
#RUN apt-get update
#RUN apt-get install -y cmake
#RUN apt-get install -y pkg-config
#RUN apt-get install -y build-essential
#RUN apt-get install -y build-essential libgtk-3-dev
#RUN apt-get install -y webkitgtk-3.0
#RUN apt-get install -y gcc-8
#RUN apt-get install -y g++-8
#RUN apt-get install -y software-properties-common
#RUN apt-get install -y nodejs
#RUN add-apt-repository --remove ppa:webkit-team/ppa -y
#RUN apt-get --allow-unauthenticated update 
#RUN apt-get install -y libwebkit2gtk-4.0-37 libwebkit2gtk-4.0-dev
#RUN cmake --version

