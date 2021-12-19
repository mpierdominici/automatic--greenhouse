

#ifndef MEM_EDGE_DETECTOR_H
#define MEM_EDGE_DETECTOR_H

class memEdgeDetector
{
  public:
  memEdgeDetector();
  bool risingEdge(void);
  bool fallingEdge(void);
  void updateMme(bool newData);
  bool getState(void);
  bool currentData;
  private:
  bool newData;
  
  bool isRising;
  bool isFalling;
};


#endif
