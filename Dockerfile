FROM ubuntu:mantic

RUN apt update
RUN apt install -y build-essential nlohmann-json3-dev suricata

COPY ./src /build/src
COPY ./include /build/include
COPY ./Makefile /build/Makefile
WORKDIR /build

RUN make
RUN mv Capefox /usr/bin/Capefox
RUN rm -rf /build

COPY ./config /Capefox/config
COPY ./rules /Capefox/rules
COPY ./output /output

WORKDIR /Capefox