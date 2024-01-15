#ifndef MA_AUDIOCHANNEL_H
#define MA_AUDIOCHANNEL_H

namespace monoAtomic {
    
    template<typename TmaAudioFile> class maAudioChannel{
        public:
            maAudioChannel(std::string label="", int32_t indexInFile=-1, bool isEmpty=true){
                m_label = label;
                m_indexInFile = indexInFile;
                m_isEmpty = isEmpty;
            }

            void setParent(TmaAudioFile* p){
                m_parentFile = p;
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

            void setPhase(bool phase) {
                m_phase = phase;
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