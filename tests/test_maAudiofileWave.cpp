// verifying that OMP environment actually works
// compile+run with: 
// clang++ -std=c++20 -o bin/test_maAudioFileWave -I../include test_maAudioFileWave.cpp && ./bin/test_maAudioFileWave <file>

// #include <monoAtomic/formats/maAudioFileWave.h>

#include <monoAtomic/monoAtomic.h>

#include <valarray>

int main(int argc, char *argv[])
{
    std::string fpath(argv[1]);
    monoAtomic::maAudioFile* wf = monoAtomic::loadAudioFile(fpath);
    wf->print();
    delete wf;


    //  std::valarray<int> foo (12);
    // for (int i=0; i<12; ++i) foo[i]=i*10;
    // std::valarray<int> bar = foo[std::slice(0,4,4)];
    // for(int &x : bar)
    //     std::cout << x << std::endl;
}