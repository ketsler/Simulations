
#include "stdafx.h"
#include "Building.h"
#include "Floor.h"
#include "Elevator.h"
#include "Rider.h"
#include <iostream>
using std::ostream;
using std::endl;

#include <iomanip>
using std::setw;

void getDifferentInts(int max, int& a, int& b)
{
  do 
  {
    a = rand() % max; // range is 0 to (max-1)
    b = rand() % max; // range is 0 to (max-1)
    
  } while (a == b); // try again if they are the same
}

Building& Building::step(int riders)
{
    simulationTime++;
  /*The following steps are executed EVERY SECOND of the simulation:
  Implement and test code blocks in the order shown in [square brackets]
  */
  // POSSIBLE RIDER ACTION [1]
  int a = 0;
  int b = 0;
  // add rider(s) in this time step (The number of riders to be added is sent to this function in the argument list, and is greater or equal to 0.) [1]
  for (int i = 0; i < riders; i++)
  {
    // create a rider with randomly selected from- and to-floors (where from- and to- are not the same!) [1]
    getDifferentInts(buildingFloors.size(), a, b);
    
    // tell the from-floor to add this rider [1]
    buildingFloors[a]->addNewRider(Rider(*buildingFloors[b]));
  }
  // ELEVATOR ACTIONS [3]
  // tell each elevator to perform an action [3]
  for (int i = 0; i < buildingElevators.size(); i++)
  {
    // if elevator door is closed (move up or down) [3]
    if (!buildingElevators[i]->isDoorOpen())
    {
      // if not near enough to destination to reach it in this time step, continue moving [3]
      if (!buildingElevators[i]->isNearDestination())
      {
        // if elevator direction is up, move up [3]
        if (buildingElevators[i]->isDirectionUp())
        {
          buildingElevators[i]->moveUp();
        }
        // otherwise, move down [3]
        else
        {
          buildingElevators[i]->moveDown();
        }
      }
      // otherwise it's near enough to destination to reach it in this time step... [4]
      else
      {
        // tell elevator to move to its destination floor [4]
        buildingElevators[i]->moveToDestinationFloor();
        
        // tell elevator to open its door [4]
        buildingElevators[i]->openDoor();
        // tell elevator to remove riders for its destination floor -- ignore returned vector of removed riders [4]
        buildingElevators[i]->removeRidersForDestinationFloor();
        // get a non-const pointer to the elevator's destination floor (using const_cast) [5]
        Floor* dest = &const_cast<Floor&>(((buildingElevators[i]->getDestination())));
        // if elevator is empty, choose a direction based on longest-waiting rider (the one with the smallest ID) on the floor: [5]
        if (!buildingElevators[i]->hasRiders())
        {
          // if the floor's chosen direction is up [5]          
          if(dest->isPreferredDirectionUp())
          {
            // tell elevator to set its direction to up [5]
            buildingElevators[i]->setDirectionUp();
          }
          // otherwise [5]
          else
          {
            // tell elevator to set its direction to down [5]
            buildingElevators[i]->setDirectionDown();
          }
        }
      
        // if there is space in the elevator after letting off riders, board new ones [6]
        if(buildingElevators[i]->getAvailableSpace() > 0)
        {
          // if elevator direction is up, board up-riders (if any)... [6]
          if (buildingElevators[i]->isDirectionUp())
          {
            buildingElevators[i]->addRiders(dest->removeUpRiders(buildingElevators[i]->getAvailableSpace()));
          }
          // otherwise, board down-riders (if any) [6]
          else
          {
            buildingElevators[i]->addRiders(dest->removeDownRiders(buildingElevators[i]->getAvailableSpace()));
          }
        }
        // reassess elevator destination based on its riders [8]
        buildingElevators[i]->setDestinationBasedOnRiders();
      }
    }
    // otherwise (then it already let off riders, or is in its initial state) [7]
    else
    {
      // if elevator has any riders (then they just boarded in the previous time step) [7]
      if(buildingElevators[i]->hasRiders())
      {
         // tell elevator to close its door [7]
         buildingElevators[i]->closeDoor();
      }
      // otherwise [9]
      else
      {
        // tell elevator to go idle [9]
        buildingElevators[i]->setIdle();
      }
    }        
  }
  
  // FLOOR ACTIONS [2]
  // check each floor (for waiting riders) [2]
  for (int i = 0; i < buildingFloors.size(); i++)
  {
    // if there are no riders waiting on this floor, continue with next floor [2]
    if (!buildingFloors[i]->hasRidersWaiting())
    {
      continue;
    }
    // look at each elevator to see if it needs to be sent here [2]
    for (int j = 0; j < buildingElevators.size(); j++)
    {
      // get elevator's relative location (negative if elevator is below floor) [2]
      int location = buildingElevators[j]->getLocation() - buildingFloors[i]->getLocation();
      // if this elevator's idle... [2]
      if (buildingElevators[j]->isIdle())
      {
        // if elevator is above the floor, set elevator direction to down [2]
        if (location > 0)
        {
          buildingElevators[j]->setDirectionDown();
        }
        // otherwise if it's below, set elevator direction to up [2]
        else
        {
          buildingElevators[j]->setDirectionUp();
        }
        // set elevator's destination to this floor [2]
        buildingElevators[j]->setDestination(buildingFloors[i]);
        // tell elevator to close its door [2]
        buildingElevators[j]->closeDoor();
      }


      // else if there are riders on this floor waiting to go up, and the elevator is going up... [10]
      else if (buildingFloors[i]->hasUpRiders() && buildingElevators[j]->isDirectionUp())
      {
        // get distance from elevator's destination floor to this floor (positive if elevator destination is above this floor) [10]
         int difference = buildingElevators[j]->getDestination().getLocation() - buildingFloors[i]->getLocation();
        // if elevator is below floor and elevator destination is above this floor [10]
          if (buildingElevators[j]->getLocation() < buildingFloors[i]->getLocation() && difference > 0)
          {
              // set elevator's destination to this floor [10]
              buildingElevators[j]->setDestination(buildingFloors[i]);
          }
      }
      // else if there are riders on this floor waiting to go down, and the elevator is going down... [10]
      else if(buildingFloors[i]->hasDownRiders() && buildingElevators[j]->isDirectionDown())
      {
        // get distance from elevator's destination floor to this floor (negative if elevator destination is below this floor) [10]
        int difference = buildingElevators[j]->getDestination().getLocation() - buildingFloors[i]->getLocation();
        // if elevator is above floor and elevator destination is below this floor [10]
        if (buildingElevators[j]->getLocation() > buildingFloors[i]->getLocation() && difference < 0)
        {
          // set elevator's destination to this floor [10]
          buildingElevators[j]->setDestination(buildingFloors[i]);
        }          
      }
    }
  }
  return *this;
}

