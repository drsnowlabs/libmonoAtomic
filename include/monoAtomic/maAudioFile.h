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
            maAudioFile(){}
            maAudioFile(std::string _path){
                m_path = _path;
                m_fileSize = std::filesystem::file_size(m_path);
            }
            ~maAudioFile(){
                std::cout<<"DESTROYING maAudioFile"<<std::endl;
            }

            std::string filePath(){ return m_path; }
            std::string fileName(){
                return m_path.filename().string();
            }
            uint32_t nChannels(){ return m_nChannels; }
            uint32_t sampleRate(){ return m_sampleRate; }
            uint8_t bitDepth(){ return maBitDepth(m_sampleFormat); }
            uint8_t byteDepth(){ return maByteDepth(m_sampleFormat); }
            uint8_t sampleSize(){return byteDepth();} // alias function

            maSampleFormat sampleFormat(){ return m_sampleFormat; }
            std::string sampleFormatStr(){
                switch(m_sampleFormat){
                    case maSampleFormat::UNKNOWN: return "Unknown";
                    case maSampleFormat::INT8: return "8 Bit";
                    case maSampleFormat::INT16: return "16 Bit";
                    case maSampleFormat::INT24: return "24 Bit";
                    case maSampleFormat::INT32: return "32 Bit";
                    case maSampleFormat::FLOAT32: return "Float 32 Bit";
                }
            }
            size_t fileSize(){ return m_fileSize; }
            std::string fileSizeStr(){ return size2units(fileSize(), sizeUnits); }
            size_t dataSize(){ return m_dataSize; }
            std::string dataSizeStr(){ return size2units(dataSize(), sizeUnits); }
            char* data(){ return m_data; }
            size_t durationMs(){return m_durationMs; }
            size_t nSamples(){ return m_nSamples; }
            size_t nFrames(){ return m_nSamples / m_nChannels; }
            std::vector<maAudioChannel> channels(){ return m_channels; }
            maAudioFileType fileType(){return m_fileType;}
            std::string fileTypeStr(){
                switch(fileType()){
                    case maAudioFileType::UNKNOWN: return "UNKNOWN";
                    case maAudioFileType::WAVE: return "WAVE";
                    case maAudioFileType::AIFF: return "AIFF";
                    }
            }

            void print(){
                std:: cout<< "\n---=== maAudioFile ===----" <<
                "\npath:\t\t" << filePath() <<
                "\nfileName:\t" << fileName() <<
                "\nfileType:\t" << fileType() <<
                "\nfileSize:\t" << fileSizeStr() <<
                "\ndataSize:\t" << dataSize() << " bytes" <<
                "\nsampleFormat:\t" << sampleFormat() <<
                "\nbitDepth:\t" << (int)bitDepth() <<
                "\nbyteDepth:\t" << (int)byteDepth() <<
                "\nnSamples:\t" << nSamples() <<
                "\nnFrames:\t" << nFrames() <<
                "\nsampleRate:\t" << sampleRate() <<
                "\nduration (ms):\t" << durationMs() <<
                "\nnChannels:\t" << nChannels() << std::endl;

                for(maAudioChannel &ch : m_channels){
                    std::cout<< "\t["<<ch.indexInFile() << "] " << ch.label() << std::endl;
                }


                std::cout << "\n-------------------------\n" << std::endl;
            }

        protected:
            std::filesystem::path m_path;
            uint32_t m_nChannels=0;
            uint32_t m_sampleRate=0;
            maSampleFormat m_sampleFormat = maSampleFormat::UNKNOWN;
            size_t m_fileSize=0;
            size_t m_dataSize=0;
            size_t m_nSamples=0;
            size_t m_durationMs=0;
            std::vector<maAudioChannel> m_channels;
            maAudioFileType m_fileType = maAudioFileType::UNKNOWN;
            char* m_data=nullptr;

    };



} // namespace monoAtomic
#endif
