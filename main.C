#include "UniversalTime_jake.hh"
#include <TRandom3>

int main() {


TRandom3 * ran = new TRandom3(5);

for (int n=0; n<90000; n++ ) {

  Int_t day = 2*(int(ran->Rndm()*100)-50);
  Int_t sec = 2*(int(ran->Rndm()*100000)-50000);
  Double_t nanosec= (ran->Rndm()-0.5)*2e9;

///  cout << day << " " <<sec <<" " << nanosec << endl;
  UniversalTime e1time(day, sec, nanosec);

//  cout << e1time.GetDays() << " ";
//  cout << e1time.GetSeconds() <<" ";
//  cout << e1time.GetNanoSeconds() << endl;
//  cout << "  " << endl;
}

  return 0;
}

