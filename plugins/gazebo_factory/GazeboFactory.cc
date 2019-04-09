/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <ignition/common/Console.hh>
#include <ignition/msgs/entity_factory.pb.h>
#include <ignition/msgs/boolean.pb.h>
#include "GazeboFactory.hh"

using namespace ignition;
using namespace ignition::launch;

/////////////////////////////////////////////////
GazeboFactory::GazeboFactory()
  : ignition::launch::Plugin()
{
}

/////////////////////////////////////////////////
void GazeboFactory::Load(const tinyxml2::XMLElement *_elem)
{
  const tinyxml2::XMLElement *elem;

  msgs::EntityFactory req;

  // Get the world file
  elem = _elem->FirstChildElement("uri");
  if (elem)
  {
    std::string sdf = std::string("<?xml version='1.0'?><sdf version='1.6'>") +
      "<include><uri>" + elem->GetText() + "</uri></include></sdf>";
    req.set_sdf(sdf);
  }

  unsigned int timeout = 2000;
  msgs::Boolean rep;
  bool result;
  bool executed = this->node.Request("/world/default/create",
      req, timeout, rep, result);

  if (executed)
  {
    if (result)
    {
      if (!rep.data())
      {
        ignerr << "Factory service call completed, but returned a false value."
          << "You may have an invalid request. Check the configuration.\n";
      }
      else
        igndbg << "Factory service call succeeded.\n";
    }
    else
    {
      ignerr << "Factory service call failed.\n";
    }
  }
  else
  {
    ignerr << "Factor service call timed out.\n";
  }

  igndbg << "Loaded GazeboFactory plugin.\n";
}
