#ifndef MA_UTILS_H
#define MA_UTILS_H

#include "maDefs.h"
#include <sstream>

namespace monoAtomic{

    static std::vector<std::string> sizeUnits = { "Bytes", "kB", "MB", "GB", "TB" };

    static std::string size2units(long int size, std::vector<std::string> unitVec, unsigned short precision=3){
        unsigned int factor =0;
        double dSize = 1.0*size;
        while(dSize >= 1000. && factor < unitVec.size() ){
            dSize /= 1000.;
            factor++;
        }

        std::stringstream output;
        output << std::fixed
            << std::setprecision(precision)
            << dSize
            <<" "
            << unitVec[factor];
        return output.str();;
    }

    static void convert24BitTo32Bit(const char* src, char* dest, size_t numSamples) {

        // this function is generalized in convertBitUp(...) below
        for (size_t i = 0; i < numSamples; ++i) {
            dest[(4*i)+0] = 0;
            dest[(4*i)+1] = src[i * 3 ];
            dest[(4*i)+2] = src[i * 3 + 1];
            dest[(4*i)+3] = src[i * 3 + 2];
        }
    }

    static void convert32BitTo24Bit(const char* src, char* dest, size_t numSamples) {

        for (size_t i = 0; i < numSamples; ++i) {
            dest[(3*i)+0] = src[ (i*4) + 1];
            dest[(3*i)+1] = src[ (i*4)  + 2];
            dest[(3*i)+2] = src[ (i*4) + 3];

        }
    }

    static void convertBitUp(const char* src, int8_t srcNBytes, char* dest, int8_t destNBytes, size_t numSamples) {


        if(destNBytes<=srcNBytes)
            return;
        // qDebug() << __FUNCTION__ << srcNBytes << "->" << destNBytes << "x" << numSamples;

        for (size_t i = 0; i < numSamples; ++i) {
            for(int8_t d=destNBytes-1, s=srcNBytes-1; d>=0; d--, s--){
                dest[ (destNBytes*i) + d] = (s<0) ? 0 : src[ (i*srcNBytes) + s ];
            }
        }
    }



    static void processSample(char* ptr, maSampleFormat sampleFormat, bool phase=false, float volume=1.0){
        int8_t tmp8=0;
        int16_t tmp16=0;
        int32_t tmp32=0;
        float tmpF=0.0;
        char tmpC[3];

        switch(sampleFormat){
        case maSampleFormat::UNKNOWN:
            break;

        case maSampleFormat::INT8:
            tmp8 = *(int8_t*)ptr;
            tmp8 = phase?-tmp8:tmp8;
            tmp8 *= volume;
            *(uint8_t*)ptr = tmp8;
            break;

        case maSampleFormat::INT16:
            tmp16 = *(int16_t*)ptr;
            tmp16 = phase?-tmp16:tmp16;
            tmp16 *= volume;
            *(int16_t*)ptr = tmp16;
            break;

        case maSampleFormat::INT24:
            convertBitUp(ptr, 3, (char*)&tmp32, 4, 1);
            tmp32 = phase?-tmp32:tmp32;
            tmp32 *= volume;
            convert32BitTo24Bit((char*)&tmp32, tmpC, 1);
            memcpy(ptr, tmpC, 3);
            break;

        case maSampleFormat::INT32:
            tmp32 = *(int32_t*)ptr;
            tmp32 = phase?-tmp32:tmp32;
            tmp32 *= volume;
            *(int32_t*)ptr = tmp32;
            break;

        case maSampleFormat::FLOAT32:
            tmpF = *(float*)ptr;
            tmpF = phase?-tmpF:tmpF;
            tmpF *= volume;
            *(float*)ptr = tmpF;
            break;
        };

    }

    template <class T> float normalizeToFloat(T t) {
        float res = 1.0*t/std::numeric_limits<T>::max();
            return res;
    }

    template <class T> void scaleSample(char* inSamplePtr, maSampleFormat inSampleFormat, char* destPtr, T* outSample, bool phase=false, size_t outSampleMax=0) {

        int8_t tmp8=0;
        int16_t tmp16=0;
        int32_t tmp32=0;
        float tmpF=0.0;
        char tmpC[3];

        outSampleMax = outSampleMax ? outSampleMax : std::numeric_limits<T>::max();

        switch(inSampleFormat){
        case maSampleFormat::UNKNOWN:
            break;

        case maSampleFormat::INT8:
            // tmp8 = *(uint8_t*)inSamplePtr;
            // tmp8 = phase?-tmp8:tmp8;
            // *outSample = tmp8 * (float) std::numeric_limits<T>::max() / std::numeric_limits<uint8_t>::max();
            // // tmp16 *= volume;
            // *(T*)destPtr = *outSample;
            break;

        case maSampleFormat::INT16:
            tmp16 = *(int16_t*)inSamplePtr;
            tmp16 = phase?-tmp16:tmp16;
            *outSample = tmp16 * (float) outSampleMax / std::numeric_limits<int16_t>::max();
            // tmp16 *= volume;

            *(T*)destPtr = *outSample;
            break;

        case maSampleFormat::INT24:
            convertBitUp(inSamplePtr, 3, (char*)&tmp32, 4, 1); // convert to 32bit (padded)
            tmp32 = phase?-tmp32:tmp32;
            *outSample = tmp32 * (float) outSampleMax / std::numeric_limits<int32_t>::max();
            *(T*)destPtr = *outSample;

            break;

        case maSampleFormat::INT32:
            tmp32 = *(int32_t*)inSamplePtr;
            tmp32 = phase?-tmp32:tmp32;
            *outSample = tmp32 * (float) outSampleMax / std::numeric_limits<int32_t>::max();
            *(T*)destPtr = *outSample;
            break;


        case maSampleFormat::FLOAT32:
            tmpF = *(float*)inSamplePtr;
            tmpF = phase?-tmpF:tmpF;
            *outSample = tmpF * outSampleMax;
        //     tmpF *= volume;

            *(T*)destPtr = *outSample;
            break;

        default:
            break;


        };



        // return toSample;
    }

    static float anyToFloat(const char* ptr, maSampleFormat sampleFormat){

        int32_t tmp32=0;

        switch(sampleFormat){
        case maSampleFormat::UNKNOWN:
            return 0.0;
        case maSampleFormat::INT8:
            return (float)*(int8_t*)ptr;
        case maSampleFormat::INT16:
            return (float)*(int16_t*)ptr;
        case maSampleFormat::INT24:
            convertBitUp(ptr, 3, (char*)&tmp32, 4, 1);
            return (float) tmp32;
        case maSampleFormat::INT32:
            return (float)*(int32_t*)ptr;
        case maSampleFormat::FLOAT32:
            return *(float*)ptr;
        };
        return 0.0;
    }

    static float anyToFloatNorm(const char* ptr, maSampleFormat sampleFormat){

        int32_t tmp32=0;

        switch(sampleFormat){
        case maSampleFormat::UNKNOWN:
            return 0.0;
        case maSampleFormat::INT8:
            return normalizeToFloat(*(int8_t*)ptr);
        case maSampleFormat::INT16:
            return normalizeToFloat(*(int16_t*)ptr);
        case maSampleFormat::INT24:
            convertBitUp(ptr, 3, (char*)&tmp32, 4, 1);
            return normalizeToFloat(tmp32);
        case maSampleFormat::INT32:
            return normalizeToFloat(*(int32_t*)ptr);
        case maSampleFormat::FLOAT32:
            return *(float*)ptr;
        };
        return 0.0;
    }


}



#endif
