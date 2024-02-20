#ifndef MA_AUDIOCHANNEL_H
#define MA_AUDIOCHANNEL_H

#include <cstdint>
#include <monoAtomic/maExceptions.h>

namespace monoAtomic {

    template<typename TmaAudioFile> class maAudioChannelTemplate{
        public:
            maAudioChannelTemplate(TmaAudioFile* f=nullptr, std::string _label="Audio Channel", int32_t _indexInFile=-1){
                m_label = _label;
                m_indexInFile = _indexInFile;
                m_isEmpty = (f==nullptr);
                m_parentFile = f;
                if(f){
                    info = &(f->info);
                } else {
                    info = new maAudioInfo();
                }
            }

            ~maAudioChannelTemplate(){
                if(info && (m_parentFile && &(m_parentFile->info)!= info) )
                    delete info;
            }

            void setParentFile(TmaAudioFile* p){
                // std::cout <<"setting parent file to "<< p << std::endl;
                m_parentFile = p;
            }

            TmaAudioFile* parentFile(){
                return m_parentFile;
            }

            std::string label(){
                return m_label;
            }

            void setLabel(std::string l){
                m_label = l;
            }

            bool phase(){
                return m_phase;
            }

            void setPhase(bool _phase) {
                m_phase = _phase;
            }

            bool isEmpty(){
                return m_isEmpty;
            }

            void setEmpty(bool empty){
                m_isEmpty = empty;
            }

            maChannelPlaybackState playbackState(){
                return m_playbackState;
            }

            void setPlaybackState(maChannelPlaybackState s){
                m_playbackState = s;
            }

            int32_t indexInFile(){
                return m_indexInFile;
            }


            char* samplePtr(size_t iSample){
                if(!m_parentFile){
                    throw maOrphanChannel();
                }

                if(iSample >= m_parentFile->info.nFrames){
                    throw maSampleOutOfRange();
                }

                size_t pos = (iSample*info->frameSize) + (m_indexInFile*info->sampleSize);

                char* s = m_parentFile->data(pos);
                return s;
            }

            float sampleF(size_t iSample){

                char* s = samplePtr(iSample);

                if(s)
                    return anyToFloatNorm(s, info->sampleFormat);
                std::cout<< "you sould not see this. Error getting sample "<< iSample << std::endl;
                return 0.0;
            }


            maAudioInfo* info;

        private:

            TmaAudioFile* m_parentFile=nullptr;
            std::string m_label;
            int32_t m_indexInFile=-1;
            bool m_isEmpty = true;
            bool m_phase=false;
            maChannelPlaybackState m_playbackState = maChannelPlaybackState::Active;
    };



} // namespace monoAtomic
#endif






