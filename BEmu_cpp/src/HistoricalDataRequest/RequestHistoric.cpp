﻿//------------------------------------------------------------------------------
// <copyright project="BEmu_cpp" file="src/HistoricalDataRequest/HistoricRequest.cpp" company="Jordan Robinson">
//     Copyright (c) 2013 Jordan Robinson. All rights reserved.
//
//     The use of this software is governed by the Microsoft Public License
//     which is included with this distribution.
// </copyright>
//------------------------------------------------------------------------------

#include "HistoricalDataRequest/HistoricRequest.h"
#include "HistoricalDataRequest/HistoricRequestElementDate.h"
#include "HistoricalDataRequest/HistoricRequestElementStringArray.h"
#include "HistoricalDataRequest/HistoricRequestElementInt.h"
#include "HistoricalDataRequest/HistoricRequestElementBool.h"
#include "HistoricalDataRequest/HistoricRequestElementString.h"
#include "BloombergTypes/Datetime.h"
#include "Types/DisplayFormats.h"

namespace BEmu
{
	namespace HistoricalDataRequest
	{
		HistoricRequest::HistoricRequest()
		{
			this->_dtStart = 0;
			this->_dtEnd = 0;
			this->_adjustmentNormalElement = 0;
			this->_adjustmentAbnormalElement = 0;
			this->_adjustmentSplitElement = 0;
			this->_maxDataPointElement = 0;
			this->_periodicityAdjustmentElement = 0;
			this->_periodicityElement = 0;
			this->_overrideOptionsElement = 0;
			this->_pricingOptionElement = 0;

            this->_securities = new HistoricRequestElementStringArray("securities");
            this->_fields = new HistoricRequestElementStringArray("fields");

			this->_periodicityAdjustment = HistDataPeriodicityAdjustmentEnum::actual;
			this->_periodicity = HistDataPeriodicityEnum::daily;
			this->_pricingOption = PricingOptionEnum::price;
			this->_overrideOptions = OverrideOptionsEnum::closingPrice;

			this->_requestType = RequestPtr::historic;
		}

		HistoricRequest::~HistoricRequest()
		{
			delete this->_dtStart;
			this->_dtStart = 0;

			delete this->_dtEnd;
			this->_dtEnd = 0;

			delete this->_securities;
			this->_securities = 0;

			delete this->_fields;
			this->_fields = 0;
		}


		std::vector<std::string> HistoricRequest::securities() const
		{
			return this->_securities->values();
		}

		std::vector<std::string> HistoricRequest::fields() const
		{
			return this->_fields->values();
		}

		bool HistoricRequest::hasStartDate() const
		{
			return this->_dtStart != 0;
		}

		bool HistoricRequest::hasEndDate() const
		{
			return this->_dtEnd != 0;
		}

		Datetime HistoricRequest::dtStart() const
		{
			return this->_dtStart->getDatetime();
		}

		Datetime HistoricRequest::dtEnd() const
		{
			return this->_dtEnd->getDatetime();
		}


		std::vector<Datetime> * HistoricRequest::getDates() const
		{
			std::vector<Datetime> * dates = this->getDatesBeforeMaxPoints();

			if(this->_maxDataPointElement == 0)
				return dates;
			else
			{
				int max = this->_maxDataPointElement->getInt();
				int dateCount = dates->size();
				
				std::vector<Datetime> * result = new std::vector<Datetime>();
				
				for(int i = 0; i < dateCount; i++, max--)
				{
					Datetime dt = dates->at(i);

					if(max >= 0)
					{
						//skip this date
					}
					else
					{
						result->push_back(dt); //add this date
					}
				}

				delete dates;
				dates = 0;

				return result;
			}
		}

		std::vector<Datetime> * HistoricRequest::getDatesBeforeMaxPoints() const
		{
			std::vector<Datetime> * result = new std::vector<Datetime>();

			Datetime dtStart; //BEmu.Session.SendRequest assures that dtStart is not null
			if(this->hasStartDate())
				dtStart = this->dtStart();
			else
			{
				dtStart = Datetime::Today();
				dtStart.addYears(-1);
			}

			Datetime dtEnd;
			if(this->hasEndDate())
				dtEnd = this->dtEnd();
			else
			{
				dtEnd = Datetime::Today();
				dtEnd.addDays(-1);
			}

			if(this->_periodicityAdjustment == HistDataPeriodicityAdjustmentEnum::fiscal) //I use the start of the month as the fiscal adjustment.  It's fake.
			{
				int day = (int)dtStart.day();
				dtStart.addDays(-day);
			}

			Datetime dtCurrent = dtStart;
			Datetime dtToday = Datetime::Today();
			do
			{
				result->push_back(dtCurrent);

				switch(this->_periodicity)
				{
					case HistDataPeriodicityEnum::weekly:
						dtCurrent.addDays(7);
						break;
					case HistDataPeriodicityEnum::monthly:
						dtCurrent.addMonths(1);
						break;
					case HistDataPeriodicityEnum::quarterly:
						dtCurrent.addMonths(3);
						break;
					case HistDataPeriodicityEnum::semi_annually:
						dtCurrent.addMonths(6);
						break;
					case HistDataPeriodicityEnum::yearly:
						dtCurrent.addYears(1);
						break;
					case HistDataPeriodicityEnum::daily:
					default:
						dtCurrent.addDays(1);
						break;
				}

				if(dtCurrent.getWeekDay() == Datetime::Saturday)
					dtCurrent.addDays(2);

				else if(dtCurrent.getWeekDay() == Datetime::Sunday)
					dtCurrent.addDays(1);


			} while(dtCurrent <= dtEnd && dtCurrent <= dtToday);

			return result;
		}


