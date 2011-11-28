#ifndef LOADSTRIDE_H
#define LOADSTRIDE_H
#include <vector>
#include <utility>
#include <map>
using namespace std;
class LoadStride {

  public:
    LoadStride();
    ~LoadStride();

    void addAddress(unsigned long);
    
    unsigned long getStrideZeroCount() {
      return strideZeroCount;
    }
    
    unsigned long getStrideZeroDifferenceCount() {
      return strideZeroDifferenceCount;
    }

    vector< pair<long, long> > *getTopStrideValues() {
      return &topStrideValues;
    }

    vector< pair<long, long> > *getTopStrideDifferenceValues() {
      return &topStrideDifferenceValues;
    }
 
    void updateTopStrideValues(long);
    void updateTopStrideDifferenceValues(long);

  private:
    static int TOPCOUNT;

    vector<unsigned long> addresses;
    vector<long> strideValues;
    vector<long> strideDifferences;
    map<long, long> strideValuesToCount;
    map<long, long> strideDifferencesToCount;
    unsigned long strideZeroCount;
    unsigned long strideZeroDifferenceCount;

    vector< pair<long, long> > topStrideValues;
    vector< pair<long, long> > topStrideDifferenceValues;
};
#endif
