
#ifndef RIDER_H
#define RIDER_H


#include <iostream>
using std::cout;
using std::endl;

class Floor;

class Rider
{
public:
  Rider(const Floor& input) : destination(&input), IdentificationNumber(IDTracker){IDTracker++;}
  bool operator<(const Rider& input) const {return ((this->IdentificationNumber) < (input.IdentificationNumber));}
  bool const operator==(const Rider& input) const {return ((this->IdentificationNumber) == (input.IdentificationNumber));}
  const Floor& getDestination() const{return (*this->destination);}
  Rider operator=(const Rider&);
private:
  const int IdentificationNumber;
  const Floor* const destination;
  static int IDTracker;
};

#endif