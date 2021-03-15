#pragma once
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>

#include "../utils/cout_utilities.h"
#include "../utils/Logger.h"

namespace engine
	{
	class Message
		{
		private:
			static std::string filter_last_newline(const std::string& string) 
				{
				if (string.length() > 0 && string[string.length() - 1] == '\n') { return string.substr(0, string.length() - 1); }
				else { return string; }
				}

		public:
			enum class Type { log, dgn, inf, wrn, err };
			Message(Type type, const std::string& string, std::chrono::time_point<std::chrono::system_clock> time) noexcept
				: type(type), string(string), time(time)
				{}
			Message(Type type, const std::string & string) noexcept
				: type(type), string(string), time(std::chrono::system_clock::now())
				{}
			Message(const std::string & string) noexcept
				: type(Type::log), string(string), time(std::chrono::system_clock::now())
				{}

			Type type = Type::log;
			std::string string = "";
			std::chrono::time_point<std::chrono::system_clock> time;

			const char* out_type() const noexcept
				{
				switch (type)
					{
					case Message::Type::log: return "[LOG]";
					case Message::Type::dgn: return "[DGN]";
					case Message::Type::inf: return "[INF]";
					case Message::Type::wrn: return "[WRN]";
					case Message::Type::err: return "[ERR]";
					}
				}
			const char* out_type_verbose() const noexcept
				{
				switch (type)
					{
					case Message::Type::log: return "[LOG]";
					case Message::Type::dgn: return "[DIAGNOSTIC]";
					case Message::Type::inf: return "[INFO]";
					case Message::Type::wrn: return "[WARNING]";
					case Message::Type::err: return "[ERROR]";
					}
				}
			utils::cout::color out_type_color() const noexcept
				{
				switch (type)
					{
					case Message::Type::log: return utils::cout::color::white;
					case Message::Type::dgn: return utils::cout::color::magenta;
					case Message::Type::inf: return utils::cout::color::cyan;
					case Message::Type::wrn: return utils::cout::color::yellow;
					case Message::Type::err: return utils::cout::color::red;
					}
				}

			static Message log(const std::string& string) { return {Type::log, filter_last_newline(string), std::chrono::system_clock::now()}; }
			static Message dgn(const std::string& string) { return {Type::dgn, filter_last_newline(string), std::chrono::system_clock::now()}; }
			static Message inf(const std::string& string) { return {Type::inf, filter_last_newline(string), std::chrono::system_clock::now()}; }
			static Message wrn(const std::string& string) { return {Type::wrn, filter_last_newline(string), std::chrono::system_clock::now()}; }
			static Message err(const std::string& string) { return {Type::err, filter_last_newline(string), std::chrono::system_clock::now()}; }

			//ON LINE VERSION
			friend std::ostream& operator<<(std::ostream& os, const Message& m)
				{
				size_t beg = 0;
				size_t end = m.string.find_first_of('\n', beg);
				if (end == std::string::npos) { end = m.string.length(); }

				//First line
				os << utils::cout::color::dw << std::setw(18) << m.time.time_since_epoch().count() << ' ';

				os << m.out_type_color() << m.out_type();

				os << utils::cout::color::dw << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

				//Other lines
				while (true)
					{
					if (end == m.string.length()) { break; }
					else
						{
						beg = end + 1;
						end = m.string.find_first_of('\n', beg);
						if (end == std::string::npos) { end = m.string.length(); }
						}

					os << std::setw(24) << m.out_type_color() << '|';
					os << utils::cout::color::dw << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';
					}

				return os;
				}

			//TAG AS SEPARATOR VERSION
			/*friend std::ostream& operator<<(std::ostream& os, const Message& m)
				{
				size_t beg = 0;
				size_t end = m.string.find_first_of('\n', beg);
				if (end == std::string::npos) { end = m.string.length(); }

				//Data line
				os << "_________________________________\n";
				os << m.out_type_color() << ' ' << std::left << std::setw(12) << m.out_type_verbose() << ' ' << std::right << std::setw(18) << m.time.time_since_epoch().count() << '\n';
				//First line
				os << utils::cout::color::dw << ' ' << std::string_view(m.string).substr(beg, end - beg) << '\n';

				//Other lines
				while (true)
					{
					if (end == m.string.length()) { break; }
					else
						{
						beg = end + 1;
						end = m.string.find_first_of('\n', beg);
						if (end == std::string::npos) { end = m.string.length(); }
						}

					os << std::string_view(m.string).substr(beg, end - beg) << '\n';
					}

				return os;
				}*/
		};

	inline utils::logger<Message> logger;
	}