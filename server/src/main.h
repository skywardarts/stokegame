

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
#include <deque>
#include <algorithm>

#include <stdlib.h>
#include <string.h>
//#include <time.h>
//#include <math.h>
#include <errno.h>
//#include <signal.h>
//#include <typeinfo.h>
#include <sys/types.h>

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

#include <asio.hpp>
#include <asio/ssl.hpp>


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

#define NEXTGEN_INITIALIZE_SHARED_DATA(this_type, data_type, ...) \
    private: boost::shared_ptr<data_type> ng_data; \
    public: this_type(this_type& t) : ng_data(t.ng_data) { } \
    public: this_type(this_type const& t) : ng_data(t.ng_data) { } \
    public: template<typename ...argument_types> this_type(argument_types&& ...argument_list) : ng_data(new data_type(argument_list...)) { __VA_ARGS__ } \
    public: template<typename element_type> this_type(element_type&& t, typename boost::enable_if<boost::is_base_of<this_type, element_type>>::type* dummy = 0) : ng_data(t.ng_data) { } \
    public: bool operator==(this_type const& t) const { return &(*this->ng_data) == &(*t.ng_data); } \
    public: bool operator==(int t) const { if(t == 0) return this->ng_data == 0; else return 0; } \
    public: bool operator!=(this_type const& t) const { return !this->operator==(t); } \
    public: bool operator!=(int t) const { return !this->operator==(t); } \
    public: boost::shared_ptr<data_type> const& operator->() const { return this->ng_data; } \


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

