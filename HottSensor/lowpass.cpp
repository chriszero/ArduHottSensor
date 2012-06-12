#include "lowpass.h"

Lowpass::Lowpass(float filterVal) {
  _filterVal = filterVal;
  
  if (_filterVal > 1){      // check to make sure param's are within range
    _filterVal = .99;
  }
  else if (_filterVal <= 0){
    _filterVal = 0;
  }
}

float Lowpass::smooth(int data){
  _smoothedVal = (data * (1 - _filterVal)) + (_smoothedVal  *  _filterVal);
  return _smoothedVal;
}

float Lowpass::getFilteredValue() {
  return _smoothedVal;
}

int Lowpass::getRawValue() {
  return _rawVal;
}
