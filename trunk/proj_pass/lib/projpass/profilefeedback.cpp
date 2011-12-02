#if FALSE
#include <iostream>
#include <vector>
using namespace std;
using namespace llvm;
//need Profile info
#define FT 10000
#define TT 100
#define SSST_T 10
#define PMST_T 10
#define WSST_T 10
#define MAXPREFETCHDISTANCE 8

struct loadInfo {
    int load_id;
    vector<long> top_freqs;
    int num_zero_diff;
    int profiled_stride;
    int num_strides;
    int dominant_stride;
    int trip_count;
};

set <Instruction*> SSST_loads;
set <Instruction*> PMST_loads;
set <Instruction*> WSST_loads;
map <Instruction*, loadInfo> instToInfo;

loadInfo getInfo(Instruction* inst)
{
    map <Instruction*, loadInfo>::iterator findInfo;
    findInfo = instToInfo.find(inst);
    if(findInfo == instToInfo.end()){
        errs() << "couldnt find " << *inst << " in getInfo\n";
    }
    return findInfo->second;
}


void profile(set<Instruction*>::iterator IT, set<Instruction*>::iterator ITe )
{
    int freq1;
    int num_strides;
    int top_4_freq;
    loadInfo profData;
    int zeroDiff;
    map <Instruction*, loadInfo>::iterator findInfo;
    for(; IT != ITe; IT++){
        freq1 = 0;
        num_strides = 0;
        top_4_freq = 0;
        zeroDiff = 0;

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
        freq1 = profData.freq[0]
        num_strides = profData.num_strides;
        for(unsigned int i = 0; i < profData.freq.size(); i++)
            top_4_freq+=profData.top_freqs[i];
        zeroDiff = profData.num_zero_diff;
        //cache line stuff...not sure yet?
        if(freq1/num_strides > SSST_T){
            SSST_loads.insert(*IT);
            printf("adding to SSST\n");
        }
        else if((top_4_freq/num_strides > PMST_T) && zeroDiff/num_strides > PMST_T){
            PMST_loads.insert(*IT);
            printf("adding to PMST\n");
        }
        else if((freq1/num_strides > WSST_T) && (zeroDiff/num_strides > PMST_T))
        {
            WSST_loads.insert(*IT);
            printf("adding to WSST\n");
        }
        else{
            printf("adding to none\n");
        }
    }
}
//insert an alloca to hold address
//insert an alloca to hold stride
//insert a subtract stride = addr(load) - scratch
//scratch = addr(load)
BinaryOperator *scratchAndSub(Instruction *inst)
{
    AllocaInst *scratchPtr;
    StoreInst *storePtr;
    BinaryOperator *subPtr;
    //inst->bb->fcn->module
    Module *M = Inst->getParent()->getParent()->getParent();
    BasicBlock *entryBlock = inst->getParent()->getParent()->getEntryBlock().begin();
    
    StringRef X = "scratch";
    StringRef Name = inst->getName();
    //make alloca for scratch reg
    Value *loadAddr = inst->getPointerOperand();
    allocaPtr = new AllocaInst(loadAddr->getType(), 
            X+Name, entryBlock); 

    //store[scratchPtr] = loadAddr
    storePtr = new StoreInst(scratchPtr, loadAddr, inst);
    
    //stride = addr(load) - scratch
    subPtr = BinaryOperator::Create(Instruction::Sub,
            loadAddr,
            scratchPtr,
            "stride",
            inst
            );
    return subPtr;
}

//inserts prefetch(addr(inst)+sub*K)
void insertPrefetch(Instruction *inst, double K, BinaryOperator *sub)
{
}
//insert just prefetch(P+K*S)
void insertSSST(Instruction *inst, double K)
{
    insertPrefetch(inst, K, NULL);
}

//scratch sub
//prefetch(addr(load)+K*stride) before the load, K is rounded to a power of 2, 
void insertPMST(Instruction *inst, double K)
{
    BinaryOperator *subPtr = scratchAndSub(inst);
    insertPrefetch(inst, K, subPtr);
}

//scratch sub
//p=(stride==profiled stride)
//p?prefetch(P+K*stride)
void insertWSST(Instruction *inst, double K)
{
    BinaryOperator *subPtr = scratchAndSub(inst);
    ICmpInst *ICmpPtr;
    loadInfo profData = getInfo(inst);
    int profiled_stride = profData.profiled_stride;
    ICmpPtr = new ICmpInst(inst, 
            ICmpInst::ICMP_EQ,
            subPtr,
            Constant::get(inst->getPointerOperand->getType(), profiled_stride), 
            "cmpweak");
    insertPrefetch(inst, K, subPtr);
}

void insertLoad(Instruction *inst)
{
   //determine K
   double K = 0;
   loadInfo profData = getInfo(inst);
   double dataArea = profData.dominant_stride * profData.trip_count;
   double C = MAXPREFETCHDISTANCE;
   K = min(profData.trip_count/TT, C);
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
    //this will be run on loop
    profile(inst);
    insertLoad(inst);
    return 0;
}
#endif
