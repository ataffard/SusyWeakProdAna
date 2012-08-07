#define LUMI1FB

const std::string TAG = "080712";
const std::string SUFF = "";//"_IP2";

#ifdef LUMI1FB
const int LUMIMODE=1;  //0: Full dataset, 1: 1/ifb, 2: weight=1
const std::string DATE=TAG+"_1fb"+ SUFF;
//const std::string DATE="080112_1fb";
const float pLUMI = 1.04; //Lumi to draw on plots periods A+B3
#else
const int LUMIMODE=0;  //0: Full dataset, 1: 1/ifb, 2: weight=1
const std::string DATE=TAG+"_5fb" + SUFF;
const float pLUMI = 5.83; //Lumi to draw on plots periods A+B14
#endif









