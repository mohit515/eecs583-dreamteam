/*
  Each load should have an instance and tell the address it loads.
  Then this can be used to calculate the top stride values and stride differences...
*/

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

int LoadStride::TOPCOUNT = 5; // number of top stride values to return

LoadStride::LoadStride() {
  strideZeroCount = 0;
  strideZeroDifferenceCount = 0;
}

LoadStride::~LoadStride() {
}

void LoadStride::updateTopStrideValues(long value) {
  long count = strideValuesToCount[value];
  if (topStrideValues.size() < LoadStride::TOPCOUNT) {
    topStrideValues.push_back(make_pair(value, count));
    return;
  }
  map<long, long>::iterator itStart, itEnd;
  long lowest_found = (topStrideValues.begin())->second;
  int location = 0; 
  for (int i = 1; i < topStrideValues.size(); i++) {
    if (topStrideValues[i].second < lowest_found) {
      lowest_found = topStrideValues[i].second;
      location = i;
    }
  }
  if (count > lowest_found) {
    topStrideValues[location] = make_pair(value, count);
  }
}

void LoadStride::updateTopStrideDifferenceValues(long value) {
  long count = strideDifferencesToCount[value];
  if (topStrideDifferenceValues.size() < LoadStride::TOPCOUNT) {
    topStrideDifferenceValues.push_back(make_pair(value, count));
    return;
  }
  map<long, long>::iterator itStart, itEnd;
  long lowest_found = (topStrideDifferenceValues.begin())->second;
  int location = 0; 
  for (int i = 1; i < topStrideDifferenceValues.size(); i++) {
    if (topStrideDifferenceValues[i].second < lowest_found) {
      lowest_found = topStrideDifferenceValues[i].second;
      location = i;
    }
  }
  if (count > lowest_found) {
    topStrideDifferenceValues[location] = make_pair(value, count);
  }
}

void LoadStride::addAddress(unsigned long addr) {
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
  updateTopStrideValues(stride);

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
  updateTopStrideDifferenceValues(strideDifference);
}


