#ifndef MONOATOMIC_H
#define MONOATOMIC_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioFile.h>
#include <monoAtomic/formats/maAudioFileWave.h>

namespace monoAtomic {
    
    
    static maAudioFile* loadAudioFile(std::string fpath){   
        // "Factory" method to load audio files         
        std::ifstream infile(fpath, std::ifstream::binary);
        if (infile.fail()) {
            // Get the error code
            std::ios_base::iostate state = infile.rdstate();

            // Check for specific error bits
            if (state & std::ios_base::eofbit) {
                std::cout << "End of file reached." << std::endl;
            }
            if (state & std::ios_base::failbit) {
                std::cout << "Non-fatal I/O error occurred." << std::endl;
            }
            if (state & std::ios_base::badbit) {
                std::cout << "Fatal I/O error occurred." << std::endl;
            }

            // Print system error message
            std::perror("Error");
            return nullptr;
        }
            
        maRiffHeader hdr;
        infile.read((char*)&hdr, sizeof(hdr));
        infile.seekg (0, infile.beg);

        if(!memcmp("RIFF", hdr.chunkID, 4)) { // Is a RIFF-based files?
            // std::cout << "Riff File" << std::endl;
            if(!memcmp("WAVE", hdr.riffTypeID, 4)) {
                // std::cout << "Wave File" << std::endl;
                return new maAudioFileWave(fpath, &infile);
            } else {
                infile.close();
                return new maAudioFile();
            }
        } else {
            infile.close();
            return new maAudioFile();
        }
        
        infile.close();
        return nullptr;
    }


} // namespace monoAtomic



#endif

           