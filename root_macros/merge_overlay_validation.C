// run by root -l merge_overlay_validation.C

void merge_overlay_validation.C(){
	TChain ch("CaliTTCreatorFinder/myTTree")
	TString number = "12631755"
	TString path = "/pnfs/uboone/scratch/users/apapadop/mcc8/v06_26_01_20/CalibratedSamplesCaliOverlay/"

	for (int i = 0;i<450;i++){ch.Add(path+number+"_"+std::to_string(i)+"/myMCC9FirstValidation_20k_Part1.root");}
	ch.Merge("myBNBOnly9_100k.root")
}
