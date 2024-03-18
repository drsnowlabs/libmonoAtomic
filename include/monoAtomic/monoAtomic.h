#ifndef MONOATOMIC_H
#define MONOATOMIC_H

#include <monoAtomic/maExceptions.h>
#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioFile.h>
#include <monoAtomic/formats/maAudioFileWave.h>
#include <monoAtomic/devices/maAudioDevicePortAudio.h>

namespace monoAtomic {





class maObjectManager {
public:
    static maObjectManager& get() {
        static maObjectManager instance;
        return instance;
    }

    const std::vector<maAudioFile*> audioFiles() const { return m_audioFileStore; }
    void addAudioFile(maAudioFile* f) {
                    std::cout << "maObjectManager: Adding "<< f << std::endl;
                    m_audioFileStore.push_back(f);
     }

private:
    maObjectManager() = default;
    ~maObjectManager(){
        std::cout << "\n=== DESTROYING maObjectManager"<< this << std::endl;
        std::cout << "Cleaning up" << m_audioFileStore.size() << " file(s)"<< std::endl;
        for(maAudioFile* f : m_audioFileStore){
            if(f){
                std::cout << "Deleting "<< f  << std::endl;
                delete f;
            } else {
                std::cout << "Cannot delete "<< f << std::endl;
            }

        }
    }
    std::vector<maAudioFile*> m_audioFileStore;
};



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
        std::cout<< "Error" << std::endl;
        // return maAudioFile();
        return nullptr;
    }

    maRiffHeader hdr;
    infile.read((char*)&hdr, sizeof(hdr));
    infile.seekg (0, infile.beg);

    if(!memcmp("RIFF", hdr.chunkID, 4)) { // Is a RIFF-based files?
        // std::cout << "Riff File" << std::endl;
        if(!memcmp("WAVE", hdr.riffTypeID, 4)) {
            // std::cout << "Wave File" << std::endl;
            maAudioFileWave* f = new maAudioFileWave(fpath, &infile);

            maObjectManager::get().addAudioFile(f);

            return f;
        } else {
            infile.close();
            return nullptr;
            // return maAudioFile();
        }
    } else {
        infile.close();
        return nullptr;
        // return maAudioFile();
    }

    infile.close();

    return nullptr;
    // return maAudioFile();
}


} // namespace monoAtomic



#endif

