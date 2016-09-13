#ifndef IOH
#define IOH

int getNumPoints(FILE * file);
void readFile(FILE * file, float * xCor, float * yCor, int nPoints);
void output(FILE * file, float * outputDensity, int nRow, int nCol);
void output(FILE * file, int * outputDensity, int nRow, int nCol);
void outputASC(FILE * file, float * outputRaster, int nRow, int nCol, float xMin, float yMin, float cellSize);
void writeGeoTiffF(char * fileName, float * result, int nRow, int nCol, float xMin, float yMax, float cellSize, int epsgCode);

#endif
