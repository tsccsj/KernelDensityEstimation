/**
 * KDE.c
 * Author: Yizhao Gao <yizhaotsccsj@gmail.com> 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"
#include <sys/time.h>

int main(int argc, char ** argv)
{
	if(argc != 9)
	{
		printf("ERROR: incorrect number of input parameters\n To run the code: KDE [inputFile] [outputFile] [xMin] [yMin] [xMax] [yMax] [cellSize] [bandwidth]\n");
		exit(1);
	}

	FILE * file;

	float xMin, yMin, xMax, yMax, cellSize, bandwidth;
	int nRow, nCol;
	int epsgCode = 900913;

	xMin = atof(argv[3]);
	yMin = atof(argv[4]);
	xMax = atof(argv[5]);
	yMax = atof(argv[6]);
	cellSize = atof(argv[7]);
	bandwidth = atof(argv[8]);

	nRow = ceil((yMax - yMin)/cellSize);
	nCol = ceil((xMax - xMin)/cellSize);
	yMax = yMin + nRow * cellSize;
	xMax = xMin + nCol * cellSize;

	float * xCor;
	float * yCor;
	int nPoints;

	float * density;

	float dist2;
	float bandwidth2 = bandwidth * bandwidth;

	int rowID;
	int colID;

	int searchWindow = (int)(ceil(bandwidth / cellSize));
	int wXMin;
	int wXMax;
	int wYMin;
	int wYMax;

	float cellX;
	float cellY;

	struct timeval tBegin;
	struct timeval tIO;
	struct timeval tEnd;
		
	gettimeofday(&tBegin, NULL);

	if(NULL == (density = (float *)malloc(sizeof(float) * nCol * nRow)))
	{
		printf("ERROR: Out of Memory at line %d at file %s\n", __LINE__, __FILE__);
		exit(1);
	}

	for(int i = 0; i < nCol * nRow; i++)
	{
		density[i] = 0.0f;
	}


	if(NULL == (file = fopen(argv[1], "r")))
	{
		printf("ERROR: Can't open the input file %s\n", argv[1]);
		exit(1);
	}

	nPoints = getNumPoints(file);

	if(NULL == (xCor = (float *)malloc(sizeof(float) * nPoints)))
	{
		printf("ERROR: Out of Memory at line %d at file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	if(NULL == (yCor = (float *)malloc(sizeof(float) * nPoints)))
	{
		printf("ERROR: Out of Memory at line %d at file %s\n", __LINE__, __FILE__);
		exit(1);
	}

	readFile(file, xCor, yCor, nPoints);

	fclose(file);

	gettimeofday(&tIO, NULL);

	for(int k = 0; k < nPoints; k++)
	{
		rowID = (int)((yMax - yCor[k]) / cellSize);
		colID = (int)((xCor[k] - xMin) / cellSize);
					
		wXMin = colID - searchWindow;
		wXMax = colID + searchWindow + 1;
		wYMin = rowID - searchWindow;
		wYMax = rowID + searchWindow + 1;

		if(wXMin < 0)
		{
			wXMin = 0;
		}
		if(wXMax > nCol)
		{
			wXMax = nCol;
		}
		if(wYMin < 0)
		{
			wYMin = 0;
		}
		if(wYMax > nRow)
		{
			wYMax = nRow;
		}

		for(int m = wYMin; m < wYMax; m++)
		{
			for(int n = wXMin; n < wXMax; n++)
			{
				cellX = xMin + cellSize * (n + 0.5);
				cellY = yMax - cellSize * (m + 0.5);

				dist2 = (cellX - xCor[k]) * (cellX - xCor[k]) + (cellY - yCor[k]) * (cellY - yCor[k]);
				if(dist2 < bandwidth2)
				{
					density[m * nCol + n] += (1 - dist2/bandwidth2) * (1 - dist2/bandwidth2);
				}
			}
		}
	}

			

	free(xCor);
	free(yCor);

	for(int i = 0; i < nRow * nCol; i++) {
		density[i] = density[i] * 2 * cellSize * cellSize / (M_PI * bandwidth2 * nPoints) * 1000000000;
	}

	gettimeofday(&tEnd, NULL);

	writeGeoTiffF(argv[2], density, nRow, nCol, xMin, yMax, cellSize, epsgCode);


	printf("Time - Input:\t%lfms\n", ((&tIO)->tv_sec - (&tBegin)->tv_sec) * 1000 + (double)((&tIO)->tv_usec - (&tBegin)->tv_usec) / 1000);
	printf("Time - Compu:\t%lfms\n", ((&tEnd)->tv_sec - (&tIO)->tv_sec) * 1000 + (double)((&tEnd)->tv_usec - (&tIO)->tv_usec) / 1000);

	

	free(density);
	return 0;
}
