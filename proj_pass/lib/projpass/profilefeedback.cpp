#include <iostream>
using namespace std;
using namespace llvm;
//need Profile info
#define FT 10000
#define TT 100
#define SSST_T 10
#define PMST_T 10
#define WSST_T 10
#define MAXPREFETCHDISTANCE 8

set <Instruction*> SSST_loads;
set <Instruction*> PMST_loads;
set <Instruction*> WSST_loads;
map <Instruction*, loadInfo> instToInfo;

instToInfo getInfo(Instruction* inst)
{
    map <Instruction*, loadInfo>::iterator findInfo;
    findInfo = instToInfo.find(inst);
    if(findInfo == instToInfo.end()){
        errs() << "couldnt find " << *inst << " in getInfo\n";
    }
    return findInfo->second;
}
struct instToInfo{
    int freq1;
    int freq2;
    int freq3;
    int freq4;
    int num_zero_diff;
    int profiledStride;
    int dominantStride;
    int tripCount;
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
//insert just prefetch(P+K*S)
void insertSSST(Instruction *inst, double K)
{
}
//insert an alloca to hold address
//insert an alloca to hold stride
//insert a subtract stride = addr(load) - scratch
//scratch = addr(load)
//prefetch(addr(load)+K*stride) before the load, K is rounded to a power of 2, 
void insertPMST(Instruction *inst, double K)
{
}
//insert an alloca to hold address
//insert an alloca to hold stride
//insert a subtract stride = addr(load) - scratch
//scratch = addr(load)
//p=(stride==profiled stride)
//p?prefetch(P+K*stride)

void insertWSST(Instruction *inst, double K)
{
}
void insertLoad(Instruction *inst)
{
   //determine K
   double K = 0;
   instInfo profData = getInfo(inst);
   double dataArea = profData.dominantStride * profData.tripCount;
   double C = MAXPREFETCHDISTANCE;
   K = min(profData.tripCount/TT, C);
   //we can incorporate cache stuff if need be
   //call the corresponding load list
   set <Instruction*>::iterator loadIT;
   if(PMST_loads.count(inst))
       insertPMST(inst,K);
   else if(SSST_loads.count(inst))
       insertSSST(inst,K);
   else if(WSST_loads.count(inst))
       insertWSST(inst, K);
   else
       errs() << "inst not inserted\n";
}
int main()
{
    profile();
    insertLoad();
    return 0;
}
