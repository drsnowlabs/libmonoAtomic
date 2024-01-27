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
                std::cout << "\n=== Reading Chunks: "<< info.filePath << "===" << std::endl;
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
                        // m_nChannels = format.nChannels;
                       info.sampleRate = format.sampleRate;
                       info.sampleFormat = format.formatTag==3 ? maSampleFormat::FLOAT32 : static_cast<maSampleFormat>(format.bitsPerSample);
                       info.bitDepth = maBitDepth(info.sampleFormat);
                       info.sampleSize = maByteDepth(info.sampleFormat);
                       info.nChannels = format.nChannels;

                        if(info.nChannels){
                            for(int i=0; i<info.nChannels; i++){
                                std::string channel_label = fileName()+std::string(" - ")+std::to_string(i+1);
                                maAudioChannel* ch = new  maAudioChannel(channel_label, i);
                                ch->setParentFile(this);
                                ch->setEmpty(false);
                                ch->info = &info;
                                m_channels.push_back(ch);
                            }
                        }

                    } else if(!memcmp("data", chunkHeader.chunkID, 4)) {
                        info.dataSize = chunkHeader.chunkDataSize;
                        m_data = new char[info.dataSize];
                        f->read(m_data, info.dataSize);
                        info.nSamples = info.dataSize / info.sampleSize;
                        size_t duration = info.nFrames/format.sampleRate;
                        info.durationMs = duration*1000.;
                        info.nFrames = info.nSamples / info.nChannels;
                        info.frameSize = info.sampleSize * info.nChannels;

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
