#include <stdio.h>
#include <unistd.h>

#include "rover.h"

int main (int argc, char* argv[])
{
    RV_start();
    sleep(10);
    RV_stop();
}
