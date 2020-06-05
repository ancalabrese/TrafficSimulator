#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <condition_variable>
#include <deque>
#include <mutex>

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase { red,
                         green };

template <class T>
class MessageQueue {
   public:
    T receive();
    void send(T &&msg);

   private:
   std::condition_variable _condition; 
   std::mutex _mutex;
   std::deque<TrafficLightPhase> _queue;
};

class TrafficLight : public TrafficObject {
   public:
    // constructor / desctructor
    TrafficLight();
    ~TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    void waitForGreen();
    void simulate();

   private:
    // typical behaviour methods
    void cycleThroughPhases();
    void toggleTrafficLightPhase();
    std::chrono::milliseconds getCurrentPhaseDuration(TrafficLightPhase &phase);

    MessageQueue<TrafficLightPhase> _phaseQueue;
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
};

#endif