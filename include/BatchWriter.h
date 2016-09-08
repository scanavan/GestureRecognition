#ifndef BATCHWRITER_H
#define BATCHWRITER_H

#include <string>

void arffBatchWriter();
void RFWriter();
void arffLeapBatchWriter();
void LeapRFWriter();
void LeapLibSVMWriter();
void DirStruct(std::string home, int subjects);
#endif