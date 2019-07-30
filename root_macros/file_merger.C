void file_merger(const char* filename){
    std::ifstream fileIN; 

    
    fileIN.open(filename); // Open the file
    
    if (!fileIN.good()) {  // Check if the file opened correctly
        std::cerr << "Error: file:\t" << filename <<"\tcould not be opened" << std::endl;
        exit(1);
    }

    
    TChain ch("CaliTTCreatorFinder/myTTree");

    std::string paths;

    if (fileIN.is_open()) { 
        
        // loop over lines in file
        while ( fileIN >> paths) {  
            ch.Add(paths.c_str());
            std::cout << paths << std::endl;
        }

        ch.Merge("myBNBOnly 9_100k.root");
        
        fileIN.close();
    }


}