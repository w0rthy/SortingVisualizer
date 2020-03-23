#include "fwk_control.h"
#include "../common.h"
#include "../sorts/sorts.h"
#include <iostream>
#include "../gfx/gfx_request.h"
#include "../aud/fwk/aud_fwk_wave.h"

#define INITIAL_SIZE 1000

//The main array to be used
ArrayList<int> array(INITIAL_SIZE);

List<int>* watchList = &array;
int watchListSize = array.sz;

thread controlThread;
thread sortThread;

Wave curWave;

string toLower(string& a) {
	string str;
	str.reserve(a.length());
	for (char c : a) {
		char tmp = c;
		if (tmp >= 'A' && tmp <= 'Z')
			tmp += 'a' - 'A';
		str += tmp;
	}
	return str;
}

struct Command;
vector<Command> commands;

using cmdFunc = function<void(vector<string>& args)>;

struct Command {
	string name;
	cmdFunc func;

	Command(string name, cmdFunc func) {
		this->name = toLower(name);
		this->func = func;
		commands.push_back(std::move(*this));
	}
};

//Parse and run the proper command
void parseCommand(string& s) {
	char token = ' ';
	int l = 0;
	string cmd;
	vector<string> args;
	while (l < s.length() && s[l] != token) {
		cmd += s[l];
		l++;
	}
	cmd = toLower(cmd);
	while (l < s.length()) {
		string tmp;
		while (l < s.length() && s[l] == token)
			l++;
		while (l < s.length() && s[l] != token) {
			tmp += s[l];
			l++;
		}
		if (tmp.size() > 0)
			args.push_back(std::move(tmp));
	}

	for (auto& c : commands) {
		if (cmd == c.name) {
			c.func(args);
			break;
		}
	}
}

//Routine for the control thread
void controlFunc() {
	string str;
	while (true) {
		std::getline(std::cin, str);
		parseCommand(str);
		//Free the token if used
		state.token_control--;
	}
}

void launchControlThread() {
	mergeWaves(&curWave, defaultWave, defaultWave); //Setup the waveform
	defaultWave = &curWave;

	state.token_control();
	for (int i = 0; i < array.sz; i++)
		array[i].val = i + 1;
	state.token_control--;

	controlThread = thread(controlFunc);
	controlThread.detach();
}

//
//Util Functions
//

int readInt() {
	int tmp;
	string s;
	std::cin >> tmp;
	std::cin.clear();
	std::getline(std::cin, s);
	return tmp;
}

void shuffleList() {
	if (state.sorting) {
		printf("Sort in progress\n");
		return;
	}
	//Need token to modify array
	state.token_control();

	srand((unsigned int)time(0));
	for (int i = 0; i < array.sz; i++) {
		int tmp = array[i].val;
		int rnd = rand() % array.sz;
		array[i].val = array[rnd].val;
		array[rnd].val = tmp;
	}

	state.token_control--;
}

void beginSort(Sort* toRun) {
	if (state.sorting) {
		printf("Sort in progress\n");
		return;
	}
	sortThread = thread([&, toRun]() {toRun->sort(array, watchList); });
	sortThread.detach();
}

//Util functions
Wave* promptForWave(vector<string>& args) {
	vector<Wave*> matches;

	for (auto& s : waves) {
		Wave* w = (Wave*)s;
		string name = toLower(w->name);
		bool success = true;
		for (auto& srch : args) {
			if (name.find(srch) == string::npos) {
				success = false;
				break;
			}
		}
		if (success)
			matches.push_back(w);
	}

	if (matches.size() <= 0) {
		printf("No waveforms found containing those terms.\n");
		return nullptr;
	}

	Wave* toUse = matches[0];

	if (matches.size() > 1) {
		printf("Enter the number of the correct result.\n");
		for (int i = 0; i < matches.size(); i++) {
			printf("  %d. %s\n", i + 1, matches[i]->name.c_str());
		}
		int res = readInt();
		res--;
		if (res < 0 || res >= matches.size()) {
			printf("Number out of bounds\n");
			return nullptr;
		}
		toUse = matches[res];
	}

	return toUse;
}

