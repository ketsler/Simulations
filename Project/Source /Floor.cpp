

#include "stdafx.h"
#include "Floor.h"
#include "Rider.h"
#include <vector>
using std::vector;

#include <iostream>
using std::ostream;
using std::endl;

#include <iomanip>
using std::setw;

void Floor::addNewRider(const Rider& rider)
{
  // if added rider's destination is greater than the floor's location
  if(rider.getDestination().getLocation() > this->getLocation())
  {
    // add rider to the upRiders vector
	upRiders.push_back(rider);
  }
  // else
  else
  {
    // add rider to the downRiders vector
	downRiders.push_back(rider);
  }
}
vector<Rider> Floor::removeUpRiders(int max) // max = #of unused spaces on elevator
{
  // create an empty vector for riders to be removed
  vector<Rider> forRemoval;
  // if there are any up riders...
  if (!upRiders.empty())
  {
    // create an empty vector for riders to remain on the floor
	vector<Rider> toRemain;
    // traverse the upRiders vector
	for (int i = 0; i < upRiders.size(); i++)
	{
      // if there are still spaces left on the elevator...
	  if(max > 0)
	  {
        // add an upRider to the vector of riders to be removed
	    forRemoval.push_back(upRiders[i]);
		max--;
	  }
      // else
	  else
	  {
        // add an upRider to the vector of riders to remain on the floor
		toRemain.push_back(upRiders[i]);
	  }
	}
    // replace the upRiders vector with the vector of remaining riders
	upRiders = toRemain;
  }
  // return the vector of removed riders
  return forRemoval;
}
vector<Rider> Floor::removeDownRiders(int max)
  // like removeUpRiders, but using the downRiders vector
{
  // create an empty vector for riders to be removed
  vector<Rider> forRemoval;
  // if there are any up riders...
  if (!downRiders.empty())
  {
    // create an empty vector for riders to remain on the floor
	vector<Rider> toRemain;
    // traverse the upRiders vector
	for (int i = 0; i < downRiders.size(); i++)
	{
      // if there are still spaces left on the elevator...
	  if(max > 0)
	  {
        // add an upRider to the vector of riders to be removed
	    forRemoval.push_back(downRiders[i]);
		max--;
	  }
      // else
	  else
	  {
        // add an upRider to the vector of riders to remain on the floor
		toRemain.push_back(downRiders[i]);
	  }
	}
    // replace the upRiders vector with the vector of remaining riders
	downRiders = toRemain;
  }
  // return the vector of removed riders
  return forRemoval;
}

bool Floor::isPreferredDirectionUp() const
{
  // if there are no downRiders, return true
  if (downRiders.empty()) return true;
  // if there are no upRiders, return false
  if (upRiders.empty()) return false;
  // if the ID of the first upRider (upRider[0]) is less than that of the first downRider...
  if (upRiders[0] < downRiders[0]) return true;
    // return true
  // return false
  return false;
}

ostream& operator<<(ostream& out, const Floor& floor)// say name, location, #'s of up/down riders waiting
{  
  out << "This floor is: " << setw(12) << floor.getName();
  out << ", at: " << setw(4) << floor.getLocation();
  out << ", # going up: " << floor.getUpRiderCount();
  out << ", # going down: " << floor.getDownRiderCount();
  return out;
}