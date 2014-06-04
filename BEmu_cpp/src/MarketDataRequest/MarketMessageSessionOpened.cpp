﻿//------------------------------------------------------------------------------
// <copyright project="BEmu_cpp" file="src/MarketDataRequest/MarketMessageSessionOpened.cpp" company="Jordan Robinson">
//     Copyright (c) 2013 Jordan Robinson. All rights reserved.
//
//     The use of this software is governed by the Microsoft Public License
//     which is included with this distribution.
// </copyright>
//------------------------------------------------------------------------------

#include "MarketDataRequest/MarketMessageSessionOpened.h"

namespace BEmu
{
	namespace MarketDataRequest
	{
		MarketMessageSessionOpened::MarketMessageSessionOpened() : MessagePtr(Name("SessionStarted"), CorrelationId((long)0))
		{
		}

		const char* MarketMessageSessionOpened::topicName() const
		{
			return "";
		}

		//std::stack<ElementPtr*> MarketMessageSessionOpened::getRootElements() const
		std::stack< boost::shared_ptr<ElementPtr> > MarketMessageSessionOpened::getRootElements() const
		{
			std::stack< boost::shared_ptr<ElementPtr> > result;
			return result;

			//std::stack<ElementPtr*> result; //no contents
			//return result;
		}

		void MarketMessageSessionOpened::markRootElementsDeleted()
		{
			//do nothing
		}

	}
}