
#include "stdafx.h"
#include "Rider.h"


int Rider::IDTracker = 0;

Rider Rider::operator=(const Rider &right)
{
  const_cast<int&>(IdentificationNumber) = right.IdentificationNumber;
  const_cast<Floor*&>(destination) = const_cast<Floor*&>(right.destination);
  
  return *this;
}
