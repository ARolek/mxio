FROM ubuntu:latest

RUN apt-get -y update

RUN apt-get install -y build-essential

RUN apt-get install -y tmux vim
