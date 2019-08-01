#ifndef myPOT_h
#define myPOT_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

//TString path = "/pnfs/uboone/persistent/users/apapadop/OverlayDirt9POTCounting/v08_00_00_17/myOverlayDirt9_POTCounting_Run1_v08_00_00_17_Part1.root";
TString path = "/uboone/data/users/kmistry/work/MCC9/test/myMCC9FirstValidation_20k_Part1.root";
// -----------------------------------------------------------------------------
class myPOT {
public:
   TTree *fChain;  //!pointer to the analyzed TTree or TChain
   Int_t fCurrent; //!current Tree number in a TChain

   // Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t POT;
   Double_t TotalPOT;

   // List of branches
   TBranch *b_POT;      //!
   TBranch *b_TotalPOT; //!

   myPOT(TTree *tree = 0);
   virtual ~myPOT();
   virtual Int_t Cut(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void Init(TTree *tree);
   virtual void Loop();
   virtual Bool_t Notify();
   virtual void Show(Long64_t entry = -1);
};

#endif
// -----------------------------------------------------------------------------
#ifdef myPOT_cxx
myPOT::myPOT(TTree *tree) : fChain(0) {
   // if parameter tree is not specified (or zero), connect the file
   // used to generate this class and read the Tree.
   if (tree == 0) {
      
      TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(path);
      
      if (!f || !f->IsOpen()) {
         f = new TFile(path);
      }

      f->GetObject("CaliTTCreatorFinder/myPOT", tree);
   
   }
   
   Init(tree);

}
// -----------------------------------------------------------------------------
myPOT::~myPOT() {
   
   if (!fChain)
      return;
   
   delete fChain->GetCurrentFile();
}
// -----------------------------------------------------------------------------
Int_t myPOT::GetEntry(Long64_t entry) {
   
   // Read contents of entry.
   if (!fChain)
      return 0;
   
   return fChain->GetEntry(entry);
}
// -----------------------------------------------------------------------------
Long64_t myPOT::LoadTree(Long64_t entry) {
   
   // Set the environment to read one entry
   if (!fChain)
      return -5;
   
   Long64_t centry = fChain->LoadTree(entry);
   
   if (centry < 0)
      return centry;
   
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   
   return centry;

}
// -----------------------------------------------------------------------------
void myPOT::Init(TTree *tree) {
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree)
      return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("POT", &POT, &b_POT);
   fChain->SetBranchAddress("TotalPOT", &TotalPOT, &b_TotalPOT);
   Notify();
}
// -----------------------------------------------------------------------------
Bool_t myPOT::Notify() {
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}
// -----------------------------------------------------------------------------
void myPOT::Show(Long64_t entry) {
   // Print contents of entry.
   // If entry is not specified, print current entry
   if (!fChain)
      return;
   fChain->Show(entry);
}
// -----------------------------------------------------------------------------
Int_t myPOT::Cut(Long64_t entry) {
   // This function may be called from Loop.
   // returns  1 if entry is accepted.
   // returns -1 otherwise.
   return 1;
}
#endif // #ifdef myPOT_cxx
// -----------------------------------------------------------------------------