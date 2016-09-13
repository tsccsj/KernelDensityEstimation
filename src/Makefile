GCC             ?= g++

# Gdal
GDALDIR = /sw/geosoft/gdal-1.11.2-fgdb
GDALLIB := -L$(GDALDIR)/lib -lgdal

all: KDE
KDE.o: KDE.c
	$(GCC) -o $@ -c $< 
io.o: io.c
	$(GCC) -I. -I$(GDALDIR)/include -o $@ -c $< 
KDE: KDE.o io.o
	$(GCC) -o ../$@ $+ $(GDALLIB)
clean:
	rm -f *.o ../KDE
