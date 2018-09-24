/**
* \file HitSorting.cxx
*
* (c) 2012 the IceCube Collaboration
*
* $Id: Hitclasses.cxx 153492 2017-02-23 16:51:31Z mzoll $
* \version $Revision: 153492 $
* \date $Date: 2017-02-23 17:51:31 +0100 (Thu, 23 Feb 2017) $
* \author Marcel Zoll <marcel.zoll@fysik.su.se>
*/

#include "ToolZ/Hitclasses.h"

using namespace std;

//================== class AbsHit =====================
AbsHit::AbsHit(
  const CompactHash di,
  const double t)
: domIndex_(di),
  time_(t)
{};

std::ostream& operator<< (std::ostream& oss, const AbsHit& h){
  oss << "Hit(" <<
  " domIndex : " << h.GetDOMIndex() <<
  ", time : " << h.GetTime() << " )";
  return oss;
};


//================== class AbsDAQHit =====================
AbsDAQHit::AbsDAQHit(
  const CompactHash di,
  const int64_t daqTicks) :
  domIndex_(di),
  daqTicks_(daqTicks)
{};

std::ostream& operator<< (std::ostream& oss, const AbsDAQHit& h){
  oss << "DAQHit(" <<
  " domIndex : " << h.GetDOMIndex() <<
  ", daqTicks : " << h.GetDAQTicks() << " )";
  return oss;
};


//================ CLASS Hit ===================

std::ostream& operator<<(std::ostream& oss, const Hit& h) {
  oss << (AbsHit)h;
  return oss;
};


//================ Helpers ===================

//specialize the GetTime() to the ResponseObject
template <>
double GetInferredTime<I3RecoPulse>(const I3RecoPulse &r)
  {return r.GetTime();};

template <>
double GetInferredTime<I3DOMLaunch>(const I3DOMLaunch &r)
  {return r.GetStartTime();};

template <>
double GetInferredTime<I3MCHit>(const I3MCHit &r)
  {return r.GetTime();};

template <>
double GetInferredTime<I3MCPulse>(const I3MCPulse &r)
  {return r.time;};