//
//Commands
//

void cmd_shuffle(vector<string>& args) {
	shuffleList();
}

void cmd_sort(vector<string>& args) {
	if (state.sorting) {
		printf("Sort in progress\n");
		return;
	}
	if (args.size() < 1) {
		printf("Must supply a name to search for.\n");
		return;
	}

	vector<Sort*> matches;

	for (auto& s : sorts) {
		string name = toLower(s->name);
		bool success = true;
		for (auto& srch : args) {
			if (name.find(srch)==string::npos) {
				success = false;
				break;
			}
		}
		if (success)
			matches.push_back(s);
	}

	if (matches.size() <= 0) {
		printf("No sorts found containing those terms.\n");
		return;
	}

	Sort* toRun = matches[0];

	if (matches.size() > 1) {
		printf("Enter the number of the correct result.\n");
		for (int i = 0; i < matches.size(); i++) {
			printf("  %d. %s\n", i + 1, matches[i]->name.c_str());
		}
		int res = readInt();
		res--;
		if (res < 0 || res >= matches.size()) {
			printf("Number out of bounds\n");
			return;
		}
		toRun = matches[res];
	}

	printf("Beginning %s\n", toRun->name.c_str());
	beginSort(toRun);
}

void cmd_ssort(vector<string>& args) {
	cmd_shuffle(args);
	cmd_sort(args);
}

void cmd_vis(vector<string>& args) {
#if GFX_ENABLED
	if (args.size() < 1) {
		printf("Must supply a name to search for.\n");
		return;
	}

	vector<Visualizer*> matches;

	for (auto& v : visualizers) {
		string name = toLower(v->name);
		bool success = true;
		for (auto& srch : args) {
			if (name.find(srch) == string::npos) {
				success = false;
				break;
			}
		}
		if (success)
			matches.push_back(v);
	}

	if (matches.size() <= 0) {
		printf("No visualizers found containing those terms.\n");
		return;
	}

	Visualizer* toUse = matches[0];

	if (matches.size() > 1) {
		printf("Enter the number of the correct result.\n");
		for (int i = 0; i < matches.size(); i++) {
			printf("  %d. %s\n", i + 1, matches[i]->name.c_str());
		}
		int res = readInt();
		res--;
		if (res < 0 || res >= matches.size()) {
			printf("Number out of bounds\n");
			return;
		}
		toUse = matches[res];
	}

	requestVisualizer = toUse;

	printf("Activated %s\n", toUse->name.c_str());

#else
	printf("Command not supported in this mode\n");
#endif
}

void cmd_wave(vector<string>& args) {
	if (args.size() < 1) {
		printf("Must supply a name to search for.\n");
		return;
	}

	Wave* toUse = promptForWave(args);
	if (!toUse)
		return;

	printf("Switching Default Waveform to %s\n", toUse->name.c_str());
	mergeWaves(&curWave, toUse, toUse);
}

void cmd_mixwave(vector<string>& args) {
	if (args.size() < 1) {
		printf("Must supply a name to search for.\n");
		return;
	}

	Wave* toUse = promptForWave(args);
	if (!toUse)
		return;

	printf("Switching Default Waveform to %s\n", toUse->name.c_str());
	mergeWaves(&curWave, &curWave, toUse);
}

void cmd_spd(vector<string>& args) {
	if (args.size() < 1)
		return;
	state.accessValueMul = 1.0 / atof(args[0]);
}

void cmd_stop(vector<string>& args) {
	sortThread.~thread();
}

void cmd_size(vector<string>& args) {
	if (args.size() < 1) {
		printf("Must enter a new size\n");
		return;
	}

	state.token_control();
	//array = ArrayList<int>(atoi(args[0]));
	for (int i = 0; i < array.sz; i++)
		array.arr[i] = i + 1;
	state.token_control--;
}

void cmd_ashuffle(vector<string>& args) {
	if (state.sorting) {
		printf("Sort in progress\n");
		return;
	}

	printf("Shuffling...\n");
	beginSort(sort_shuffle);
}

