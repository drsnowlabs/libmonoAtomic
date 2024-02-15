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
        info.fileType = maAudioFileType::WAVE;
        readChunks(f);
    }
    ~maAudioFileWave(){
        std::cout << "\n=== DESTROYING maAudioFileWave"<<  std::endl;
        delete[] m_data;
    }

    void readChunks(std::ifstream* f){
        std::cout << "\n=== Reading Chunks: "<< info.filePath << "=> fileSize:" << info.fileSize << std::endl;
        f->seekg (12); // skip the RIFF header

        maChunk chunkHeader; // generic chunk header: type(4) + size(4)
        maRiffFormatChunkExt format;
        size_t chunkPos = 0;
        while(true){

            chunkPos = f->tellg();

            if (f->eof() || f->bad() || chunkPos == info.fileSize){
                std::cout << "===  ["<< chunkPos << "] " << "EOF (out of bounds) ===\n\n" << std::endl;
                break;
            }


            f->read((char*)&chunkHeader, sizeof(chunkHeader));

            std::cout << "=== ["<< chunkPos << "] "   <<   std::string((char*)chunkHeader.chunkID, 4)<< ": " << chunkHeader.chunkDataSize << " bytes"<<std::endl;

            if((chunkPos+chunkHeader.chunkDataSize)>info.fileSize){
                std::cout << "Chunk size will overflow file size; " <<std::endl;
                return;

            }

            if(!memcmp("fmt", chunkHeader.chunkID, 3)) {

                uint32_t _fmt_size = sizeof(format);
                f->read((char*)&format, std::min(chunkHeader.chunkDataSize, _fmt_size));
                info.sampleRate = format.sampleRate;
                info.sampleFormat = format.formatTag==3 ? maSampleFormat::FLOAT32 : static_cast<maSampleFormat>(format.bitsPerSample);
                info.bitDepth = maBitDepth(info.sampleFormat);
                info.sampleSize = maByteDepth(info.sampleFormat);
                info.nChannels = format.nChannels;

                if(info.nChannels){
                    for(int i=0; i<info.nChannels; i++){
                        std::string channel_label = fileName()+std::string(" - ")+std::to_string(i+1);
                        maAudioChannel* ch = new  maAudioChannel(this, channel_label, i);
                        m_channels.push_back(ch);
                    }
                }

            } else if(!memcmp("data", chunkHeader.chunkID, 4)) {
                info.dataSize = chunkHeader.chunkDataSize;
                m_data = new char[info.dataSize];
                f->read(m_data, info.dataSize);
                info.nSamples = info.dataSize / info.sampleSize;
                info.nFrames = info.nSamples / info.nChannels;
                info.frameSize = info.sampleSize * info.nChannels;
                size_t duration = info.nFrames/format.sampleRate;
                info.durationMs = duration*1000.;

            } else if(!memcmp("MAWF", chunkHeader.chunkID, 4)) {
                char* _chunkData = new char[chunkHeader.chunkDataSize];
                f->read(_chunkData, chunkHeader.chunkDataSize);
                m_sessionDataJSON = std::string(_chunkData,chunkHeader.chunkDataSize);
                delete[] _chunkData;
            } else{
                // unparsed chunks
                char* _chunkData = new char[chunkHeader.chunkDataSize];
                std::cout << "read2; " <<std::endl;
                f->read(_chunkData, chunkHeader.chunkDataSize);
                delete[] _chunkData; // just to move the file cursor ahead, data not used
                // TODO PARSE METADATA CHUNKS
            }

        }
        std::cout << "done reading chunks" <<std::endl;
    }

};



} // namespace monoAtomic
#endif