Building::Building()
:simulationTime(0)
{
  Floor *_F1 = new Floor (0, "Floor One");
  buildingFloors.push_back(_F1);
  Floor *_F2 = new Floor (120, "Floor Two");
  buildingFloors.push_back(_F2);
  Floor *_F3 = new Floor (240, "Floor Three");
  buildingFloors.push_back(_F3);
  Floor *_F4 = new Floor (360, "Floor Four");
  buildingFloors.push_back(_F4);
  Floor *_F5 = new Floor (480, "Floor Five");
  buildingFloors.push_back(_F5);
  
  Elevator *_E1 = new Elevator(10, 12, *_F1);
  buildingElevators.push_back(_E1);
  Elevator *_E2 = new Elevator(10, 20, *_F5);
  buildingElevators.push_back(_E2);
  Elevator *_E3 = new Elevator(8, 16, *_F5);
  buildingElevators.push_back(_E3);
  //Elevator(const int, const int, const Floor&)// cap, givenSpeed, start
}

Building::~Building()
{
  for(int i = 0; i < buildingFloors.size(); i++)
  {
    delete buildingFloors[i];
  }
  for(int i = 0; i < buildingElevators.size(); i++)
  {
    delete buildingElevators[i];
  }
}


ostream& operator<<(ostream& output, const Building& b)
{
  output << "T: " << setw(3) << b.simulationTime << "\n";
  for (int i = 0; i < b.buildingFloors.size(); i++)
  {
    if (b.buildingFloors[i]->hasRidersWaiting())
    {
      output << *b.buildingFloors[i] << "\n";
    }
  }
  for (int i = 0; i < b.buildingElevators.size(); i++)
  {
    output << *b.buildingElevators[i];
  }
  output << endl;
  return output;
}
