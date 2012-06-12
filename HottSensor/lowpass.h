#ifndef lowpass_h
#define lowpass_h

class Lowpass {
 
 public:
   Lowpass(float filterval); //  this determines smoothness  - .0001 is max  1 is off (no smoothing)
   float smooth(int data);
   float getFilteredValue();
   int getRawValue();
   
   operator float() { return _smoothedVal; }
    
 private:
   float _filterVal;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
   int _rawVal;           // for raw sensor values 
   float _smoothedVal;
 
};

#endif