void cmd_asort(vector<string>& args) {
	if (state.sorting) {
		printf("Sort in progress\n");
		return;
	}
		printf("Shuffling...\n");
		sort_shuffle->sort(array, watchList);
		cmd_sort(args);
}

void cmd_vol(vector<string>& args) {
	if (args.size() < 1)
		return;
	aud_volume = atof(args[0]);
	printf("Volume adjusted\n");
}

//
//BEGIN DEMO STUFF, DELETE
//
#if GFX_ENABLED
#include "../gfx/visualizers/visualizers.h"

inline void sleep(unsigned int amt) {
	std::this_thread::sleep_for(std::chrono::milliseconds(amt));
}

void waitForSortFinish() {
	while (!state.sorting)
		sleep(10);
	while (state.sorting)
		sleep(10);
}

void runSort(Sort* s) {
	beginSort(s);
	waitForSortFinish();
}

struct DemoPair {
	Sort* sort;
	Visualizer* vis;
};

#define DEMOVIS visualizer_tree

//Need to implement a better solution for this
vector<DemoPair> demos = {
	{sort_selection,DEMOVIS},
	{sort_bubble,DEMOVIS},
	{sort_insertion,DEMOVIS},
	{sort_merge_inplace_weave,DEMOVIS},
	{sort_cocktail_shaker,DEMOVIS},
	//{sort_gravity,DEMOVIS},
	{sort_heap_max,DEMOVIS},
	//{sort_counting,DEMOVIS},
	{sort_shell,DEMOVIS},
	{sort_quick,DEMOVIS},
	//{sort_merge_agg_oop,DEMOVIS},
	{sort_bwradix_msd,DEMOVIS},
	{sort_bwradix_msd_inplace,DEMOVIS},
	{sort_bwradix_lsd,DEMOVIS},
	{sort_bwradix_lsd_inplace,DEMOVIS}
};

#undef DEMOVIS

/*
vector<DemoPair> demos = {
	{sort_selection,visualizer_basic},
	{sort_bubble,visualizer_basic_galaxy},
	{sort_insertion,visualizer_tile_floor},
	{sort_merge_inplace_weave,visualizer_cube_amalgam},
	{sort_cocktail_shaker,visualizer_galaxy_spiral},
	{sort_merge_inplace_shaker,visualizer_tile_floor},
	{sort_gravity,visualizer_basic},
	{sort_heap_max,visualizer_basic_galaxy},
	{sort_counting,visualizer_basic},
	{sort_shell,visualizer_basic_galaxy},
	{sort_quick,visualizer_galaxy_spiral},
	{sort_merge_agg_oop,visualizer_basic},
	{sort_bwradix_msd,visualizer_basic},
	{sort_bwradix_msd_inplace,visualizer_tile_floor},
	{sort_bwradix_lsd,visualizer_basic},
	{sort_bwradix_lsd_inplace,visualizer_cube_amalgam},
	{sort_bogo_improved,visualizer_basic}
};*/

void cmd_demo(vector<string>& args) {
	for (auto& d : demos) {
		if (currentVisualizer != d.vis) {
			requestVisualizer = d.vis;
			sleep(1500);
		}
		runSort(sort_shuffle);
		sleep(1000);
		runSort(d.sort);
		sleep(1500);
	}
}

#else

void cmd_demo(vector<string>& args) {

}

#endif
//
//END DEMO STUFF, DELETE
//

namespace {
	struct _ {
		_() {
			Command("shuffle", cmd_shuffle);
			Command("ashuffle", cmd_ashuffle);
			Command("sort", cmd_sort);
			Command("ssort", cmd_ssort);
			Command("asort", cmd_asort);
			Command("spd", cmd_spd);
			Command("stop", cmd_stop);
			Command("size", cmd_size);
			Command("vis", cmd_vis);
			Command("wave", cmd_wave);
			Command("mixwave", cmd_mixwave);
			Command("vol", cmd_vol);
			Command("demo", cmd_demo);
		}
	} _;
}