/*======================================================================
 FILE: icalvalue_cxx.h
 CREATOR: fnguyen 12/13/01
 (C) COPYRIGHT 2001, Critical Path

 This program is free software; you can redistribute it and/or modify
 it under the terms of either:

    The LGPL as published by the Free Software Foundation, version
    2.1, available at: http://www.gnu.org/licenses/lgpl-2.1.html

  Or:

    The Mozilla Public License Version 1.0. You may obtain a copy of
    the License at http://www.mozilla.org/MPL/
======================================================================*/

#ifndef ICALVALUE_CXX_H
#define ICALVALUE_CXX_H

#include "libical_ical_export.h"
#include "icptrholder_cxx.h"

extern "C"
{
#include "icalerror.h"
#include "icalvalue.h"
};

#include <string>

namespace LibICal
{

    class LIBICAL_ICAL_EXPORT ICalValue
    {
      public:
        ICalValue() throw(icalerrorenum);
        ICalValue(const ICalValue &) throw(icalerrorenum);
        ICalValue & operator=(const ICalValue &) throw(icalerrorenum);
        ~ICalValue();

        explicit ICalValue(icalvalue *) throw(icalerrorenum);
        explicit ICalValue(const icalvalue_kind & kind) throw(icalerrorenum);
        ICalValue(const icalvalue_kind & kind, const std::string & str) throw(icalerrorenum);

        operator  icalvalue *()
        {
            return imp;
        }

        void detach();

      public:
        std::string as_ical_string();
        bool is_valid();
        icalvalue_kind isa();
        int isa_value(void *);

        /* Special, non autogenerated value accessors */
        void set_recur(const struct icalrecurrencetype &v);
        struct icalrecurrencetype get_recur();

        void set_trigger(const struct icaltriggertype &v);
        struct icaltriggertype get_trigger();

        void set_datetimeperiod(const struct icaldatetimeperiodtype &v);
        struct icaldatetimeperiodtype get_datetimeperiod();

      public:
        static icalparameter_xliccomparetype compare(ICalValue & a, ICalValue & b);

        /* Convert enumerations */
        static icalvalue_kind string_to_kind(const std::string & str);
        std::string kind_to_string(const icalvalue_kind & kind);

      public:
        /* BOOLEAN */
        int get_boolean() const;
        void set_boolean(const int &v);

        /* UTC-OFFSET */
        int get_utcoffset() const;
        void set_utcoffset(const int &v);

        /* METHOD */
        enum icalproperty_method get_method() const;
        void set_method(const enum icalproperty_method &v);

        /* CAL-ADDRESS */
        std::string get_caladdress() const;
        void set_caladdress(const std::string & v);

        /* PERIOD */
        struct icalperiodtype get_period() const;
        void set_period(const struct icalperiodtype &v);

        /* STATUS */
        enum icalproperty_status get_status() const;
        void set_status(const enum icalproperty_status &v);

        /* BINARY */
        std::string get_binary() const;
        void set_binary(const std::string & v);

        /* TEXT */
        std::string get_text() const;
        void set_text(const std::string & v);

        /* DURATION */
        struct icaldurationtype get_duration() const;
        void set_duration(const struct icaldurationtype &v);

        /* INTEGER */
        int get_integer() const;
        void set_integer(const int &v);

        /* URI */
        std::string get_uri() const;
        void set_uri(const std::string & v);

        /* ATTACH */
        icalattach *get_attach();
        void set_attach(icalattach *v);

        /* CLASS */
        enum icalproperty_class get_class() const;
        void set_class(const enum icalproperty_class &v);

        /* FLOAT */
        float get_float() const;
        void set_float(const float &v);

        /* QUERY */
        std::string get_query() const;
        void set_query(const std::string & v);

        /* STRING */
        std::string get_string() const;
        void set_string(const std::string & v);

        /* TRANSP */
        enum icalproperty_transp get_transp() const;
        void set_transp(const enum icalproperty_transp &v);

        /* DATE-TIME */
        struct icaltimetype get_datetime() const;
        void set_datetime(const struct icaltimetype &v);

        /* GEO */
        struct icalgeotype get_geo() const;
        void set_geo(const struct icalgeotype &v);

        /* DATE */
        struct icaltimetype get_date() const;
        void set_date(const struct icaltimetype &v);

        /* ACTION */
        enum icalproperty_action get_action() const;
        void set_action(const enum icalproperty_action &v);

      private:
        icalvalue *imp;
    };

}       // namespace LibICal

typedef ICPointerHolder < LibICal::ICalValue > ICalValueTmpPtr;

#endif
