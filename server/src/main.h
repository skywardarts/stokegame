#include <cstddef>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <csignal>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>
#include <set>
#include <list>
#include <queue>
#include <algorithm>         // copy, min.
#include <cassert>
#include <deque>

#include <stdlib.h>
#include <string.h>
//#include <time.h>
//#include <math.h>
#include <errno.h>
//#include <signal.h>
//#include <typeinfo.h>
#include <sys/types.h>
#include <stdio.h>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/config.hpp>  // BOOST_NO_STDC_NAMESPACE.
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/detail/ios.hpp>  // failure.
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/ref.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/random.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/algorithm/string.hpp>




#define NEXTGEN_PLATFORM_DOS 0
#define NEXTGEN_PLATFORM_WINDOWS 1
#define NEXTGEN_PLATFORM_OS2 2
#define NEXTGEN_PLATFORM_APPLE 3
#define NEXTGEN_PLATFORM_INTEL 4
#define NEXTGEN_PLATFORM_UNIX 5

#if defined(__MSDOS__) || defined(MSDOS)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_DOS
#elif defined(_WIN64)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_WINDOWS
#elif defined(_WIN32) || defined(_WIN32_WCE) || defined(__WIN32__) || defined(WIN32)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_WINDOWS
#elif defined(OS_2) || defined(__OS2__) || defined(OS2)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_OS2
#elif defined(__APPLE_CC__)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_APPLE
#elif defined(__INTEL_COMPILER)
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_INTEL
#else
    #define NEXTGEN_PLATFORM NEXTGEN_PLATFORM_UNIX
#endif

#if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
    #define _WIN32_WINNT 0x0900
    #include <iphlpapi.h>
    #include <Psapi.h>
    #include <stdio.h>
    #include <Wincrypt.h>
#endif

#define NEXTGEN_SHARED_DATA(this_type, data_type, ...) \
    private: boost::shared_ptr<data_type> ng_data; \
    public: this_type(this_type& t) : ng_data(t.ng_data) { } \
    public: this_type(this_type const& t) : ng_data(t.ng_data) { } \
    public: template<typename ...ng_argument_types> this_type(ng_argument_types&& ...argument_list) : ng_data(new data_type(argument_list...)) { __VA_ARGS__ } \
    public: template<typename ng_argument_type> this_type(ng_argument_type&& t, typename boost::enable_if<boost::is_base_of<this_type, ng_argument_type>>::type* dummy = 0) : ng_data(t.ng_data) { } \
    public: bool operator==(this_type const& t) const { return &(*this->ng_data) == &(*t.ng_data); } \
    public: bool operator==(int t) const { if(t == 0) return this->ng_data == 0; else return 0; } \
    public: bool operator!=(this_type const& t) const { return !this->operator==(t); } \
    public: bool operator!=(int t) const { return !this->operator==(t); } \
    public: boost::shared_ptr<data_type> const& operator->() const { return this->ng_data; }


bool DEBUG_MESSAGES = 1;
bool DEBUG_MESSAGES2 = 1;
bool DEBUG_MESSAGES3 = 1;
bool DEBUG_MESSAGES4 = 1;

void find_and_replace(std::string& source, std::string const& find, std::string const& replace)
{
	for(std::string::size_type i = 0; (i = source.find(find, i)) != std::string::npos;)
	{
		source.replace(i, find.length(), replace);

		i += replace.length() - find.length() + 1;
	}
}

template<typename element_type>
std::string to_string(element_type element)
{
	return boost::lexical_cast<std::string>(element);
}

template<typename element_type>
int to_int(element_type element)
{
	return boost::lexical_cast<int>(element);
}

namespace nextgen
{
    typedef std::size_t size_t;
    typedef std::string string;
    typedef std::vector<string> string_list;
    typedef std::wstring wstring;
    typedef std::ostream ostream;
    typedef std::istream istream;
    typedef std::runtime_error error;

	typedef boost::int8_t int8_t;
    typedef boost::int32_t int16_t;
    typedef boost::int32_t int32_t;
    typedef boost::int32_t int32_t;
    typedef boost::uint8_t uint8_t;
    typedef boost::uint8_t byte; //typedef unsigned char byte;
    typedef boost::uint16_t uint16_t;
    typedef boost::uint32_t uint32_t;
    typedef boost::uint64_t uint64_t;

    template<typename A, typename B>
    class hash_map : public boost::unordered_map<A, B> {};

    template<typename A>
    class deque : public std::deque<A> {};

    template<typename A>
    class list : public std::list<A> {};

    template<typename A>
    class array : public std::vector<A> {};

    template<typename A, typename B>
    class tuple : public boost::tuple<A, B> {};

    template<typename T>
    class reference : public boost::reference_wrapper<T> {};

    template<typename T, typename A>
    inline T any_cast(A a)
    {
        return boost::any_cast<T>(a);
    }

    template<typename T, typename A>
    inline T cast(A a)
    {
        return boost::lexical_cast<T>(a);
    }


    #if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
        const int null = NULL;
        typedef HANDLE handle;
        typedef HWND window_handle;
        typedef HMODULE module_handle;
    	typedef PVOID void_pointer;
    	typedef STARTUPINFOA startup_information;
    	typedef PROCESS_INFORMATION pProcess_information;
    #else
        const int null = 0;
        typedef int handle;
        typedef int window_handle;
        typedef int module_handle;
        typedef void* void_pointer;
    	typedef int startup_information;
    	typedef int process_information;
    #endif

    class byte_array
    {
        public: void reverse_bytes(byte* input, byte* output, size_t length)
        {
            for(size_t i = 0; i < length; ++i)
                output[i] = input[length - i - 1];
        }

        public: template<typename element_type> void read(element_type&& output, size_t length = 0)
        {
            auto self = *this;

            std::istream data_stream(&self->data);

            length = length > 0 ? length : sizeof(element_type);
std::cout << length << std::endl;
            memset((byte*)&output, 0, length);

            byte* input = (byte*)&output;

            if(length > self.length())
                length = self.length();
std::cout << length << std::endl;
            //bool little_endian = true;

            if(self->little_endian)
                for(size_t i = length; i > 0; --i)
                    data_stream >> input[i-1];
            else
                for(size_t i = 0; i < length; ++i)
                    data_stream >> input[i];
        }

        public: template<typename element_type> void write(element_type&);
        public: template<typename element_type> void write(element_type&&, size_t);

        public: template<typename element_type> byte_array operator>>(element_type&& output)
        {
            auto self = *this;

            self.read(output);

            return *this;
        }

