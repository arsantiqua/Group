TARGET := EvenOdd

SRC_INCDIRS := . ../GroupLib

TGT_LDLIBS  := -lGroups
TGT_PREREQS :=  libGroups.a
TGT_LDFLAGS := -L${TARGET_DIR}

SOURCES := EvenOdd.cpp
