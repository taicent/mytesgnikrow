VPATH = ../detect

UNAME = $(shell uname)

INC = -I$(VPATH) -I../include

FLAGS=-DNoTcl -DNoHEBP -DCLOCK_DONE -DNoVista -DNoDebug

ifeq ($(UNAME),IRIX64)
  UNAME=IRIX
endif

ifeq ($(UNAME),IRIX)

  CC=cc
  CXX=CC
  CFLAGS=$(FLAGS) $(INC) -woff 3262,3672 -O2 
  CXXFLAGS=$(FLAGS) $(INC) -woff 3262,3672 -O2 \
           -Olimit 1500 +pp -ptall

  ifeq ($(ABI),-n32)
    CC=cc -n32
    CXX=CC -n32
    CFLAGS=$(FLAGS) $(INC) -O2
    CXXFLAGS=$(FLAGS) $(INC) -O2 -ptall
  endif

endif

ifeq ($(UNAME),SunOS)

ifeq ($(shell uname -r),5.5)
  SOLARIS=-Dsolaris
endif

  CC=gcc
  CXX=g++ -fno-for-scope
  CFLAGS=$(FLAGS) $(SOLARIS) $(INC) -O 
  CXXFLAGS=$(FLAGS) $(SOLARIS) $(INC) -O -fno-implicit-templates

endif

ifeq ($(UNAME),HP-UX)

  CC=gcc
  CXX=g++ -fno-for-scope
  CFLAGS=$(FLAGS) $(SOLARIS) $(INC) -O 
  CXXFLAGS=$(FLAGS) $(SOLARIS) $(INC) -O -fno-implicit-templates

endif

ifeq ($(UNAME),Linux)

  CC=gcc
  CXX=g++ -fno-for-scope -fno-implicit-templates
  CFLAGS=$(FLAGS) $(INC) -O3 -march=pentiumpro
  CXXFLAGS=$(FLAGS) $(INC) -O3 -march=pentiumpro
endif

ifeq ($(UNAME),OSF1)

  CC=gcc
  CXX=g++ -fno-for-scope
  CFLAGS=$(FLAGS) $(INC) -O 
  CXXFLAGS=$(FLAGS) $(INC) -O -fno-implicit-templates

endif

ifeq ($(UNAME),AIX)

#  CC=xlc
#  CXX=xlC
#  CFLAGS=$(FLAGS) $(INC) -g -O 
#  CXXFLAGS=$(FLAGS) $(INC) -g -O

  CC=gcc
  CXX=g++ -fno-for-scope
  CFLAGS=$(FLAGS) $(INC) -O 
  CXXFLAGS=$(FLAGS) $(INC) -O -fno-implicit-templates

endif

.SUFFIXES: .cc

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

.c.o:
	$(CC) $(CFLAGS) -c $<

OBJS = im_face.o search.o img.o mat.o fast2.o myhebp.o \
       follow.o gif.o system.o nn.o tclHash.o eye.o faces.o fft.o \
       ip.o tool.o kmain.o kdoi.o vote.o just.o

libdetect.a: $(OBJS)
	ar cr libdetect.a $(OBJS)
ifneq ($(UNAME),IRIX)
	ranlib libdetect.a
endif