        public: template<typename element_type> byte_array operator<<(element_type&& input)
        {
            auto self = *this;

            self.write(input);

            return *this;
        }

        public: size_t available()
        {
            auto self = *this;

            return self->data.size();
        }

        public: size_t size()
        {
            auto self = *this;

            return self->data.size();
        }

        public: size_t length()
        {
            auto self = *this;

            return self->data.size();
        }

        public: bool is_little_endian()
        {
            int i = 1;

            char *p = (char*)&i;

            if (p[0] == 1)
                return true;
            else
                return false;
        }

        public: std::string debug()
        {
            auto self = *this;

            std::string output;
/*
            char ch[3];

            for(int i = 0, l = self->data.size(); i < l; ++i)
            {
                sprintf(ch, "\\x%02x", self->data[i]);

                output += ch;
            }*/

            return output;
        }

        public: asio::streambuf& get_buffer()
        {
            auto self = *this;

            return self->data;
        }

        private: struct variables
        {
            variables()
            {

            }

            variables(byte_array& ba, size_t length)
            {
                // todo(daemn) fix this
                std::ostream ostream(&this->data);

                ostream << &ba->data;
            }

            ~variables()
            {

            }

            bool little_endian;
            asio::streambuf data;
        };

        NEXTGEN_SHARED_DATA(byte_array, variables,
        {
            auto self = *this;

            self->little_endian = self.is_little_endian();
        });
    };

    template<typename element_type>
    inline void byte_array::write(element_type&& input, size_t length)
    {
        auto self = *this;

        std::ostream data_stream(&self->data);

        length = length > 0 ? length : sizeof(element_type);
std::cout << "A" << length << std::endl;
        byte* output = (byte*)&input;

        //bool little_endian = true;

        if(self->little_endian)
            for(size_t i = length; i > 0; --i)
                data_stream << output[i-1];
        else
            for(size_t i = 0; i < length; ++i)
                data_stream << output[i];
    }


    template<typename element_type>
    inline void byte_array::write(element_type& input)
    {
        auto self = *this;
std::cout << "Z" << std::endl;
        self.write(input, 0);
    }


    template<>
    inline void byte_array::write(string& input)
    {
        auto self = *this;

        std::ostream data_stream(&self->data);

        size_t length = input.length();

        for(size_t i = 0; i < length; ++i)
            data_stream << input[i];
    }

    template<>
    inline void byte_array::write(byte_array& input)
    {
        auto self = *this;

        std::ostream ostream(&self->data);

        ostream << &input->data;
    }

    template<typename element_type>
    class singleton
    {
        public: explicit singleton();

        public: virtual ~singleton();

        public: static element_type& instance();
        public: static element_type* pointer_instance();

        private: static element_type* element;
    };

    template<typename element_type>
    element_type* singleton<element_type>::element = 0;

    template<typename element_type>
    inline singleton<element_type>::singleton()
    {
        //Assert(!instance);
    }

    template<typename element_type>
    inline singleton<element_type>::~singleton()
    {
        //Assert(instance);

        element = 0;
    }

    template<typename element_type>
    inline element_type& singleton<element_type>::instance()
    {
        return *pointer_instance();
    }

    template<typename element_type>
    inline element_type* singleton<element_type>::pointer_instance()
    {
        if(element == null)
            element = new element_type();

        //Assert(element);

        return element;
    }

	template<typename callback_type>
	class event
	{
	    public: typedef std::list<callback_type> callback_list_type;

		public: template<typename ...element_type_list> void call(element_type_list&& ...element_list)
		{
			for(typename callback_list_type::const_iterator i = this->list.begin(), l = this->list.end(); i != l; ++i)
			{
				(*i)(element_list...);
			}
		}

		public: void add(callback_type t)
		{
			this->list.push_back(t);
		}

		public: void remove(callback_type& t)
		{
			this->list.remove(t);
		}

		/**
		* Operators
		*/

		public: bool operator!()
		{
			if(this->list.size() == 0)
				return true;
			else
				return false;
		}

		public: template<typename ...element_list_type> event<callback_type>& operator()(element_list_type&& ...element_list)
		{
			this->call(element_list...);

			return *this;
		}

		public: event<callback_type>& operator+(callback_type& t)
		{
			this->add(t);

			return *this;
		}

		public: event<callback_type>& operator-(callback_type& t)
		{
			this->remove(t);

			return *this;
		}

		public: void operator+=(callback_type& t)
		{
			this->add(t);
		}

		public: template<typename element_type> void operator+=(element_type&& a)
		{
			this->add(callback_type(a));
		}

		public: void operator-=(callback_type& t)
		{
			this->remove(t);
		}

		public: template<typename element_type> void operator-=(element_type&& a)
		{
			this->remove(callback_type(a));
		}

		private: callback_list_type list;
	};

    namespace detail
    {
        template<typename element_type>
        class random
        {
            public: random(element_type a1) : gen(static_cast<unsigned long>(std::time(0))), dst(0, a1), rand(gen, dst) {}
            public: random(element_type a1, element_type a2) : gen(static_cast<unsigned long>(std::time(0))), dst(a1, a2), rand(gen, dst) {}

            public: element_type get() { return this->rand(); }

            public: std::ptrdiff_t operator()(std::ptrdiff_t arg)
            {
                return static_cast<std::ptrdiff_t>(this->rand());
            }

            private: boost::mt19937 gen;
            private: boost::uniform_int<element_type> dst;
            private: boost::variate_generator<boost::mt19937, boost::uniform_int<element_type>> rand;
        };
    }

    template<typename element_type>
    element_type random(element_type a1, element_type a2)
    {
        return detail::random<element_type>(a1, a2).get();
    }

    #if PLATFORM == PLATFORM_UNIX
        #include <sys/time.h>
        #include <stdio.h>
        #include <unistd.h>
    #endif

    class timer
    {
        public: void start() const
        {
            auto self = *this;

            #if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
                QueryPerformanceCounter(&self->begin);
            #elif NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_UNIX
                gettimeofday(&self->begin, NULL);
            #endif
        }

        public: float stop() const
        {
            auto self = *this;

            #if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
                LARGE_INTEGER end;
                QueryPerformanceCounter(&end);

                return set_precision(float(end.QuadPart - self->begin.QuadPart) / self->freq.QuadPart, 2);
            #elif NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_UNIX
                timeval end;

                gettimeofday(&end, NULL);

                long seconds  = end.tv_sec  - self->begin.tv_sec;
                long useconds = end.tv_usec - self->begin.tv_usec;

                return float((((seconds) * 1000 + useconds/1000.0) + 0.5) / 1000);
            #endif
        }

