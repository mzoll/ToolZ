#include "ToolZ/HitSorting_FirstHit.h"

template<>
I3RecoPulse
HitSorting::Accumulator<I3RecoPulse>::operator()(
  const std::vector<I3RecoPulse>& responsevec) const
{
  if (responsevec.empty())
    log_fatal("empty response vector");
  
  I3RecoPulse acc_response=responsevec.front();
    
  double acc_charge(0.);
  uint8_t acc_flags(0);
  BOOST_FOREACH(const I3RecoPulse& pulse, responsevec) {
    acc_charge += pulse.GetCharge();
    acc_flags = (acc_flags | pulse.GetFlags());
  }
  acc_response.SetCharge(acc_charge);
  acc_response.SetFlags(acc_flags);

  return acc_response;
};