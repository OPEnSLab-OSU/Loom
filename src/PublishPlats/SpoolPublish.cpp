///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SpoolPublish.cpp
/// @brief		File for Loom_SpoolPublish implementation.
/// @author		Eli Winkelman, Maddie Smith, and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "SpoolPublish.h"

/////////////////////////////////////////////////////////////////////
Loom_SpoolPublish::Loom_SpoolPublish(
		LoomManager* manager,
		const char*				module_name,
		const LoomModule::Type	internet_type,
		const char*             coordinator_id,
    	const char*             device_id,
		const char* 			data_run,
		const char* 			cli_cert,
		const char*				cli_key,
		const char*				spool_domain,
		const char*				device_data_endpoint
	)
	: LoomPublishPlat(manager, module_name, LoomModule::Type::SpoolPub, internet_type)
	, m_spool_domain(spool_domain)
	, m_device_data_endpoint(device_data_endpoint)
	, m_device_id(device_id)
  	, m_coordinator_id(coordinator_id)
	, m_data_run(data_run)
	, m_params(SSLClientParameters::fromPEM(cli_cert, strlen(cli_cert), cli_key, strlen(cli_key)))
 {
	LPrintln(m_device_id);
	LPrintln(m_coordinator_id);
	LPrintln(m_spool_domain);
	LPrintln(m_data_run);
	LPrintln(m_device_data_endpoint);
	LPrintln(cli_cert);
	LPrintln(cli_key);
 }

/////////////////////////////////////////////////////////////////////
Loom_SpoolPublish::Loom_SpoolPublish(LoomManager* manager, JsonArrayConst p)
: Loom_SpoolPublish(manager, p[0], (LoomModule::Type)(int)p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8] ) {}


/////////////////////////////////////////////////////////////////////
static std::vector<unsigned char> make_key_from_asn1(std::vector<unsigned char> asn1) {
	// we need to extract the private key from the ASN1 object given to us
	// to do that, some ASN.1 parsing!
	// get the tag and length of the first octet string, and verify it's reasonable
	if (asn1[5] != 0x04
		|| asn1.size() < 6
		|| asn1[6] >= asn1.size() - 6) return {};

	const unsigned char length = asn1[6];
	// delete everything around the key that we don't need
	asn1.erase(asn1.begin(), asn1.begin() + 7);
	asn1.erase(asn1.begin() + length, asn1.end());
	// shrink it
	asn1.shrink_to_fit();
	return asn1;
}

/////////////////////////////////////////////////////////////////////
void Loom_SpoolPublish::print_config() const
{
	LoomPublishPlat::print_config();
	print_module_label();
	LPrint("Domain: ", m_spool_domain, '\n');
	print_module_label();
	LPrint("URL: ", m_device_data_endpoint, '\n');
	print_module_label();
	LPrint("Coordinator Id: ", m_coordinator_id, '\n');
}

/////////////////////////////////////////////////////////////////////
bool Loom_SpoolPublish::send_to_internet(const JsonObject json, LoomInternetPlat* plat)
{
	// serialize the data, checking for an error
	// not sure if this is the right way to check if there is a overflow
	// set mutual auth, if needed

	if (m_params.getCertChain() == nullptr || !(m_params.getCertChain() -> data_len)) {
		print_module_label();
		LPrintln("Failed to decode client certificate");
		return false;
	}
	if (m_params.getECKey() == nullptr || !(m_params.getECKey() -> xlen)) {
		print_module_label();
		LPrintln("Failed to decode client private key");
		return false;
	}

	print_module_label();
	LPrintln("Adding mutual auth!");
		// setup the certificate
		plat->set_mutual_auth(m_params);

	auto network = plat->connect_to_domain(m_spool_domain.c_str());

	if (!network) {
		print_module_label();
		LPrintln("Could not connect to Spool instance.");
		return false;
	}

	// POST DEVICE_DATA_ENDPOINT
	network -> print("POST ");
	network -> print(m_device_data_endpoint);
	network -> println(" HTTP/1.1");

	//Headers
	network -> println("Content-Type: application/json");
	network -> print("Content-Length: ");
	// Equivalent to:
	// strlen("\",\"device_id\":\""") + m_device_id + strlen("{\"coordinator_id\":\"") + m_coordinator_id + strlen("\",\"data_run\":\"") + m_data_run + strlen("\"strlen("\",\"data\":") + json + strlen("}")
	network -> println(15 + m_device_id.length() + 19 + m_coordinator_id.length() + 14 + m_data_run.length() + 9 + measureJson(json) + 1);
	network -> print("Host: ");
	network -> println(m_spool_domain);

	//print the body
	network -> println();
	network -> print("{\"coordinator_id\":\"");
	network -> print(m_coordinator_id);
    network -> print("\",\"device_id\":\"");
    network -> print(m_device_id);
	network -> print("\",\"data_run\":\"");
	network -> print(m_data_run);
	network -> print("\",\"data\":");

	serializeJson(json, *network);

	network -> println("}");
	//network -> println("}");
	network -> println();

	if (!network->connected()) {
		print_module_label();
		LPrintln("Internet disconnected during transmission!");
		return false;
	}

	network -> flush();

	// all done!
	print_module_label();
	LPrint("Published successfully!\n");
	return true;
}

/////////////////////////////////////////////////////////////////////
