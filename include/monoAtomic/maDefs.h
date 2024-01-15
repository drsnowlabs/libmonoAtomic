#ifndef MA_DEFS_H
#define MA_DEFS_H

#include <iostream>
#include <string>
#include <stddef.h>
#include <cstdint>
#include <vector>
#include <fstream>

namespace monoAtomic{
    ////////////////////////////////////////////////////////////////////////////////////
    // RIF and WAVE Chunks

    struct  maRiffChunk {
        // RIFF Generic Chunk
        uint8_t         chunkID[4];        // RIFF
        uint32_t        chunkDataSize;      // RIFF Chunk data Size
    };

    struct  maRiffHeader {
        // RIFF Header Chunk
        uint8_t         chunkID[4];        // RIFF
        uint32_t        chunkDataSize;      // RIFF Chunk data Size
        uint8_t         riffTypeID[4];        // WAVE
    };

    struct maRiffFormatChunkExt {
         // EXTENSIBLE
        uint16_t        formatTag;      //  format_Tag 1=PCM, 3 = float
        uint16_t        nChannels;      //  Number of channels
        uint32_t        sampleRate;     // Sampling Frequency in (44100)Hz
        uint32_t        byteRate;       // Byte rate
        uint16_t        blockAlign;     // 4
        uint16_t        bitsPerSample;  // 16 <- if formatTag=1 , only fill up to this

        uint16_t        cbSize=0;       //  size of the extended chunk
        uint16_t        validBitsPerSample;
        uint32_t        channelMask;
        uint8_t         subFormat[16];
    };


////////////////////////////////////////////////////////////////////////////////////
// Error codes

    enum class maError{
        NOERROR=0,
        NULLPTR,
        SAMPLERATE_MISMATCH,
        BITDEPTH_MISMATCH,
        LENGTH_MISTMATCH,
        UNKNOWN
    };


    static std::ostream& operator<< ( std::ostream& stm, maError err )
    {
        std::string s ="";
        switch(err){
        case maError::NOERROR: s="NOERROR"; break;
        case maError::NULLPTR: s="NULLPTR"; break;
        case maError::SAMPLERATE_MISMATCH: s="SAMPLERATE_MISMATCH"; break;
        case maError::BITDEPTH_MISMATCH: s="BITDEPTH_MISMATCH"; break;
        case maError::LENGTH_MISTMATCH: s="LENGTH_MISTMATCH"; break;
        case maError::UNKNOWN: s="UNKNOWN"; break;

        }
        return stm << "maError::"+s+"{"  << int(err) << "}" ;
    }



////////////////////////////////////////////////////////////////////////////////////
// Audio File Types 

    enum maAudioFileType {
            UNKNOWN=0,
            WAVE,
            AIFF
        };

        static std::ostream&  operator<< ( std::ostream&  stm, maAudioFileType sf )
    {
        std::string s ="";
        switch(sf){
        case maAudioFileType::UNKNOWN: s="UNKNOWN"; break;
        case maAudioFileType::WAVE: s="WAVE"; break;
        case maAudioFileType::AIFF: s="AIFF"; break;

        }
        return stm << "maAudioFileType::"+s+"{"  << int(sf) <<  "}" ;
    }

////////////////////////////////////////////////////////////////////////////////////
// Audio Player States

    enum class maPlayerState{
        UNKNOWN = -1,
        ACTIVE,
        SUSPENDED,
        STOPPED,
        IDLE,
        STARTING,
        STOPPING
    };

    static std::ostream&  operator<< ( std::ostream&  stm, maPlayerState sf )
    {
        std::string s ="";
        switch(sf){
        case maPlayerState::UNKNOWN: s="UNKNOWN"; break;
        case maPlayerState::ACTIVE: s="ACTIVE"; break;
        case maPlayerState::SUSPENDED: s="SUSPENDED"; break;
        case maPlayerState::STOPPED: s="STOPPED"; break;
        case maPlayerState::IDLE: s="IDLE"; break;
        case maPlayerState::STARTING: s="STARTING"; break;
        case maPlayerState::STOPPING: s="STOPPING"; break;

        }
        return stm << "maPlayerState::"+s+"{"  << int(sf) <<  "}" ;
    }

////////////////////////////////////////////////////////////////////////////////////
// Sample Formats

    

    enum class maSampleFormat{
        UNKNOWN = 0,
        INT8 = 8,
        INT16 = 16,
        INT24 = 24,
        INT32 = 32,
        FLOAT32 = 33

    };

    static uint8_t maBitDepth(maSampleFormat sf){
        switch(sf){
            case maSampleFormat::UNKNOWN: return 0;
            case maSampleFormat::INT8: return 8;
            case maSampleFormat::INT16: return 16;
            case maSampleFormat::INT24: return 24;
            case maSampleFormat::INT32: return 32;
            case maSampleFormat::FLOAT32: return 32;
        }
    }

    static uint8_t maByteDepth(maSampleFormat sf){
        switch(sf){
            case maSampleFormat::UNKNOWN: return 0;
            case maSampleFormat::INT8: return 1;
            case maSampleFormat::INT16: return 2;
            case maSampleFormat::INT24: return 3;
            case maSampleFormat::INT32: return 4;
            case maSampleFormat::FLOAT32: return 4;
        }
    }

    

    static bool operator < ( maSampleFormat l,  maSampleFormat r){

        return static_cast<int>(l) < static_cast<int>(r);
    }

    static bool operator <= ( maSampleFormat l,  maSampleFormat r){
        return static_cast<int>(l) <= static_cast<int>(r);
    }

    static bool operator > ( maSampleFormat l,  maSampleFormat r){

        return static_cast<int>(l) > static_cast<int>(r);
    }

    static bool operator >= ( maSampleFormat l,  maSampleFormat r){
        return static_cast<int>(l) >= static_cast<int>(r);
    }

    static bool operator == ( maSampleFormat l,  maSampleFormat r){
        return static_cast<int>(l) == static_cast<int>(r);
    }

    static std::ostream& operator<< ( std::ostream& stm, maSampleFormat sf ){
        std::string s ="";
        switch(sf){
            case maSampleFormat::UNKNOWN: s="UNKNOWN"; break;
            case maSampleFormat::INT8: s="INT8"; break;
            case maSampleFormat::INT16: s="INT16"; break;
            case maSampleFormat::INT24: s="INT24"; break;
            case maSampleFormat::INT32: s="INT32"; break;
            case maSampleFormat::FLOAT32: s="FLOAT32"; break;
        }
        return stm << "maSampleFormat::"+s+"{" << int(sf) << "}" ;
    }


} // end namespace

#endif
