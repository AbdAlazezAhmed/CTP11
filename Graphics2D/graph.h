#include "graphicsInclude.h"
#include <time.h>
float pressureGauge;
float pressureNormal;
float pressureAbs;
struct timeval stop, start;
struct temp
{
    float *temp;
    float *time;
    float numberOfReadings;
} typedef temp;

temp currentTemp;

void paintPressure(GtkWidget *widget,
                   GdkEventExpose *eev,
                   gpointer data);
void paintTemp(GtkWidget *widget, GdkEventExpose *eev, gpointer data);
