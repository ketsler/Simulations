

#include "stdafx.h"
#include <iostream>
using std::ostream;
using std::endl;

#include <iomanip>
using std::setw;

#include <cstdlib>

#include "Elevator.h"
#include "Rider.h"
class Floor;

int Elevator::elevatorID = 0;
const int Elevator::IDLE = 1;
const int Elevator::UP = 2;
const int Elevator::DOWN = 3;

bool Elevator::isNearDestination() const
{
  int final = toFloor->getLocation();
  if (abs(final - location) <= speed)
  {
    return true;
  }
  return false;
}

void Elevator::moveToDestinationFloor()
{
  if(isNearDestination())
  {
    location = toFloor->getLocation();
    
  }
}

// constructor

Elevator::Elevator(const int cap, const int givenSpeed, const Floor& start)
  :ID(elevatorID), capacity(cap), speed(givenSpeed), toFloor(0)
{
  elevatorID++;
  location = start.getLocation();
  direction = IDLE;
  doorOpen = true;
}

ostream& operator<<(ostream& output, const Elevator& e)
{
  output << "The elevator is now at: "  << setw(4) << e.location;
  output << ", Direction: " << setw(4);
  if (e.isIdle()) output << "Idle";
  if (e.isDirectionUp()) output << "Up";
  if (e.isDirectionDown()) output << "Down";
  output << ", the door is: " << setw(8);
  if (e.isDoorOpen()) output << "Open, ";
  else output << "Closed, ";
  int x = e.riders.size();
  if (x == 0) output << "No riders";
  else if (x == 1) output << "1 rider";
  else if (x > 1) output << x << " riders";
  output << endl;
  return output;
}

vector<Rider> Elevator::removeRidersForDestinationFloor()
{
  // create empty vector of removed riders to be used as return value
  vector<Rider> holder;
  // if elevator has any riders
  if (hasRiders())
  {
    // create an empty vector for riders who remain on elevator
    vector<Rider> leftBehind;
    // traverse vector of current riders
    for(int i = 0; i < riders.size(); i++)
    {
      // if a rider's destination floor is same as elevator's destination...
      if(riders[i].getDestination().getLocation() == toFloor->getLocation())
      {
        holder.push_back(riders[i]);
        // add rider to vector of removed riders
      }
      else
       {
     // else
       // add rider to vector of remaining riders
         leftBehind.push_back(riders[i]);
       }
    }
    // reassign elevator rider vector to vector of remaining riders
    riders = leftBehind;
  }
  // return vector of removed riders
  return holder;
}


void Elevator::addRiders(const vector<Rider>& ridersToBeAdded)
{
  // if the parameter vector is empty
  if (ridersToBeAdded.size() == 0){}
  // exit the function
  else
  {
  // traverse the parameter vector
    for (int i = 0; i < ridersToBeAdded.size(); i++)
    {
    // if there is still room on the elevator
      if (getAvailableSpace() > 0)
      {
        riders.push_back(ridersToBeAdded[i]);
      // add the rider to the elevator's rider vector
      }
    }
  }
}


void Elevator::setDestinationBasedOnRiders()
{
  // if there are no riders on the elevator
  if (riders.size() == 0){}
    // exit the function
  else
  {
  // create an int to track the closest rider's distance to his destination
  // initialize it to a negative number (to signify that it is not yet set)
    int tracker = -1;
  // traverse the vector of elevator riders
    for (int i = 0; i < riders.size(); i++)
    {
    // calculate the distance from the elevator to the rider's destination floor
      int holder = location - riders[i].getDestination().getLocation();
    // convert to absolute value
      holder = abs(holder);
    // if closest rider's distance is negative OR is greater than the above value
      if (tracker < 0 || tracker > holder)
      {
      // set closest rider's distance to the above value
        tracker = holder;
      // set elevator's destination floor to rider's destination floor
        setDestination(&riders[i].getDestination());
      }
    }
  }
}