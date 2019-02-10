#ifndef AnalogReader_h
#define AnalogReader_h

class AnalogReader {
  public:
    AnalogReader(float weight, int pin);
    void update(void);
    int get(void);
    byte get_byte(void);
    byte update_and_get_byte(void);
  
  private:
    float weight  = 30.0/100.0;
    float current = 0;
    int pin       = A0;
    
};

#endif