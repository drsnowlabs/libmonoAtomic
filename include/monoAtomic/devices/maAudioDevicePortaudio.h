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

        // outputParameters.channelCount =  info.nChannels<0 ? maximumChannelCount(): info.nChannels;
        outputParameters.channelCount =  maximumChannelCount();
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
        std::cout << "maAudioDevicePortAudio::start " <<std::endl;
        maReturnCode r = stop();
        if(r!= maReturnCode::OK)
            return r;
        r = resume();
        return r;
    }

    maReturnCode stop() override {
        std::cout << "maAudioDevicePortAudio::stop " <<std::endl;
        setState(maPlayerState::STOPPED);
        m_easeFrames = -m_EASE_FRAMES;
        return maReturnCode::OK;
    }


    maReturnCode suspend() override {
        std::cout << "maAudioDevicePortAudio::suspend " <<std::endl;
        // Pa_StopStream( m_stream );
        setState(maPlayerState::SUSPENDED);
        m_easeFrames = -m_EASE_FRAMES;
        return maReturnCode::OK;
    }

    maReturnCode resume() override {
        std::cout << "maAudioDevicePortAudio::resume " <<std::endl;
        m_easeFrames = m_EASE_FRAMES;
        // Pa_StopStream( m_stream );
        PaError err = Pa_StartStream( m_stream );
        if( err != paNoError ) {
            std::cout << "maAudioDevicePortAudio::resume " << "error: "<< Pa_GetErrorText( err ) << std::endl;
            return maReturnCode::ERROR;
        }
        setState(maPlayerState::ACTIVE);
        return maReturnCode::OK;
    }


    maReturnCode reset() override {
        std::cout << "maAudioDevicePortAudio::reset " <<std::endl;
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
    size_t m_bytesRead = 0;
    int32_t m_easeFrames = 0;
    int32_t m_EASE_FRAMES=10000;

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


    void stopCallbackPrivate(){
        // if(Pa_IsStreamActive(m_stream)){
            PaError err = Pa_StopStream( m_stream );
            if( err != paNoError ) {
                std::cout << "maAudioDevicePortAudio::stop " << "error: "<< Pa_GetErrorText( err ) << std::endl;
                return;
            }

        // } else{
        //     std::cout << "Stream is stopped" << std::endl;
        // }
        if(m_state==maPlayerState::IDLE){
            m_bytesRead=0;
        }

        stopCallback(); // Call user defined callback at the end
    }

    virtual void stopCallback(){
        std::cout<< "stop callback (virtual)" << std::endl;
    }

    virtual size_t fillBuffer(void *output, unsigned long frameCount, int32_t nChannels){

        size_t bytesRead = 0;

        // subclass should implement this

        return bytesRead;
    }

    int readData(const void *input,
                 void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo* timeInfo,
                 PaStreamCallbackFlags statusFlags){

        size_t framesRead= fillBuffer(output, frameCount,  m_info.nChannels);
        bool endOfStream = framesRead < frameCount;



        if(endOfStream || m_easeFrames<0){
        // if(endOfStream || m_easeFrames==-1){
            // std::cout<< "stop request" << std::endl;
            if(endOfStream){
                std::cout<< "end of stream" << std::endl;
                setState(maPlayerState::IDLE);
            }

            m_easeFrames =0;
            return paComplete;
            // return paAbort;
        }

        // To ensure that the callback continues to be called,
        // it should return paContinue (0).
        // Either paComplete or paAbort can be returned to finish
        // stream processing,
        if(m_easeFrames>0){
        // if(m_easeFrames==1){
            m_easeFrames = 0;
        }
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
        ((maAudioDevicePortAudio*)userData) -> stopCallbackPrivate();
    }



};

} // end namespace
#endif
