#ifndef MA_AUDIOCHANNEL_H
#define MA_AUDIOCHANNEL_H

#include <cstdint>
namespace monoAtomic {

    template<typename TmaAudioFile> class maAudioChannelTemplate{
        public:
            maAudioChannelTemplate(std::string _label="Audio Channel", int32_t _indexInFile=-1, bool _isEmpty=true){
                m_label = _label;
                m_indexInFile = _indexInFile;
                m_isEmpty = _isEmpty;
            }

            void setParentFile(TmaAudioFile* p){
                std::cout <<"setting parent file to "<< p << std::endl;
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

            float sampleF(size_t iSample){

                if(!m_parentFile)
                    return 0.0;

                size_t pos = (m_indexInFile*info->sampleSize) +
                             (iSample*info->frameSize);
                char* s = m_parentFile->sample(pos);
                if(s)
                    return anyToFloatNorm(s, info->sampleFormat);
                // std::cout<< "you sould not see this" << std::endl;
                return 0.0;
            }

            maAudioInfo* info;

        private:
            TmaAudioFile* m_parentFile;
            std::string m_label;
            int32_t m_indexInFile=-1;
            bool m_isEmpty = true;
            bool m_phase=false;
            maChannelPlaybackState m_playbackState = maChannelPlaybackState::Active;
    };



} // namespace monoAtomic
#endif






