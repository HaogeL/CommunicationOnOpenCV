#pragma once
#include <opencv2/core.hpp>
#include <time.h>
using namespace cv;

class AWGN{
public:
  double SNR_db;
  double SNR_li;
  cv::Mat noise;
  double TransSymbolPower;
  double NoisePower;
  cv::Mat ReceiSymbolArray;
  double CalculatePowerOfSymbolArray(cv::Mat2d SymbolArray){
    cv::Mat complexRI[2];
    cv::split(SymbolArray, complexRI);
    magnitude(complexRI[0], complexRI[1], complexRI[0]);
    complexRI[0] = complexRI[0].mul(complexRI[0]);
    return cv::mean(complexRI[0])[0];
  }
  AWGN(cv::Mat TransSymbolArray, double SNR_db, uint64 seed=getTickCount()){
    this->SNR_db = SNR_db;
    this->SNR_li = pow(10,SNR_db/10);
    this->noise = TransSymbolArray.clone();
    this->TransSymbolPower = CalculatePowerOfSymbolArray(TransSymbolArray);
    this->NoisePower = TransSymbolPower/SNR_li;
    cv::RNG rng = cv::theRNG().state = seed;
    rng.fill(noise, cv::RNG::NORMAL, 0, sqrt(NoisePower/2)); //sigma = sqrt(measuredSymbolPower/pow(10, SNR_db/10)/2)
    ReceiSymbolArray = noise+TransSymbolArray;
    /*
    cout<<"debug "<< CalculatePowerOfSymbolArray(TransSymbolArray)<<";"<<CalculatePowerOfSymbolArray(noise)<<";"<<CalculatePowerOfSymbolArray(ReceiSymbolArray)<<endl;
    cout << "--------------------------------------" << endl;
    cout<<noise<<endl;
    cout<<TransSymbolArray<<endl;
    cout<<ReceiSymbolArray<<endl;
    */
  }
};