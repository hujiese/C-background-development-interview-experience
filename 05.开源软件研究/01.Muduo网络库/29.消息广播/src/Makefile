HOME = /home/jack/workspace/
MUDUO_DIRECTORY ?= $(HOME)/build/release-install
#MUDUO_DIRECTORY ?= $(HOME)/build/install
MUDUO_INCLUDE = $(MUDUO_DIRECTORY)/include
MUDUO_LIBRARY = $(MUDUO_DIRECTORY)/lib
SRC = .

CXXFLAGS = -g -O0 -Wall -Wextra -Werror \
	   -Wconversion -Wno-unused-parameter \
	   -Wold-style-cast -Woverloaded-virtual \
	   -Wpointer-arith -Wshadow -Wwrite-strings \
	   -march=native -rdynamic \
	   -I$(MUDUO_INCLUDE)

LDFLAGS = -L$(MUDUO_LIBRARY) -lmuduo_net -lmuduo_base -lpthread -lrt

all: pub sub hub
clean:
	rm -f pub sub hub core

pub: $(SRC)/pub.cc pubsub.cc codec.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sub: $(SRC)/sub.cc pubsub.cc codec.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

hub: $(SRC)/hub.cc codec.cc
	g++ $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
