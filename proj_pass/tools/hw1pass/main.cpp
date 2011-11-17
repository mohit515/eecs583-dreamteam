#include <stdio.h>
#include <stdarg.h>
#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ProfileInfo.h"
using namespace llvm;

bool isOP(int op, int op1, ...)
{
    bool res = false;
    va_list Numbers;
    va_start (Numbers, op1);
    for (int i = op1; i >= 0; i = va_arg(Numbers, int)) {
        if (op == i) {
            res = true;
            break;
        }
    }
    va_end(Numbers); 
    return res;
}
namespace {
    struct statComp: public FunctionPass {
        static char ID;
        ProfileInfo* PI;
        statComp():FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) {
            PI = &getAnalysis<ProfileInfo>();
            double intIALU = 0;
            double intMEM = 0;
            double intBRANCH = 0;
            double intFALU = 0;
            double intOTHERS = 0;
            for(Function::iterator b = F.begin(), be = F.end(); 
                    b != be;++b) {
                for(BasicBlock::iterator i = b->begin(), ie = b->end();
                        i != ie; i++){
                    int op = i->getOpcode();
                    if(isOP(op, 8, 10, 12, 14, 15, 17, 18, 20, 21, 22, 23,
                                24, 25, 30, 31, 32, 35, 36, 39, 40, 41, 42)){
                        intIALU += PI->getExecutionCount(b);
                    }
                    else if((op >= 26) && (op <= 29)){
                        intMEM += PI->getExecutionCount(b);
                    }
                    else if((op >= 1) && (op <= 7)){
                        intBRANCH += PI->getExecutionCount(b);
                    }
                    else if(isOP(op, 9, 11, 13, 16, 19, 33, 34, 37, 38, 43)) {
                        intFALU += PI->getExecutionCount(b);
                    }
                    else
                        intOTHERS += PI->getExecutionCount(b);
                }
            }
            return false;
        }
    };
}

int main(){
    printf("hello world\n");
    return 0;
}
