VPATH = ../example

UNAME = $(shell uname)

INC = -I../include

ifeq ($(UNAME),IRIX64)
  UNAME=IRIX
endif

ifeq ($(UNAME),IRIX)

  LIBS=-L. -ldetect -lm
  CC=cc $(ABI)
  CXX=CC $(ABI)

endif

ifeq ($(UNAME),SunOS)

  LIBS=libdetect.a -lm
  CC=gcc
  CXX=g++ -fno-for-scope

endif

ifeq ($(UNAME),Linux)

  LIBS=libdetect.a -lm
  CC=gcc
  CXX=g++ -fno-for-scope

endif

ifeq ($(UNAME),HP-UX)

  LIBS=libdetect.a -lm
  CC=gcc
  CXX=g++ -fno-for-scope

endif


ifeq ($(UNAME),OSF1)

  LIBS=-L/afs/cs.cmu.edu/local/libgxx/@sys/2.7.1/lib libdetect.a -lm 
  CC=gcc
  CXX=g++ -fno-for-scope

endif

ifeq ($(UNAME),AIX)

  LIBS=libdetect.a -lm 
  CC=gcc
  CXX=g++ -B/usr/ccs/bin

#  CC=xlc
#  CXX=xlC

endif

example: rot_test_ext \
	xavier_test xaviercolor_test im_test im2_test track_test astro_test \
	siemens_test siemens2_test imorient_test imgray_test slow_test \
	rot_test 

astro_test: astro_test.o libdetect.a
	$(CXX) -o astro_test astro_test.o $(LIBS)

xavier_test: xavier_test.o libdetect.a
	$(CXX) -o xavier_test xavier_test.o $(LIBS)

xaviercolor_test: xaviercolor_test.o libdetect.a
	$(CXX) -o xaviercolor_test xaviercolor_test.o $(LIBS)

track_test: track_test.o libdetect.a
	$(CXX) -o track_test track_test.o $(LIBS)

siemens_test: siemens_test.o libdetect.a
	$(CXX) -o siemens_test siemens_test.o $(LIBS)

siemens2_test: siemens2_test.o libdetect.a
	$(CXX) -o siemens2_test siemens2_test.o $(LIBS)

im_test: im_test.o libdetect.a
	$(CXX) -o im_test im_test.o $(LIBS)

imorient_test: imorient_test.o libdetect.a
	$(CXX) -o imorient_test imorient_test.o $(LIBS)

im2_test: im2_test.o libdetect.a
	$(CXX) -o im2_test im2_test.o $(LIBS)

imgray_test: imgray_test.o libdetect.a
	$(CXX) -o imgray_test imgray_test.o $(LIBS)

slow_test: slow_test.o libdetect.a
	$(CXX) -o slow_test slow_test.o $(LIBS)

rot_test: rot_test.o libdetect.a
	$(CXX) -o rot_test rot_test.o $(LIBS)

rot_test_ext: rot_test_ext.o libdetect.a
	$(CXX) -o rot_test_ext rot_test_ext.o $(LIBS)

.c.o:
	$(CC) $(INC) -c $<

