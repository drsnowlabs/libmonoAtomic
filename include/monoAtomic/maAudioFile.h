#ifndef MA_AUDIOFILE_H
#define MA_AUDIOFILE_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioChannel.h>
#include <filesystem>

namespace monoAtomic {

    class maAudioFile{
        public: 
            maAudioFile(){}
            maAudioFile(std::string path){
                m_path = path;                
                m_fileSize = std::filesystem::file_size(path);          
            }
            ~maAudioFile(){
                if(m_data){
                    delete [] m_data;
                }   
            }

            std::string path(){ return m_path; }
            uint32_t nChannels(){ return m_nChannels; }
            uint32_t sampleRate(){ return m_sampleRate; }
            uint8_t bitDepth(){ return maBitDepth(m_sampleFormat); }
            uint8_t byteDepth(){ return maByteDepth(m_sampleFormat); }
            maSampleFormat sampleFormat(){ return m_sampleFormat; }
            size_t fileSize(){ return m_fileSize; }
            size_t dataSize(){ return m_dataSize; }
            char* data(){ return m_data; }
            size_t durationMs(){return m_durationMs; }
            size_t nSamples(){ return m_nSamples; }
            size_t nFrames(){ return m_nSamples / m_nChannels; }
            std::vector<maAudioChannel<maAudioFile>> channels(){ return m_channels; }
            maAudioFileType fileType(){return m_fileType;}
            void print(){
                std:: cout<< "\n---=== maAudioFile ===----" <<
                "\npath:\t\t" << path() << 
                "\nfileType:\t" << fileType() << 
                "\nfileSize:\t" << fileSize() << " bytes" << 
                "\ndataSize:\t" << dataSize() << " bytes" << 
                "\nsampleFormat:\t" << sampleFormat() <<
                "\nbitDepth:\t" << (int)bitDepth() <<
                "\nbyteDepth:\t" << (int)byteDepth() <<
                "\nnSamples:\t" << nSamples() <<
                "\nnFrames:\t" << nFrames() <<
                "\nsampleRate:\t" << sampleRate() <<
                "\nduration (ms):\t" << durationMs() <<
                "\nnChannels:\t" << nChannels() << std::endl;

                for(maAudioChannel<maAudioFile> &ch : m_channels){
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
            std::vector<maAudioChannel<maAudioFile>> m_channels;
            maAudioFileType m_fileType = maAudioFileType::UNKNOWN;
            char* m_data = nullptr;

    };



} // namespace monoAtomic 
#endif