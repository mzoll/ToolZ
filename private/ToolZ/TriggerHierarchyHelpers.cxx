/**
 * \file TriggerHierarchyHelpers.cxx
 *
 * copyright (c) 2011
 * the IceCube Collaboration
 * $Id: TriggerHierarchyHelpers.cxx 153958 2017-03-17 13:42:47Z mzoll $
 *
 * @date $Date: 2013-10-22$
 * @author mzoll <marcel.zoll@fysik.su.se>
 */

#include "ToolZ/TriggerHierarchyHelpers.h"

#include <boost/foreach.hpp>
#include <algorithm>
#include <vector>

I3TriggerHierarchy ClipTriggerHierarchy(
  const I3TriggerHierarchy &trigHier,
  const double tw_start,
  const double tw_stop,
  const std::vector<int>& configIDs) 
{
  /*  The Structure of a TriggerHierarchy is as follows
   *  object<>:
   *  -> globalTrigger (Merged Readout window including all paddings)
   *  -> individual trigger throughput (The padding arround each trigger)
   *  --> the actual trigger (time whenever the trigger condition was fullfilled)
   *  -> next trigger troughput
   *  --> next trigger window
   */
  I3TriggerHierarchy subtrigs; // new trig hierarchy

  // first make and insert global merged trigger, it'll
  // ALWAYS MAKE THIS EVEN IF THERE AREN'T TRIGS TO MERGE
  I3Trigger globTrigger;
  globTrigger.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::MERGED);
  globTrigger.SetTriggerFired(true);
  globTrigger.SetTriggerTime(tw_start);
  globTrigger.SetTriggerLength(tw_stop-tw_start);

  I3TriggerHierarchy::iterator subIter = subtrigs.insert(subtrigs.end(), globTrigger);
  
  if (configIDs.size()==0) { //take every trigger there is
    //BOOST_FOREACH(const I3Trigger &trigger, trigHier) {
    for (I3TriggerHierarchy::iterator trigIter = trigHier.begin(); trigIter != trigHier.end(); ++trigIter) {
      if ((trigIter->GetTriggerKey().GetSource() != TriggerKey::GLOBAL) //no merged or gloabltriggers in the concideration
        && ((tw_start <= trigIter->GetTriggerTime()) && (trigIter->GetTriggerTime() < tw_stop) )) 
      {
        //make and insert throughput
        I3Trigger tpTrigger;
        tpTrigger.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
        tpTrigger.SetTriggerFired(true);
        tpTrigger.SetTriggerTime(trigIter->GetTriggerTime());
        tpTrigger.SetTriggerLength(trigIter->GetTriggerLength());

        //append the troughput trigger to the global trigger
        I3TriggerHierarchy::iterator eachTrig = subtrigs.append_child(subIter, tpTrigger);
        //insert actual trigger as child of the individual throughput
        subtrigs.append_child(eachTrig, trigIter);       
      }
    }
  } 
  else { //take only triggers in the list
    for (I3TriggerHierarchy::iterator trigIter = trigHier.begin(); trigIter != trigHier.end(); ++trigIter) {
      if ( (find(configIDs.begin(), configIDs.end(), trigIter->GetTriggerKey().GetConfigIDOptional()) != configIDs.end()) 
        && ((tw_start <= trigIter->GetTriggerTime()) && (trigIter->GetTriggerTime() < tw_stop) ))
      {      
        //make and insert throughput
        I3Trigger tpTrigger;
        tpTrigger.GetTriggerKey() = TriggerKey(TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
        tpTrigger.SetTriggerFired(true);
        tpTrigger.SetTriggerTime(trigIter->GetTriggerTime());
        tpTrigger.SetTriggerLength(trigIter->GetTriggerLength());

        //append the troughput trigger to the global trigger
        I3TriggerHierarchy::iterator eachTrig = subtrigs.append_child(subIter, tpTrigger);
        //insert actual trigger as child of the individual throughput
        subtrigs.append_child(eachTrig, trigIter);
      }
    } 
  }

  return subtrigs;
}

//____________________________________________________________________________
I3TriggerHierarchy ClipTriggerHierarchy(
  const I3TriggerHierarchy &trigHier,
  const I3TimeWindow& twindow,
  const std::vector<int> &configIDs)
{ return ClipTriggerHierarchy(trigHier, twindow.GetStart(), twindow.GetStop(), configIDs); };
  
  
//____________________________________________________________________________
I3TriggerHierarchy UniteTriggerHierarchies (const I3TriggerHierarchy &trigA,
                                            const I3TriggerHierarchy &trigB) {
  log_fatal("Implement me");
  //There are two ways to approach this:
  // 1. just take the united triggerHiererarchy as the direct sum of both trigger UniteTriggerHierarchies
  // -OR-
  // 2. try to figure out matching triggers in the Hierarchies and write them only once; also throughput triggers have to be adapted accordingly
  using namespace I3TriggerHierarchyUtils;
  
  I3TriggerHierarchy uth; //the united TriggerHierarchy

  return uth; //NOTE fix warning for now, by returning a blank I3TriggerHierarchy; however this should be rectified somewhere in the future
  
  /*
  ///for now assume that there is only one global throughput per trigger    
  I3TriggerHierarchy::iterator globA = Find(trigA, TriggerKey::GLOBAL, TriggerKey::THROUGHPUT); // with iter at end i can forward iterate
  I3TriggerHierarchy::iterator globB = Find(trigB, TriggerKey::GLOBAL, TriggerKey::THROUGHPUT);
  
  const double separation = TimeSeparation(globA->GetTriggerTime(),
                                            globA->GetTriggerTime()+globA->GetTriggerLength(),
                                            globB->GetTriggerTime(),
                                            globB->GetTriggerTime()+globB->GetTriggerLength());
  
  if (std::isnan(separation)) { //inclusion
    //take the eralier trigger and insert there the includes subtriggers of
    // TODO make an effort not to double insert subtriggers 
    
    if (globA->GetTriggerTime()<globB->GetTriggerTime()) { // A before B
      uth = trigA;
      // append triggers from B into th except the global one
    }
    else { // B before A
      uth = trigB;
      // append triggers from A to th except the global one
    }
  }
  else if (separation<=0) { //partial inclusion
    // append triggers from A and B to th; create a new global readout
  }
  else if (separation>0) { //separted
    // just glue together A and B
  }
  //usefull function in this context
  //const std::vector<Type> GetDaughters(const I3Tree<Type>& t, const Type& p)
  //void AppendChild(I3Tree<Type>& t, const Type& parent, const Type& child)
  */
}