        private: struct variables
        {
            variables()
            {
                #if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
                    QueryPerformanceFrequency(&this->freq);

                    QueryPerformanceCounter(&this->begin);
                #elif NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_UNIX
                    gettimeofday(&this->begin, NULL);
                #endif
            }

            ~variables()
            {

            }

            #if NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_WINDOWS
                LARGE_INTEGER freq, begin;
            #elif NEXTGEN_PLATFORM == NEXTGEN_PLATFORM_UNIX
                timeval begin;
            #endif
        };

        NEXTGEN_SHARED_DATA(timer, variables);
    };

    namespace network
    {
        typedef asio::streambuf streambuf;

        class stream
        {
            public: typedef streambuf streambuf_type;

            public: streambuf_type& get_buffer() const
            {
                auto self = *this;

                return self->streambuf_;
            }

            private: struct variables
            {
                variables()
                {

                }

                ~variables()
                {

                }

                streambuf_type streambuf_;
            };

            NEXTGEN_SHARED_DATA(stream, variables);
        };

        class service
        {
            private: typedef asio::io_service service_type;

            public: void update()
            {
                auto self = *this;

                self->service.poll();
                self->service.reset();
            }

            public: service_type& get_service()
            {
                auto self = *this;

                return self->service;
            }

            private: struct variables
            {
                variables()
                {

                }

                ~variables()
                {

                }

                service_type service;
            };

            NEXTGEN_SHARED_DATA(service, variables);
        };

        namespace ip
        {
            namespace network
            {
                class layer_base
                {

                };

                namespace ipv4
                {
                    class layer : public layer_base
                    {
                        public: typedef string host_type;
                        public: typedef uint32_t port_type;

                        public: virtual host_type const& get_host()
                        {
                            auto self = *this;

                            return self->host_;
                        }

                        public: virtual void set_host(host_type const& host_)
                        {
                            auto self = *this;

                            self->host_ = host_;
                        }

                        public: virtual port_type get_port()
                        {
                            auto self = *this;

                            return self->port_;
                        }

                        public: virtual void set_port(port_type port_)
                        {
                            auto self = *this;

                            self->port_ = port_;
                        }

                        private: struct variables
                        {
                            variables() : host_("undefined"), port_(0)
                            {

                            }

                            variables(host_type const& host_, port_type port_) : host_(host_), port_(port_)
                            {

                            }

                            ~variables()
                            {

                            }

                            host_type host_;
                            port_type port_;
                        };

                        NEXTGEN_SHARED_DATA(layer, variables);
                    };
                }

                namespace ipv6
                {
                    class layer : public layer_base
                    {

                    };
                }
            }
        }

        typedef ip::network::ipv4::layer ipv4_address;
        typedef ip::network::ipv6::layer ipv6_address;

        namespace ip
        {
            namespace transport
            {
                class layer_base
                {

                };

                class accepter_base
                {

                };

                namespace tcp
                {

                    class accepter : public accepter_base
                    {
                        private: typedef asio::ip::tcp::acceptor accepter_type;
                        private: typedef asio::ip::tcp::socket socket_type;
                        private: typedef asio::ip::tcp::endpoint endpoint_type;
                        public: typedef service service_type;
                        public: typedef uint32_t port_type;

                        public: void open(port_type port)
                        {
                            auto self = *this;

                            if(!self.is_open())
                            {
                                asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);

                                try
                                {
                                    self->accepter_.open(endpoint.protocol());
                                    self->accepter_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

                                    self->accepter_.bind(endpoint);
                                    self->accepter_.listen();
                                }
                                catch(std::exception& e)
                                {
                                    std::cout << "[nextgen:network:ip:transport:tcp:accepter] Failed to bind port " << port << "." << std::endl;
                                }
                            }
                        }

                        public: bool is_open()
                        {
                            auto self = *this;

                            return self->accepter_.is_open();
                        }

                        public: template<typename handler_type> void accept(socket_type& socket_, handler_type handler_)
                        {
                            auto self = *this;

                            if(self.is_open())
                            {
                                self->accepter_.async_accept(socket_, handler_);
                            }
                            else
                            {
                                asio::error_code ec = asio::error_code(asio::error::try_again);
                                handler_(ec);
                            }
                        }

                        private: struct variables
                        {
                            variables(service_type service_) : accepter_(service_.get_service())
                            {

                            }

                            ~variables()
                            {

                            }

                            accepter_type accepter_;
                        };

                        NEXTGEN_SHARED_DATA(accepter, variables);
                    };

                    template<typename network_layer_type>
                    class layer : public layer_base
                    {
                        public: typedef layer<network_layer_type> this_type;
                        public: typedef service service_type;
                        public: typedef string host_type;
                        public: typedef uint32_t port_type;
                        public: typedef uint32_t timeout_type;

                        private: typedef asio::ip::tcp::socket socket_type;
                        private: typedef asio::ip::tcp::resolver resolver_type;
                        private: typedef asio::deadline_timer timer_type;
                        public: typedef accepter accepter_type;

                        public: typedef std::function<void(asio::error_code const&)> cancel_handler_type;

                        public: typedef std::function<void()> base_event_type;
                        public: typedef base_event_type connection_successful_event_type;
                        public: typedef base_event_type connection_failure_event_type;
                        public: typedef base_event_type receive_successful_event_type;
                        public: typedef base_event_type receive_failure_event_type;
                        public: typedef base_event_type send_successful_event_type;
                        public: typedef base_event_type send_failure_event_type;
                        public: typedef base_event_type quit_successful_event_type;
                        public: typedef base_event_type quit_failure_event_type;
                        public: typedef std::function<void(this_type)> accept_successful_event_type;
                        public: typedef base_event_type accept_failure_event_type;

                        public: virtual void initialize()
                        {
                            auto self = *this;

                            if(DEBUG_MESSAGES)
                                std::cout << "[nextgen:network:ip:transport:tcp:socket] (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            self->cancel_handler_ = [=](asio::error_code const& error)
                            {
                                if(error == asio::error::operation_aborted)
                                {
                                    if(DEBUG_MESSAGES)
                                        std::cout << "[nextgen:network:ip:transport:tcp:socket:cancel_handler] Timer cancelled (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES)
                                        std::cout << "[nextgen:network:ip:transport:tcp:socket:cancel_handler] Timer called back. Closing socket (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                    // bugfix(daemn): read timer doesn't actually cancel
                                    self.cancel();
                                }
                            };
                        }

