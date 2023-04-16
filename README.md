# CommunicationOnOpenCV
This repo provides basic communication functions based on OpenCV framework.
# Example
## Generate QPSK samples
```cpp
int main(int argc, char* argv[]){
  Constellation_2D MyCon(pow(2, 8), Constellation_2D::QPSK, 1);
  AWGN awgn(MyCon.TransSymbolArray, 20);
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
read Tx&Rx and plot in python
