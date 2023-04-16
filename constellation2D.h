#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <atomic> 
#include <iostream>

using namespace std;
/*
ToDo:
1. support mor constellation type, currently it only support QPSK
2. [ToDo2]
*/
class Constellation_2D{
public:
  enum CSTLTType_t {BPSK=1, QPSK=2, QAM16=4};
  unsigned long nofsymbol; //number of symbol
  double SymPower;
  CSTLTType_t CSTLTType;
  cv::Mat IntArray;
  cv::Mat TransSymbolArray;

  Constellation_2D(uint nofsymbol, CSTLTType_t CSTLTType, double SymPower, uint64 seed=cv::getTickCount()){
    this->nofsymbol = nofsymbol;
    this->SymPower = SymPower;
    this->CSTLTType = CSTLTType;
    //[ToDo2:change CV_32S to CV_32U when OPENCV supports it.]
    this->IntArray = cv::Mat::zeros(nofsymbol, 1, CV_32S);
    uint64 nofCSTLT = pow(2, (uint)CSTLTType);
    cv::RNG rng = cv::theRNG().state = seed;
    rng.fill(this->IntArray, cv::RNG::UNIFORM, 0, nofCSTLT);
    this->TransSymbolArray = QPSKSchemeEncoder(IntArray, SymPower);
  }
  cv::Mat QPSKSchemeEncoder(cv::Mat IntArray, double SymPower){ //return N x 1 double column vector of CVFC2_
    cv::Mat ret(IntArray.size(), CV_64FC2);
    IntArray.forEach<int32_t>(
      [&](int32_t& symbol, const int * position) -> void{
        switch(symbol){
          case(0):
            ret.at<cv::Vec2d>(position[0],position[1]) = {1.0,1.0}; //00
            break;
          case(1):
            ret.at<cv::Vec2d>(position[0],position[1]) = {1.0,-1.0}; //01
            break;
          case(2):
            ret.at<cv::Vec2d>(position[0],position[1]) = {-1.0,-1.0}; //11
            break;
          case(3):
            ret.at<cv::Vec2d>(position[0],position[1]) = {-1.0,1.0}; //10
            break;
          default:
            cout<< "integer is out of range [0,4)"<<endl;
            exit(0);
        }
      }
    );
    return sqrt(SymPower)*ret/sqrt(2);
  }
  static cv::Mat QPSKSchemeDecoder(cv::Mat ReceiSymbolArray){
    cv::Mat DecodedSymbolArray = ReceiSymbolArray.clone();
    double A=1/sqrt(2);
    cv::Vec2d symbol00( 1, 1);
    cv::Vec2d symbol01( 1,-1);
    cv::Vec2d symbol11(-1,-1);
    cv::Vec2d symbol10(-1, 1);
    ReceiSymbolArray.forEach<cv::Vec2d>(
      [&](cv::Vec2d& symbol, const int * position) -> void{
        if(symbol[0] >=0 )
          if(symbol[1] >= 0)
            DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1]) = A*symbol00;
          else
            DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1]) = A*symbol01;
        else
          if(symbol[1] >= 0)
            DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1]) = A*symbol10;
          else
            DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1]) = A*symbol11;
      }
    );
    return DecodedSymbolArray;
  }
  static double QPSKSchemeBER(cv::Mat DecodedSymbolArray, cv::Mat TransSymbolArray){ //use Received Symbols Array as function input is okay as well
    atomic<unsigned long> Bnum(0);
    if(DecodedSymbolArray.size() != TransSymbolArray.size()){
      cout<< "Two inputs of SER function has different size"<<endl;
      exit(0);
    }
    unsigned long len = DecodedSymbolArray.cols * DecodedSymbolArray.rows;
    TransSymbolArray.forEach<cv::Vec2d>(
      [&](cv::Vec2d& symbol, const int * position) -> void{
        if (symbol[0] * DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1])[0] < 0)
          Bnum++;
        if (symbol[1] * DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1])[1] < 0)
          Bnum++;
      }
    );
    return (double)Bnum/len/2;
  }
  static double QPSKSchemeSER(cv::Mat DecodedSymbolArray, cv::Mat TransSymbolArray){ //use Received Symbols Array as function input is okay as well
    atomic<unsigned long> Snum(0);
    if(DecodedSymbolArray.size() != TransSymbolArray.size()){
      cout<< "Two inputs of SER function has different size"<<endl;
      exit(0);
    }
    unsigned long len = DecodedSymbolArray.cols * DecodedSymbolArray.rows;
    TransSymbolArray.forEach<cv::Vec2d>(
      [&](cv::Vec2d& symbol, const int * position) -> void{
        if(!(
          (symbol[0] * DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1])[0] > 0) &&
          (symbol[1] * DecodedSymbolArray.at<cv::Vec2d>(position[0], position[1])[1] > 0)
         )
        ){
          Snum++;
        }
      }
    );
    return (double)Snum/len;
  }
};
