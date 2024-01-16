#ifndef MA_AUDIODEVICEPORTAUDIO_H
#define MA_AUDIODEVICEPORTAUDIO_H

#include <monoAtomic/devices/maAudioDeviceBase.h>
#include <monoAtomic/portaudio/include/portaudio.h>

namespace monoAtomic{

    class maAudioDevicePortAudio : public maAudioDeviceBase {

        public:
        maAudioDevicePortAudio() : maAudioDeviceBase(){
                PaError err = Pa_Initialize();
                if( err != paNoError ) {
                    printf( "maAudioDevicePortAudio error: %s\n", Pa_GetErrorText( err ) );
                }

                m_deviceIndex = Pa_GetDefaultOutputDevice();
                init();
                // std::cout<<"CREATING PORTAUDIO" <<std::endl;
        }

        ~maAudioDevicePortAudio(){
            PaError err = Pa_Terminate();
            if( err != paNoError )
                printf( "~maAudioDevicePortAudio error: %s\n", Pa_GetErrorText( err ) );
            // std::cout<<"DESTROYING PORTAUDIO" <<std::endl;
        }


        // void reset(AudioFormat* f){
        maReturnCode init( int32_t deviceIndex=-1, 
                            int32_t nChannels=-1, 
                            int32_t sampleRate=41100,
                            maSampleFormat sf = maSampleFormat::FLOAT32){ 
            reset(); // close the stream

            PaStreamParameters outputParameters;
            
            outputParameters.channelCount =  nChannels<0?maximumChannelCount():nChannels;
            outputParameters.device = deviceIndex<0?m_deviceIndex:deviceIndex;
            outputParameters.hostApiSpecificStreamInfo = NULL;
            outputParameters.sampleFormat =convertSampleFormat(sf); 
            outputParameters.suggestedLatency = Pa_GetDeviceInfo(m_deviceIndex)->defaultLowOutputLatency ;
            outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

            std::cout <<  ">>> Maximum Playable Channels" << outputParameters.channelCount <<std::endl;
            std::cout <<  ">>> SampleFormat" << outputParameters.sampleFormat <<std::endl;
            PaError err = Pa_OpenStream(
                &m_stream,
                nullptr, // output only
                &outputParameters,
                sampleRate,
                paFramesPerBufferUnspecified,
                paNoFlag, //flags that can be used to define dither, clip settings and more
                &maAudioDevicePortAudio::PaCallbackWrapper, //your callback function
                this ); //data to be passed to callback. In C++, it is frequently (void *)this
            if( err != paNoError ) {
                std::cout << "maAudioDevicePortAudio::init " << Pa_GetErrorText( err ) <<std::endl;
                return maReturnCode::ERROR;
            }
            m_channelCount = outputParameters.channelCount;
            Pa_SetStreamFinishedCallback(m_stream, &maAudioDevicePortAudio::PaStopCallbackWrapper);
            return maReturnCode::OK;

        }

        maReturnCode start() override {
            maReturnCode r = stop();
            if(r!= maReturnCode::OK)
                return r;
            r = resume();
            return r;
        }

        maReturnCode stop() override {
            if(Pa_IsStreamActive(m_stream)){
                m_easeFrames = -m_EASE_FRAMES;
                PaError err = Pa_StopStream( m_stream );
                m_bytesRead=0;
                if( err != paNoError ) {
                    std::cout << "maAudioDevicePortAudio::stop " << "error: "<< Pa_GetErrorText( err ) << std::endl;
                    return maReturnCode::ERROR;
                }
                setState(maPlayerState::STOPPED);
            } else{
                std::cout << "Stream is stopped" << std::endl;
            }
            return maReturnCode::OK;
        }

        maReturnCode resume() override {
            m_easeFrames = m_EASE_FRAMES;
            PaError err = Pa_StartStream( m_stream );
            if( err != paNoError ) {
                std::cout << "maAudioDevicePortAudio::resume " << "error: "<< Pa_GetErrorText( err ) << std::endl;
                return maReturnCode::ERROR;
            }
            setState(maPlayerState::ACTIVE);
            return maReturnCode::OK;
        }

        maReturnCode suspend() override {
            m_easeFrames = -m_EASE_FRAMES;
            setState(maPlayerState::SUSPENDED);
            return maReturnCode::OK;
        }

        maReturnCode reset() override {
            if(m_stream) {
                PaError err = Pa_CloseStream(m_stream);
                if( err != paNoError ) {
                    std::cout << "maAudioDevicePortAudio::reset " << "error: "<< Pa_GetErrorText( err ) << std::endl;
                    return maReturnCode::ERROR;
                }
            }
            setState(maPlayerState::IDLE);
            return maReturnCode::OK;
        }

        int32_t channelCount() override {
            return m_channelCount;
        }

        int32_t sampleRate() override {
            if(m_stream){
                const PaStreamInfo* sInfo = Pa_GetStreamInfo(m_stream);
                return sInfo->sampleRate;
            }
            return -1;
        }

        int maximumChannelCount() override {
            if(m_deviceIndex>=0){
                const   PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo( m_deviceIndex );
                return deviceInfo->maxOutputChannels;
            }
            return 0;
        }
        
        std::string description(){
            if(m_deviceIndex>=0){
                const   PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo( m_deviceIndex );
                return deviceInfo->name;
            }
            return "ERROR";
        }

        void printDeviceList(){
            int numDevices;
            const   PaDeviceInfo *deviceInfo;
            numDevices = Pa_GetDeviceCount();
            if( numDevices < 0 )
            {
                printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
                PaError err = numDevices;
            }

            std::cout << "\n🔈 Available Devices:"<< std::endl;
            for(int i=0; i<numDevices; i++ )
            {
                deviceInfo = Pa_GetDeviceInfo( i );
                std::cout<< (i==m_deviceIndex?"* ":"  " )
                        << i << deviceInfo->name
                        << "; maxChannels:" << deviceInfo->maxOutputChannels  <<std::endl;
            }
             std::cout << "\n\n" << std::endl;
        }

        int audioDeviceIndex(){
            return m_deviceIndex;
        }

        void setAudioDevice(int index){
            if(index>=0){
                m_deviceIndex = index;
                // const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(m_deviceIndex);
            }
        }
    
    protected:
    void* m_stream;
    int32_t m_deviceIndex=0;
    size_t m_bytesRead = 0;
    int32_t m_easeFrames = 0;
    int32_t m_EASE_FRAMES=10;
    int32_t m_channelCount=-1;

    PaSampleFormat convertSampleFormat(maSampleFormat f){
        switch(f){
            case maSampleFormat::INT8: return paInt8;
            case maSampleFormat::UNKNOWN:
            case maSampleFormat::INT16: return paInt16;
            case maSampleFormat::INT24: return paInt24;
            case maSampleFormat::INT32: return paInt32;
            case maSampleFormat::FLOAT32: return paFloat32;
        }
    }


    int readData(const void *input,
                 void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo* timeInfo,
                 PaStreamCallbackFlags statusFlags){

                
                return 0;
                }

      static int PaCallbackWrapper(
                const void *input, void *output,
                unsigned long frameCount,
                const PaStreamCallbackTimeInfo* timeInfo,
                PaStreamCallbackFlags statusFlags,
                void *userData )
        {
            return ((maAudioDevicePortAudio*)userData) -> readData(input, output, frameCount, timeInfo, statusFlags);
        }

        static void PaStopCallbackWrapper(void *userData){
            // ((maAudioDevicePortAudio*)userData) -> actualStop();
            // TODO
        }



    };

} // end namespace
#endif