int to_int(std::string const& element)
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
    typedef boost::uint8_t byte;
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

        NEXTGEN_INITIALIZE_SHARED_DATA(timer, variables);
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

            NEXTGEN_INITIALIZE_SHARED_DATA(stream, variables);
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

            NEXTGEN_INITIALIZE_SHARED_DATA(service, variables);
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

                        NEXTGEN_INITIALIZE_SHARED_DATA(layer, variables);
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

                        public: template<typename handler_type> void accept(port_type port, socket_type& socket_, handler_type handler_)
                        {
                            auto self = *this;

                            asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);

                            self->accepter_.open(endpoint.protocol());
                            self->accepter_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

                            self->accepter_.bind(endpoint);
                            self->accepter_.listen();

                            self->accepter_.async_accept(socket_, handler_);
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

                        NEXTGEN_INITIALIZE_SHARED_DATA(accepter, variables);
                    };

                    template<typename network_layer_type>
                    class layer : public layer_base
                    {
                        public: typedef layer<network_layer_type> this_type;
                        public: typedef service service_type;
                        public: typedef stream stream_type;
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
                                else if(error == asio::error::operation_aborted)
                                {
                                    if(DEBUG_MESSAGES3)
                                        std::cout << "<socket::connect handler> Resolve operation aborted." << std::endl;

                                    self.close();

                                    failure_handler();
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

                        public: virtual void send(stream_type stream2, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self2 = *this;
                            auto self = self2; // bugfix(daemn) weird lambda stack bug, would only accept PBR
                            auto stream = stream2; // bugfix(daemn) weird lambda stack bug, would only accept PBR

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

                            asio::async_write(self->socket_, stream.get_buffer(),
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
                                else if(error == asio::error::eof)
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::write handler> eof" << std::endl;

                                    successful_handler();
                                }
                                else if(error == asio::error::operation_aborted)
                                {
                                    if(DEBUG_MESSAGES3)
                                        std::cout << "<socket::write handler> Send operation aborted." << std::endl;

                                    self.close();

                                    failure_handler();
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::write handler> Error: " << asio::system_error(error).what() << std::endl;

                                    self.close();

                                    failure_handler();
                                }
                            });
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            stream_type stream;

                            self.receive("#all#", stream, successful_handler, failure_handler);
                        }

                        public: virtual void receive(std::string const& delimiter, stream_type stream, receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self2 = *this;
                            auto self = self2; // bugfix(daemn) weird lambda stack bug, would only accept PBR

                            if(successful_handler == 0)
                                successful_handler = self->receive_successful_event;

                            if(failure_handler == 0)
                                failure_handler = self->receive_failure_event;

                            if(DEBUG_MESSAGES)
                                std::cout << "<socket::receive> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                            auto on_read = [=](asio::error_code const& error, uint32_t total)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "<socket::receive handler> (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                if(self->timeout_ > 0)
                                    self.cancel_timer();

                                if(!self.is_connected())
                                {
                                    failure_handler();

                                    return;
                                }

                                if(!error)
                                {
                                    if(self.get_socket().available())
                                    {
                                        if(self->timeout_ > 0)
                                        {
                                            self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                            self->timer_.async_wait(self->cancel_handler_);
                                        }

                                        self.receive(delimiter, stream, successful_handler, failure_handler);
                                    }
                                    else
                                    {
                                        successful_handler();
                                    }
                                }
                                /*else if(error == asio::error::eof)
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::receive handler> Read EOF (" << self->network_layer_.get_host() << ":" << self->network_layer_.get_port() << ")" << std::endl;

                                    successful_handler();
                                }*/
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "<socket::receive handler> Error: " << asio::system_error(error).what() << std::endl;

                                    self.close();

                                    failure_handler();
                                }
                            };

                            /*if(delimiter == "#all#")
                            {
                                self.receive("#begin#", stream,
                                [=]()
                                {
                                    self.receive("#end#", stream, successful_handler, failure_handler);
                                },
                                [=]
                                {
                                    failure_handler();
                                });
                            }
                            else */if(delimiter == "#all#")
                            {
                                if(self->timeout_ > 0)
                                {
                                    self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                    self->timer_.async_wait(self->cancel_handler_);
                                }

                                asio::async_read(self.get_socket(), stream.get_buffer(), asio::transfer_at_least(1), on_read);
                            }
                            /*else if(delimiter == "#end#")
                            {
                                if(self->timeout_ > 0)
                                {
                                    self->timer_.expires_from_now(boost::posix_time::seconds(2));

                                    self->timer_.async_wait([=](asio::error_code const& error)
                                    {
                                        if(error != asio::error::operation_aborted)
                                        {
                                            if(DEBUG_MESSAGES)
                                                std::cout << "[nextgen:network:ip:transport:tcp:socket:receive] Cancelling receive due to EOF." << std::endl;

                                            self.cancel(asio::error_code(asio::error::eof));
                                        }
                                    });
                                }

                                asio::async_read(self.get_socket(), stream.get_buffer(), asio::transfer_at_least(1), on_read);
                            }*/
                            else
                            {
                                if(self->timeout_ > 0)
                                {
                                    self->timer_.expires_from_now(boost::posix_time::seconds(self->timeout_));
                                    self->timer_.async_wait(self->cancel_handler_);
                                }

                                asio::async_read_until(self.get_socket(), stream.get_buffer(), delimiter, on_read);
                            }
                        }

                        public: virtual void accept(port_type port_, accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0) const
                        {
                            auto self2 = *this;
                            auto self = self2; // bugfix(daemn) weird lambda stack bug, would only accept PBR

                            this_type client(self.get_service());

                            self->accepter_.accept(port_, client.get_socket(), [=](asio::error_code const& error)
                            {
                                if(DEBUG_MESSAGES)
                                    std::cout << "[nextgen:network:ip:transport:tcp:socket:accept] " << std::endl;

                                if(!error)
                                {
                                    successful_handler(client);
                                }
                                else
                                {
                                    if(DEBUG_MESSAGES4)
                                        std::cout << "[nextgen:network:ip:transport:tcp:socket:accept] Error: " << asio::system_error(error).what() << std::endl;

                                    failure_handler();
                                }

                                //self.accept(port_, successful_handler, failure_handler);
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

                        NEXTGEN_INITIALIZE_SHARED_DATA(layer, variables,
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
                    public: typedef stream stream_type;

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

                };

                namespace http
                {


                    class message : public message_base
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

                        public: stream_type& get_stream() const
                        {
                            auto self = *this;

                            return self->stream;
                        }

                        public: void parse() const
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
                            else if(self->method.length())
                            {
                                std::ostream data_stream(&self->stream.get_buffer());

                                if(!self->post_list.empty())
                                // parse post list
                                {

                                }

                                if(DEBUG_MESSAGES4)
                                    std::cout << self->method + " " + self->url + " " + "HTTP" + "/" + self->version + "\r\n" << std::endl;

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

                                data_stream << self->raw_header_list + "\r\n";
                                data_stream << self->content;
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
                            stream_type stream;
                            string username;
                            string password;
                            string scheme;
                        };

                        NEXTGEN_INITIALIZE_SHARED_DATA(message, variables);
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

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;

                            self.send(request_.get_stream(), successful_handler, failure_handler);
                        }

                        public: virtual void send(stream_type stream, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
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
                            auto response = response2; // bugfix(daemn) weird lambda stack bug, would only accept PBR

                            self->transport_layer_.receive("#all#", response.get_stream(),
                            [=]()
                            {
                                response.parse();

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

                        NEXTGEN_INITIALIZE_SHARED_DATA(layer, variables);
                    };
                }

                namespace xml
                {
                    class message : public message_base
                    {

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

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;
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
                            variables(service_type& service_) : transport_layer_(service_)
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

                        NEXTGEN_INITIALIZE_SHARED_DATA(layer, variables);
                    };
                }

                namespace ngp
                {
                    class message : public message_base
                    {

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

                        public: virtual void send(message_type request_, send_successful_event_type successful_handler = 0, send_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;
                        }

                        public: virtual void receive(receive_successful_event_type successful_handler = 0, receive_failure_event_type failure_handler = 0) const
                        {
                            auto self = *this;
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
                            variables(service_type& service_) : transport_layer_(service_)
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

                        NEXTGEN_INITIALIZE_SHARED_DATA(layer, variables);
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
        void create_server(service service_, uint32_t port_, std::function<void(layer_type)> successful_handler = 0, std::function<void()> failure_handler = 0)
        {
            if(DEBUG_MESSAGES2)
                std::cout << "[nextgen:network:server:accept] Waiting for client..." << std::endl;

            layer_type server(service_);

            server.accept(port_,
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
/*
        template<typename layer_type>
        class server
        {
            public: typedef service service_type;
            public: typedef uint32_t port_type;

            public: typedef std::function<void()> base_event_type;
            public: typedef std::function<void(layer_type)> accept_successful_event_type;
            public: typedef base_event_type accept_failure_event_type;

            public: void accept(accept_successful_event_type successful_handler = 0, accept_failure_event_type failure_handler = 0) const
            {
                auto self = *this;

                if(successful_handler == 0)
                    successful_handler = self->accept_successful_event;

                if(failure_handler == 0)
                    failure_handler = self->accept_failure_event;

                if(DEBUG_MESSAGES)
                    std::cout << "[nextgen:network:server:accept] Accepting client..." << std::endl;

                layer_type layer(self->service_);

                layer.set_port(self->port_);

                layer.accept(
                [self, layer, successful_handler, failure_handler]()
                {
                    if(DEBUG_MESSAGES)
                        std::cout << "[nextgen::network::server::accept] Successfully accepted client." << std::endl;

                    successful_handler(layer);

                    self.accept(successful_handler, failure_handler);
                },
                [self, successful_handler, failure_handler]()
                {
                    if(DEBUG_MESSAGES)
                        std::cout << "[nextgen::network::server::accept] Failed to accept client." << std::endl;

                    failure_handler();

                    self.accept(successful_handler, failure_handler);
                });
            }

            private: struct variables
            {
                variables(service_type service_, port_type port_) : service_(service_), port_(port_)
                {

                }

                ~variables()
                {

                }

                event<accept_successful_event_type> accept_successful_event;
                event<accept_failure_event_type> accept_failure_event;

                service_type service_;
                port_type port_;
            };

            NEXTGEN_INITIALIZE_SHARED_DATA(server, variables);
        };*/
    }
}

namespace stoke
{
	class game : public nextgen::singleton<game>
	{
		public: void initialize()
		{
		    auto self = *this;

        }

		public: void run();

        private: struct variables
        {
            variables()
            {

            }

            ~variables()
            {

            }

            nextgen::network::service service;
        };

        NEXTGEN_INITIALIZE_SHARED_DATA(game, variables);
	};
}
