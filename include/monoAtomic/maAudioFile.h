#ifndef MA_AUDIOFILE_H
#define MA_AUDIOFILE_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maUtils.h>
#include <monoAtomic/maAudioChannel.h>
#include <filesystem>

namespace monoAtomic {

    class maAudioFile;
    typedef maAudioChannelTemplate<maAudioFile> maAudioChannel;

    class maAudioFile{
        public:
            maAudioFile(){
                std::cout<<"CREATING maAudioFile"<< this <<std::endl;

            }
            maAudioFile(std::string _path){
                info.filePath = _path;
                info.fileSize = std::filesystem::file_size(_path);
            }
            ~maAudioFile(){
                std::cout<<"DESTROYING maAudioFile"<<std::endl;
            }

            std::string fileName(){
                return info.filePath.filename().string();
            }

            std::string sampleFormatStr(){
                switch(info.sampleFormat){
                    case maSampleFormat::UNKNOWN: return "Unknown";
                    case maSampleFormat::INT8: return "8 Bit";
                    case maSampleFormat::INT16: return "16 Bit";
                    case maSampleFormat::INT24: return "24 Bit";
                    case maSampleFormat::INT32: return "32 Bit";
                    case maSampleFormat::FLOAT32: return "Float 32 Bit";
                }
            }

            std::string fileSizeStr(){ return size2units(info.fileSize, sizeUnits); }

            std::string dataSizeStr(){ return size2units(info.dataSize, sizeUnits); }
            char* data(){ return m_data; }

            std::vector<maAudioChannel*>* channels(){ return &m_channels; }
            std::string fileTypeStr(){
                switch(info.fileType){
                    case maAudioFileType::UNKNOWN: return "UNKNOWN";
                    case maAudioFileType::WAVE: return "WAVE";
                    case maAudioFileType::AIFF: return "AIFF";
                    }
            }

            void print(){
                std:: cout<< "\n---=== maAudioFile ===----" <<
                    "\nAddress" << this <<
                    "\npath:\t" << info.filePath <<
                    "\nfileName:\t" << fileName() <<
                    "\nfileType:\t" << info.fileType <<
                    "\nfileSize:\t" << fileSizeStr() <<
                    "\ndataSize:\t" << info.dataSize << " bytes" <<
                    "\nformat:\t" << info.sampleFormat <<
                    "\nbitDepth:\t" << (int)info.bitDepth <<
                    "\nsampleSize:\t" << (int)info.sampleSize <<
                    "\nnSamples:\t" << info.nSamples <<
                    "\nnFrames:\t" << info.nFrames <<
                    "\nframeSize:\t" << info.frameSize <<
                    "\nsampleRate:\t" << info.sampleRate <<
                    "\ndurationMs:\t" <<info. durationMs <<
                    "\nnChannels:\t" << info.nChannels << std::endl;
                for(maAudioChannel* ch : m_channels){
                    std::cout<< "\t["<<ch->indexInFile() << "] " << ch->label() << std::endl;
                }
                std::cout << "\n-------------------------\n" << std::endl;
            }


            maAudioInfo info;

            char* data(size_t pos){
                if(pos >= info.dataSize){
                    std::cout << "maAudioFile Error" << pos << ">=" << info.dataSize <<std::endl;;
                    return nullptr;
                }
                return &m_data[pos];
            }

            std::string sessionDataJSON(){
                return m_sessionDataJSON;
            }

            void printMetadata(){
                std::cout << "\n------ METADATA---------\n" << std::endl;
                for(maMetaChunk &chk : m_metadata){
                  std::cout << chk.uid() << std::endl;
                    chk.printFields();
                }
            }

        protected:
            // std::filesystem::path m_path;
            // maSampleFormat m_sampleFormat = maSampleFormat::UNKNOWN;
            // size_t m_nSamples=0;


            // Subclass must assign the following:


            ////////////////////////////////////////////
            std::vector<maAudioChannel*> m_channels;

            char* m_data=nullptr;
            std::string m_sessionDataJSON="";
            std::vector<maMetaChunk> m_metadata;

    };



} // namespace monoAtomic
#endif
