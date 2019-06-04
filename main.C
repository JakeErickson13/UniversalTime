#include <./UniversalTime.hh>

int main() {

  UniversalTime e1time(0, 62310, 1.5477e06);
  UniversalTime e2time(0, 62309, 9.93522e08);

  e1time-=e2time;
  
  cout << "In main(): " << endl; 
  cout << e1time.GetNanoSeconds() << endl;
  cout << e1time.GetSeconds() << endl;
  cout << e1time.GetDays() << endl;

  return 0;
}

