FROM gcc:latest as build

ARG WORKDIR=/usr/app

ADD ../../src/server.c $WORKDIR/src/server.c

RUN mkdir "$WORKDIR/bin/"
RUN gcc -o $WORKDIR/bin/server $WORKDIR/src/server.c

FROM alpine:latest

#WORKDIR /usr/app/bin
COPY --from=build /usr/app/bin/server /usr/app/bin/server

ENTRYPOINT ["cat","/usr/app/bin/server"]
