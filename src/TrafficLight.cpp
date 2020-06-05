#include "TrafficLight.h"

#include <iostream>
#include <random>
 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}



TrafficLight::TrafficLight() {
    _type = ObjectType::objectTrafficLight;
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen() {
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
    // runs and repeatedly calls the receive function on the message queue.
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase() {
    return _currentPhase;
}

std::chrono::milliseconds TrafficLight::getCurrentPhaseDuration(TrafficLightPhase &phase) {
    return phase == TrafficLightPhase::red ? std::chrono::milliseconds(4000) : std::chrono::milliseconds(6000);
}

void TrafficLight::toggleTrafficLightPhase() {
    _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
}

void TrafficLight::simulate() {
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {
    std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now();
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));  // cycle throttle
        if ((std::chrono::system_clock::now() - lastUpdate) < getCurrentPhaseDuration(_currentPhase)) {
            continue;
        }
        toggleTrafficLightPhase();
        _phaseQueue.send(std::move(_currentPhase));
        lastUpdate = std::chrono::system_clock::now();
    }
}
