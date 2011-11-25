/*
  Each load should have an instance and tell the address it loads.
  Then this can be used to calculate the top stride values and stride differences...
*/

#include <vector>

using namespace std;

class LoadStride {

  public:
    LoadStride();
    ~LoadStride();

    addAddress(unsigned long);
    
    unsigned long getStrideZeroCount() {
      return strideZeroCount;
    }
    
    unsigned long getStrideZeroDifferenceCount() {
      return strideZeroDifferenceCount;
    }

    long *getTopStrideValues() {
      return topStrideValues;
    }

    long *getTopStrideDifferenceValues() {
      return topStrideDifferenceValues;
    }
    
  private:
    vector<unsigned long> addresses;
    vector<long> strideValues;
    vector<long> strideDifferences;
    map<long, long> strideValuesToCount;
    map<long, long> strideDifferencesToCount;
    unsigned long strideZeroCount;
    unsigned long strideZeroDifferenceCount;

    long topStrideValues[5];
    long topStrideDifferenceValues[5];
}

LoadStride::LoadStride() {
  strideZeroCount = 0;
  strideZeroDifferenceCount = 0;
}

LoadStride::~LoadStride() {
}

void addAddress(unsigned long addr) {
  if (addresses.size() == 0) {
    addresses.push_back(addr);
    return;
  }
  
  unsigned long last_address = addresses.back();
  addresses.push_back(addr);
  
  long stride = addr - last_address;
  if (stride == 0) {
    strideZeroCount++;
  }

  if (strideValues.size() == 0) {
    strideValues.push_back(stride);
    return;
  }

  long last_stride = strideValues.back();
  strideValues.push_back(stride);
  if (strideValuesToCount.count(stride)) {
    strideValuesToCount[stride]++;
  } else {
    strideValuesToCount[stride] = 1;
  }

  long strideDifference = stride - last_stride;
  if (strideDifference == 0) {
    strideZeroDifferenceCount++;
  }
  strideDifferences.push_back(strideDifference);
  if (strideDifferencesToCount.count(strideDifference)) {
    strideDifferencesToCount[strideDifference]++;
  } else {
    strideDifferencesToCount[strideDifference] = 1;
  }
}