                        public: virtual host_type const& get_host()
                        {
                            auto self = *this;

                            return self->network_layer_.get_host();
                        }

                        public: virtual void set_host(host_type const& host_)
                        {
                            auto self = *this;

                            self->network_layer_.set_host(host_);
                        }

                        public: virtual port_type get_port()
                        {
                            auto self = *this;

                            return self->network_layer_.get_port();
                        }

                        public: virtual void set_port(port_type port)
                        {
                            auto self = *this;

                            self->network_layer_.set_port(port);
                        }

                        public: virtual bool is_connected() const
                        {
                            auto self = *this;

                            return self->socket_.is_open();
                        }

                        public: virtual void cancel() const
                        {
                            auto self = *this;

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::cancel> Cancelling socket (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            if(self->socket_.native() != asio::detail::invalid_socket)
                                self->socket_.cancel();
                            //else
                            //    std::cout << "<ClientSocket> Guarded an invalid socket." << std::endl;
                        }

                        public: virtual void close() const
                        {
                            auto self = *this;

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::close> Closing socket normally. (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            if(self->socket_.native() != asio::detail::invalid_socket)
                                self->socket_.close();
                        }

                        public: virtual size_t bytes_readable()
                        {
                            auto self = *this;

                            asio::socket_base::bytes_readable command(true);
                            self->socket_.io_control(command);

                            return command.get();
                        }

                        public: virtual void connect(host_type const& host_, port_type port_, connection_successful_event_type successful_handler = 0, connection_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->connection_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->connection_failure_event;

                            self->network_layer_.set_host(host_);
                            self->network_layer_.set_port(port_);

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::connect> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            resolver_type::query query_(host_, port_);

                            if(self->timeout_ > 0)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "<socket::connect> create timer (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                self->timer_.async_wait(self->cancel_handler_);
                            }


                            self->resolver_.async_resolve(query_, [=](asio::error_code const& error, resolver_type::iterator endpoint_iterator)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "<socket::connect handler> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                if(!error)
                                {
                                    asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

                                    ++endpoint_iterator;

                                    if(DEBUG_MESSAGES)
                                        std::cout << "<socket::connect handler> create timer (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                    if(DEBUG_MESSAGES2)
                                        std::cout << "<socket::connect handler> Trying to connect to proxy: " << endpoint.address().to_string() << ":" << endpoint.port() << std::endl;

                                    if(self->timeout_ > 0)
                                    {
                                        self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                        self->timer_.async_wait(self->cancel_handler_);
                                    }

                                    //todo(daemn) add additional endpoint connection tries
                                }
                                else
                                {
                                    if(self->timeout_ > 0)
                                        self.cancel_timer();

                                    if(error == asio::error::fd_set_failure)
                                    {
                                        std::cout << "<socket::connect handler> System Error: " << error.message() << std::endl;
                                    }
                                    else
                                    {
                                        if(DEBUG_MESSAGES4)
                                            std::cout << "<socket::connect handler> Error: " << error.message() << std::endl;
                                    }

                                    self.close();

                                    failure_handler();
                                }
                            });
                        }

                        public: virtual void cancel_timer() const
                        {
                            auto self = *this;

                            self->timer_.cancel();
                        }

                        public: template<typename stream_type> void send(stream_type& stream, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->send_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->send_failure_event;

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::write> create timer (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            if(self->timeout_ > 0)
                            {
                                self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                self->timer_.async_wait(self->cancel_handler_);
                            }

                            asio::async_write(self->socket_, stream,
                            [=](asio::error_code const& error, size_t& total)
                            {
                                if(self->timeout_ > 0)
                                    self.cancel_timer();

                                if(!self.is_connected())
                                {
                                    failure_handler();

                                    return;
                                }

                                if(DEBUG_MESSAGES)
                                    std::cout << "<socket::write handler> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                if(!error)
                                {
                                    successful_handler();
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::write handler> Error: " << error.message() << std::endl;

                                    self.close();

                                    failure_handler();
                                }
                            });
                        }

                        public: template<typename stream_type> void receive(std::string const& delimiter, stream_type& stream, receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self2 = *this;
                            auto self = self2; // bugfix(daemn) weird lambda stack bug, would only accept PBR

                            if(successful_handler == 0)
                                successful_handler = self->receive_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->receive_failure_event;

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::receive> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;



                            auto on_read = [=, &stream](asio::error_code const& error, uint32_t total)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "<socket::receive handler> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                if(self->timeout_ > 0)
                                    self.cancel_timer();
std::cout << "4" << std::endl;
                                if(!self.is_connected())
                                {
                                    failure_handler();

                                    return;
                                }
std::cout << "3" << std::endl;
                                if(!error)
                                {
                                    std::cout << "7" << std::endl;
                                    if(self.get_socket().available() > 0)
                                    {
                                        std::cout << "6" << std::endl;
                                        if(self->timeout_ > 0)
                                        {
                                            std::cout << "8" << std::endl;
                                            self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                            self->timer_.async_wait(self->cancel_handler_);
                                        }
std::cout << "1" << std::endl;
                                        self.receive(delimiter, stream, successful_handler, failure_handler);std::cout << "2" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "5" << std::endl;
                                        successful_handler();
                                    }
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::receive handler> Error: " << error.message() << std::endl;

                                    self.close();

                                    failure_handler();
                                }
                            };

                            if(self->timeout_ > 0)
                            {
                                self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                self->timer_.async_wait(self->cancel_handler_);
                            }

                            if(delimiter == "#all#")
                            {
                                asio::async_read(self.get_socket(), stream, asio::transfer_at_least(1), on_read);
                            }
                            else
                            {
                                asio::async_read_until(self.get_socket(), stream, delimiter, on_read);
                            }
                        }

                        public: virtual void accept(port_type port_, accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(DEBUG_MESSAGES)
                                std::cout << "[nextgen:network:ip:transport:tcp:socket:accept] " << std::endl;

                            this_type client(self.get_service());

                            if(!self->accepter_.is_open())
                                self->accepter_.open(port_);

                            self->accepter_.accept(client.get_socket(), [=](asio::error_code const& error)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "[nextgen:network:ip:transport:tcp:socket:accept] Trying to accept client..." << std::endl;

                                if(!error)
                                {
                                    successful_handler(client);

                                    self.accept(port_, successful_handler, failure_handler);
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "[nextgen:network:ip:transport:tcp:socket:accept] Error: " << error.message() << std::endl;

                                    failure_handler();
                                }
                            });
                        }

