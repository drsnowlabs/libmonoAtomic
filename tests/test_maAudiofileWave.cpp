// verifying that OMP environment actually works
// compile+run with: 
// clang++ -std=c++20 -o bin/test_maAudioFileWave -I../include test_maAudioFileWave.cpp && ./bin/test_maAudioFileWave <file>

// #include <monoAtomic/formats/maAudioFileWave.h>

#include <monoAtomic/monoAtomic.h>

int main(int argc, char *argv[])
{
    std::string fpath(argv[1]);
    monoAtomic::maAudioFile* wf = monoAtomic::loadAudioFile(fpath);
    wf->print();

}