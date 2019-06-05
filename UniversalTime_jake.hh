////////////////////////////////////////////////////////////////////
/// \class RAT::DS::UniversalTime
///
/// \brief  This class represents a time in the SNO+ Universal time system
///
/// \author Phil G Jones <p.g.jones@qmul.ac.uk>
///
/// REVISION HISTORY:\n
///  2013-1-21 : P. Jones - New file as part of ds review.
///
/// \details Universal time is the time elapsed since the start of the
///         SNO+ epoch, t0, which is midnight on 01 Jan 2010 (GMT).
///
////////////////////////////////////////////////////////////////////
#ifndef __RAT_DS_UniversalTime__
#define __RAT_DS_UniversalTime__

#include <TObject.h>

#include <ctime>

#include <typeinfo>

class UniversalTime :
{
public:
  /// Construct the class
  UniversalTime() : TObject(), days(0), seconds(0), nanoSeconds(0) { };

  /// Construct the class with relevant timing values
  ///
  /// @param[in] days_ since t0
  /// @param[in] seconds_ since t0
  /// @param[in] nanoSeconds_ since t0
  UniversalTime( const Int_t days_, const Int_t seconds_, const Double_t nanoSeconds_ );

  /// Get the days
  ///
  /// @return days
  Int_t GetDays() const { return days; }

  /// Get the seconds
  ///
  /// @return seconds
  Int_t GetSeconds() const { return seconds; }

  /// Get the nano seconds
  ///
  /// @return nano seconds
  Double_t GetNanoSeconds() const { return nanoSeconds; }

  /// Get the time as a std time structure
  ///
  /// @param[in] snoPlus should the (default) snoplus (true) or sno (false) offset be used?
  /// @return the time as a time structure
  inline std::tm GetTime( const Bool_t snoPlus=true ) const;

  /// Add a universal time to this
  ///
  /// @param[in] rhs to add
  /// @return reference to this
  inline UniversalTime& operator+=( const UniversalTime& rhs );

  /// Add a universal time
  ///
  /// @param[in] rhs to add
  /// @return new universal time
  UniversalTime& operator+( const UniversalTime& rhs ) const { return UniversalTime(*this) += rhs; }

  /// Subtract a universal time to this
  ///
  /// @param[in] rhs to subtract
  /// @return reference to this
  inline UniversalTime& operator-=( const UniversalTime& rhs );

  /// Subtract a universal time
  ///
  /// @param[in] rhs to subtract
  /// @return new universal time
  UniversalTime& operator--( const UniversalTime& rhs ) const { return UniversalTime(*this) -= rhs; }

  /// Check if this time is the same as another
  ///
  /// @param[in] rhs to test
  /// @return true if they are the same
  Bool_t operator==( const UniversalTime& rhs ) const { return days == rhs.days && seconds == rhs.seconds && nanoSeconds == rhs.nanoSeconds; }

  /// Check if this time is NOT the same as another
  ///
  /// @param[in] rhs to test
  /// @return true if they are NOT the same
  Bool_t operator!=( const UniversalTime& rhs ) const { return !(*this == rhs ); }

  /// Check if this time is less than (before) another
  ///
  /// @param[in] rhs to test
  /// @return true if this is less than (before) rhs
  inline Bool_t operator<( const UniversalTime& rhs ) const;

  /// Check if this time is less than (before) or equal to another
  ///
  /// @param[in] rhs to test
  /// @return true if this is less than (before) or equal to rhs
  Bool_t operator<=( const UniversalTime& rhs ) const { return *this < rhs || *this == rhs; }

  /// Check if this time is greater than (before) another
  ///
  /// @param[in] rhs to test
  /// @return true if this is greater than (before) rhs
  Bool_t operator>( const UniversalTime& rhs ) const { return !(*this <= rhs); }

  /// Check if this time is greater than (before) or equal to another
  ///
  /// @param[in] rhs to test
  /// @return true if this is greater than (before) or equal to rhs
  Bool_t operator>=( const UniversalTime& rhs ) const { return *this > rhs || *this == rhs; }

  // This ROOT macro adds dictionary methods to this class.
  // The number should be incremented whenever this class's members are changed.
  // It assumes this class has no virtual methods, use ClassDef if change this.
//  ClassDefNV( UniversalTime, 1 );
protected:
  /// Normalises the time i.e. ensures that nanoSeconds < 1 second and seconds < 1 day
  inline void Normalise();

  inline Bool_t IsNegative();

  inline Bool_t TimeOrder();

  Int_t days; ///< Universal time i.e. relative to the world (days since SNO+ day0)
  Int_t seconds; ///< Universal time i.e. relative to the world (secs)
  Double_t nanoSeconds; ///< Universal time i.e. relative to the world (nsecs)
};

