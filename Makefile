########################
# Makefile for IK lib  #
########################

LIB = -lm

OBJECTS = \
	sequence.o\
	toolbox.o\
	align.o\


APP = ik-test
SRC = ik-test.c
OBJ = ik-test.o

DATE = $(shell date +\%Y-\%m-\%d)

###########
# Targets #
###########

default:
	make gcc
	make archive

$(APP): $(OBJ) $(OBJECTS)
	$(CC) -o $(APP) $(CFLAGS) $(OBJ) $(OBJECTS) $(LIB)

clean:
	rm -f *.o $(APP)

depend: $(OBJECTS:.o=.c)
	gcc -MM $^ > $@

test: $(APP)
	./ik-test -all

tar:
	rm -rf /tmp/$(APP)
	mkdir /tmp/$(APP)
	cp -r * /tmp/$(APP)
	cd /tmp/$(APP); make clean; rm -rf CVS */CVS
	cd /tmp; tar -zcvf $(APP)-$(DATE).tar.gz $(APP)
	rm -rf /tmp/$(APP)


#################
# Architectures #
#################

gcc:
	make $(APP) CC="gcc" CFLAGS="-O2 -Wall -Werror"

archive:
	ar rvs libik.a $(OBJECTS)


###################
# Inference Rules #
###################

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

################
# Dependancies #
################

include depend

