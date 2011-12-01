#include <iostream>
using namespace std;
using namespace llvm;
//need Profile info
#define FT 10000
#define TT 100
#define SSST_T 10
#define PMST_T 10
#define WSST_T 10

set <Instruction*> SSST_loads;
set <Instruction*> PMST_loads;
set <Instruction*> WSST_loads;
map <Instruction*, loadInfo> instToInfo;

struct instToInfo{
    int freq1;
    int freq2;
    int freq3;
    int freq4;
    int num_zero_diff;
};

void profile(set<Instruction*>::iterator IT, set<Instruction*>::iterator ITe )
{
    int freq1;
    int totalFreq;
    instToInfo profData;
    int zeroDiff;
    map <Instruction*, loadInfo>::iterator findInfo;
    for(; IT != ITe; IT++){
        findInfo = instToInfo.find(*IT);
        if(findInfo == instToInfo.end()){
            printf("couldn't find instruction\n");
            errs() << *IT << "\n";
        }
        profData = findInfo->second;
        if(PI->getExecutionCount(IT->getParent()) =< FT)
            continue;
        //assume that loads passed in are in loops
        if(PI->getExecutionCount(Preheader) =< TT)
            continue;
        freq1 = profData.freq1;
        totalFreq = profData.freq1+profData.freq2+profData.freq3+profData.freq4;
        zeroDiff = instToInfo.num_zero_diff;
        //cache line stuff...not sure yet?
        if(freq1/totalfreq > SSST_T){
            SSST_loads.insert(*IT);
            printf("adding to SSST\n");
        }
        else if((profData.freq4/totalFreq > PMST_T) && zeroDIFF/totalFreq > PMST_T){
            PMST_loads.insert(*IT);
            printf("adding to PMST\n");
        }
        else if((freq1/totalFreq > WSST_T) && (zeroDiff/totalFreq > PMST_T))
        {
            WSST_loads.insert(*IT);
            printf("adding to WSST\n");
        }
        else{
            printf("adding to none\n");
        }
    }
}
void insertLoad()
{

}
int main()
{
    profile();
    insertLoad();
    return 0;
}
