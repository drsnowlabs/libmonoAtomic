#ifndef MA_AUDIOFILE_H
#define MA_AUDIOFILE_H

#include <monoAtomic/maDefs.h>
#include <monoAtomic/maAudioChannel.h>

namespace monoAtomic {

    class maAudioFile{
        public: 
            maAudioFile(){}
            maAudioFile(std::string path){m_path = path;}
            std::string path(){ return m_path; }
            uint32_t nChannels(){ return m_nChannels; }
            uint32_t sampleRate(){ return m_sampleRate; }
            uint8_t bitDepth(){ return maBitDepth(m_sampleFormat); }
            uint8_t byteDepth(){ return maByteDepth(m_sampleFormat); }
            maSampleFormat sampleFormat(){ return m_sampleFormat; }
            size_t size(){ return m_size; }
            size_t durationMs(){return m_durationMs; }
            size_t nSamples(){ return m_nSamples; }
            std::vector<maAudioChannel*> channels(){ return m_channels; }
            maAudioFileType fileType(){return m_fileType;}
            void print(){
                std:: cout<< "\n---=== maAudioFile ===----" <<
                "\npath:\t\t" << path() << 
                "\nfileType:\t" << fileType() << 
                "\nsize:\t\t" << size() << " bytes" << 
                "\nnSamples:\t" << nSamples() <<
                "\nnChannels:\t" << nChannels() <<
                "\nsampleRate:\t" << sampleRate() <<
                "\nsampleFormat:\t" << sampleFormat() <<
                "\nbitDepth:\t" << (int)bitDepth() <<
                "\nbyteDepth:\t" << (int)byteDepth() <<
                "\n-------------------------\n" <<
                std::endl;
            }

        protected:
            std::string m_path="";
            uint32_t m_nChannels=0;
            uint32_t m_sampleRate=0;
            maSampleFormat m_sampleFormat = maSampleFormat::UNKNOWN;
            size_t m_size=0;
            size_t m_nSamples=0;
            size_t m_durationMs=0;
            std::vector<maAudioChannel*> m_channels;
            maAudioFileType m_fileType = maAudioFileType::UNKNOWN;
            

    };



} // namespace monoAtomic 
#endif