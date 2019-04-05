#pragma once

#include "../Engine.hpp"
#include "../Client.hpp"

namespace source
{
	namespace feature
	{
		enum notify_type
		{
			nt_error,
			nt_warning,
			nt_info,
			nt_success
		};

		struct notification_s
		{
			notify_type m_type;
			string		m_msg = "";
			DWORD		m_show_time = 0;
			DWORD		m_show_sec = 0;
			bool		m_draw = true;
		};

		class Notification : public Singleton< Notification >
		{
		public:
			auto RenderNotification() -> void;
			auto AddNotification( DWORD show_sec , notify_type type , const char* format , ... ) -> void;

		public:
			vector<notification_s> m_notification;
		};
	}
}