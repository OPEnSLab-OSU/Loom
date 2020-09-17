///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Module_Factory.h
/// @brief		File for LoomFactory, FactoryBase definition and implementations.
///				Also includes all the supporting code to build the LoomFactory lookup table.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Module.h"
#include <ArduinoJson.h>

// Need to undef max and min for vector to work
#undef max
#undef min
#include <vector>


///Forward declare any classes that will be served the created objects
// class Scope;
class LoomManager;

///////////////////////////////////////////////////////////////////////////////

/// Creates a new T, returned as S*. S must be base of T
/// @return The created T
template <class S, class T>
S* ConstructDefault(LoomManager *manager) { return new T(manager); }

/// Creates a new T, returned as S*. S must be base of T
/// @return The created T
template <class S, class T>
S* ConstructDefaultJson(LoomManager* manager, JsonArrayConst p) { return new T(manager, p); }

///////////////////////////////////////////////////////////////////////////////
/// A generic registry/factory that collects constructors of a give base type and allows for any number
/// of base classes as static singleton templates will only create new instances of a new type is introduced
template <typename T>
class Registry
{

public:
	using FactoryFunction = T *(*)(LoomManager*); ///< Pointer to function of type //* void T::funct (void) *//
	using FactoryFunctionJson = LoomModule *(*)(LoomManager*, JsonArrayConst);
	using FactoryPair = struct
	{										///<  *Needed as an alternative to std::map
		const char* name;					///< Name of module that will be used to make a new copy
		const FactoryFunction ctor;			///< Pointer to the Creation function which will be used to CTOR
		const FactoryFunctionJson ctorJson; ///< Pointer to the CreationJSON function which will be used to CTOR form JSON
	};

	
	template <class U>
	static bool add(const char*);
	
	template <class U>
	static bool addNoJson(const char*);
	
	template <class U>
	static bool addNoDefault(const char*);

	static T* create(const char*, LoomManager *scope); ///< Returns an instance of type T created by using the FactoryFunction coresponding to the provided name
	static T *create(JsonVariant, LoomManager *);   ///< Returns an instance of type T created from provided JSON

	static void print_registry();

private:

	static bool add(const char*, const FactoryFunction, const FactoryFunctionJson); ///< Adds a new Factory Pair derived from args to lookup table

	/// Use Meyer's singleton to prevent SIOF
	static std::vector<FactoryPair>& getFactoryTable();
};

///////////////////////////////////////////////////////////////////////////////
// Registers a module that hasn't implemented a default constructor
template <typename T>
template <class U>
bool Registry<T>::addNoDefault(const char* name)
{
	return add(name, nullptr, ConstructDefaultJson<T, U>);
}

///////////////////////////////////////////////////////////////////////////////
// Registers a module that hasn't implemented a JSON constructor
template <typename T>
template <class U>
bool Registry<T>::addNoJson(const char* name)
{
	return add(name, ConstructDefault<T, U>, nullptr);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
template <class U>
bool Registry<T>::add(const char* name)
{
	return add(name, ConstructDefault<T, U>, ConstructDefaultJson<T, U>);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool Registry<T>::add(const char* name, const Registry<T>::FactoryFunction ctor, const Registry<T>::FactoryFunctionJson ctorJson)
{
	auto lookUp = getFactoryTable();
	for (auto elem : lookUp) {
		// Add string processing for fuzzy search and debug suggestions?
		if (!strcmp(name, elem.name)) {
			return false; // On match, fail. Items two items cannot have the same creation name;
		}
	} // No match found, exiting for

	getFactoryTable().push_back(FactoryPair{name, ctor, ctorJson});
	return true;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
T* Registry<T>::create(const char* name, LoomManager* scope)
{
	auto lookUp = getFactoryTable();
	for (auto elem : lookUp) {
		// Add string processing for fuzzy search and debug suggestions?
		if (!strcmp(name, elem.name) && elem.ctor) {
			return elem.ctor(scope); // On match, return a *new* item of type name
		}
	} // No match found, exiting for
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
T *Registry<T>::create(JsonVariant target, LoomManager *scope)
{
	const char* name = target["name"].as<const char*>();

	for (auto elem : getFactoryTable()) {
		if (strcmp(name, elem.name) == 0) {

			// Parameters are provided and a ctor exists to accept json parameters
			if (elem.ctorJson && target["params"].is<JsonArray>()) {
				// Generate according to list of parameters
				return elem.ctorJson(scope, target["params"].as<JsonArrayConst>());
			}

			// 'params' key exists, but simply specifies 'default' (for legacy configs)
			else if (target["params"].is<const char*>() && strcmp(target["params"], "default") == 0 ) {
				if (elem.ctor) {
					return elem.ctor(scope);
				} else {
					LPrintln("Module [", name, "] does not have default settings, please provides params");
					return nullptr;
				}
			}

			// 'params' key omitted, assume default settings if default ctor available
			// If 'params' does exist and wasn't covered by the above two cases, assume there is 
			// something wrong with the parameters and let the else case catch it instead
			else if (elem.ctor && !target["params"]) {
				return elem.ctor(scope);
			}

			// Something was wrong with the config or the user specified to use a ctor
			// that didn't exist
			else {
				LPrintln("Check the config for component: ", name);
				return nullptr;
			}			
		}
	} // No match found, exiting for
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// This function exploits C++'s treatment of static initialization, where 
/// the compiler only expects a single instance to ever exist 
/// Due to this behavior, we can use this:
template <typename T>
std::vector<typename Registry<T>::FactoryPair>& Registry<T>::getFactoryTable()
{
	static std::vector<FactoryPair> lookUpTable;
	return lookUpTable;
}

///////////////////////////////////////////////////////////////////////////////
#include <typeinfo>

template <typename T>
void Registry<T>::print_registry()
{
	auto lookUp = getFactoryTable();
	LPrintln(typeid(T).name()+2, " Factory [", lookUp.size(), " classes]");

	for (auto elem : lookUp) {
		LPrintln(" - ", elem.name);
	}
}

///////////////////////////////////////////////////////////////////////////////

#define REGISTER(Factory, ModuleName, Key) \
	static const bool ModuleName##_entry = Registry<Factory>::add<ModuleName>(Key)

#define REGISTER_NODEFAULT(Factory, ModuleName, Key) \
	static const bool ModuleName##_entry = Registry<Factory>::addNoDefault<ModuleName>(Key)

#define REGISTER_NOJSON(Factory, ModuleName, Key) \
	static const bool ModuleName##_entry = Registry<Factory>::addNoJson<ModuleName>(Key)

///////////////////////////////////////////////////////////////////////////////
