#ifndef MA_AUDIOCHANNEL_H
#define MA_AUDIOCHANNEL_H

namespace monoAtomic {

    template<typename TmaAudioFile> class maAudioChannelTemplate{
        public:
            maAudioChannelTemplate(std::string _label="uouo", int32_t _indexInFile=-1, bool _isEmpty=true){
                m_label = _label;
                m_indexInFile = _indexInFile;
                m_isEmpty = _isEmpty;
            }

            void setParentFile(TmaAudioFile* p){
                std::cout <<"setting parent file to "<< p << std::endl;
                m_parentFile = p;
            }

            TmaAudioFile* parentFile(){
                std::cout << "Getting parent file" << m_parentFile <<std::endl;
                return m_parentFile;
            }

            std::string label(){
                std::cout << "getting label" << m_label << std::endl;
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

            maSampleFormat sampleFormat(){
                if(m_parentFile)
                    return m_parentFile->sampleFormat();
                return maSampleFormat::UNKNOWN;
            }

            uint8_t sampleSize(){
                if(m_parentFile)
                    return m_parentFile->sampleSize();
                return 0;
                }

            size_t nSamples(){
                if(m_parentFile)
                    return m_parentFile->nSamples();
                return 0;
            }

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
