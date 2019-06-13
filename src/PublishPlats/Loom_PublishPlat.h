
#ifndef LOOM_PUBLISH_PLAT_h
#define LOOM_PUBLISH_PLAT_h

#include "Loom_Module.h"
#include "../InternetPlats/Loom_InternetPlat.h"

// ### (LoomPublishPlat) | dependencies: [] | conflicts: []
/// Abstract internet publishing module, implementing google sheets and mongodb functionality
// ###
class LoomPublishPlat : public LoomModule
{
public:
	/// Loom Publish Platform module constructor.
	///
	/// \param[in]	module_name		String | <"Internet-Plat"> | null | Publish platform module name
	/// \param[in]  internet_module_index   Int | <0> | [0-5] | Index from zero of of the desired internet platform based on the JSON configuration
	LoomPublishPlat( 
		const char* module_name,
		const uint	internet_index
	);

	// --- DESTRUCTOR ---
	virtual ~LoomPublishPlat() = default;

	/// Grab the internet platform specified by the ctor parameters
    void second_stage_ctor() override;

	void measure() override {}
	void package(JsonObject json) override { /* do nothing for now */ }
	bool cmd_route(JsonObject json) override { /* do nothing for now */}
	virtual void print_state() override;
	virtual void print_config() override;

    /// \param[in] json JSON object to publish
    /// \returns Whether or not the publish succeded
    bool publish(const JsonObject json);

protected:
	/// Send JSON to the internet, assumes that a valid internet plat was found and can be used
	virtual bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) = 0;

private:

	LoomInternetPlat* m_internet;
	const uint m_internet_index;
};

#endif