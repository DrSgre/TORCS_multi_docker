##############################################################################
#
#    file                 : Makefile
#    created              : Wed May 20 00:51:12 CET 2010
#    copyright            : (C) 2010 by Bernhard Wymann                         
#    email                : berniw@bluewin.ch   
#    version              : $Id: Makefile,v 1.1.2.1 2011/12/28 15:11:57 berniw Exp $                                  
#
##############################################################################
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
##############################################################################
CC            =  g++

CPPFLAGS      = -Wall -g 

all: $(OBJECTS) musicplayer

.SUFFIXES : .o .cpp .c

.cpp.o :
	$(CC) $(CPPFLAGS) $(EXTFLAGS) -c $<

.c.o :
	$(CC) $(CPPFLAGS) $(EXTFLAGS) -c $<


musicplayer: main.cpp $(OBJECTS)
		$(CC) $(CPPFLAGS) $(EXTFLAGS) musicplayer.cpp tgf.cpp params.cpp OggSoundStream.cpp OpenALMusicPlayer.cpp SoundStream.cpp directory.cpp os.cpp module.cpp hashtable.c hash.cpp xmlparse.c xmltok.c xmlrole.c -o main main.cpp $(OBJECTS) -lvorbisfile -lvorbisenc -lvorbis -lGL -lGLU -lglut -lopenal

clean:
	rm -f *.o main 

SOLIBRARY = libmusicplayer.so

SOLIBDIR  = .

SOURCES   = SoundStream.cpp OggSoundStream.cpp OpenALMusicPlayer.cpp musicplayer.cpp

EXPDIR  = include/musicplayer

EXPORTS = musicplayer.h SoundStream.h OggSoundStream.h OpenALMusicPlayer.h

src_PKGFILES	= $(shell find * -maxdepth 0 -type f -print)

src_PKGDIR	= ${PACKAGE}-${VERSION}/$(subst ${TORCS_BASE},,$(shell pwd))

include ${MAKE_DEFAULT}


