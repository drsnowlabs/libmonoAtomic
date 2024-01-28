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

        m_info.deviceIndex = Pa_GetDefaultOutputDevice();
        init(m_info);
        // std::cout<<"CREATING PORTAUDIO" <<std::endl;
    }

    ~maAudioDevicePortAudio(){
        PaError err = Pa_Terminate();
        if( err != paNoError )
            printf( "~maAudioDevicePortAudio error: %s\n", Pa_GetErrorText( err ) );
        // std::cout<<"DESTROYING PORTAUDIO" <<std::endl;
    }


    // void reset(AudioFormat* f){
    maReturnCode init(maPlayerInfo info){
        reset(); // close the stream

        PaStreamParameters outputParameters;

        outputParameters.channelCount =  info.nChannels<0 ? maximumChannelCount(): info.nChannels;
        outputParameters.device = info.deviceIndex<0 ? m_info.deviceIndex : info.deviceIndex;
        outputParameters.hostApiSpecificStreamInfo = NULL;
        outputParameters.sampleFormat = convertSampleFormat(info.sampleFormat);
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(m_info.deviceIndex)->defaultLowOutputLatency ;
        outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

        m_info = info;
        // update m_info based on the logic above
        m_info.nChannels = outputParameters.channelCount;
        m_info.deviceIndex = outputParameters.device;
        m_info.sampleFormat = info.sampleFormat;

        // std::cout <<  ">>> Maximum Playable Channels" << outputParameters.channelCount <<std::endl;
        // std::cout <<  ">>> SampleFormat" << outputParameters.sampleFormat <<std::endl;

        PaError err = Pa_OpenStream(
            &m_stream,
            nullptr, // output only
            &outputParameters,
            m_info.sampleRate,
            m_info.framesPerBuffer? m_info.framesPerBuffer : paFramesPerBufferUnspecified,
            paNoFlag, //flags that can be used to define dither, clip settings and more
            &maAudioDevicePortAudio::PaCallbackWrapper, //your callback function
            this ); //data to be passed to callback. In C++, it is frequently (void *)this
        if( err != paNoError ) {
            std::cout << "maAudioDevicePortAudio::init " << Pa_GetErrorText( err ) <<std::endl;
            return maReturnCode::ERROR;
        }

        Pa_SetStreamFinishedCallback(m_stream, &maAudioDevicePortAudio::PaStopCallbackWrapper);

        printInfo();

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

    int32_t nChannels() override {
        return m_info.nChannels;
    }

    int32_t sampleRate() override {
        if(m_stream){
            const PaStreamInfo* sInfo = Pa_GetStreamInfo(m_stream);
            return sInfo->sampleRate;
        }
        return -1;
    }

    int maximumChannelCount() override {
        if(m_info.deviceIndex>=0){
            const   PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo( m_info.deviceIndex );
            return deviceInfo->maxOutputChannels;
        }
        return 0;
    }

    std::string name() override{
        if(m_info.deviceIndex>=0){
            const   PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo( m_info.deviceIndex );
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
            std::cout<< (i==m_info.deviceIndex?"* ":"  " )
                      << i << deviceInfo->name
                      << "; maxChannels:" << deviceInfo->maxOutputChannels  <<std::endl;
        }
        std::cout << "\n\n" << std::endl;
    }

    int audioDeviceIndex(){
        return m_info.deviceIndex;
    }

    void setAudioDevice(int index){
        if(index>=0){
            m_info.deviceIndex = index;
            // const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(m_deviceIndex);
        }
    }



protected:
    void* m_stream;

    // int32_t m_deviceIndex=0;
    // int32_t m_channelCount=-1;
    size_t m_bytesRead = 0;
    int32_t m_easeFrames = 0;
    int32_t m_EASE_FRAMES=10;

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


    size_t fillBuffer(const void *input,
                      void *output,
                      unsigned long frameCount){

        size_t bytesRead = 0;

        // TODO - make it virtual: subclass should implement this

        return bytesRead;
    }

    int readData(const void *input,
                 void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo* timeInfo,
                 PaStreamCallbackFlags statusFlags){

        size_t bytesRequested = 0; //TODO
        size_t bytesRead = fillBuffer(input, output, frameCount);
        bool endOfStream = bytesRead < bytesRequested;

        // TODO Easy in

        if(endOfStream){

            // TODO Easy out

            setState(maPlayerState::IDLE);
            return paComplete;
        }

        // To ensure that the callback continues to be called,
        //it should return paContinue (0).
        // Either paComplete or paAbort can be returned to finish
        // stream processing,
        return paContinue;
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