                        public: virtual socket_type& get_socket() const
                        {
                            auto self = *this;

                            return self->socket_;
                        }

                        public: virtual service_type get_service() const
                        {
                            auto self = *this;

                            return self->service_;
                        }

                        public: virtual timer_type& get_timer() const
                        {
                            auto self = *this;

                            return self->timer_;
                        }

                        private: struct variables
                        {
                            variables(service_type service_) : service_(service_), socket_(service_.get_service()), accepter_(service_), resolver_(service_.get_service()), timer_(service_.get_service()), timeout_(180)
                            {

                            }

                            ~variables()
                            {

                            }


                            event<send_successful_event_type> send_successful_event;
                            event<send_failure_event_type> send_failure_event;
                            event<connection_successful_event_type> connection_successful_event;
                            event<connection_failure_event_type> connection_failure_event;
                            event<receive_successful_event_type> receive_successful_event;
                            event<receive_failure_event_type> receive_failure_event;
                            event<accept_failure_event_type> accept_failure_event;
                            event<accept_successful_event_type> accept_successful_event;

                            service_type service_;
                            socket_type socket_;
                            accepter_type accepter_;
                            resolver_type resolver_;
                            timer_type timer_;
                            network_layer_type network_layer_;
                            timeout_type timeout_;
                            cancel_handler_type cancel_handler_;
                        };

