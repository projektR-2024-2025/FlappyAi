#ifndef FITNESSCALCULATOR_H
#define FITNESSCALCULATOR_H

#include <chrono>
#include <thread>
#include "Simulator.h"
#include "Agent.h"
#include "Controller.h"

// Calculates the fitness based on the controller's performance
float fitness(Controller *controller);

#endif // FITNESSCALCULATOR_H