		void HistoricRequest::append(const char* name, const char* value)
		{
			if(strncmp(name, "securities", 11) == 0)
				this->_securities->appendValue(value);

			else if(strncmp(name, "fields", 7) == 0)
				this->_fields->appendValue(value);

			else
				throw requestEx;
		}


		void HistoricRequest::set(const char* name, const char* value)
		{
			if(strncmp(name, "startDate", 10) == 0)
			{
				Datetime dt;
				if(DisplayFormats::HistoricalOrReferenceRequests_TryParseInput(value, dt))
					this->_dtStart = new HistoricRequestElementDate("startDate", dt);
				else
					throw requestEx;
			}
			else if(strncmp(name, "endDate", 8) == 0)
			{
				Datetime dt;
				if(DisplayFormats::HistoricalOrReferenceRequests_TryParseInput(value, dt))
					this->_dtEnd = new HistoricRequestElementDate("endDate", dt);
				else
					throw requestEx;
				
			}
			else if(strncmp(name, "periodicityAdjustment", 22) == 0)
			{
				if(strncmp(value, "CALENDAR", 9) == 0)
					this->_periodicityAdjustment = HistDataPeriodicityAdjustmentEnum::calendar;
				
				else if(strncmp(value, "FISCAL", 7) == 0)
					this->_periodicityAdjustment = HistDataPeriodicityAdjustmentEnum::fiscal;
				
				else if(strncmp(value, "ACTUAL", 7) == 0)
					this->_periodicityAdjustment = HistDataPeriodicityAdjustmentEnum::actual;

				else
					throw requestEx;
			}
			else if(strncmp(name, "periodicitySelection", 21) == 0)
			{
				if(strncmp(value, "WEEKLY", 7) == 0)
					this->_periodicity = HistDataPeriodicityEnum::weekly;
				
				else if(strncmp(value, "MONTHLY", 8) == 0)
					this->_periodicity = HistDataPeriodicityEnum::monthly;
				
				else if(strncmp(value, "QUARTERLY", 10) == 0)
					this->_periodicity = HistDataPeriodicityEnum::quarterly;
				
				else if(strncmp(value, "SEMI_ANNUALLY", 14) == 0)
					this->_periodicity = HistDataPeriodicityEnum::semi_annually;
				
				else if(strncmp(value, "YEARLY", 7) == 0)
					this->_periodicity = HistDataPeriodicityEnum::yearly;
				
				else if(strncmp(value, "DAILY", 6) == 0)
					this->_periodicity = HistDataPeriodicityEnum::daily;

				else
					throw requestEx;
			}
			else if(strncmp(name, "pricingOption", 14) == 0)
			{
				if(strncmp(value, "PRICING_OPTION_YIELD", 21) == 0)
					this->_pricingOption = PricingOptionEnum::yield;
				
				else if(strncmp(value, "PRICING_OPTION_PRICE", 21) == 0)
					this->_pricingOption = PricingOptionEnum::price;

				else
					throw requestEx;
			}
			else if(strncmp(name, "overrideOption", 15) == 0)
			{
				if(strncmp(value, "OVERRIDE_OPTION_GPA", 20) == 0)
					this->_overrideOptions = OverrideOptionsEnum::averagePrice;

				else if(strncmp(value, "OVERRIDE_OPTION_CLOSE", 22) == 0)
					this->_overrideOptions = OverrideOptionsEnum::closingPrice;

				else
					throw requestEx;
			}
			else
				throw requestEx;
		}

		void HistoricRequest::set(const char* name, bool value)
		{
			if(strncmp(name, "adjustmentNormal", 17) == 0)
				this->_adjustmentNormalElement = new HistoricRequestElementBool("adjustmentNormal", value);

			else if(strncmp(name, "adjustmentAbnormal", 19) == 0)
				this->_adjustmentAbnormalElement = new HistoricRequestElementBool("adjustmentAbnormal", value);
			
			else if(strncmp(name, "adjustmentSplit", 16) == 0)
				this->_adjustmentSplitElement = new HistoricRequestElementBool("adjustmentSplit", value);

			else
				throw requestEx;
		}

		void HistoricRequest::set(const char* name, int value)
		{
			if(strncmp(name, "maxDataPoints", 14) == 0)
				this->_maxDataPointElement = new HistoricRequestElementInt("maxDataPoints", value);
			
			else
				throw requestEx;
		}

	}
}