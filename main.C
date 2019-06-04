#include <UniversalTime.hh>
int main() {
  UniversalTime timeobj(10, 400000, 10);
  cout << timeobj.GetSeconds() << endl;
  cout << timeobj.GetDays() << endl;

  return 0;
}

