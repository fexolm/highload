FROM ubuntu:latest
WORKDIR /root

add ./cmake-build-release/src/highload ./
EXPOSE 80

CMD ./highload

