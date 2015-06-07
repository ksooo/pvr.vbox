/*
*      Copyright (C) 2015 Sam Stenvall
*
*  This Program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  This Program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with XBMC; see the file COPYING.  If not, write to
*  the Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
*  MA 02110-1301  USA
*  http://www.gnu.org/copyleft/gpl.html
*
*/

#include "Guide.h"
#include "Utilities.h"
#include "tinyxml2.h"
#include "../vbox/ContentIdentifier.h"

using namespace xmltv;
using namespace tinyxml2;

Guide::Guide(const XMLElement *m_content)
{
  for (const XMLElement *element = m_content->FirstChildElement("channel");
    element != NULL; element = element->NextSiblingElement("channel"))
  {
    // Populate the lookup table which maps XMLTV IDs to display names
    std::string channelId = Utilities::UrlDecode(element->Attribute("id"));
    std::string displayName = element->FirstChildElement("display-name")->GetText();

    AddDisplayNameMapping(displayName, channelId);

    // Create a schedule for the channel
    m_schedules[channelId] = SchedulePtr(new Schedule);
  }

  for (const XMLElement *element = m_content->FirstChildElement("programme");
    element != NULL; element = element->NextSiblingElement("programme"))
  {
    // Extract the channel name and the programme
    std::string channelId = Utilities::UrlDecode(element->Attribute("channel"));
    xmltv::ProgrammePtr programme(new Programme(element));

    // Add the programme to the channel's schedule
    m_schedules[channelId]->AddProgramme(programme);
  }
}

const SchedulePtr Guide::GetSchedule(const std::string &channelId) const
{
  auto it = m_schedules.find(channelId);

  if (it != m_schedules.cend())
    return it->second;

  return nullptr;
}
