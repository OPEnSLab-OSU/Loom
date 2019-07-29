
#include "Loom_InternetPlat.h"


///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::LoomInternetPlat(	
		const char* module_name
	) 
	: LoomModule( module_name ) {
		this->module_type = LoomModule::Type::Internet;
	}

///////////////////////////////////////////////////////////////////////////////
Client& LoomInternetPlat::http_request(const char* domain, const char* url, const char* body, const char* verb) {
	// * the rainbow connection *
	Client& client = connect_to_domain(domain);
	if (!client.connected()) return client;
	// ok next, make the http request
	print_module_label();
	LPrint("Writing http: ", domain, "\n");
	write_http_request(client, domain, url, body, verb);
	// return the client for data reception
	return client;
}


///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlat::write_http_request(Stream& client, const char* domain, const char* url, const char* body, const char* verb) {
	/// print the initial http request
	client.print(verb);
	client.print(" ");
	client.print(url == nullptr ? "/" : url);
	client.print(" ");
	client.print("HTTP/1.1\r\nUser-Agent: LoomOverSSLClient\r\nHost: ");
	client.print(domain);
	client.print("\r\nConnection: close\r\n");
	/// add the optional body
	if(body != nullptr) client.print(body);
	client.print("\r\n");
	/// all ready to go!
}

///////////////////////////////////////////////////////////////////////////////