inline
UniversalTime::UniversalTime( const Int_t days_, const Int_t seconds_, const Double_t nanoSeconds_ )
  : TObject(), days(days_), seconds(seconds_), nanoSeconds(nanoSeconds_)
{
  Normalise();
}

inline std::tm
UniversalTime::GetTime( const Bool_t snoPlus ) const
{
  std::tm time;
  time.tm_sec = seconds;
  time.tm_min = 0;
  time.tm_hour = 0;
  time.tm_mday = 1 + days;
  time.tm_mon = 0;
  if( snoPlus ) // SNO+ starts at 2010, SNO at 1996, tm_year is years since 1900
    time.tm_year = 110;
  else
    time.tm_year = 96;
  time.tm_isdst = 0;
  mktime(&time); // Normalises i.e. deals with roll-overs etc...
  return time;
}

inline UniversalTime&
UniversalTime::operator+=( const UniversalTime& rhs )
{
  nanoSeconds += rhs.nanoSeconds;
  seconds += rhs.seconds;
  days += rhs.days;
  Normalise();
  return *this;
}

inline UniversalTime&
UniversalTime::operator-=( const UniversalTime& rhs )
{
  nanoSeconds -= rhs.nanoSeconds;
  seconds -= rhs.seconds;
  days -= rhs.days;
  Normalise();
  return *this;
}

inline Bool_t
UniversalTime::operator<( const UniversalTime& rhs ) const
{
  if( days > rhs.days ) return false;
  else if( days == rhs.days && seconds > rhs.seconds ) return false;
  else if( days == rhs.days && seconds == rhs.seconds && nanoSeconds >= rhs.nanoSeconds ) return false;
  return true;
}

inline Bool_t
UniversalTime::IsNegative()
{
  if (days < 0 || seconds < 0 || nanoSeconds < 0.0)return true;
  return false;
}

inline Bool_t
UniversalTime::TimeOrder()
{
  if (days < 0)return false;
  if (days == 0 && seconds < 0)return false;
  if (days == 0 && seconds == 0 && nanoSeconds < 0.0)return false;
  return true;
}


inline void
UniversalTime::Normalise()
{

//  cout << " " << endl;
//  cout << "Renormalizing ..."  << endl;
//  Bool_t inOrder = IsNegative();
//  cout << "Any negatives? " << inOrder << endl;
//
//  cout << "Before: " <<endl;
//  cout << nanoSeconds << " " << typeid(nanoSeconds).name() << endl;
//  cout << seconds << " " << typeid(seconds).name() << endl;
//  cout << days << " " << typeid(days).name() << endl;
//  cout << "" << endl;


  //Begin testing

//If events are in order, proceed normally correcting for negative times
if (TimeOrder())
  {
//  cout << "IN ORDER" << endl;
  if (nanoSeconds < 0.0)
    {
    seconds -= 1;
    nanoSeconds += 1.0e9;
    }
  if (seconds < 0)
    {
    days -= 1;
    seconds += 86400;
    }
//  if (days < 0)
//    {
//    days = -1 * days;
//    }
  }

if (!TimeOrder())
  {
//  cout << "NOT IN ORDER" << endl;
  if (nanoSeconds > 0.0)
    {
    seconds += 1;
    nanoSeconds = nanoSeconds - 1.0e9;
    }
  if (seconds > 0)
    {
    days += 1;
    seconds = seconds - 86400;
    }
//  nanoSeconds = fabs(nanoSeconds);
//  seconds = fabs(seconds);
//  days = fabs(days);
  }


//  cout << "After negative treatment: " << endl;
//  cout << nanoSeconds << " " << typeid(nanoSeconds).name() << endl;
//  cout << seconds << " " << typeid(seconds).name() << endl;
//  cout << days << " " << typeid(days).name() << endl;
//  cout << "" << endl;


  //End testing
  
  const int overflowSeconds = static_cast<int>( nanoSeconds / 1.0e9 ); // Floors
  seconds += overflowSeconds;
  nanoSeconds -= overflowSeconds * 1.0e9;
  const int overflowDays = static_cast<int>( seconds / ( 60.0 * 60.0 * 24.0 ) );
  days += overflowDays;
  seconds -= overflowDays * 60.0 * 60.0 * 24.0;

  //Begin testing

//  cout << "After normalization: " << endl;  
//  cout << nanoSeconds << " " << typeid(nanoSeconds).name() << endl;
//  cout << seconds << " " << typeid(seconds).name() << endl;
//  cout << days << " " << typeid(days).name() << endl;
//  cout << "=========================================" << endl;

 
  //End testing
}

#endif
