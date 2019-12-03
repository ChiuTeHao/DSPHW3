SRIPATH ?= /root/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
INC_PATH ?= inc
SRC_PATH ?= src

CXX = g++
CXXFLAGS = -O2 -I$(SRIPATH)/include -I$(INC_PATH)
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)
vpath %.c $(SRC_PATH)
vpath %.cpp $(SRC_PATH)

TARGET = mydisambig
SRC = mydisambig.cpp mymap.cpp
OBJ = $(SRC:.cpp=.o)
FROM ?= Big5-ZhuYin.map
TO ?= ZhuYin-Big5.map
.PHONY: all clean map

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

all: $(TARGET)

map:mapping.py
	@#TODO
	@python3 mapping.py $(FROM) $(TO)

try:try.cpp
	g++ try.cpp $(CXXFLAGS) -c try.o
	g++ try.o -o try
clean:
	$(RM) $(OBJ) $(TARGET)
