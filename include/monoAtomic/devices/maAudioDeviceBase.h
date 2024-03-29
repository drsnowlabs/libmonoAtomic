#ifndef MA_AUDIODEVICEBASE_H
#define MA_AUDIODEVICEBASE_H

#include <monoAtomic/maDefs.h>

namespace monoAtomic{

    class maAudioDeviceBase {

        public:
            maAudioDeviceBase(){}
            maSampleFormat sampleFormat(){ return m_info.sampleFormat; }
            maSampleFormat setSampleFormat(maSampleFormat sf){ m_info.sampleFormat = sf; return m_info.sampleFormat;}
            uint8_t bitDepth(){ return maBitDepth(m_info.sampleFormat); }
            uint8_t byteDepth(){ return maByteDepth(m_info.sampleFormat); }
            float volume(){ return m_volume;}
            float setVolume(float vol){
                if(vol<VOLUME_MIN)
                    m_volume = VOLUME_MIN;
                else if(vol>VOLUME_MAX)
                    m_volume = VOLUME_MAX;
                else
                    m_volume = vol;
                return m_volume;
            }

            maPlayerState state(){ return m_state;}
            maPlayerState setState(const maPlayerState newState){m_state = newState; return m_state;}

            maPlayerInfo info(){
                return  m_info;
            }

            int32_t sampleRate(){ return m_info.sampleRate;};

            void printInfo(){
                std::cout << "\n🔈 "<< m_info.deviceIndex <<" " << name() << ":"<< std::endl;
                std::cout << "nChannels: "<< m_info.nChannels << std::endl;
                std::cout << "sampleFormat: "<< m_info.sampleFormat << std::endl;
                std::cout << "sampleRate: "<< m_info.sampleRate << std::endl;
            }
            virtual maReturnCode start() = 0;
            virtual maReturnCode stop() = 0;
            virtual maReturnCode resume() = 0;
            virtual maReturnCode suspend() = 0;
            virtual maReturnCode reset() = 0;
            virtual int32_t nChannels() = 0; // the current channel count the device is set up
            virtual int32_t maximumChannelCount() = 0; // maximum channel count device is capable
            // virtual int32_t sampleRate() = 0;
            virtual std::string name() = 0;

        protected:
            float m_volume=1.0;
            float VOLUME_MIN = 0.0;
            float VOLUME_MAX = 1.0;
            maPlayerState m_state = maPlayerState::UNKNOWN;
            maPlayerInfo m_info;
    };

} // namespace monoAtomic
#endif
