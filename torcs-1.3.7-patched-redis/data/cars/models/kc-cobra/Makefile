##############################################################################
#
#    file                 : Makefile
#    created              : Sun Feb 18 21:02:29 /etc/localtime 2001
#    copyright            : (C) 2000 by Eric Espi?                        
#    email                : Eric.Espie@torcs.org   
#    version              : $Id: Makefile,v 1.3 2004/11/06 14:41:48 torcs Exp $                                  
#
##############################################################################
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
##############################################################################

CAR       = kc-cobra

DATADIR   = cars/$(CAR)

DATA      = $(shell find * -maxdepth 0 -type f -print)

DATA_PKG  = 	$(shell find *.acc -maxdepth 0 -type f -print) \
		$(shell find *.rgb -maxdepth 0 -type f -print) \
		$(shell find *.xml -maxdepth 0 -type f -print) \
		$(shell find *.txt -maxdepth 0 -type f -print)

#data-cars-kcendra-sport_PKGFILES	= $(DATA_PKG) $(shell find *.xcf -maxdepth 0 -type f -print)

#data-cars-kcendra-sport_PKGDIR	= $(DATADIR)

include ${MAKE_DEFAULT}


