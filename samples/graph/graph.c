#include "pbPlots.h"
#include "supportLib.h"

int main(){
	double xs [] = {1, 2, 3, 4, 5};
	double ys [] = {20, 10, 33, 15, 0};

	ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
	series->xs = xs;
	series->xsLength = 5;
	series->ys = ys;
	series->ysLength = 5;
	series->linearInterpolation = true;
	series->lineType = L"dotted";
	series->lineTypeLength = wcslen(series->lineType);
	series->lineThickness = 2;
	series->color = GetGray(0.3);
	_Bool success;

	RGBABitmapImageReference *canvasReference = CreateRGBABitmapImageReference();
	StringReference *errorMessage = CreateStringReference(L"", 0);

    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
	settings->width = 600;
	settings->height = 400;
	settings->autoBoundaries = true;
	settings->autoPadding = true;
	settings->title = L"Salario";
	settings->titleLength = wcslen(settings->title);
	settings->xLabel = L"X eixo";
	settings->xLabelLength = wcslen(settings->xLabel);
	settings->yLabel = L"Y eixo";
	settings->yLabelLength = wcslen(settings->yLabel);
	ScatterPlotSeries *s [] = {series};
	settings->scatterPlotSeries = s;
	settings->scatterPlotSeriesLength = 1;

    success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);

	if(success){
		size_t length;
		double *pngdata = ConvertToPNG(&length, canvasReference->image);
		WriteToFile(pngdata, length, "graph.png");
		DeleteImage(canvasReference->image);
	} else {
		fprintf(stderr, "Error: ");
		for(int i = 0; i < errorMessage->stringLength; i++){
			fprintf(stderr, "%c", errorMessage->string[i]);
		}
		fprintf(stderr, "\n");
	}

	FreeAllocations();

	return success ? 0 : 1;
}