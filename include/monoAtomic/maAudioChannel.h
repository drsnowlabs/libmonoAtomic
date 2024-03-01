#ifndef MA_AUDIOCHANNEL_H
#define MA_AUDIOCHANNEL_H

#include <cstdint>
#include <monoAtomic/maExceptions.h>
#include <monoAtomic/maUtils.h>

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





    void loadSample(size_t iSample, char* dest, maSampleFormat outFmt, bool burnTrackState){
        if(outFmt == maSampleFormat::UNKNOWN)
            return;

        if(!m_parentFile){
            memset(dest, 0, maByteDepth(outFmt));
            return;
        }

        if(burnTrackState && m_playbackState > maChannelPlaybackState::Solo ){
            memset(dest, 0, maByteDepth(outFmt));
            return;
        }

        char* sPtr = nullptr;
        try{
            sPtr = samplePtr(iSample);
            if(!sPtr)
                throw maInvalidSample();

        }
        catch ( const std::exception& e ){
            // std::cout << "loadSample ERROR: " << e.what() << std::endl;
            // memset(dest, 0, maByteDepth(outFmt));
            return;
        }

        uint8_t thisSampleSize = maByteDepth(outFmt);
        maSampleFormat thisFmt = m_parentFile->info.sampleFormat;

        // no conversion needed, just copy bytes
        if(outFmt == thisFmt && sPtr){
            memcpy(dest, sPtr, thisSampleSize);
            return;
        }



        // do some conversions
        int8_t out8=0;
        char uout8=0;
        int16_t out16=0;
        int32_t out32=0;
        int32_t tmp32=0;

        switch(outFmt){
        case maSampleFormat::UNKNOWN:
            // this should be handled earlier
            return;
        case maSampleFormat::INT8:
            scaleSample(sPtr, thisFmt, dest, &out8, burnTrackState && m_phase);
            // convert signed int to usigned int
            uout8 = (char)out8;
            *dest = uout8;
            return;
        case maSampleFormat::INT16:
            scaleSample(sPtr, thisFmt, dest, &out16, burnTrackState && m_phase);
            return;
        case maSampleFormat::INT24:
            scaleSample(sPtr, thisFmt, dest, &out32, burnTrackState && m_phase, 8388607);
            return;
        case maSampleFormat::INT32:
            scaleSample(sPtr, thisFmt, dest, &out32, burnTrackState && m_phase);
            return ;
        case maSampleFormat::FLOAT32:
            float s=sampleF(iSample);
            if(burnTrackState && m_phase)
                s = -s;
            memcpy(dest, (char*)&s, 4);
            return ;
        };
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






