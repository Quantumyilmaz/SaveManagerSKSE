#pragma once

#include "Utils.h"


//namespace GameLock {
//    enum State { None, Locked, Unlocked };
//    inline State lastState = State::None;
//    void SetState(State currentState);
//};

const std::uint32_t kDataKey = 'ASSE';
const std::string settings_save_path = std::format("Data/SKSE/Plugins/{}/Settings.json", Utilities::mod_name);

namespace PluginSettings {
	inline bool running = true;
};

namespace LogSettings {
	inline bool log_trace = true;
	inline bool log_info = true;
	inline bool log_warning = true;
	inline bool log_error = true;
};

namespace SaveSettings {

    inline float last_save_time = 0.0f;     // in in-game hours
	inline bool block_autosaving_notif = false;
    inline int temp_min_save_interval = 1;  // in in-game minutes

	using namespace rapidjson;

	void SaveJSON();
	void LoadJSON();

	uint32_t GetSaveFlag();
	
	inline bool auto_save_to_json = true;
    inline bool regular_saves = false;
    inline float min_save_interval = std::max(0.0f, SaveSettings::temp_min_save_interval / 60.f); // in in-game hours

	rapidjson::Value to_json_main_stuff(Document::AllocatorType& a);
	rapidjson::Value to_json_timer_stuff(Document::AllocatorType& a);

	inline bool block = false;
    inline int queue_delay = 10;  // in seconds
    inline int ticker_interval = 1; // in seconds

	inline bool notifications = true;
	inline int timer_minutes = 0;
	inline int timer_seconds = 0;
	inline bool timer_running = false;
	inline bool timer_periodic = false;
	inline bool close_game_warning = false;
	inline bool close_game = false;
	inline bool save_on_quit = false;

	//inline bool freeze_game = true;

	enum Scenarios {
		MenuOpenContainerMenu,
		//MenuOpenBarterMenu,
		MenuOpenCraftingMenu,
		MenuOpenDialogueMenu,
		MenuOpenFavoritesMenu,
		MenuOpenInventoryMenu,
		MenuOpenLockpickingMenu,
		MenuOpenMagicMenu,
		MenuOpenMapMenu,

		MenuCloseContainerMenu,
        //MenuCloseBarterMenu,
        MenuCloseCraftingMenu,
        MenuCloseDialogueMenu,
        MenuCloseFavoritesMenu,
		MenuCloseInventoryMenu,
        MenuCloseLockpickingMenu,
        MenuCloseMagicMenu,
        MenuCloseMapMenu,

		Timer,

		SleepWaitStop,

		CombatEnter,
		CombatExit,

		QuitGame,

		LevelUp,
        SkillUp,
        QuestComplete,
        LocationDiscover,


	};

	namespace Menu {

        extern std::map<std::string, std::pair<bool, Scenarios>> Open;

		extern std::map<std::string, std::pair<bool, Scenarios>> Close;

		extern std::map < std::string, int> After;  // after menu is closed, wait x seconds before saving

		extern std::map<std::string, int> MinTimeSpent;  // min. time spent in the menu before saving

		rapidjson::Value to_json(Document::AllocatorType& a);

	};

	namespace SleepWait {
		inline bool sleep = false;
		inline bool wait = false;
		inline int sleep_time = 0; // save after in seconds
		inline int wait_time = 0; // save after in seconds

		rapidjson::Value to_json(Document::AllocatorType& a);
	};

	namespace Combat {
		inline bool entering_combat = false;
		inline bool exiting_combat = false;
		inline int combat_time = 0; // save after in seconds
		inline int min_combat_time_exit = 0; // min. time spent before save in seconds

		rapidjson::Value to_json(Document::AllocatorType& a);
	};

	namespace Other {
        inline bool save_on_level_up = false;
        inline bool save_on_skill_up = false;
        inline bool save_on_quest_complete = false;
        inline bool save_on_location_discover = false;
        //inline bool save_on_quit = false;

        rapidjson::Value to_json(Document::AllocatorType& a);
	
	};

	static std::map<Scenarios, std::string> scenario_names = {
        {MenuOpenContainerMenu,"MenuOpenContainerMenu"},
		//{MenuOpenBarterMenu,"MenuOpenBarterMenu"},
		{MenuOpenCraftingMenu,"MenuOpenCraftingMenu"},
		{MenuOpenDialogueMenu,"MenuOpenDialogueMenu"},
		{MenuOpenFavoritesMenu,"MenuOpenFavoritesMenu"},
		{MenuOpenInventoryMenu,"MenuOpenInventoryMenu"},
		{MenuOpenLockpickingMenu,"MenuOpenLockpickingMenu"},
		{MenuOpenMagicMenu,"MenuOpenMagicMenu"},
		{MenuOpenMapMenu,"MenuOpenMapMenu"},

		{MenuCloseContainerMenu, "MenuCloseContainerMenu"},
        //{MenuCloseBarterMenu, "MenuCloseBarterMenu"},
        {MenuCloseCraftingMenu, "MenuCloseCraftingMenu"},
        {MenuCloseDialogueMenu, "MenuCloseDialogueMenu"},
        {MenuCloseFavoritesMenu, "MenuCloseFavoritesMenu"},
        {MenuCloseInventoryMenu, "MenuCloseInventoryMenu"},
        {MenuCloseLockpickingMenu, "MenuCloseLockpickingMenu"},
        {MenuCloseMagicMenu, "MenuCloseMagicMenu"},
        {MenuCloseMapMenu, "MenuCloseMapMenu"},

		{Timer,"Timer"},

		{SleepWaitStop, "SleepWaitStop"},

		{CombatEnter, "CombatEnter"},
		{CombatExit, "CombatExit"},

		{QuitGame, "QuitGame"},

		{LevelUp, "LevelUp"},
        {SkillUp, "SkillUp"},
        {QuestComplete, "QuestComplete"},
        {LocationDiscover, "LocationDiscover"},

    };
};


inline void MainSaveFunction() {
    const auto curr_time = RE::Calendar::GetSingleton()->GetHoursPassed();
    if (curr_time - SaveSettings::last_save_time < SaveSettings::min_save_interval) return;
    SaveSettings::block_autosaving_notif = true;
    Utilities::AutoSave(SaveSettings::GetSaveFlag());
    SaveSettings::last_save_time = curr_time;
    if (SaveSettings::notifications) RE::DebugNotification((Utilities::mod_name + ": Game saved.").c_str());
}