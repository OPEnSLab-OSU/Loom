// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// @file		Loom_Temperature_Sync.h
// /// @brief		File for TempSync definition.
// /// @author		Luke Goertzen
// /// @date		2019
// /// @copyright	GNU General Public License v3.0
// ///
// ///////////////////////////////////////////////////////////////////////////////
//
// #ifdef LOOM_INCLUDE_SENSORS
// #pragma once
//
// #include "Module.h"
//
// namespace Loom {
//
//
// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// Used to synchronize temperatures between sensors that read sensors
// /// and modules that need it.
// ///
// /// @note	Sources include:
// ///	- Loom_MS5803 (code: 2110)
// ///	- Loom_SHT31D (code: 2111)
// /// @note	Dependants include:
// ///	- Loom_Analog (code 2001)
// ///
// /// @par Resources
// /// - [Documentation](https://openslab-osu.github.io/Loom/html/class_loom_temp_sync.html)
// ///
// ///////////////////////////////////////////////////////////////////////////////
// class TempSync : public Module
// {
//
// protected:
//
// public:
//
// //=============================================================================
// ///@name	CONSTRUCTORS / DESTRUCTOR
// /*@{*/ //======================================================================
//
// 	/// Constructor.
// 	///
// 	/// @param[in]  source		Module type to get temperature from
// 	/// @param[in]  dependant	Module type to forward temperature to
// 	TempSync(
// 		const Module::Type		source		= Module::Type::MS5803,
// 		const Module::Type		dependant	= Module::Type::Analog		// might be an array in the future
// 	);
//
// 	/// Constructor that takes Json Array, extracts args
// 	/// and delegates to regular constructor
// 	/// @param[in]	p		The array of constuctor args to expand
// 	TempSync(JsonArrayConst p);
//
// 	/// Destructor.
// 	~TempSync() = default;
//
// 	/// Verify that source and dependant modules exist, sync temperatures
// 	void	second_stage_ctor() override;
//
// //=============================================================================
// ///@name	OPERATION
// /*@{*/ //======================================================================
//
// 	/// Sync the temperature.
// 	/// Simply calls synchronization implementation TempSync::sync_temp().
// 	/// Allows the module to run regularly by emulating a sensor, which have
// 	/// thier measure methods called regularly.
// 	void		measure() { sync_temp(); }
//
// 	/// No package necessary.
// 	/// Implement with empty body.
// 	void		package(JsonObject json) override { /* do nothing */ };
//
// //=============================================================================
// ///@name	PRINT INFORMATION
// /*@{*/ //======================================================================
//
// 	void		print_config() const override;
//
// private:
//
// 	void		sync_temp();
//
// 	Module*	source;			///< Pointer to module to get temperature from
// 	Module* dependant;		///< Pointer to module to forward temperature to
//
// 	Module::Type source_type;		///< Type of source module
// 	Module::Type dependant_type;	///< Type of dependant module
// };
//
//
// // ///////////////////////////////////////////////////////////////////////////////
// REGISTER(Module, TempSync, "TempSync");
// // ///////////////////////////////////////////////////////////////////////////////
//
// }; // namespace Loom
//
// #endif // ifdef LOOM_INCLUDE_SENSORS
