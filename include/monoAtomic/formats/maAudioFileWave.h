#ifndef MA_AUDIOFILEWAVE_H
#define MA_AUDIOFILEWAVE_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioFile.h>
#include <algorithm>
#include <monoAtomic/maAudioChannel.h>

namespace monoAtomic {

    class maAudioFileWave : public maAudioFile {
        public: 
        
            maAudioFileWave(std::string path, std::ifstream* f) : maAudioFile(path) {
                m_fileType = maAudioFileType::WAVE;
                readChunks(f);                
            }
            ~maAudioFileWave(){
                std::cout << "\n=== DESTROYING maAudioFileWave"<<  std::endl;

            }

            void readChunks(std::ifstream* f){
                std::cout << "\n=== Reading Chunks: "<< filePath() << "===" << std::endl;
                f->seekg (12); // skip the RIFF header
                
                maChunk chunkHeader; // generic chunk header: type(4) + size(4)
                maRiffFormatChunkExt format;

                while(true){
                     if (f->eof() || f->bad()){
                        std::cout << "=== EOF (out of bounds) ===\n\n" << std::endl;
                        break;
                     }

                    f->read((char*)&chunkHeader, sizeof(chunkHeader));

                    std::cout << "=== "<<  std::string((char*)chunkHeader.chunkID, 4)<< " " << chunkHeader.chunkDataSize << std::endl;
                    if(!memcmp("fmt", chunkHeader.chunkID, 3)) {
                        
                        uint32_t _fmt_size = sizeof(format);
                        f->read((char*)&format, std::min(chunkHeader.chunkDataSize, _fmt_size));
                        m_nChannels = format.nChannels;
                        m_sampleRate = format.sampleRate;
                        m_sampleFormat = format.formatTag==3 ? maSampleFormat::FLOAT32 : static_cast<maSampleFormat>(format.bitsPerSample);

                        if(format.nChannels){
                            for(int i=0; i<format.nChannels; i++){
                                std::string channel_label = fileName()+std::string(" - ")+std::to_string(i+1);
                                maAudioChannel ch(channel_label, i);
                                ch.setParentFile(this);
                                m_channels.push_back((maAudioChannel)ch);
                            }
                        }

                    } else if(!memcmp("data", chunkHeader.chunkID, 4)) {
                        m_dataSize = chunkHeader.chunkDataSize;
                        m_data = new char[m_dataSize];
                        f->read(m_data, m_dataSize);
                        m_nSamples = m_dataSize / byteDepth();
                        size_t duration = nFrames()/format.sampleRate;
                        m_durationMs = duration*1000.;
                    
                    } else{
                        // unparsed chunks
                        char _chunkData[chunkHeader.chunkDataSize];
                        f->read(_chunkData, chunkHeader.chunkDataSize);    

                        // TODO PARSE METADATA CHUNKS
                    }


                    // char chunkData[chunkHeader.chunkDataSize];
                    // f->read(chunkData, chunkHeader.chunkDataSize);
                    // parseChunk(&chunkHeader, chunkData);
                }
                std::cout << "done reading chunks" <<std::endl;
            }
          
    };



} // namespace monoAtomic 
#endif
