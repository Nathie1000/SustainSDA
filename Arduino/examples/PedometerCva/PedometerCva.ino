#include <SustainWork.h>
#include "StepDetection.h"

void setup() {
  DEBUG_BEGIN(9600);
  new StepDetection();
  TaskBase::startAllTasks();
}

void loop() {
}