                        NEXTGEN_SHARED_DATA(layer, variables,
                        {
                            this->initialize();
                        });
                    };
                }
            }
        }

        typedef ip::transport::tcp::layer<ipv4_address> tcp_socket;

        namespace ip
        {
            namespace application
            {
                template<typename message>
                class layer_base
                {
                    public: typedef service service_type;
                    public: typedef message message_type;
                    public: typedef timer timer_type;
                    public: typedef string host_type;
                    public: typedef uint32_t port_type;

                    public: typedef std::function<void()> base_event_type;
                    public: typedef base_event_type send_successful_event_type;
                    public: typedef base_event_type send_failure_event_type;
                    public: typedef std::function<void(message_type)> receive_successful_event_type;
                    public: typedef base_event_type receive_failure_event_type;
                    public: typedef std::function<void(message_type)> request_successful_event_type;
                    public: typedef base_event_type request_failure_event_type;
                    public: typedef base_event_type connection_successful_event_type;
                    public: typedef base_event_type connection_failure_event_type;
                };

                class message_base
                {
                    public: typedef string raw_header_list_type;
                    public: typedef hash_map<string, string> header_list_type;
                    public: typedef uint32_t status_code_type;
                    public: typedef string referer_type;
                    public: typedef hash_map<string, string> post_list_type;
                    public: typedef string content_type;
                    public: typedef string path_type;
                    public: typedef uint32_t id_type;
                    public: typedef string version_type;
                    public: typedef string network_layer_type;
                    public: typedef string host_type;
                    public: typedef uint32_t port_type;
                    public: typedef stream stream_type;
                    public: typedef string url_type;
                    public: typedef string status_description_type;
                    public: typedef string method_type;
                };

                namespace http
                {
                    class message : public message_base
                    {
                        public: stream_type get_stream() const
                        {
                            auto self = *this;

                            return self->stream;
                        }

                        public: void pack() const
                        {
                            auto self = *this;

                            std::ostream data_stream(&self->stream.get_buffer());

                            if(self->status_code)
                            {
                                switch(self->status_code)
                                {
                                    case 200: self->status_description = "OK"; break;
                                    default: self->status_description = "UNDEFINED"; break;
                                }

                                data_stream << "HTTP/" << self->version << ".x" << self->status_code << self->status_description << "\r\n";

                                if(!self->header_list.empty())
                                // header list already exists
                                {
                                    if(self->username.length() && self->password.length())
                                    // add authentication into header list
                                    {
                                        if(self->header_list.find("Proxy-Authorization") != self->header_list.end())
                                        // authentication header doesn't already exist
                                        {
                                            //self->header_list["Proxy-Authorization"] = "Basic " << base64encode(ps->username + ":" + ps->password);
                                        }
                                    }

                                    // turn header list into raw header string
                                    for(header_list_type::iterator i = self->header_list.begin(), l = self->header_list.end(); i != l; ++i)
                                    {
                                        self->raw_header_list += (*i).first + ": " + (*i).second + "\r\n";
                                    }
                                }

                                self->raw_header_list += "Content-Length: " + to_string(self->content.length()) + "\r\n";

                                if(DEBUG_MESSAGES4)
                                    std::cout << self->raw_header_list << std::endl;

                                if(DEBUG_MESSAGES4)
                                    std::cout << self->content << std::endl;

                                data_stream << self->raw_header_list + "\r\n";
                                data_stream << self->content;
                            }
                            else if(self->method.length())
                            {
                                if(!self->post_list.empty())
                                // parse post list
                                {

                                }

                                data_stream << self->method + " " + self->url + " " + "HTTP" + "/" + self->version + "\r\n";

                                if(self->header_list.empty() && self->raw_header_list.length())
                                // turn raw header string into a header list
                                {

                                }

                                if(!self->header_list.empty())
                                // header list already exists
                                {
                                    if(self->username.length() && self->password.length())
                                    // add authentication into header list
                                    {
                                        if(self->header_list.find("Proxy-Authorization") != self->header_list.end())
                                        // authentication header doesn't already exist
                                        {
                                            //self->header_list["Proxy-Authorization"] = "Basic " << base64encode(ps->username + ":" + ps->password);
                                        }
                                    }

                                    // turn header list into raw header string

                                }

                                if(DEBUG_MESSAGES4)
                                    std::cout << self->raw_header_list << std::endl;

                                if(DEBUG_MESSAGES4)
                                    std::cout << self->content << std::endl;

                                data_stream << self->raw_header_list + "\r\n";
                                data_stream << self->content;
                            }
                        }

                        public: void unpack() const
                        {
                            auto self = *this;

                            if(self->stream.get_buffer().in_avail())
                            {
                                std::istream data_stream(&self->stream.get_buffer());

                                string status_message;
                                std::getline(data_stream, status_message);

                                string data((std::istreambuf_iterator<char>(data_stream)), std::istreambuf_iterator<char>());


                                //if(http_version.substr(0, 5) != "HTTP/")
                                //{
                                //	std::cout << "Invalid response" << std::endl;
                                //	return;
                                //}

                                size_t header_end = data.find("\r\n\r\n");

                                if(header_end != string::npos)
                                {
                                    self->raw_header_list = data.substr(0, header_end) + "\r\n";

                                    boost::erase_head(data, header_end);
                                }

                                self->content = data;

                                boost::regex_error paren(boost::regex_constants::error_paren);

                                try
                                {
                                    boost::match_results<std::string::const_iterator> what;
                                    boost::match_flag_type flags = boost::regex_constants::match_perl | boost::regex_constants::format_perl;

                                    string::const_iterator start = self->raw_header_list.begin();
                                    string::const_iterator end = self->raw_header_list.end();

                                    while(boost::regex_search(start, end, what, boost::regex("(.+?)\\: (.+?)\r\n"), flags))
                                    {
                                        if(what[1].length() > 0)
                                        {
                                            //std::cout << what[1] << ": " << what[2] << std::endl;
                                            self->header_list[what[1]] = what[2];
                                        }

                                        // update search position:
                                        start = what[0].second;

                                        // update flags:
                                        flags |= boost::match_prev_avail;
                                        flags |= boost::match_not_bob;
                                    }
                                }
                                catch(boost::regex_error const& e)
                                {
                                    std::cout << "regex error: " << (e.code() == paren.code() ? "unbalanced parentheses" : "?") << std::endl;
                                }
                            }

                        }

                        private: struct variables
                        {
                            variables() : status_code(0)
                            {

                            }

                            ~variables()
                            {

                            }

                            referer_type referer;
                            url_type url;
                            content_type content;
                            header_list_type header_list;
                            raw_header_list_type raw_header_list;
                            status_code_type status_code;
                            //raw_post_list_type raw_post_list;
                            post_list_type post_list;
                            status_description_type status_description;
                            id_type id;
                            method_type method;
                            version_type version;
                            network_layer_type address;
                            port_type port;
                            host_type host;
                            string username;
                            string password;
                            string scheme;
                            byte_array stream;
                        };

                        NEXTGEN_SHARED_DATA(message, variables);
                    };

                    template<typename transport_layer_type>
                    class layer : public layer_base<message>
                    {
                        public: typedef layer<transport_layer_type> this_type;
                        public: typedef std::function<void(this_type)> accept_successful_event_type;
                        public: typedef base_event_type accept_failure_event_type;

                        public: virtual void connect(host_type const& host_, port_type port_, connection_successful_event_type successful_handler = 0, connection_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->connection_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->connection_failure_event;

                            self->transport_layer_.connect(host_, port_,
                            [=]
                            {
                                successful_handler();
                            },
                            [=]
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void disconnect() const
                        {
                            auto self = *this;

                            self->transport_layer_.close();
                        }

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            request_.pack();

                            self.send(request_->stream.get_buffer(), successful_handler, failure_handler);
                        }

                        public: template<typename stream_type> void send(stream_type& stream, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->send_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->send_failure_event;

                            self->transport_layer_.send(stream,
                            [=]()
                            {
                                successful_handler();
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->receive_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->receive_failure_event;

                            message_type response2;
                            auto response = response2; // bugfix(daemn)

                            self->transport_layer_.receive("#all#", response->stream.get_buffer(),
                            [=]()
                            {
                                response.unpack();

                                successful_handler(response);

                                self.receive(successful_handler, failure_handler);
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void accept(port_type port_, accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0)
                        {
                            auto self = *this;

                            self->transport_layer_.accept(port_,
                            [=](transport_layer_type client)
                            {
                                successful_handler(this_type(client));
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        private: struct variables
                        {
                            variables(service_type service_) : transport_layer_(service_)
                            {

                            }

                            variables(transport_layer_type transport_layer_) : transport_layer_(transport_layer_)
                            {

                            }

                            ~variables()
                            {

                            }

                            event<send_successful_event_type> send_successful_event;
                            event<send_failure_event_type> send_failure_event;
                            event<receive_successful_event_type> receive_successful_event;
                            event<receive_failure_event_type> receive_failure_event;
                            event<connection_successful_event_type> connection_successful_event;
                            event<connection_failure_event_type> connection_failure_event;
                            event<accept_failure_event_type> accept_failure_event;
                            event<accept_successful_event_type> accept_successful_event;

                            transport_layer_type transport_layer_;
                        };

                        NEXTGEN_SHARED_DATA(layer, variables);
                    };
                }

                namespace xml
                {
                    class message : public message_base
                    {
                        typedef string data_type;

                        public: stream_type get_stream() const
                        {
                            auto self = *this;

                            return self->stream;
                        }

                        public: void pack() const
                        {
                            auto self = *this;

                            std::ostream data_stream(&self->stream.get_buffer());

                            data_stream << self->data;
                        }

                        public: void unpack() const
                        {
                            auto self = *this;

                            if(self->stream.get_buffer().in_avail())
                            {
                                std::istream data_stream(&self->stream.get_buffer());

                                self->data = string((std::istreambuf_iterator<char>(data_stream)), std::istreambuf_iterator<char>());
                            }

                        }

                        private: struct variables
                        {
                            variables()
                            {

                            }

                            ~variables()
                            {

                            }

                            data_type data;
                            stream_type stream;
                        };

                        NEXTGEN_SHARED_DATA(message, variables);
                    };

                    template<typename transport_layer_type>
                    class layer : public layer_base<message>
                    {
                        public: typedef layer<transport_layer_type> this_type;
                        public: typedef std::function<void(this_type)> accept_successful_event_type;
                        public: typedef base_event_type accept_failure_event_type;

                        public: virtual void connect(host_type const& host_, port_type port_, connection_successful_event_type successful_handler = 0, connection_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->connection_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->connection_failure_event;

                            self->transport_layer_.connect(host_, port_,
                            [=]
                            {
                                successful_handler();
                            },
                            [=]
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void disconnect() const
                        {
                            auto self = *this;

                            self->transport_layer_.close();
                        }

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            request_.pack();

                            self.send(request_.get_stream().get_buffer(), successful_handler, failure_handler);
                        }

                        public: template<typename stream_type> void send(stream_type& stream, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->send_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->send_failure_event;

                            self->transport_layer_.send(stream,
                            [=]()
                            {
                                successful_handler();
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->receive_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->receive_failure_event;

                            message_type response;

                            self->transport_layer_.receive("#all#", response.get_stream().get_buffer(),
                            [=]()
                            {
                                response.unpack();

                                successful_handler(response);
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void accept(port_type port_, accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0)
                        {
                            auto self = *this;

                            self->transport_layer_.accept(port_,
                            [=](transport_layer_type client)
                            {
                                successful_handler(this_type(client));
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        private: struct variables
                        {
                            variables(service_type service_) : transport_layer_(service_)
                            {

                            }

                            variables(transport_layer_type transport_layer_) : transport_layer_(transport_layer_)
                            {

                            }

                            ~variables()
                            {

                            }

                            event<send_successful_event_type> send_successful_event;
                            event<send_failure_event_type> send_failure_event;
                            event<receive_successful_event_type> receive_successful_event;
                            event<receive_failure_event_type> receive_failure_event;
                            event<connection_successful_event_type> connection_successful_event;
                            event<connection_failure_event_type> connection_failure_event;
                            event<accept_failure_event_type> accept_failure_event;
                            event<accept_successful_event_type> accept_successful_event;

                            transport_layer_type transport_layer_;
                        };

                        NEXTGEN_SHARED_DATA(layer, variables);
                    };
                }

                namespace ngp
                {
                    class message : public message_base
                    {
                        public: void pack() const
                        {
                            auto self = *this;

                           // std::ostream data_stream(&self->stream.get_buffer());

                            //data_stream << self->content;

                            std::cout << "packing" << std::endl;
                            std::cout << self->data.length() << std::endl;

                            self->stream << self->id << self->data.length() << self->data;

                            std::cout << "LEN: " << self->stream.length() << std::endl;
                        }

                        public: void unpack() const
                        {
                            auto self = *this;

                            //if(self->stream.get_buffer().in_avail())
                            //{
                                //std::istream data_stream(&self->stream.get_buffer());
std::cout << "11" << std::endl;
self->stream->little_endian = true;
                                //data = string((std::istreambuf_iterator<char>(data_stream)), std::istreambuf_iterator<char>());
                                if(self->stream.available() >= 8)
                                {
                                    //byte message_id;
                                    //byte message_length;
                                    //byte messade_data[message_length];
std::cout << "14" << std::endl;
                                    self->stream >> self->id;
                                    self->stream >> self->length;
std::cout << "13" << std::endl;
                                    if(self->stream.available() >= self->length)
                                    {

                                    //self->message_id = read_int32(message_id);
                                    //self->message_length = read_int32(message_length);

                                    //data_stream.get(messade_data, message_length);

                                        self->data = byte_array(self->stream, self->length);
                                    }

                                    //successful_handler(message_id, message_data);
                                }
                            //}
std::cout << "12" << std::endl;
                        }

                        private: struct variables
                        {
                            variables()
                            {

                            }

                            ~variables()
                            {

                            }

                            uint32_t id;
                            uint32_t length;
                            byte_array data;
                            byte_array stream;
                        };

                        NEXTGEN_SHARED_DATA(message, variables);
                    };

                    template<typename transport_layer_type>
                    class layer : public layer_base<message>
                    {
                        public: typedef layer<transport_layer_type> this_type;
                        public: typedef std::function<void(this_type)> accept_successful_event_type;
                        public: typedef base_event_type accept_failure_event_type;

                        public: virtual void connect(host_type const& host_, port_type port_, connection_successful_event_type successful_handler = 0, connection_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;
                        }


                        public: virtual void disconnect() const
                        {
                            auto self = *this;

                            self->transport_layer_.close();
                        }

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            request_.pack();

                            self.send(request_->stream.get_buffer(), successful_handler, failure_handler);
                        }

                        public: template<typename stream_type> void send(stream_type& stream, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->send_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->send_failure_event;

                            self->transport_layer_.send(stream,
                            [=]()
                            {
                                successful_handler();
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            if(successful_handler == 0)
                                successful_handler = self->receive_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->receive_failure_event;

                            //byte_array s;//stream_type s;

                            message_type response2;
                            auto response = response2; // bugfix(daemn)

                            self->transport_layer_.receive("#all#", response->stream.get_buffer(),
                            [=]()
                            {
                                //std::istream data_stream(&s.get_buffer());

                                //string content = string((std::istreambuf_iterator<char>(data_stream)), std::istreambuf_iterator<char>());

                                //read_int32(s
std::cout << "9" << std::endl;
                                response.unpack();
std::cout << "10" << std::endl;

                                successful_handler(response);

                                self.receive(successful_handler, failure_handler);
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        public: virtual void accept(port_type port, accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0)
                        {
                            auto self = *this;

                            self->transport_layer_.accept(port,
                            [=](transport_layer_type client)
                            {
                                successful_handler(this_type(client));
                            },
                            [=]()
                            {
                                failure_handler();
                            });
                        }

                        private: struct variables
                        {
                            variables(service_type service_) : transport_layer_(service_)
                            {

                            }

                            variables(transport_layer_type transport_layer_) : transport_layer_(transport_layer_)
                            {

                            }

                            ~variables()
                            {

                            }

                            event<send_successful_event_type> send_successful_event;
                            event<send_failure_event_type> send_failure_event;
                            event<receive_successful_event_type> receive_successful_event;
                            event<receive_failure_event_type> receive_failure_event;
                            event<connection_successful_event_type> connection_successful_event;
                            event<connection_failure_event_type> connection_failure_event;
                            event<accept_failure_event_type> accept_failure_event;
                            event<accept_successful_event_type> accept_successful_event;

                            transport_layer_type transport_layer_;
                        };

                        NEXTGEN_SHARED_DATA(layer, variables);
                    };
                }
            }
        }

        typedef ip::application::http::layer<tcp_socket> http_client;
        typedef ip::application::http::message http_message;

        typedef ip::application::xml::layer<tcp_socket> xml_client;
        typedef ip::application::xml::message xml_message;

        typedef ip::application::ngp::layer<tcp_socket> ngp_client;
        typedef ip::application::ngp::message ngp_message;


        template<typename layer_type>
        void create_server(service service_, uint32_t port, std::function<void(layer_type)> successful_handler = 0, std::function<void()> failure_handler = 0)
        {
            if(DEBUG_MESSAGES2)
                std::cout << "[nextgen:network:server:accept] Waiting for client..." << std::endl;

            layer_type server(service_);

            server.accept(port,
            [=](layer_type client)
            {
                if(DEBUG_MESSAGES2)
                    std::cout << "[nextgen::network::server::accept] Successfully accepted client." << std::endl;

                if(successful_handler != 0)
                    successful_handler(client);
            },
            [=]()
            {
                if(DEBUG_MESSAGES2)
                    std::cout << "[nextgen::network::server::accept] Failed to accept client." << std::endl;

                if(failure_handler != 0)
                    failure_handler();
            });
        }
    }
}

namespace nextgen
{
    namespace content
    {

    }
}

namespace nextgen
{
    namespace math
    {
        /**
        Old vector class I had laying around. Will do for now. Added NEXTGEN_SHARED_DATA. Removed std::vector.
        */
        template<typename element_type>
        class vector;

        template<typename element_type> vector<element_type> operator+(vector<element_type> const&, vector<element_type> const&);
        template<typename element_type> vector<element_type> operator-(vector<element_type> const&, vector<element_type> const&);
        template<typename element_type> vector<element_type> operator*(vector<element_type> const&, float);
        template<typename element_type> vector<element_type> operator*(float, vector<element_type> const&);
        template<typename element_type> vector<element_type> operator/(vector<element_type> const&, float);
        template<typename element_type> vector<element_type> operator/(float, vector<element_type> const&);
        template<typename element_type> vector<element_type> operator--(vector<element_type> const&);
        template<typename element_type> vector<element_type> operator++(vector<element_type> const&);

        template <typename element_type>
        class vector
        {
            //public: virtual const string to_string() const;

            public: static vector<element_type> null() { return vector<element_type>(0, 0, 0, 0); }

            public: static vector<element_type> up() { return vector<element_type>(0, 1, 0, 0); }
            public: static vector<element_type> down() { return vector<element_type>(0, -1, 0, 0); }

            public: static vector<element_type> left() { return vector<element_type>(-1, 0, 0, 0); }
            public: static vector<element_type> right() { return vector<element_type>(1, 0, 0, 0); }

            public: static vector<element_type> unit_x() { return vector<element_type>(1, 0, 0, 0); }
            public: static vector<element_type> unit_y() { return vector<element_type>(0, 1, 0, 0); }
            public: static vector<element_type> unit_z() { return vector<element_type>(0, 0, 1, 0); }

            public: static vector<element_type> zero() { return vector<element_type>(0, 0, 0, 0); }
            public: static vector<element_type> one() { return vector<element_type>(1, 1, 1, 1); }

            public: vector<element_type> operator=(vector<element_type> v)
            {
                auto self = *this;

                self->x_ = v->x_;
                self->y_ = v->y_;
                self->z_ = v->z_;
                self->w_ = v->w_;

                return self;
            }

            public: element_type x() { auto self = *this; return self->x_; }
            public: element_type y() { auto self = *this; return self->y_; }
            public: element_type z() { auto self = *this; return self->z_; }
            public: element_type w() { auto self = *this; return self->w_; }

            public: void x(element_type x_) { auto self = *this; if(x_ != self->x_) { self->x_ = x_; self->changed = true; } }
            public: void y(element_type y_) { auto self = *this; if(y_ != self->y_) { self->y_ = y_; self->changed = true; } }
            public: void z(element_type z_) { auto self = *this; if(z_ != self->z_) { self->z_ = z_; self->changed = true; } }
            public: void w(element_type w_) { auto self = *this; if(w_ != self->w_) { self->w_ = w_; self->changed = true; } }

            public: bool is_changed() { auto self = *this; return self->changed; }

            //private: friend std::ostream& operator<<(std::ostream&, const vector<element_type>&);
            private: template<typename y> friend vector<y> operator+(vector<y>, vector<y>);
            private: template<typename y> friend vector<y> operator-(vector<y>, vector<element_type>);
            private: template<typename y> friend vector<y> operator*(vector<y>, float);
            private: template<typename y> friend vector<y> operator*(float, vector<y>);
            private: template<typename y> friend vector<y> operator/(vector<y>, float);
            private: template<typename y> friend vector<y> operator/(float, vector<y>);
            private: template<typename y> friend vector<y> operator--(vector<y>);
            private: template<typename y> friend vector<y> operator++(vector<y>);

            private: struct variables
            {
                variables(element_type const x_ = 0, element_type const y_ = 0, element_type const z_ = 0, element_type const w_ = 0) : changed(true)
                {
                    this->x_ = x_;
                    this->y_ = y_;
                    this->z_ = z_;
                    this->w_ = w_;
                }

                ~variables()
                {

                }

                element_type x_;
                element_type y_;
                element_type z_;
                element_type w_;
                bool changed;
            };

            NEXTGEN_SHARED_DATA(vector, variables);
        };

        //template<class element_type> using vector2 = vector<element_type>;
        //template<class element_type> using vector3 = vector<element_type>;
/*
        template <typename element_type>
        class vector2 : public vector<element_type>
        {
            //public: explicit vector2() : vector<element_type>() { }

            //public: operator vector3();
            //public: operator vector3() const;
            //public: operator vector4();
            //public: operator vector4() const;
        };*/

        /*
        template <typename element_type>
        inline std::ostream& operator<<(std::ostream& os, const vector<element_type>& v)
        {
            os << v.to_string();

            return os;
        }*/

        template <typename element_type>
        inline vector<element_type> operator+(vector<element_type> v1, vector<element_type> v2)
        {
            return v1.clone().add(v2);
        }

        template <typename element_type>
        inline vector<element_type> operator-(vector<element_type> v1, vector<element_type> v2)
        {
            return v1.clone().subtract(v2);
        }

        template <typename element_type>
        inline vector<element_type> operator*(vector<element_type> v, float m)
        {
            return v.clone().multiply(m);
        }

        template <typename element_type>
        inline vector<element_type> operator*(float m, vector<element_type> v)
        {
            return v.clone().multiply(m);
        }

        template <typename element_type>
        inline vector<element_type> operator/(vector<element_type> v, float m)
        {
            return v.clone().divide(m);
        }

        template <typename element_type>
        inline vector<element_type> operator/(float m, vector<element_type> v)
        {
            return v.clone().divide(m);
        }

        template <typename element_type>
        inline vector<element_type> operator--(vector<element_type> v)
        {
            v->x_ = --v->x_;
            v->y_ = --v->y_;
            v->z_ = --v->z_;
            v->w_ = --v->w_;

            return v;
        }

        template <typename element_type>
        inline vector<element_type> operator++(vector<element_type> v)
        {
            v->x_ = ++v->x_;
            v->y_ = ++v->y_;
            v->z_ = ++v->z_;
            v->w_ = ++v->w_;

            return v;
        }
/*
        template <typename element_type>
        inline vector<element_type> operator=(vector<element_type> const& v1, vector<element_type> const& v2)
        {
            v1->x(v2->x());
            v1->y(v2->y());
            v1->z(v2->z());
            v1->w(v2->w());

            return v1;
        }*/
    }
}



