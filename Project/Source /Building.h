
#ifndef BUILDING_H
#define BUILDING_H

#include <iostream>
using std::ostream;

#include <vector>
using std::vector;

class Floor;
class Elevator;

class Building
{
  int simulationTime;
  vector<Floor*> buildingFloors;
  vector<Elevator*> buildingElevators;
public:
  Building& step(int);
  int getFloorCount() const {return buildingFloors.size();}// return #of floors in the vector of Floor*'s
  int getElevatorCount() const {return buildingElevators.size();}// return #of elevators in the vector of Elevator*'s
  const Floor& getFloor(int index) const {return *buildingFloors[index];}// return a reference to the "indexth" floor
  const Elevator& getElevator(int index) const {return *buildingElevators[index];}// return a reference to the "indexth" elevator
  Building();
  ~Building();
  friend ostream& operator<<(ostream&, const Building&); 
};
#endif