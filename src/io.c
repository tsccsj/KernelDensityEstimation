#include <stdio.h>
#include <cstdlib>
#include "io.h"

int getNumPoints(FILE * file)
{
	float x, y;
	int temp;
	int count = 0;

	rewind(file);

	while(fscanf(file, "%f,%f\n", &x, &y, &temp) != EOF)
	{
		count ++;
	}

	return count;	
}

void readFile(FILE * file, float * xCor, float * yCor, int nPoints)
{
	rewind(file);

	int temp;

	int n = 0;
	while(fscanf(file, "%f,%f\n", (xCor + n), (yCor + n), &temp) != EOF)
	{
		n ++;
	}

	if(n != nPoints)
	{
		printf("ERROR: Incorrest input file name.\n");
		exit(1);
	}

	return;
}

void output(FILE * file, float * outputDensity, int nRow, int nCol)
{
	for(int j = 0; j < nRow; j++)
	{
		for(int i = 0; i < nCol; i++)
		{
			fprintf(file, "\t%f", outputDensity[j * nCol + i]);
		}
		fprintf(file, "\n");
	}
	
	return;
}

void output(FILE * file, int * outputDensity, int nRow, int nCol)
{
	for(int j = 0; j < nRow; j++)
	{
		for(int i = 0; i < nCol; i++)
		{
			fprintf(file, "\t%d", outputDensity[j * nCol + i]);
		}
		fprintf(file, "\n");
	}
	
	return;
}

void outputASC(FILE * file, float * outputRaster, int nRow, int nCol, float xMin, float yMin, float cellSize)
{
	fprintf(file, "ncols\t%d\n", nCol);
	fprintf(file, "nrows\t%d\n", nRow);
	fprintf(file, "xllcorner\t%f\n", xMin);
	fprintf(file, "yllcorner\t%f\n", yMin);
	fprintf(file, "cellsize\t%f\n", cellSize);
	fprintf(file, "NODATA_value\t-9999\n");
	for(int i = 0; i < nRow; i++)
	{
		for(int j = 0; j < nCol; j++)
		{
			fprintf(file, "\t%f", outputRaster[i * nCol + j]);
		}
		fprintf(file, "\n");
	}

}
