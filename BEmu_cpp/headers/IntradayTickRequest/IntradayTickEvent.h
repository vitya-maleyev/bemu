//------------------------------------------------------------------------------------------------
// <copyright project="BEmu_cpp" file="headers/IntradayTickRequest/IntradayTickEvent.h" company="Jordan Robinson">
//     Copyright (c) 2013 Jordan Robinson. All rights reserved.
//
//     The use of this software is governed by the Microsoft Public License
//     which is included with this distribution.
// </copyright>
//------------------------------------------------------------------------------------------------

#pragma once

#include <vector>
#include "BloombergTypes/EventPtr.h"
#include <boost/shared_ptr.hpp>

namespace BEmu
{
	class MessagePtr;

	namespace IntradayTickRequest
	{
		class IntradayTickRequest;

		class IntradayTickEvent : public EventPtr
		{
			private:
				//std::vector<MessagePtr*> *_messages;
				std::vector< boost::shared_ptr<MessagePtr> > * _messages;

				//std::vector<MessagePtr*>* GenerateMessages();
				std::vector< boost::shared_ptr<MessagePtr> > * GenerateMessages();
				
				//IntradayTickRequest *_internal;
				boost::shared_ptr<IntradayTickRequest> _internalP;

			public:
				//IntradayTickEvent(IntradayTickRequest *request);
				IntradayTickEvent(boost::shared_ptr<IntradayTickRequest> request);
				~IntradayTickEvent();

				//virtual std::vector<MessagePtr*> * getMessages() const;
				virtual std::vector< boost::shared_ptr<MessagePtr> > getMessages() const;
		};
	}
}