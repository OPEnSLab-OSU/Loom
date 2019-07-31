#pragma once

#include "Loom_Module.h"

#include "Client.h"
#include "Udp.h"
#undef min
#undef max
#include <memory>

///////////////////////////////////////////////////////////////////////////////


// ### (LoomInternetPlat) | dependencies: [] | conflicts: []
/// Abstract internet communication module, implementing wifi and ethernet.
// ###
class LoomInternetPlat : public LoomModule
{

protected:	
	
	/// Utility function to write an http reqest based on parameters specified by LoomInternetPlat::httpo_request to a Client class
	void write_http_request(Stream& client, const char* domain, const char* url, const char* body, const char* verb);

public:
//=============================================================================
///@name	TYPES
/*@{*/ //======================================================================
	/// Close the socket and delete the UDP object when the unique ptr dissapears
	struct UDPDeletor {
		void operator() (UDP* p) {
			if(p != nullptr) {
				p->stop();
				delete p;
			}
		}
	};

	using UDPPtr = std::unique_ptr<UDP, UDPDeletor>;

	/// Simply close the socket when the client dissapears, we don't want to delete the
	/// object because the client needs to cache sessions
	struct ClientCleanup {
		void operator() (Client* c) {
			if (c != nullptr)
				c->stop();
		}
	};

	using ClientSession = std::unique_ptr<Client, ClientCleanup>;

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Internet Platform module constructor.
	///
	/// \param[in]	module_name		String | <"Internet-Plat"> | null | Internet Platform module name
	LoomInternetPlat( 
			const char*			module_name,  
			LoomModule::Type	module_type
		);

	/// Destructor
	virtual ~LoomInternetPlat() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void package(JsonObject json) override { /* do nothing for now */ }
	virtual bool dispatch(JsonObject json) override { /* do nothing for now */}

	/// Try to connect to internet
	virtual void connect() = 0;

	/// Get if connected to internet
	/// \return True if connected
	virtual bool is_connected() = 0;

	/// Make HTTP request
	/// \param[in]	domain	The domain to connect to "www.google.com"
	/// \param[in]	url		The URL string to send with the http request, not including the domain (ex. "/arduino?thing=otherthing").
	/// \param[in]	body	The body string to use for the rest of the request, including additional headers. Will be appended
	/// 					directly to the request (right after the last header appended by the library) and will need newlines at the top or between
	///						additional headers and body. Example: "some-header: thing\r\nsome-other-header:29\r\n\r\n{\"my-json-body\":\"nice\"}"
	/// 					adds two headers (some-header and some-other-header) and has a simple JSON body. Can be nullptr for none.
	/// \param[in]			result_size	Size of the buffer pointed to by result, in bytes. If the size of the buffer is exceeded,
	/// 					this function will return false.
	/// \param[out]	result	Pointer to the buffer to put data returned from the request in. Can be nullptr if result data is to be
	/// 					discarded.
	/// \param[in]	verb	HTTP verb ("PUT", "GET", etc.) to use with this request.
	/// \returns A client session pointer, or a nullptr if the connection failed. When this pointer is destroyed the client will automatically close the connection.
	virtual ClientSession http_request(const char* domain, const char* url, const char* body, const char* verb);
	ClientSession http_get_request(const char* domain, const char* url, const char* body = nullptr) { return http_request(domain, url, body, "GET"); }
	ClientSession http_post_request(const char* domain, const char* url, const char* body = nullptr) { return http_request(domain, url, body, "POST"); }

	/// Connect to a domain, but don't write any HTTP stuff, Let the module figure that out.
	/// \param[in]	domain	The domain to connect to "www.google.com"
	/// \returns A client reference. The client::connected method will return true if the connection succeeded, and false otherwise.
	virtual ClientSession connect_to_domain(const char* domain) = 0;

	/// Open a UDP socket for sending and recieving incoming data (WARNING: Be careful about recieving data from an open socket!)
	/// \returns A UDP socket for transmitting and recieving, or a nullptr if opening the socket failed. The socket will automatically
	/// close when the pointer is destructed.
	virtual UDPPtr open_socket(const uint port) = 0;

	/// make NTP request to get UTC time, using the UDP function above
	/// \returns a unix timestamp if success, or 0 if failure.
	uint32_t get_time();
	
private:
	void m_send_NTP_packet(UDP& udp_dev, byte packet_buffer[]) const;
};
