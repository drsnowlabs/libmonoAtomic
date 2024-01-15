#ifndef MA_AUDIOFILEWAVE_H
#define MA_AUDIOFILEWAVE_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioFile.h>


namespace monoAtomic {

    class maAudioFileWave : public maAudioFile {
        public: 
        
            maAudioFileWave(std::string path, std::ifstream* f) : maAudioFile(path) {
                m_fileType = maAudioFileType::WAVE;
                readChunks();                
            }

            void readChunks(){
                std::cout << "Reading Chunks" << std::endl;
            }
          
    };



} // namespace monoAtomic 
#endif