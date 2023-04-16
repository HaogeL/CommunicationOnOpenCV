#include "AWGNChannel.h"
#include "constellation2D.h"
#include "util.h"

#include <iostream>
#include <time.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]){

  Constellation_2D MyCon(pow(2, 10), Constellation_2D::QPSK, 1);
  AWGN awgn(MyCon.TransSymbolArray, 10);
  cv::Mat DecodedSymbols = Constellation_2D::QPSKSchemeDecoder(awgn.ReceiSymbolArray);
  cout<<"SER: ";
  cout<<Constellation_2D::QPSKSchemeSER(DecodedSymbols, MyCon.TransSymbolArray)<<endl;
  cout<<"BER: ";
  cout<<Constellation_2D::QPSKSchemeBER(DecodedSymbols, MyCon.TransSymbolArray)<<endl;
  matwrite("Tx.bin", MyCon.TransSymbolArray);
  matwrite("Rx.bin", awgn.ReceiSymbolArray);

  cv::Mat BER(100,1,CV_64FC1);
  parallel_for_(Range(0, BER.rows), [&](const Range& range){
      for (int r = range.start; r < range.end; r++) {
        Constellation_2D *MyCon = new Constellation_2D(pow(2, 18), Constellation_2D::QPSK, 1);
        AWGN *awgn=new AWGN(MyCon->TransSymbolArray, r*0.1);
        cv::Mat DecodedSymbols = Constellation_2D::QPSKSchemeDecoder(awgn->ReceiSymbolArray);
        BER.at<double>(r,0) = Constellation_2D::QPSKSchemeBER(DecodedSymbols, MyCon->TransSymbolArray);
      }
  });
  matwrite("BER.bin", BER);
  return 0;
}
