SRC		= io_data.cpp
HDR		= io_data.h io_data_constants.h
LIBRARY		= libio_data.a

INCLUDE_DIRS	= -I ../include

CC		= g++
CFLAGS		= -g -Wall $(INCLUDE_DIRS)

AR		= ar
AR_FLAGS	= rcs 

DEPFILE		= .depends
DEPFLAGS	= 

OBJ		= $(SRC:%.cpp=%.o)

.PHONY:		clean


$(LIBRARY): $(DEPFILE) $(OBJ) 
	$(AR) $(AR_FLAGS) $@ $(OBJ) 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPFILE):
	$(CC) -M $(DEPFLAGS) $(CFLAGS) $(SRC) > $@

clean: 
	rm -f *.s *.o $(LIBRARY) $(DEPFILE)

sinclude $(DEPFILE)
