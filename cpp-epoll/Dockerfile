FROM ubuntu:latest
WORKDIR /root

RUN apt-get update
RUN apt-get -y install  unzip
RUN apt-get -y install libboost-system-dev libboost-filesystem-dev

add ./cmake-build-release/src/highload ./
add ./start.bash ./start.bash
EXPOSE 80

CMD bash start.bash