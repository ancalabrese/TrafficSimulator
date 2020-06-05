#include "TrafficLight.h"

#include <iostream>
#include <random>

template <typename T>
T MessageQueue<T>::receive() {
    std::unique_lock<std::mutex> lck(_mutex);
    _condition.wait(lck, [this] {
        return !_queue.empty();
    });
    return std::move(_queue.back());
}

template <typename T>
void MessageQueue<T>::send(T &&msg) {
    std::lock_guard<std::mutex> lck(_mutex);
    _queue.push_back(std::move(msg));
    _condition.notify_one();
}

TrafficLight::TrafficLight() {
    _type = ObjectType::objectTrafficLight;
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 1);
    std::srand(std::time(0));
    //Give random initial state to traffic light
    _currentPhase = TrafficLightPhase((int)distribution(generator););
}
TrafficLight::~TrafficLight() {}

void TrafficLight::waitForGreen() {
    while (_phaseQueue.receive() != TrafficLightPhase::green) {
        continue;
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase() {
    return _currentPhase;
}

std::chrono::milliseconds TrafficLight::getCurrentPhaseDuration(TrafficLightPhase &phase) {
    //Red and green light duration is the same for every traffic light. 
    return phase == TrafficLightPhase::red ? std::chrono::milliseconds(4000) : std::chrono::milliseconds(6000);
}

void TrafficLight::toggleTrafficLightPhase() {
    _currentPhase = _currentPhase == TrafficLightPhase::red ? TrafficLightPhase::green : TrafficLightPhase::red;
}

void TrafficLight::simulate() {
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
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
