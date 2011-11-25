/*
  Stride Profiling & Prefetching

  EECS 583 - Project
  Jason Varbedian, Patryk Mastela, Matt Viscomi

  Based off of:
    Efficient Discovery of Regular Stride Patterns in Irregular
    Programs and Its Use in Compiler Prefetching
      by Youfeng Wu
*/

#include "llvm/Transforms/Scalar.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IntrinsicInst.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/LLVMContext.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/Dominators.h"
#include "llvm/Analysis/ProfileInfo.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/SSAUpdater.h"
#include "llvm/Support/CFG.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/ADT/Statistic.h"

using namespace llvm;
using namespace std;

namespace llvm {
  void initializeStridePrefetchPass(llvm::PassRegistry&);
}

namespace {
  struct StridePrefetch : public LoopPass {

      static char ID;
      
      StridePrefetch() : LoopPass(ID) {
        initializeStridePrefetchPass(*PassRegister::getPassRegistry());
      }

      virtual bool runOnLoop(Loop *L, LPPassManager &LPM);

      /// This transformation requires natural loop information & requires that
      /// loop preheaders be inserted into the CFG...
      ///
      virtual void getAnalysisUsage(AnalysisUsage &AU) const {
        AU.addRequired<DominatorTree>();
        AU.addRequired<LoopInfo>();
        AU.addRequired<ProfileInfo>();
        AU.addRequired<AliasAnalysis>();
      }

    private:
      AliasAnalysis *AA;
      LoopInfo *LI;
      DominatorTree *DT;
      ProfileInfo *PI;
  };
}

namespace llvm {
  Pass *createStridePrefetch() {
    return new StridePrefetch();
  }
}

char StridePrefetch::ID = 0;
INITIALIZE_PASS_BEGIN(StridePrefetch, "strideprefetch", "Stride Prefetching", false, false)
INITIALIZE_PASS_DEPENDENCY(DominatorTree)
INITIALIZE_PASS_DEPENDENCY(LoopInfo)
INITIALIZE_PASS_DEPENDENCY(LoopSimplify)
INITIALIZE_AG_DEPENDENCY(AliasAnalysis)
INITIALIZE_PASS_END(StridePrefetch, "strideprefetch", "Stride Prefetching", false, false)
static RegisterPass<StridePrefetch> X("projpass", "LICM Pass", true, true);

bool StridePrefetch::runOnLoop(Loop *L, LPPassManager &LPM) {

}
