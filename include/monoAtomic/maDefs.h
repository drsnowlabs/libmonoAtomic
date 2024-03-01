#ifndef MA_DEFS_H
#define MA_DEFS_H

#include <iostream>
#include <string>
#include <stddef.h>
#include <cstdint>
#include <vector>
#include <fstream>

#define MA_DEBUG_COUT true

namespace monoAtomic{



    ////////////////////////////////////////////////////////////////////////////////////
    // RIF and WAVE Chunks

    struct  maChunk {
        // RIFF Generic Chunk
        uint8_t         chunkID[4];        // CHunk ID
        uint32_t        chunkDataSize=0;     // Chunk data Size
    };

    struct  maRiffHeader {
        // RIFF Header Chunk
        uint8_t         chunkID[4];         // RIFF
        uint32_t        chunkDataSize;      // RIFF Chunk data Size
        uint8_t         riffTypeID[4];      // WAVE
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
        uint16_t        validBitsPerSample = 0;
        uint32_t        channelMask =0 ;
        uint8_t         subFormat[16] = {0};
    };


////////////////////////////////////////////////////////////////////////////////////
// Error codes

    enum class maReturnCode{
        OK=0,
        ERROR,
        NULLPTR,
        SAMPLERATE_MISMATCH,
        BITDEPTH_MISMATCH,
        LENGTH_MISTMATCH,
        UNKNOWN
    };


    static std::ostream& operator<< ( std::ostream& stm, maReturnCode err )
    {
        std::string s ="";
        switch(err){
        case maReturnCode::OK: s="OK"; break;
        case maReturnCode::ERROR: s="ERROR"; break;
        case maReturnCode::NULLPTR: s="NULLPTR"; break;
        case maReturnCode::SAMPLERATE_MISMATCH: s="SAMPLERATE_MISMATCH"; break;
        case maReturnCode::BITDEPTH_MISMATCH: s="BITDEPTH_MISMATCH"; break;
        case maReturnCode::LENGTH_MISTMATCH: s="LENGTH_MISTMATCH"; break;
        case maReturnCode::UNKNOWN: s="UNKNOWN"; break;

        }
        return stm << "maReturnCode::"+s+"{"  << int(err) << "}" ;
    }



////////////////////////////////////////////////////////////////////////////////////
// Audio File Types

    enum class maAudioFileType {
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
// Channel Playback States

enum class maChannelPlaybackState{
    Active = 0,
    Solo,
    Mute,
    SoloMuted
};


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

    struct maAudioInfo {
        maAudioFileType fileType = maAudioFileType::UNKNOWN;
        maSampleFormat sampleFormat = maSampleFormat::UNKNOWN;
        std::filesystem::path filePath;
        size_t nSamples=0;
        uint32_t sampleRate=0;
        size_t fileSize=0;
        size_t dataSize=0;
        size_t durationMs=0;
        uint8_t bitDepth=0;
        uint8_t sampleSize=0; // bitdepth in bytes
        uint32_t nChannels=0;
        size_t nFrames=0;
        uint32_t frameSize=0;
    };

    struct maPlayerInfo {
        int32_t deviceIndex=-1;
        int32_t nChannels=-1;
        uint32_t sampleRate=44100;
        maSampleFormat sampleFormat = maSampleFormat::FLOAT32;
        size_t framesPerBuffer=0;
    };


    ////////////////////////////////////////////////////////////////////////////////////
    /// METADATA
    ///


    class maMetaChunk {
    public:
        maMetaChunk(maChunk chunkHeader, char* chunkData){
                memcpy(&m_id, chunkHeader.chunkID, sizeof(chunkHeader.chunkID));
                m_uid = m_id;
                if(!memcmp("LIST", chunkHeader.chunkID, 4)){
                    parseLISTFields(chunkData);

                }
        }

        size_t readChunk(FILE* f);
        std::string id(){ return m_id;}
        std::string uid(){ return m_uid;}
        std::unordered_map<std::string, std::string> fields() {return m_fields;}
        void printFields(){
            auto print_key_value = [](const auto& key, const auto& value)
            {
                std::cout << key << ": " << value << std::endl;
            };
            for(const std::pair<const std::string, std::string>& f : m_fields){
                print_key_value(f.first, f.second);
            }
        }

        void parseLISTFields(char* chunkData){

                int b=0; // byte counter
                char list_type[] = "xxxx";
                memcpy(&list_type, chunkData, 4);
                b+=4;

                std::cout << "list parse data: " << list_type << std::endl;

                if(!memcmp("INFO", list_type, 4)){
                    m_uid = "LIST_INFO";
                    while (b<m_header.chunkDataSize) {
                        char key[]="xxxx";
                        memcpy(&key, &chunkData[b], 4);
                        b+=4;
                        uint32_t size;
                        memcpy(&size, &chunkData[b], 4);
                        b+=4;
                        char val[size];
                        memcpy(&val, &chunkData[b], size);
                        b+=size;
                        m_fields["LIST_INFO_"+std::string(key)] = val;
                    }
                }
        }

    protected:

        maChunk m_header;
        // std::vector<char> m_chunkData;
        //    std::string m_id{4, '0'};
        char m_id[5] = "xxxx";
        std::string m_uid = "NONE";
        std::unordered_map<std::string, std::string> m_fields;

    };

    // class maLISTMetaChunk : public maMetaChunk{
    // public:
    //     explicit maLISTMetaChunk(maChunk hdr);
    //     // void parseFields() override;
    // };


} // end namespace

#endif
