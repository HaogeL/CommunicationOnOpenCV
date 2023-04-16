# CommunicationOnOpenCV
This repo provides basic tools for communication simulation based on OpenCV framework.
# Build Examples
```bash
cd <repo/path>
mkdir build && cd build && cmake ..
build$ cmake --build .
[ 50%] Building CXX object CMakeFiles/AWGN.dir/main.cpp.o
[100%] Linking CXX executable AWGN
[100%] Built target AWGN
build$ ./ComOpenCV
SER: 0.000976562
BER: 0.000488281
```
# Examples
## Example0-generate QPSK samples
```cpp
int main(int argc, char* argv[]){
  Constellation_2D MyCon(pow(2, 8), Constellation_2D::QPSK, 1);
  AWGN awgn(MyCon.TransSymbolArray, 5);
  cv::Mat DecodedSymbols = Constellation_2D::QPSKSchemeDecoder(awgn.ReceiSymbolArray);
  cout<<"SER: ";
  cout<<Constellation_2D::QPSKSchemeSER(DecodedSymbols, MyCon.TransSymbolArray)<<endl;
  cout<<"BER: ";
  cout<<Constellation_2D::QPSKSchemeBER(DecodedSymbols, MyCon.TransSymbolArray)<<endl;
  matwrite("Tx.bin", MyCon.TransSymbolArray); //write Transmitted symbols to Tx.bin
  matwrite("Rx.bin", awgn.ReceiSymbolArray);//write Received symbols to Rx.bin
  return 0;
}
```
```bash
build$ cmake --build . && ./AWGN
[ 50%] Building CXX object CMakeFiles/AWGN.dir/main.cpp.o
[100%] Linking CXX executable AWGN
[100%] Built target AWGN
SER: 0.0625
BER: 0.03125
```
Read Rx and plot in Python.
```python
import numpy as np
import PyUtils
import cv2 
import matplotlib.pyplot as plt
from scipy import special
Rx = PyUtils.GetMatFromBin("../build/Rx.bin")
RE,IM = cv2.split(Rx)
plt.scatter(RE, IM, marker='.')
plt.xlabel("RE")
plt.ylabel("IM")
plt.axhline(0, color='black')
plt.axvline(0, color='black')
```
![](https://github.com/HaogeL/CommunicationOnOpenCV/blob/main/QPSKSymbolsForREADME.png)
## Example1-Compare simulated BER with theoretic value
```cpp
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
```
```python
import numpy as np
import PyUtils
import cv2 
import matplotlib.pyplot as plt
from scipy import special
BER = PyUtils.GetMatFromBin("../build/BER.bin")
BER = np.resize(BER,(BER.shape[0],1))
SNR_log = np.arange(0,BER.shape[0])*0.1
SNR_log = np.resize(SNR_log, (BER.shape[0],1))
SNR_lin = np.power(10, SNR_log/10)
EbN0 = SNR_lin/2;
EbN0_log = 10*np.log10(EbN0);
BER_theo = 0.5*special.erfc(np.sqrt(EbN0))
plt.plot(EbN0_log, np.log10(BER), 'x', markersize=3)
plt.plot(EbN0_log, np.log10(BER_theo), linewidth=1)
plt.grid(axis = 'y')
```
![](https://github.com/HaogeL/CommunicationOnOpenCV/blob/main/BERCheckForREADME.png)
