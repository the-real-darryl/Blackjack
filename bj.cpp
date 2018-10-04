#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <limits>
#include <random>


#undef max
#undef min

#define DATA_QUEU_SIZE 100// 1 Gib
#define SPLIT_INIT 10000
#define LAYER_INIT 254
#define DECK 1
#define PLAYER 1
#define WORKERS 31
#define CONSUMMER 1
#define GAME_LIMIT 10000
#define DEBUG 0
#define DISPLAY 0//1000000000
#define STOP_AT 0

using std::endl;

template <class lenght_range, class max_range, class value_range>
class array_management
{
public:
	inline array_management();
	inline array_management(lenght_range init_buffer);
	inline array_management(lenght_range init_buffer, lenght_range init_increment);
	inline ~array_management();
	inline void instantiate(lenght_range init_buffer, lenght_range init_inc);
	inline void reset_state();
	inline void create_clean_array(lenght_range cells);
	inline void create_array(lenght_range cells);
	inline void insert_element(value_range input);
	inline void insert_element(value_range* input, lenght_range start, lenght_range end);
	inline void increment_if_equal(max_range input);
	inline void increment(max_range input);
	inline void reset_if_bigger(max_range input);
	inline void if_too_big_resize();
	inline void if_too_big_resize(lenght_range an_increment);
	inline void convert_to_array(uint8_t *&array_, size_t &counter);
	inline void convert_to_array(uint16_t *&array_, size_t &counter);
	inline void convert_to_array(uint32_t *&array_, size_t &counter);
	inline void convert_to_array(uint64_t *&array_, size_t &counter);
public:
	static lenght_range size;
	value_range * container;
	max_range max, min;
	lenght_range limit, counter, reading_counter, buffer, increment_value;
};

template <class lenght_range, class max_range, class value_range>
inline array_management<lenght_range, max_range, value_range>::array_management()
{
	// basic initialisation for when declared on the heap.
	buffer = 0;
	counter = 0;
	reading_counter = 0;
	increment_value = 0;
	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
}


template <class lenght_range, class max_range, class value_range>
inline array_management<lenght_range, max_range, value_range>::array_management(lenght_range init_buffer) : buffer(init_buffer), increment_value(init_buffer)
{
	// basic initialisation for when declared on the heap.
	counter = 0;
	reading_counter = 0;
	container = new(std::nothrow) value_range[buffer];
	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
}

template <class lenght_range, class max_range, class value_range>
inline array_management<lenght_range, max_range, value_range>::array_management(lenght_range init_buffer, lenght_range init_increment) : buffer(init_buffer), increment_value(init_increment)
{
	// basic initialisation for when declared on the heap.
	counter = 0;
	container = new(std::nothrow) value_range[buffer]();
	reading_counter = 0;
	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
}

template <class lenght_range, class max_range, class value_range>
inline array_management<lenght_range, max_range, value_range>::~array_management()
{
	if (buffer != 0)
	{
		delete[] container;
		buffer = 0;
	}

	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
	counter = 0;
	reading_counter = 0;

}


template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::instantiate(lenght_range init_buffer, lenght_range init_inc)
{
	// basic initialisation for when declared on the heap.
	buffer = init_buffer;
	container = new(std::nothrow) value_range[buffer]();
	counter = 0;
	reading_counter = 0;
	increment_value = init_inc;
	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
}

template <class lenght_range, class max_range, class value_range>

inline void array_management<lenght_range, max_range, value_range>::create_clean_array(lenght_range input)
{
	// basic initialisation for when declared on the heap.
	counter = 0;
	buffer = input;
	container = new(std::nothrow) value_range[buffer]();
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::create_array(lenght_range input)
{
	// basic initialisation for when declared on the heap.
	counter = 0;
	buffer = input;
	container = new(std::nothrow) value_range[buffer];
}
template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::reset_state()
{
	if (buffer != 0)
	{
		delete[] container;
		buffer = 0;
		counter = 0;
	}
	max = std::numeric_limits<max_range>::min();// we start by the lowest value to make sure that the max value only increase.
	min = std::numeric_limits<max_range>::max();// we start by the highest value to make sure that the min value only decrease.
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::insert_element(value_range input)
{
	container[counter] = input;
	counter++;
	if_too_big_resize();
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::insert_element(value_range* input, lenght_range start, lenght_range end)
{
	if_too_big_resize(counter + end + 1 - start);
	while (start <= end)
	{
		container[counter + start] = input[start];
		start++;
	}
	counter += end + 1 - start;
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::increment_if_equal(max_range input)
{
	if (max < input)
	{
		reset_if_bigger(input);
		max = input;
		container[max] = 1;
	}
	else
	{
		if (input == max)
		{
			container[input]++;
		}
	}
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::increment(max_range input)
{
	if (max < input)
	{
		reset_if_bigger(input);
		max = input;
		container[max] = 1;
	}
	else
	{
		container[input]++;
	}
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::reset_if_bigger(max_range input)
{
	buffer = input + 2;
	temporary_container = new(std::nothrow) value_range[buffer];

	for (lenght_range i = 0; i < max; i++)
	{
		temporary_container[i] = container[i];
	}
	delete[] container;

	container = temporary_container;
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::if_too_big_resize()
{
	if (counter >= buffer)
	{
		buffer += increment_value;

		temporary_container = new(std::nothrow) value_range[buffer]();
		uint64_t *copy;// going faster if the value range is not 64 bit wide

		copy = (uint64_t*)temporary_container;
		uint64_t *temp = (uint64_t*)container;

		lenght_range i = 0;
		while (i < (counter >> 3))
		{
			copy[i] = temp[i];
			i++;
		}

		i <<= 3;

		while (i < counter)
		{
			temporary_container[i] = container[i];
			i++;
		}
		delete[] container;
		container = temporary_container;
	}
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::if_too_big_resize(lenght_range an_increment)
{
	if (counter + an_increment >= buffer)
	{
		if (counter != 0)
		{
			if (increment_value > an_increment)
			{
				buffer += increment_value;
			}
			else
			{
				buffer += an_increment;
			}

			temporary_container = new(std::nothrow) value_range[buffer]();
			uint64_t *copy;// going faster if the value range is not 64 bit wide



			copy = (uint64_t*)temporary_container;
			uint64_t *temp = (uint64_t*)container;

			lenght_range i = 0;
			while (i < (counter >> 3))
			{
				copy[i] = temp[i];
				i++;
			}

			i <<= 3;

			while (i < counter)
			{
				temporary_container[i] = container[i];
				i++;
			}

			delete[] container;
			container = temporary_container;
		}
		else
		{
			if (buffer == 0)
			{
				container = new(std::nothrow) value_range[an_increment]();
			}
			else
			{
				delete[] container;
				container = new(std::nothrow) value_range[an_increment]();
			}
			buffer = an_increment;
		}
	}
}

template <class lenght_range, class max_range, class value_range>
inline  void array_management<lenght_range, max_range, value_range>::convert_to_array(uint8_t *&array_, size_t &counter)
{
	uint64_t *copy = (uint64_t*)(array_ + counter);
	uint64_t *temp = (uint64_t*)container;

	lenght_range i = 0;
	while (i < (counter >> 3))
	{
		copy[i] = temp[i];
		i++;
	}

	i <<= 3;

	while (i < counter)
	{
		array_[i] = temp[i];
		i++;
	}
	counter += this::counter;
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::convert_to_array(uint16_t *&array_, size_t &counter)
{
	uint64_t *copy = (uint64_t*)(array_ + counter);
	uint64_t *temp = (uint64_t*)container;

	lenght_range i = 0;
	while (i < (counter >> 3))
	{
		copy[i] = temp[i];
		i++;
	}

	i <<= 3;

	while (i < counter)
	{
		array_[i] = temp[i];
		i++;
	}
	counter += this::counter;
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::convert_to_array(uint32_t *&array_, size_t &counter)
{
	uint64_t *copy = (uint64_t*)(array_ + counter);
	uint64_t *temp = (uint64_t*)container;

	lenght_range i = 0;
	while (i < (counter >> 3))
	{
		copy[i] = temp[i];
		i++;
	}

	i <<= 3;

	while (i < counter)
	{
		array_[i] = temp[i];
		i++;
	}
	counter += this::counter;
}

template <class lenght_range, class max_range, class value_range>
inline void array_management<lenght_range, max_range, value_range>::convert_to_array(uint64_t *&array_, size_t &counter)
{
	uint64_t *copy = (uint64_t*)(array_ + counter);
	uint64_t *temp = (uint64_t*)container;

	lenght_range i = 0;
	while (i < (counter >> 3))
	{
		array_[i] = temp[i];
		i++;
	}

	i <<= 3;

	while (i < counter)
	{
		array_[i] = temp[i];
		i++;
	}
	counter += this::counter;
}


class blackjack
{
protected:
	inline void display_at_each();
	inline void stop_at();
	inline void insert_best_games_optimisation();
	inline void insert_best_games();
	inline void find_optimization();
public:
	static inline void evaluate_performance(uint32_t hit_table, uint32_t split_table);
	static void insert_into_table();
	static inline void consume();
	inline void shuffling();

	inline void simulation_();
	std::thread simulation_thread()
	{
		return std::thread([&] {simulation_(); });
	}
	inline void test_a_game();

	inline void insert_game_data(array_management<uint32_t, uint32_t, uint32_t> &v, array_management<uint8_t, uint32_t, uint32_t> &t);

	inline void insert_order_of_cards();

public:
	inline void find_played_hands();

	inline void reset_splitted_hand();

	inline void card_held_tcheck();
	inline void distributing_single_hand(uint8_t hands_counter_distribution);
	inline void ace_or_normal_card(uint8_t hands_counter);
	inline void ace_or_normal_card(uint8_t hand_coordinate, uint8_t card_coordinate);
	inline void not_busted_if_soft_hand(uint8_t Handscounter);
	inline void not_busted_if_soft_hand_coordinate(uint8_t hand_coordinate);
	inline void reconstituting_one_hand(uint8_t coordinate);
	inline void insert_hit_array_and_turns();
	inline void tcheck_for_dulicate(uint64_t start, uint64_t end);
	std::thread tcheck_for_dulicate_thread(uint64_t start, uint64_t end)
	{
		return std::thread([&] {tcheck_for_dulicate(start, end); });
	}
public:
	blackjack();
	~blackjack();
public:
	inline void first_game_distribution();
	inline void first_game_decision();
	inline void first_game_verification();

	inline void distribution_in_loop();
	inline void stand_in_loop();
	inline void verefication_in_loop();

	inline void distribution_split(uint8_t hands_counter_decision);
	inline void decision_split(uint8_t hands_counter_decision);
	inline void verefication_split();

	inline void where_to_raise_hit_array();

	inline void single_hit_no_split();
	inline void single_hit_split();

	inline uint8_t organizing_split_0();
	inline uint8_t organizing_split_n();

	inline void extract_data_mem2_0();

	inline void split_0();
	inline void split_n();

	inline void find_split_0();
	inline void find_split_n();

	inline void first_extract_0();
	inline void n_extract_0();
	inline void first_extract_n();
	inline void n_extract_n();

	inline void reset_for_next_game();
	inline void testing_all_possibilitys();

private:
	uint8_t * **layer;

public:
	static uint64_t free_store;
	static array_management<uint32_t, array_management<uint32_t, uint8_t, uint8_t>, array_management<uint32_t, uint8_t, uint8_t>> *produced_hit, *produced_hands;
	static uint8_t **produced_card;//produced refers to a producer/consummer phenomenon
	static uint32_t produced_card_counter;
	static bool volatile *memory_zone_is_full, *memory_zone_is_beeing_accesed;
	static volatile uint8_t consummer_reading;
	static uint32_t *hit_table, *split_table;// [664062500] = {}
	static uint16_t memory_blocks;
	static size_t block_size, card_size, hand_size, hit_size;
	static size_t *current_card_size, *current_hand_size, *current_hit_size;

	static double average_ratio_hands_card, average_ratio_hit_card;

	uint16_t mem_block_assigned;
	uint8_t worker_id;
	volatile uint64_t tested_possibillity, game_simulated;
	uint64_t max_simulation;

	array_management<uint32_t, uint16_t, uint16_t> played_order_of_cards;// 6 bit shift for every coordinate ( if 1 deck )

	uint64_t size_of_game;

	uint64_t random;

	bool all_hit_stand_have_been_tested = 0;
	bool all_splits_have_been_tested = 0;
	uint8_t ace_counter[26 * DECK] = {};
	bool breaker = 0;
	uint8_t buffer_for_layers;
	uint8_t card[52 * DECK] = {};
#if(DECK > 2)
	uint16_t card_held;
#else
	uint8_t card_held;
#endif
	array_management<uint64_t, uint8_t, uint8_t> best_hands_per_player[26 * DECK] = {}, best_hit_array[26 * DECK] = {};
	uint8_t card_end_of_decision[52 * DECK] = {};
	uint8_t cumulated_hands_per_turn[26 * DECK] = {};
	uint8_t distributed_cards[52 * DECK] = {};
	uint16_t hand_coordinate;
	uint8_t hands_per_player[26 * DECK] = {};
	uint8_t hand_value[26 * DECK] = {};
	uint8_t hit_array[26 * DECK] = {};
	int8_t hit_limit_form_last_split;
	uint8_t last_game_played_hands;
	uint8_t last_modified;
	uint8_t layer_being_tested;
	uint8_t loosing_per_round[26 * DECK] = {};
	uint8_t min_loosing[26 * DECK] = {};
	int8_t *modified_hand;
	int8_t *modified_round;
	int8_t *modified_turn;
	uint8_t  *new_split_spot;
	bool *no_split_found;
	uint8_t optimization[3 * 52 * DECK + 1] = {};
	uint8_t played_hands;
	int8_t played_hands_copy;
	uint16_t players_position[PLAYER] = { 1 };
	uint8_t round;
	uint8_t round_copy;
	uint16_t split_counter;
	uint32_t *split_spot;
	uint32_t *split_spot_buffer;
	uint8_t splited_hands[26 * DECK] = {};
	uint8_t *sub_split_spot;
	uint16_t temporary_hands_counter;
	uint8_t turn = 0;
	uint16_t turn_copy;
	uint8_t turn_per_hands[26 * DECK] = {};
	uint8_t ****TT;
	uint64_t *TT_counter = 0, *TT_size = 0;
	bool TT_resized = 0;

	uint8_t split_spots_for_next_layer[52 * DECK] = {}, number_of_split_found_in_game = 0;
};


inline blackjack::blackjack() : played_order_of_cards((uint32_t)10000, (uint32_t)100)
{
	for (uint8_t i = 0; i < 26 * DECK; i++)
	{
		best_hands_per_player[i].instantiate(1000, 1000);
		best_hit_array[i].instantiate(1000, 1000);
		min_loosing[i] = 255;
	}
	//loosing_per_round = new(std::nothrow) uint8_t[26 * DECK]();
	last_modified = 0;
	for (uint8_t i = 0; i <= 3 * 52 * DECK; i++)
	{
		optimization[i] = 255;
	}
	/*
	TT = new uint8_t ***[26];
	TT_size = new uint64_t[26];
	TT_counter = new uint64_t[26]();
	for (uint8_t i = 0; i < 26 * DECK; i++)
	{
	TT_size[i] = 100000;
	TT[i] = new uint8_t**[TT_size[i]];
	}
	*/
	//Basic initialisation for when declared on the heap.
	random = 70067;
	tested_possibillity = 0;
	game_simulated = 0;
	size_of_game = 0;
	max_simulation = GAME_LIMIT;

	std::cout << "bJ_constructpr called !!" << "     ";

	temporary_hands_counter = 0;

	breaker = 0;
	card_held = 0;
	turn = 0;
	round = 0;
	played_hands = 0;
	all_hit_stand_have_been_tested = 0;
	all_splits_have_been_tested = 0;
	hit_limit_form_last_split = 0;

	// Layers are created 6 at the time to reduce the resizing and copyinblackjack::
	buffer_for_layers = LAYER_INIT;
	layer_being_tested = 0;

	layer = new(std::nothrow) uint8_t**[buffer_for_layers];// up to 6 splitted hand in a row without resizinblackjack::

	split_spot = new(std::nothrow) uint32_t[buffer_for_layers]();
	sub_split_spot = new(std::nothrow) uint8_t[buffer_for_layers]();

	split_spot_buffer = new(std::nothrow) uint32_t[buffer_for_layers];

	split_spot_buffer[0] = SPLIT_INIT;
	layer[0] = new(std::nothrow) uint8_t*[SPLIT_INIT];
	/*
	for (uint16_t i = 0; i < LAYER_INIT; i++)
	{
	split_spot_buffer[i] = SPLIT_INIT;
	layer[i] = new(std::nothrow) uint8_t*[SPLIT_INIT];
	}*/

	no_split_found = new(std::nothrow) bool[2 * buffer_for_layers]();

	//distributed_cards = new(std::nothrow) uint8_t[52 * DECK];

	modified_hand = new(std::nothrow) int8_t[buffer_for_layers];

	modified_round = new(std::nothrow) int8_t[buffer_for_layers];

	modified_turn = new(std::nothrow) int8_t[buffer_for_layers];
#if(DECK > 2)
	for (uint16_t i = 2; i < 15; i++)
	{
		for (uint16_t i1 = 0; i1 < 4 * DECK; i1++)
		{
			if (i < 12)
			{
				card[(((i - 2) * DECK) << 2) + i1] = i;
			}
			else
			{
				card[(((i - 2) * DECK) << 2) + i1] = 10;
			}
		}
	}
#else
	for (uint8_t i = 2; i < 15; i++)
	{
		for (uint8_t i1 = 0; i1 < 4 * DECK; i1++)
		{
			if (i < 12)
			{
				card[(((i - 2) * DECK) << 2) + i1] = i;
			}
			else
			{
				card[(((i - 2) * DECK) << 2) + i1] = 10;
			}
		}
	}
#endif
	/*
	card[0] = 2;
	card[1] = 10;
	card[2] = 11;
	card[3] = 11;
	card[4] = 5;
	card[5] = 6;
	card[6] = 11;
	card[7] = 4;
	card[8] = 10;
	card[9] = 6;
	card[10] = 10;
	card[11] = 5;
	card[12] = 3;
	card[13] = 10;
	card[14] = 2;
	card[15] = 3;
	card[16] = 8;
	card[17] = 7;
	card[18] = 10;
	card[19] = 10;
	card[20] = 3;
	card[21] = 9;
	card[22] = 5;
	card[23] = 6;
	card[24] = 11;
	card[25] = 10;
	card[26] = 10;
	card[27] = 10;
	card[28] = 2;
	card[29] = 3;
	card[30] = 4;
	card[31] = 10;
	card[32] = 4;
	card[33] = 10;
	card[34] = 4;
	card[35] = 10;
	card[36] = 8;
	card[37] = 9;
	card[38] = 9;
	card[39] = 10;
	card[40] = 10;
	card[41] = 8;
	card[42] = 9;
	card[43] = 10;
	card[44] = 10;
	card[45] = 8;
	card[46] = 7;
	card[47] = 6;
	card[48] = 5;
	card[49] = 7;
	card[50] = 2;
	card[51] = 7;
	*/
}

inline blackjack::~blackjack()
{
	delete[] layer[0];

	delete[] layer;

	delete[] split_spot_buffer;// last crash 06 oct 11:45 ( LastState 130 ), 132
	delete[] split_spot;// last crash 06 oct 11:09 last crash 06 oct 11:29 ( last state 132 )
	delete[] sub_split_spot;
	delete[] no_split_found;
	delete[] modified_hand;
	delete[] modified_round;
	delete[] modified_turn;

	delete[] distributed_cards;
}

inline void blackjack::find_optimization()
{
	uint8_t w = 0;
	int8_t start;
	int8_t r = round - 1;
	while (r > last_modified)
	{
		start = cumulated_hands_per_turn[(r - 1)*(PLAYER + 1) + PLAYER];
		w = loosing_per_round[round - 1] - loosing_per_round[r - 1];
		optimization[3 * card_end_of_decision[start - 1]] = w;
		optimization[3 * card_end_of_decision[start - 1] + 1] = round - r;
		optimization[3 * card_end_of_decision[start - 1] + 2] = played_hands - start;
		r--;
	}

	if (last_modified)
	{
		start = cumulated_hands_per_turn[(r - 1)*(PLAYER + 1) + PLAYER];
		w = loosing_per_round[round - 1] - loosing_per_round[r - 1];
		optimization[3 * card_end_of_decision[start - 1]] = w;
		optimization[3 * card_end_of_decision[start - 1] + 1] = round - r;
		optimization[3 * card_end_of_decision[start - 1] + 2] = played_hands - start;
	}
	else
	{
		w = loosing_per_round[round - 1];
		optimization[0] = w;
		optimization[1] = round;
		optimization[2] = played_hands;
	}
}

inline void blackjack::display_at_each()
{
	std::cout << "b : " << best_hands_per_player[played_hands - 1].counter << endl;
	if (tested_possibillity % DISPLAY == 1)
	{
		std::cout << tested_possibillity << " - ";
		for (uint8_t i = 0; i < layer_being_tested; i++)
		{
			std::cout << split_spot[i] << " ";
		}
		std::cout << endl;
	}
}
inline void blackjack::reset_splitted_hand()
{
	if (modified_round[layer_being_tested] == modified_round[layer_being_tested - 1])
	{
		if (modified_turn[layer_being_tested] == modified_turn[layer_being_tested - 1])
		{
			splited_hands[modified_hand[layer_being_tested] + 1] = 0;
			return;
		}
		else
		{
			splited_hands[modified_hand[layer_being_tested]] = 0;
			splited_hands[modified_hand[layer_being_tested] + 1] = 0;
			return;
		}
	}
	else
	{
		splited_hands[modified_hand[layer_being_tested]] = 0;
		splited_hands[modified_hand[layer_being_tested] + 1] = 0;
	}
}

inline void blackjack::insert_best_games()
{
	if (loosing_per_round[round - 1] < min_loosing[played_hands - 1])
	{
		min_loosing[played_hands - 1] = loosing_per_round[round - 1];

		best_hit_array[played_hands - 1].counter = 0;
		best_hit_array[played_hands - 1].if_too_big_resize(played_hands);
		for (uint8_t hands = 0; hands < played_hands; hands++)
		{
			best_hit_array[played_hands - 1].insert_element(hit_array[hands]);
		}

		best_hands_per_player[played_hands - 1].counter = 0;
		best_hands_per_player[played_hands - 1].if_too_big_resize(round*PLAYER + 1);
		best_hands_per_player[played_hands - 1].insert_element(round*PLAYER);
		for (uint8_t round_ = 0; round_ < round*PLAYER; round_++)
		{
			best_hands_per_player[played_hands - 1].insert_element(hands_per_player[round_]);
		}
	}
	else if (loosing_per_round[round - 1] == min_loosing[played_hands - 1])
	{
		best_hit_array[played_hands - 1].if_too_big_resize(played_hands);
		for (uint8_t hands = 0; hands < played_hands; hands++)
		{
			best_hit_array[played_hands - 1].insert_element(hit_array[hands]);
		}
		best_hands_per_player[played_hands - 1].if_too_big_resize(round*PLAYER + 1);
		best_hands_per_player[played_hands - 1].insert_element(round*PLAYER);
		for (uint8_t round_ = 0; round_ < round*PLAYER; round_++)
		{
			best_hands_per_player[played_hands - 1].insert_element(hands_per_player[round_]);
		}
	}
}
inline void blackjack::insert_best_games_optimisation()
{

	if ((loosing_per_round[round - 1] + optimization[3 * card_held]) < min_loosing[played_hands - 1])
	{
		min_loosing[played_hands - 1] = loosing_per_round[round - 1] + optimization[3 * card_held];

		best_hit_array[played_hands + optimization[3 * card_held + 2]].counter = 0;
		best_hit_array[played_hands + optimization[3 * card_held + 2]].if_too_big_resize(played_hands + optimization[3 * card_held + 2]);
		for (uint8_t hands = 0; hands < played_hands; hands++)
		{
			best_hit_array[played_hands + optimization[3 * card_held + 2]].insert_element(hit_array[hands]);
		}
		best_hit_array[played_hands + optimization[3 * card_held + 2]].counter += optimization[3 * card_held + 2];

		best_hands_per_player[played_hands + optimization[3 * card_held + 2]].counter = 0;
		best_hands_per_player[played_hands + optimization[3 * card_held + 2]].if_too_big_resize((round + optimization[3 * card_held + 1])*PLAYER + 1);
		best_hands_per_player[played_hands + optimization[3 * card_held + 2]].insert_element((round + optimization[3 * card_held + 1])*PLAYER);
		for (uint8_t round_ = 0; round_ < (round + optimization[3 * card_held + 1])*PLAYER; round_++)
		{
			best_hands_per_player[played_hands + optimization[3 * card_held + 2]].insert_element(hands_per_player[round_]);
		}
	}
	else if ((loosing_per_round[round - 1] + optimization[3 * card_held]) == min_loosing[played_hands - 1])
	{
		best_hit_array[played_hands + optimization[3 * card_held + 2]].if_too_big_resize(played_hands + optimization[3 * card_held + 2]);
		for (uint8_t hands = 0; hands < played_hands; hands++)
		{
			best_hit_array[played_hands + optimization[3 * card_held + 2]].insert_element(hit_array[hands]);
		}
		best_hit_array[played_hands + optimization[3 * card_held + 2]].counter += optimization[3 * card_held + 2];

		best_hands_per_player[played_hands + optimization[3 * card_held + 2]].if_too_big_resize((round + optimization[3 * card_held + 1])*PLAYER + 1);
		best_hands_per_player[played_hands + optimization[3 * card_held + 2]].insert_element((round + optimization[3 * card_held + 1])*PLAYER);
		for (uint8_t round_ = 0; round_ < (round + optimization[3 * card_held + 1])*PLAYER; round_++)
		{
			best_hands_per_player[played_hands + optimization[3 * card_held + 2]].insert_element(hands_per_player[round_]);
		}
	}
}

inline void blackjack::insert_hit_array_and_turns()
{
	TT[played_hands - 1][TT_counter[played_hands - 1]] = new(std::nothrow) uint8_t*[3];
	TT[played_hands - 1][TT_counter[played_hands - 1]][0] = new(std::nothrow) uint8_t[played_hands + 1];
	TT[played_hands - 1][TT_counter[played_hands - 1]][1] = new(std::nothrow) uint8_t[played_hands + 1];
	TT[played_hands - 1][TT_counter[played_hands - 1]][2] = new(std::nothrow) uint8_t[2 * (played_hands)];

	for (uint8_t i = 0; i < played_hands; i++)
	{
		TT[played_hands - 1][TT_counter[played_hands - 1]][0][i] = hit_array[i];
		TT[played_hands - 1][TT_counter[played_hands - 1]][1][i] = turn_per_hands[i];
		TT[played_hands - 1][TT_counter[played_hands - 1]][2][i << 1] = distributed_cards[i << 1];
		TT[played_hands - 1][TT_counter[played_hands - 1]][2][(i << 1) + 1] = turn_per_hands[(i << 1) + 1];
	}
	TT_counter[played_hands - 1]++;
	if (TT_counter[played_hands - 1] == TT_size[played_hands - 1])
	{
		TT_resized = 1;
		TT_size[played_hands - 1] += TT_size[played_hands - 1];
		uint8_t*** copy = new(std::nothrow) uint8_t**[TT_size[played_hands - 1]];
		for (uint64_t i = 0; i < TT_counter[played_hands - 1]; i++)
		{
			copy[i] = TT[played_hands - 1][i];
			copy[i][0] = TT[played_hands - 1][i][0];
			copy[i][1] = TT[played_hands - 1][i][1];
			copy[i][2] = TT[played_hands - 1][i][2];
		}
		delete[] TT[played_hands - 1];
		TT[played_hands - 1] = copy;
	}
}

inline void blackjack::tcheck_for_dulicate(uint64_t start, uint64_t end)
{
	for (uint64_t i = start; i < end; i++)
	{
		if (TT[played_hands - 1][i][0][0] == TT[played_hands - 1][TT_counter[played_hands - 1] - 1][0][0])
		{
			int hit_same = 1, turn_same = 1, distrib_same = 1;
			for (uint8_t i1 = 0; i1 < played_hands; i1++)
			{
				hit_same &= (TT[played_hands - 1][TT_counter[played_hands - 1] - 1][0][i1] == TT[played_hands - 1][i][0][i1]);
				turn_same &= (TT[played_hands - 1][TT_counter[played_hands - 1] - 1][1][i1] = TT[played_hands - 1][i][1][i1]);
				distrib_same &= (TT[played_hands - 1][TT_counter[played_hands - 1] - 1][2][i1 << 1] = TT[played_hands - 1][i][2][i1 << 1]);
				distrib_same &= (TT[played_hands - 1][TT_counter[played_hands - 1] - 1][2][(i1 << 1) + 1] = TT[played_hands - 1][i][2][(i1 << 1) + 1]);
				if (!(hit_same & turn_same & distrib_same))
				{
					return;
				}
			}
			if (hit_same & turn_same & distrib_same)
			{
				std::cout << "there was a dupliacate !" << endl;
			}
		}
	}
}

inline void blackjack::extract_data_mem2_0()
{
	layer_being_tested = 0;

	split_spot[0]--;

	// Setting up the next layer, the layer in witch we will store the result for further splittin
	no_split_found[2] = 1;// initializing the next layer as false. ( nothing found until...)
	split_spot_buffer[1] = SPLIT_INIT;

	layer[1] = new(std::nothrow) uint8_t*[split_spot_buffer[1]];

	sub_split_spot[0] = layer[0][split_spot[0]][0] - 1;// Testing the last possibility first.
													   // Highest splitspot coordinate first
	round = layer[0][split_spot[0]][2 * sub_split_spot[0] + 2];
	played_hands_copy = layer[0][split_spot[0]][2 * sub_split_spot[0] + 3];

	// copying back the data related to Round
	first_extract_0();
}

inline void blackjack::stop_at()
{
	if (tested_possibillity >= STOP_AT)
	{
		int a = 0;
	}
}


inline void blackjack::split_0()
{
	if (no_split_found[2])
	{
		if (sub_split_spot[0])
		{
			splited_hands[modified_hand[0]] = 0;
			splited_hands[modified_hand[0] + 1] = 0;
			//saving the result of the split

			//saving the result of the split
			hands_per_player[modified_round[0] * PLAYER + modified_turn[0]]--;
			cumulated_hands_per_turn[modified_round[0] * (PLAYER + 1) + modified_turn[0]]--;

			sub_split_spot[0]--;


			round = layer[0][split_spot[0]][2 * sub_split_spot[0] + 2];
			played_hands_copy = layer[0][split_spot[0]][2 * sub_split_spot[0] + 3];
#if(PLAYER != 1)
			if (modified_round[0] == round)
			{
				n_extract_0();
			}
#endif

			return;
		}
		else
		{
			if (split_spot[0])
			{
				delete[] layer[0][split_spot[0]];// delete split spot beeing tested.

				hands_per_player[modified_round[0] * PLAYER + modified_turn[0]]--;
				cumulated_hands_per_turn[modified_round[0] * (PLAYER + 1) + modified_turn[0]]--;

				split_spot[0]--;

				splited_hands[modified_hand[0]] = 0;
				splited_hands[modified_hand[0] + 1] = 0;

				no_split_found[2] = 1;// initializing to false.

				split_spot[1] = 0;

				sub_split_spot[0] = layer[0][split_spot[0]][0] - 1;// Testing the last possibility first.

																   // Highest splitspot coordinate first
				round = layer[0][split_spot[0]][2 * sub_split_spot[0] + 2];
				played_hands_copy = layer[0][split_spot[0]][2 * sub_split_spot[0] + 3];

				// copying back the data related to Round
				first_extract_0();


				return;
			}
			else
			{
				all_splits_have_been_tested = 1;
				delete[] layer[1];// delete the layer used to store the splitspots from the layer after the layer beeing tested.

				delete[] layer[0][0];// delete the split spot beeing tested from the layer beeing tested.

				delete[] layer[0];// delete the whole layer actualy beeing tested.


				delete[] layer;// delete the cha bang

				delete[] split_spot_buffer;
				delete[] split_spot;
				delete[] sub_split_spot;
				delete[] no_split_found;
				delete[] modified_hand;
				delete[] modified_round;
				delete[] modified_turn;

				buffer_for_layers = LAYER_INIT;
				return;
			}
		}
	}
	else
	{
		all_splits_have_been_tested = 0;
		layer_being_tested = 1;

		// Preparing the next round to be tested.
		split_spot[1]--;
		sub_split_spot[1] = layer[1][split_spot[1]][0] - 1;// Testing the last possibility first.
														   // Highest splitspot coordinate first
		round = layer[1][split_spot[1]][2 * sub_split_spot[1] + 2];
		played_hands_copy = layer[1][split_spot[1]][2 * sub_split_spot[1] + 3];

		// copying back the data related to Round
		first_extract_n();

		// Preparing to receive the splitspots from the layer after the one that is going to be tested.
		no_split_found[4] = 1;// Initialize as false.
		split_spot_buffer[2] = SPLIT_INIT;

		layer[2] = new(std::nothrow) uint8_t*[split_spot_buffer[2]];
		split_spot[2] = 0;
		return;
	}
}

inline void blackjack::split_n()
{
	if (no_split_found[2 * (layer_being_tested + 1)])
	{
		if (sub_split_spot[layer_being_tested])
		{
		diminish_sub:
			reset_splitted_hand();

			hands_per_player[modified_round[layer_being_tested] * PLAYER + modified_turn[layer_being_tested]]--;
			cumulated_hands_per_turn[modified_round[layer_being_tested] * (PLAYER + 1) + modified_turn[layer_being_tested]]--;



			sub_split_spot[layer_being_tested]--;

			round = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 2];
			played_hands_copy = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 3];
#if(PLAYER != 1)
			if (modified_round[layer_being_tested] == round)
			{
				n_extract_n();
			}
#endif

			return;
		}
		else
		{
			if (split_spot[layer_being_tested])
			{
			diminish_split:
				hands_per_player[modified_round[layer_being_tested] * PLAYER + modified_turn[layer_being_tested]]--;
				cumulated_hands_per_turn[modified_round[layer_being_tested] * (PLAYER + 1) + modified_turn[layer_being_tested]]--;

				reset_splitted_hand();
				delete[] layer[layer_being_tested][split_spot[layer_being_tested]];

#if(OPTIMISE_SPLIT)
				current_split_min = 255;
#endif
				split_spot[layer_being_tested]--;

				no_split_found[2 * (layer_being_tested + 1)] = 1;
				split_spot[layer_being_tested + 1] = 0;

				sub_split_spot[layer_being_tested] = layer[layer_being_tested][split_spot[layer_being_tested]][0] - 1;// Testing the last possibility first.

																													  // Highest splitspot coordinate first
				round = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 2];
				played_hands_copy = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 3];

				// copying back the data related to Round
				first_extract_n();


				return;
			}
			else
			{
				delete[] layer[layer_being_tested + 1];// delete the one created to store the splitspots form the next layer, because there is none.

				split_spot_buffer[layer_being_tested + 1] = SPLIT_INIT;

				all_splits_have_been_tested = 0;
				do
				{
					delete[] layer[layer_being_tested][0];// delete the splitspot beeing tested from the layer beeing tested.

					delete[] layer[layer_being_tested];// delete the layer witch is beeing tested.

					split_spot_buffer[layer_being_tested] = SPLIT_INIT;// make sure nothing goes out of bound.

																	   //*(layer + LayerBeingTested) = NULL;// This is shit beacause the layer array is still active.
					hands_per_player[modified_round[layer_being_tested] * PLAYER + modified_turn[layer_being_tested]]--;
					cumulated_hands_per_turn[modified_round[layer_being_tested] * (PLAYER + 1) + modified_turn[layer_being_tested]]--;

					if (layer_being_tested)
					{
						reset_splitted_hand();
					}
					else
					{
						splited_hands[modified_hand[layer_being_tested]] = 0;
						splited_hands[modified_hand[layer_being_tested] + 1] = 0;

						all_splits_have_been_tested = 1;
						delete[] layer;

						delete[] split_spot_buffer;// last crash 06 oct 11:45 ( LastState 130 ), 132
						delete[] split_spot;// last crash 06 oct 11:09 last crash 06 oct 11:29 ( last state 132 )
						delete[] sub_split_spot;
						delete[] no_split_found;
						delete[] modified_hand;
						delete[] modified_round;
						delete[] modified_turn;

						buffer_for_layers = LAYER_INIT;
						return;
					}
					layer_being_tested--;
				} while ((!split_spot[layer_being_tested]) && (!sub_split_spot[layer_being_tested]));

				split_spot_buffer[layer_being_tested + 1] = SPLIT_INIT;
				layer[layer_being_tested + 1] = new(std::nothrow) uint8_t*[split_spot_buffer[layer_being_tested + 1]];


				if (layer_being_tested)
				{
					if (sub_split_spot[layer_being_tested])
					{
						no_split_found[2 * (layer_being_tested + 1)] = 1;
						split_spot[layer_being_tested + 1] = 0;
						goto diminish_sub;
					}
					else
					{
						goto diminish_split;
					}
				}
				else
				{
					no_split_found[2] = 1;
					layer[1] = new(std::nothrow) uint8_t*[SPLIT_INIT];
					split_spot_buffer[1] = SPLIT_INIT;

					if (sub_split_spot[0])
					{
						splited_hands[modified_hand[0]] = 0;
						splited_hands[modified_hand[0] + 1] = 0;


						hands_per_player[modified_round[0] * PLAYER + modified_turn[0]]--;
						cumulated_hands_per_turn[modified_round[0] * (PLAYER + 1) + modified_turn[0]]--;

						sub_split_spot[0]--;

						round = layer[0][split_spot[0]][2 * sub_split_spot[0] + 2];
						played_hands_copy = layer[0][split_spot[0]][2 * sub_split_spot[0] + 3];
#if(PLAYER != 1)
						if (modified_round[0] == round)
						{
							n_extract_0();
						}
#endif
						return;
					}
					else
					{
						hands_per_player[modified_round[0] * PLAYER + modified_turn[0]]--;
						cumulated_hands_per_turn[modified_round[0] * (PLAYER + 1) + modified_turn[0]]--;


						splited_hands[modified_hand[0]] = 0;
						splited_hands[modified_hand[0] + 1] = 0;
						delete[] layer[0][split_spot[0]];

						split_spot[0]--;

						no_split_found[2 * (1)] = 1;
						split_spot[1] = 0;

						sub_split_spot[0] = layer[layer_being_tested][split_spot[0]][0] - 1;// Testing the last possibility first.

																							// Highest splitspot coordinate first
						round = layer[0][split_spot[0]][2 * sub_split_spot[0] + 2];
						played_hands_copy = layer[0][split_spot[0]][2 * sub_split_spot[0] + 3];

						// copying back the data related to Round
						first_extract_0();


						return;
					}
				}
			}
		}
	}
	else
	{
		all_splits_have_been_tested = 0;
		if (layer_being_tested == (buffer_for_layers - 3))// Resize the whole layer arrays
		{
			// Creating the copy arrays.
			uint32_t* split_spot_copy = split_spot;
			uint32_t* split_spot_buffer_copy = split_spot_buffer;
			bool* bool_temp = no_split_found;
			uint8_t* sub_split_spot_copy = sub_split_spot;
			int8_t* modified_hand_copy = modified_hand;
			int8_t* modified_round_copy = modified_round;
			int8_t* modified_turn_copy = modified_turn;

			buffer_for_layers += LAYER_INIT;

			// Resize the original arrays.
			split_spot = new(std::nothrow) uint32_t[buffer_for_layers]();
			sub_split_spot = new(std::nothrow) uint8_t[buffer_for_layers]();
			split_spot_buffer = new(std::nothrow) uint32_t[buffer_for_layers]();
			no_split_found = new(std::nothrow) bool[2 * buffer_for_layers]();
			modified_hand = new(std::nothrow) int8_t[buffer_for_layers]();
			modified_round = new(std::nothrow) int8_t[buffer_for_layers];
			modified_turn = new(std::nothrow) int8_t[buffer_for_layers];


			if (no_split_found[2 * (layer_being_tested + 1)])
			{
				delete[] layer[layer_being_tested + 1];

				uint8_t ***layer_copy = new(std::nothrow) uint8_t**[buffer_for_layers];// 1 to convert coordinate in quantity and 1 to because


				for (uint8_t i = 0; i <= layer_being_tested; i++)
				{
					layer_copy[i] = new(std::nothrow) uint8_t*[split_spot[i] + 1];
					layer_copy[i] = layer[i];
					for (uint32_t i1 = 0; i1 <= split_spot[i]; i1++)
					{
						layer_copy[i][i1] = layer[i][i1];
					}
				}

				for (uint8_t i = 0; i <= layer_being_tested; i++)
				{
					split_spot[i] = split_spot_copy[i];
					split_spot_buffer[i] = split_spot_buffer_copy[i];
					no_split_found[i << 1] = bool_temp[i << 1];
					no_split_found[(i << 1) + 1] = bool_temp[(i << 1) + 1];
					sub_split_spot[i] = sub_split_spot_copy[i];
					modified_hand[i] = modified_hand_copy[i];
					modified_round[i] = modified_round_copy[i];
					modified_turn[i] = modified_turn_copy[i];
				}

				delete[] layer;

				layer = layer_copy;
			}
			else
			{

				uint8_t*** layer_copy = new(std::nothrow) uint8_t**[buffer_for_layers];// 1 to convert coordinate in quantity and 1 to because

				uint8_t i = 0;
				while (i <= layer_being_tested)
				{
					layer_copy[i] = new(std::nothrow) uint8_t*[split_spot[i] + 1];
					layer_copy[i] = layer[i];
					for (uint32_t i1 = 0; i1 <= split_spot[i]; i1++)
					{
						layer_copy[i][i1] = layer[i][i1];
					}
					i++;
				}
				layer_copy[i] = new(std::nothrow) uint8_t*[split_spot[i]];
				layer_copy[i] = layer[i];
				for (uint32_t i1 = 0; i1 < split_spot[i]; i1++)
				{
					layer_copy[i][i1] = layer[i][i1];
				}
				for (uint16_t i = 0; i <= (layer_being_tested + 1); i++)
				{
					split_spot[i] = split_spot_copy[i];
					split_spot_buffer[i] = split_spot_buffer_copy[i];
					no_split_found[i << 1] = bool_temp[i << 1];
					no_split_found[(i << 1) + 1] = bool_temp[(i << 1) + 1];
					sub_split_spot[i] = sub_split_spot_copy[i];
					modified_hand[i] = modified_hand_copy[i];
					modified_round[i] = modified_round_copy[i];
					modified_turn[i] = modified_turn_copy[i];
				}

				delete[] layer;

				layer = layer_copy;
			}


			// Done copying back the data in the original arrays.

			// Deleting the temporary arrays.
			delete[] split_spot_copy;
			delete[] sub_split_spot_copy;
			delete[] split_spot_buffer_copy;
			delete[] bool_temp;
			delete[] modified_hand_copy;
			delete[] modified_round_copy;
			delete[] modified_turn_copy;
		}
		layer_being_tested++;
		// preparing the layer witch is going to be tested.
		split_spot[layer_being_tested]--;
		sub_split_spot[layer_being_tested] = layer[layer_being_tested][split_spot[layer_being_tested]][0] - 1;// Testing the last possibility first.																											 // Highest splitspot coordinate first
		round = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 2];
		played_hands_copy = layer[layer_being_tested][split_spot[layer_being_tested]][2 * sub_split_spot[layer_being_tested] + 3];

		// copying back the data related to Round
		first_extract_n();


		// preparing to receive the splitspots from the layer after the layer that is going to be tested.
		no_split_found[2 * (layer_being_tested + 1)] = 1;// Initiate as false.
		split_spot_buffer[layer_being_tested + 1] = SPLIT_INIT;
		split_spot[layer_being_tested + 1] = 0;

		layer[layer_being_tested + 1] = new(std::nothrow) uint8_t*[split_spot_buffer[layer_being_tested + 1]];
	}
}


inline void blackjack::find_played_hands()
{
	if (round)
	{
		played_hands = cumulated_hands_per_turn[(round - 1)*(PLAYER + 1) + PLAYER];
		return;
	}
	else
	{
		played_hands = 0;
	}
}


inline void blackjack::first_extract_0()
{
#if(DECK > 2)
	uint32_t copy_counter = 2 + 2 * (layer[0][split_spot[0]][0]);
#else
	uint16_t copy_counter = 2 + 2 * (layer[0][split_spot[0]][0]);
#endif

	for (uint8_t i = 0; i <= round; i++)
	{
#if(PLAYER != 1)
		for (uint8_t i1 = 0; i1 < PLAYER; i1++)
		{
			cumulated_hands_per_turn[i*(PLAYER + 1) + i1] = layer[0][split_spot[0]][copy_counter];
			copy_counter++;
		}
		cumulated_hands_per_turn[i*(PLAYER + 1) + PLAYER] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
#else
		cumulated_hands_per_turn[(i << 1)] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
		cumulated_hands_per_turn[(i << 1) + 1] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
#endif
		loosing_per_round[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
	}

	for (uint8_t i = 0; i < layer[0][split_spot[0]][1]; i++)
	{
		turn_per_hands[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;

		card_end_of_decision[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;

		distributed_cards[i << 1] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
		distributed_cards[(i << 1) + 1] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
		ace_counter[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
		hand_value[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;

		hit_array[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
	}
}

inline void blackjack::n_extract_0()
{
#if(DECK > 2)
	uint32_t copy_counter = 2 + 2 * (layer[0][split_spot[0]][0]) + (2 + PLAYER)*(round);
#else
	uint16_t copy_counter = 2 + 2 * (layer[0][split_spot[0]][0]) + (2 + PLAYER)*(round);
#endif

#if(PLAYER != 1)
	for (uint8_t i1 = 0; i1 < PLAYER; i1++)
	{
		cumulated_hands_per_turn[round*(PLAYER + 1) + i1] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
	}
	cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER] = layer[0][split_spot[0]][copy_counter];
	copy_counter++;
#else
	cumulated_hands_per_turn[(round << 1)] = layer[0][split_spot[0]][copy_counter];
	copy_counter++;
	cumulated_hands_per_turn[(round << 1) + 1] = layer[0][split_spot[0]][copy_counter];
	copy_counter++;
#endif
	loosing_per_round[round] = layer[0][split_spot[0]][copy_counter];
	copy_counter++;

	uint8_t start;
	if (round)
	{
		start = cumulated_hands_per_turn[(round - 1) * (PLAYER + 1) + PLAYER];
	}
	else
	{
		start = 0;
	}
	copy_counter = 2 + 2 * (layer[0][split_spot[0]][0]);
	copy_counter += (2 + PLAYER)*(layer[0][split_spot[0]][(layer[0][split_spot[0]][0] << 1)] + 1);
	copy_counter += start * 7;
	for (uint8_t i = start; i < cumulated_hands_per_turn[round * (PLAYER + 1) + PLAYER]; i++)
	{
		turn_per_hands[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;

		card_end_of_decision[i] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;

		distributed_cards[i << 1] = layer[0][split_spot[0]][copy_counter];
		copy_counter++;
		distributed_cards[(i << 1) + 1] = layer[0][split_spot[0]][copy_counter];
		copy_counter += 4;
	}
}


inline void blackjack::first_extract_n()
{
#if(DECK > 2)
	uint32_t copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
#else
	uint16_t copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
#endif
	for (uint8_t i = modified_round[layer_being_tested - 1]; i <= round; i++)
	{
#if(PLAYER != 1)
		for (uint8_t i1 = 0; i1 < PLAYER; i1++)
		{
			cumulated_hands_per_turn[i*(PLAYER + 1) + i1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
			copy_counter++;
		}
		cumulated_hands_per_turn[i*(PLAYER + 1) + PLAYER] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
#else
		cumulated_hands_per_turn[(i << 1)] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
		cumulated_hands_per_turn[(i << 1) + 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
#endif
		loosing_per_round[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
	}
	uint8_t start;
	if (modified_round[layer_being_tested - 1])
	{
		start = cumulated_hands_per_turn[(modified_round[layer_being_tested - 1] - 1) * (PLAYER + 1) + PLAYER];
	}
	else
	{
		start = 0;
	}
	for (uint8_t i = start; i < layer[layer_being_tested][split_spot[layer_being_tested]][1]; i++)
	{
		turn_per_hands[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;

		card_end_of_decision[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;

		distributed_cards[i << 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
		distributed_cards[(i << 1) + 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
		ace_counter[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
		hand_value[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;

		hit_array[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
	}
}

inline void blackjack::n_extract_n()
{
#if(DECK > 2)
	uint32_t copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
#else
	uint16_t copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
#endif
	copy_counter += (2 + PLAYER)*(round - modified_round[layer_being_tested - 1]);

#if(PLAYER != 1)
	for (uint8_t i1 = 0; i1 < PLAYER; i1++)
	{
		cumulated_hands_per_turn[round*(PLAYER + 1) + i1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
	}
	cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
	copy_counter++;
#else
	cumulated_hands_per_turn[(round << 1)] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
	copy_counter++;
	cumulated_hands_per_turn[(round << 1) + 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
	copy_counter++;
#endif
	loosing_per_round[round] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
	copy_counter++;

	uint8_t start;
	if (round)
	{
		start = cumulated_hands_per_turn[(round - 1) * (PLAYER + 1) + PLAYER];
		copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
		copy_counter += (2 + PLAYER)*(layer[layer_being_tested][split_spot[layer_being_tested]]
			[(layer[layer_being_tested][split_spot[layer_being_tested]][0] << 1)] + 1 - modified_round[layer_being_tested - 1]);
		if (modified_round[layer_being_tested - 1])
		{
			copy_counter += (start - cumulated_hands_per_turn[(modified_round[layer_being_tested - 1] - 1) * (PLAYER + 1) + PLAYER]) * 7;
		}
		else
		{
			copy_counter += start * 7;
		}
	}
	else
	{
		start = 0;
		copy_counter = 2 + 2 * (layer[layer_being_tested][split_spot[layer_being_tested]][0]);
		copy_counter += (2 + PLAYER)*(layer[layer_being_tested][split_spot[layer_being_tested]]
			[(layer[layer_being_tested][split_spot[layer_being_tested]][0] << 1)] + 1 - modified_round[layer_being_tested - 1]);
		copy_counter += start * 7;
	}
	for (uint8_t i = start; i < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER]; i++)
	{
		turn_per_hands[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;

		card_end_of_decision[i] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;

		distributed_cards[i << 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter++;
		distributed_cards[(i << 1) + 1] = layer[layer_being_tested][split_spot[layer_being_tested]][copy_counter];
		copy_counter += 4;
	}
}


inline void blackjack::distributing_single_hand(uint8_t hands_counter_distribution)
{
	hand_value[played_hands + hands_counter_distribution] = card[card_held];
	distributed_cards[(played_hands + hands_counter_distribution) << 1] = card[card_held];
	if (card[card_held] == 11)
	{
		ace_counter[played_hands + hands_counter_distribution] = 1;
	}
	else
	{
		ace_counter[played_hands + hands_counter_distribution] = 0;
	}
	card_held++;

	hand_value[played_hands + hands_counter_distribution] += card[card_held];
	distributed_cards[((played_hands + hands_counter_distribution) << 1) + 1] = card[card_held];
	if (card[card_held] == 11)
	{
		if (hand_value[played_hands + hands_counter_distribution] == 22)
		{
			hand_value[played_hands + hands_counter_distribution] = 12;
		}
		else
		{
			ace_counter[played_hands + hands_counter_distribution]++;
		}
	}
	card_held++;
}

inline void blackjack::reconstituting_one_hand(uint8_t coordinate)
{
	hand_value[coordinate] = distributed_cards[coordinate << 1];

	if (distributed_cards[coordinate << 1] == 11)
	{
		ace_counter[coordinate] = 1;
	}
	else
	{
		ace_counter[coordinate] = 0;
	}

	hand_value[coordinate] += distributed_cards[(coordinate << 1) + 1];

	if (distributed_cards[(coordinate << 1) + 1] == 11)
	{
		if (hand_value[coordinate] == 22)
		{
			hand_value[coordinate] = 12;
			return;
		}
		else
		{
			ace_counter[coordinate]++;
		}
	}
}

inline void blackjack::not_busted_if_soft_hand(uint8_t Handscounter)
{
	if (hand_value[played_hands + Handscounter] > 21)
	{
		if (card[card_held] == 11)
		{
			hand_value[played_hands + Handscounter] -= 10;
			return;
		}
		else if (ace_counter[played_hands + Handscounter])
		{
			hand_value[played_hands + Handscounter] -= 10;
			ace_counter[played_hands + Handscounter]--;
			return;
		}
	}
	else if (card[card_held] == 11)
	{
		ace_counter[played_hands + Handscounter]++;
	}
}

inline void blackjack::not_busted_if_soft_hand_coordinate(uint8_t hand_coordinate)
{
	if (hand_value[hand_coordinate] > 21)
	{
		if (card[card_held] == 11)
		{
			hand_value[hand_coordinate] -= 10;
			return;
		}
		else if (ace_counter[hand_coordinate])
		{
			hand_value[hand_coordinate] -= 10;
			ace_counter[hand_coordinate]--;
			return;
		}
	}
	else if (card[card_held] == 11)
	{
		ace_counter[hand_coordinate]++;
	}
}

inline void blackjack::ace_or_normal_card(uint8_t hands_counter)
{
	if (card[card_held] == 11)
	{
		if (hand_value[played_hands + hands_counter] > 10)
		{
			hand_value[played_hands + hands_counter] += 1;
			return;
		}
		else
		{
			ace_counter[played_hands + hands_counter]++;
			hand_value[played_hands + hands_counter] += 11;
			return;
		}
	}
	else
	{
		hand_value[played_hands + hands_counter] += card[card_held];
	}
}

inline void blackjack::ace_or_normal_card(uint8_t hand_coordinate, uint8_t card_coordinate)
{
	if (card[card_coordinate] == 11)
	{
		if (hand_value[hand_coordinate] > 10)
		{
			hand_value[hand_coordinate] += 1;
			return;
		}
		else
		{
			ace_counter[hand_coordinate]++;
			hand_value[hand_coordinate] += 11;
			return;
		}
	}
	else
	{
		hand_value[hand_coordinate] += card[card_coordinate];
	}
}


inline void blackjack::card_held_tcheck()
{
	if (card_held >= 52 * DECK)
	{
		breaker = 1;
	}
}

inline void blackjack::reset_for_next_game()
{
	for (uint8_t i = 0; i < 26 * DECK; i++)
	{
		best_hands_per_player[i].counter = 0;
		best_hit_array[i].counter = 0;

		min_loosing[i] = 0;
	}

	last_modified = 0;
	for (uint8_t i = 0; i <= 3 * 52 * DECK; i++)
	{
		optimization[i] = 255;
	}
	/*
	TT_counter--;
	do
	{
	delete[] TT[TT_counter][0];
	delete[] TT[TT_counter][1];
	delete[] TT[TT_counter][2];
	delete[] TT[TT_counter];
	TT_counter--;
	} while (TT_counter != 0);

	delete[] TT;
	TT = new uint8_t **[10000000];
	TT_size = 10000000;
	TT_resized = 0;
	*/
	for (uint16_t i = 0; i < DECK * 26; i++)
	{
		splited_hands[i] = 0;
	}
	number_of_split_found_in_game = 0;
	hit_limit_form_last_split = 0;
	played_hands = 0;
	round = 0;
	all_hit_stand_have_been_tested = 0;
	card_held = 0;

	// Layers are created 6 at the time to reduce the resizing and copyinblackjack::
	buffer_for_layers = LAYER_INIT;
	layer_being_tested = 0;


	layer = new(std::nothrow) uint8_t**[buffer_for_layers];// up to 6 splitted hand in a row without resizinblackjack::

	split_spot = new(std::nothrow) uint32_t[buffer_for_layers]();
	sub_split_spot = new(std::nothrow) uint8_t[buffer_for_layers]();
	split_spot_buffer = new(std::nothrow) uint32_t[buffer_for_layers]();

	split_spot_buffer[0] = SPLIT_INIT;

	layer[0] = new(std::nothrow) uint8_t*[split_spot_buffer[0]];

	no_split_found = new(std::nothrow) bool[2 * buffer_for_layers]();
	all_splits_have_been_tested = 0;
	breaker = 0;

	for (uint16_t i = 0; i < 26 * DECK; i++)
	{
		hands_per_player[i] = 1;
	}

	modified_hand = new(std::nothrow) int8_t[buffer_for_layers]();// If Layer bigger than 4, resize.
	modified_round = new(std::nothrow) int8_t[buffer_for_layers]();// If Layer bigger than 4 , resize.
	modified_turn = new(std::nothrow) int8_t[buffer_for_layers]();// If Layer bigger than 4 , resize.
}


inline void blackjack::first_game_distribution()
{
	uint8_t hands_counter_distribution = 0;

	while (hands_counter_distribution < PLAYER)
	{
		distributing_single_hand(hands_counter_distribution);
		hands_counter_distribution++;
	}
	// distributing the dealers cards
	hand_value[played_hands + hands_counter_distribution] = card[card_held];
	distributed_cards[(played_hands + hands_counter_distribution) << 1] = card[card_held];
	if (card[card_held] == 11)
	{
		ace_counter[played_hands + hands_counter_distribution] = 1;
	}
	else
	{
		ace_counter[played_hands + hands_counter_distribution] = 0;
	}
	card_held++;

	hand_value[played_hands + hands_counter_distribution] += card[card_held];
	distributed_cards[((played_hands + hands_counter_distribution) << 1) + 1] = card[card_held];
	if (card[card_held] == 11)
	{
		if (hand_value[played_hands + hands_counter_distribution] == 22)
		{
			hand_value[played_hands + hands_counter_distribution] = 12;
		}
		else
		{
			ace_counter[played_hands + hands_counter_distribution]++;
		}
	}
	card_held++;

	if (card_held == 52 * DECK)
	{
		hands_counter_distribution = 0;
		while (hands_counter_distribution < PLAYER)
		{
			turn_per_hands[played_hands + hands_counter_distribution] = hands_counter_distribution;
			card_end_of_decision[played_hands + hands_counter_distribution] = card_held;
			hit_array[played_hands + hands_counter_distribution] = 0;
			hands_counter_distribution++;
		}
		hit_array[played_hands + hands_counter_distribution] = 0;
		turn_per_hands[played_hands + hands_counter_distribution] = hands_counter_distribution;
		card_end_of_decision[played_hands + hands_counter_distribution] = card_held;

		breaker = 1;
		return;
	}
	else
	{
		first_game_decision();
	}
}

inline void blackjack::first_game_decision()
{
	uint8_t hands_counter_decision = 0;

#if(PLAYER != 1)
	while (hands_counter_decision < PLAYER)
	{
#endif
		turn_per_hands[played_hands + hands_counter_decision] = hands_counter_decision;
		hit_array[played_hands + hands_counter_decision] = 0;

		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);

			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;

			if (card_held >= 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;

#if(PLAYER != 1)
				while (hands_counter_decision < PLAYER)
				{
					turn_per_hands[played_hands + hands_counter_decision] = hands_counter_decision;// Player's turn.
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
				}
#endif
				turn_per_hands[played_hands + hands_counter_decision] = PLAYER;
				hit_array[played_hands + hands_counter_decision] = 0;
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;

				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;
		hands_counter_decision++;
#if(PLAYER != 1)
	}
#endif
	//dealers turn
	turn_per_hands[played_hands + hands_counter_decision] = PLAYER;// Player's turn.

	hit_array[played_hands + hands_counter_decision] = 0;

	while (hand_value[played_hands + hands_counter_decision] < 17)	// Dealers turn
	{
		hand_value[played_hands + hands_counter_decision] += card[card_held];

		not_busted_if_soft_hand(hands_counter_decision);

		card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
		hit_array[played_hands + hands_counter_decision]++;
		if (card_held >= 52 * DECK)
		{
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			breaker = 1;
			return;
		}
	}
	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
}

inline void blackjack::first_game_verification()
{
	uint8_t hands_counter_verefication = 0;
	if (round)
	{
		loosing_per_round[round] = loosing_per_round[round - 1];
	}
	else
	{
		loosing_per_round[0] = 0;
	}

#if(PLAYER != 1)
	while (hands_counter_verefication < PLAYER)
	{
#endif

		if (hand_value[played_hands + hands_counter_verefication] > 21)
		{
			loosing_per_round[round]++;
		}
		else
		{
			if (hand_value[played_hands + hands_counter_verefication] < hand_value[played_hands + PLAYER]
				&& hand_value[played_hands + PLAYER] <= 21)
			{
				loosing_per_round[round]++;
			}
		}

		if (distributed_cards[(played_hands + hands_counter_verefication) << 1] == distributed_cards[((played_hands + hands_counter_verefication) << 1) + 1])
		{
			if (card_held <= 52 * DECK - 2)
			{
				split_spots_for_next_layer[number_of_split_found_in_game << 1] = round;
				split_spots_for_next_layer[(number_of_split_found_in_game << 1) + 1] = played_hands + hands_counter_verefication;
				number_of_split_found_in_game++;
			}
		}
		hands_per_player[round*PLAYER + hands_counter_verefication] = 1;

		hands_counter_verefication++;
		cumulated_hands_per_turn[round*(PLAYER + 1) + hands_counter_verefication - 1] = played_hands + hands_counter_verefication;
#if(PLAYER != 1)
	}
#endif

	played_hands += hands_counter_verefication + 1;
	cumulated_hands_per_turn[round*(PLAYER + 1) + hands_counter_verefication] = played_hands;

	round++;
	//*(statistic.KnowncardscoordinatePerPlayerPerRound_Hit_blackjack::array + round*(Player + 1)) = card_held;

	if (card_held > (52 * DECK - 2 * (PLAYER + 1)))
	{
		breaker = 1;
	}
}


inline void blackjack::distribution_in_loop()//tester avec goto a la place de tester breaker a chaque fois quob hit
{
	uint8_t hands_counter_distribution = 0;

	while (hands_counter_distribution < PLAYER)
	{
		distributing_single_hand(hands_counter_distribution);

		hands_counter_distribution++;
	}
	// distributing the dealers cards
	hand_value[played_hands + hands_counter_distribution] = card[card_held];
	distributed_cards[(played_hands + hands_counter_distribution) << 1] = card[card_held];
	if (card[card_held] == 11)
	{
		ace_counter[played_hands + hands_counter_distribution] = 1;
	}
	else
	{
		ace_counter[played_hands + hands_counter_distribution] = 0;
	}
	card_held++;

	hand_value[played_hands + hands_counter_distribution] += card[card_held];
	distributed_cards[((played_hands + hands_counter_distribution) << 1) + 1] = card[card_held];
	if (card[card_held] == 11)
	{
		if (hand_value[played_hands + hands_counter_distribution] == 22)
		{
			hand_value[played_hands + hands_counter_distribution] = 12;
		}
		else
		{
			ace_counter[played_hands + hands_counter_distribution]++;
		}
	}
	card_held++;


	if (card_held == 52 * DECK)
	{
		hands_counter_distribution = 0;
		while (hands_counter_distribution < PLAYER)
		{
			card_end_of_decision[played_hands + hands_counter_distribution] = card_held;
			hit_array[played_hands + hands_counter_distribution] = 0;
			hands_counter_distribution++;
		}

		//required for the testing function, NEED TO BE REMOVED FOR THE RELEASE VERSION !!!!
		hit_array[played_hands + hands_counter_distribution] = 0;
		//required for the testing function, NEED TO BE REMOVED FOR THE RELEASE VERSION !!!!

		card_end_of_decision[played_hands + hands_counter_distribution] = card_held;

		breaker = 1;
		return;
	}
	else
	{
		stand_in_loop();
	}
}

inline void blackjack::stand_in_loop()
{
	uint8_t hands_counter_decision = 0;
#if(PLAYER != 1)
	while (hands_counter_decision < PLAYER)
	{
#endif
		hit_array[played_hands + hands_counter_decision] = 0;

		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);

			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;
			if (card_held >= 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;
#if(PLAYER != 1)
				while (hands_counter_decision < PLAYER)
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
				}
#endif
				hit_array[played_hands + hands_counter_decision] = 0;
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;

				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;
		hands_counter_decision++;
#if(PLAYER != 1)
	}
#endif
	//dealers turn
	hit_array[played_hands + hands_counter_decision] = 0;

	while (hand_value[played_hands + hands_counter_decision] < 17)	// Dealers turn
	{
		hand_value[played_hands + hands_counter_decision] += card[card_held];

		not_busted_if_soft_hand(hands_counter_decision);

		card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
		hit_array[played_hands + hands_counter_decision]++;
		if (card_held >= 52 * DECK)
		{
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			breaker = 1;
			return;
		}
	}

	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
}

inline void blackjack::verefication_in_loop()
{
	uint8_t hands_counter_verefication = 0;
	if (round)
	{
		loosing_per_round[round] = loosing_per_round[round - 1];
	}
	else
	{
		loosing_per_round[0] = 0;
	}
#if(PLAYER != 1)
	while (hands_counter_verefication < PLAYER)
	{
#endif

		if (hand_value[played_hands + hands_counter_verefication] > 21)
		{
			loosing_per_round[round]++;
		}
		else
		{
			if (hand_value[played_hands + hands_counter_verefication] < hand_value[played_hands + PLAYER]
				&& hand_value[played_hands + PLAYER] <= 21)
			{
				loosing_per_round[round]++;
			}
		}

		if (distributed_cards[(played_hands + hands_counter_verefication) << 1] == distributed_cards[((played_hands + hands_counter_verefication) << 1) + 1])
		{
			if (card_held <= 52 * DECK - 2)
			{
				split_spots_for_next_layer[number_of_split_found_in_game << 1] = round;
				split_spots_for_next_layer[(number_of_split_found_in_game << 1) + 1] = played_hands + hands_counter_verefication;
				number_of_split_found_in_game++;
			}
		}
		hands_per_player[round*PLAYER + hands_counter_verefication] = 1;
		turn_per_hands[played_hands + hands_counter_verefication] = hands_counter_verefication;
		cumulated_hands_per_turn[round*(PLAYER + 1) + hands_counter_verefication] = played_hands + hands_counter_verefication + 1;
		hands_counter_verefication++;
#if(PLAYER != 1)
	}
#endif
	played_hands += hands_counter_verefication + 1;
	turn_per_hands[played_hands - 1] = PLAYER;
	cumulated_hands_per_turn[round*(PLAYER + 1) + hands_counter_verefication] = played_hands;

	round++;

	if (card_held > (52 * DECK - 2 * (PLAYER + 1)))
	{
		breaker = 1;
	}
}


inline void blackjack::distribution_split(uint8_t hands_counter_decision)
{
	// Adding one card in parrallel to each one of the new hands.
	if (distributed_cards[temporary_hands_counter << 1] == 11)
	{
		ace_counter[temporary_hands_counter] = 1;
		hand_value[temporary_hands_counter] = 11;
	}
	else
	{
		ace_counter[temporary_hands_counter] = 0;
		hand_value[temporary_hands_counter] = distributed_cards[temporary_hands_counter << 1];
	}

	if (card[card_held] == 11)
	{
		if (hand_value[temporary_hands_counter] > 10)
		{
			hand_value[temporary_hands_counter] += 1;
		}
		else
		{
			ace_counter[temporary_hands_counter]++;
			hand_value[temporary_hands_counter] += 11;
		}
	}
	else
	{
		hand_value[temporary_hands_counter] += card[card_held];
	}
	distributed_cards[(temporary_hands_counter << 1) + 1] = card[card_held];

	card_held++;

	if (distributed_cards[temporary_hands_counter << 1] == 11)
	{
		ace_counter[temporary_hands_counter + 1] = 1;
		hand_value[temporary_hands_counter + 1] = 11;
	}
	else
	{
		ace_counter[temporary_hands_counter + 1] = 0;
		hand_value[temporary_hands_counter + 1] = distributed_cards[temporary_hands_counter << 1];
	}
	distributed_cards[(temporary_hands_counter + 1) << 1] = distributed_cards[temporary_hands_counter << 1];

	if (card[card_held] == 11)
	{
		if (hand_value[temporary_hands_counter + 1] > 10)
		{
			hand_value[temporary_hands_counter + 1] += 1;
		}
		else
		{
			ace_counter[temporary_hands_counter + 1]++;
			hand_value[temporary_hands_counter + 1] += 11;
		}
	}
	else
	{
		hand_value[temporary_hands_counter + 1] += card[card_held];
	}

	distributed_cards[((temporary_hands_counter + 1) << 1) + 1] = card[card_held];

	card_held++;

	turn_per_hands[temporary_hands_counter + 1] = turn;

	modified_hand[layer_being_tested] = temporary_hands_counter;
	modified_round[layer_being_tested] = round;

	if (card_held >= 52 * DECK)
	{
		for (uint8_t i = 0; i < hands_per_player[round*PLAYER + turn]; i++)
		{
			hit_array[played_hands + hands_counter_decision] = 0;
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			hands_counter_decision++;
		}
		cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;

		turn++;
#if(PLAYER != 1)
		while (turn < PLAYER)
		{
			hit_array[played_hands + hands_counter_decision] = 0;
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			hands_counter_decision++;
			cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
			turn++;
		}
#endif
		cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision + 1;
		hit_array[played_hands + hands_counter_decision] = 0;
		card_end_of_decision[played_hands + hands_counter_decision] = card_held;

		breaker = 1;
		return;
	}
	else
	{
		decision_split(hands_counter_decision);
	}
}

inline void blackjack::decision_split(uint8_t hands_counter_decision)
{
	for (uint16_t i = 0; i < hands_per_player[round*PLAYER + turn]; i++)
	{
		hit_array[played_hands + hands_counter_decision] = 0;

		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);

			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;

			if (card_held >= 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;
				i++;

				while (i < hands_per_player[round*PLAYER + turn])
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					i++;
					hands_counter_decision++;
				}
				cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
				turn++;
#if(PLAYER != 1)
				while (turn < PLAYER)
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
					cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
					turn++;
				}
#endif
				hit_array[played_hands + hands_counter_decision] = 0;
				cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision + 1;
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;

				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;

		hands_counter_decision++;
	}
	cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
	turn++;
#if(PLAYER != 1)
	while (turn < PLAYER)// all of the player hands have been played , they stay to their distributed value. No hit. we now need to reach the dealers hand.
	{
		hit_array[played_hands + hands_counter_decision] = 0;
		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);

			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;
			if (card_held >= 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;
				cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
				turn++;
				while (turn < PLAYER)
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
					cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
					turn++;
				}

				cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision + 1;
				hit_array[played_hands + hands_counter_decision] = 0;
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;

				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;

		hands_counter_decision++;
		cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision;
		turn++;
	}
#endif
	hit_array[played_hands + hands_counter_decision] = 0;

	while (hand_value[played_hands + hands_counter_decision] < 17)	// Dealers turn
	{
		hand_value[played_hands + hands_counter_decision] += card[card_held];

		not_busted_if_soft_hand(hands_counter_decision);
		card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
		hit_array[played_hands + hands_counter_decision]++;
		if (card_held >= 52 * DECK)
		{
			cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision + 1;
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			breaker = 1;
			return;
		}
	}
	cumulated_hands_per_turn[round*(PLAYER + 1) + turn] = played_hands + hands_counter_decision + 1;
	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
}

inline void blackjack::verefication_split()
{
	uint8_t turn_couter = 0;
	uint8_t hands_counter_verefication = 0;
	bool split_not_found = 1;
	if (round)
	{
		loosing_per_round[round] = loosing_per_round[round - 1];
	}
	else
	{
		loosing_per_round[0] = 0;
	}

#if(PLAYER != 1)
	while (turn_couter < PLAYER)
	{
#endif

		uint8_t hands_per_player_copy = 0;
		while (hands_per_player_copy < hands_per_player[round*PLAYER + turn_couter])
		{

			if (hand_value[played_hands + hands_counter_verefication] > 21)
			{
				loosing_per_round[round]++;
			}
			else
			{
				if (hand_value[played_hands + hands_counter_verefication] < hand_value[cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER] - 1] &&
					hand_value[cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER] - 1] <= 21)
				{
					loosing_per_round[round]++;
				}
			}

			if ((modified_hand[layer_being_tested] == played_hands + hands_counter_verefication) ||
				((modified_hand[layer_being_tested] + 1) == played_hands + hands_counter_verefication))
			{
				if (distributed_cards[(played_hands + hands_counter_verefication) << 1] == distributed_cards[((played_hands + hands_counter_verefication) << 1) + 1])
				{
					if (split_not_found)
					{
						if (hands_per_player[round*PLAYER + turn_couter] < 4)
						{
							if (card_held <= 52 * DECK - 2)
							{
								split_spots_for_next_layer[number_of_split_found_in_game << 1] = round;
								split_spots_for_next_layer[(number_of_split_found_in_game << 1) + 1] = played_hands + hands_counter_verefication;
								number_of_split_found_in_game++;
								split_not_found = 0;
							}
						}
					}
				}
			}
			turn_per_hands[played_hands + hands_counter_verefication] = turn_couter;

			hands_counter_verefication++;
			hands_per_player_copy++;
		}
		turn_couter++;
#if(PLAYER != 1)
	}

#endif
	played_hands += hands_counter_verefication + 1;

	turn_per_hands[played_hands - 1] = PLAYER;
	last_modified = round;
	round++;

	if (card_held > (52 * DECK - 2 * (PLAYER + 1)))
	{
		breaker = 1;
		return;
	}
}


inline void blackjack::find_split_0()
{
	if (number_of_split_found_in_game)
	{
		if (split_spot[0] == split_spot_buffer[0])// If another SplitSpot cant fit, resize.
		{
			// There will be 12 more array in the resized version of Layer0.
			split_spot_buffer[0] += SPLIT_INIT;
			// Finding the size of all the arrays contained in the Layer0.

			uint8_t** c;
			c = new(std::nothrow) uint8_t*[split_spot_buffer[0]];

			for (uint32_t i = 0; i < split_spot[0]; i++)
			{
				c[i] = layer[0][i];
			}

			delete[] layer[0];

			layer[0] = c;
		}

		no_split_found[1] = 0;
		// Creating a new splitspot array.

		layer[0][split_spot[0]] = new(std::nothrow) uint8_t[(2 * number_of_split_found_in_game + 2) + (split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] + 1)*(2 + PLAYER) + 7 * cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER]]();


		//Insert splitspot coordinate
		uint32_t copy_counter = 0;
		// First data is the number of splits contained in the played game.
		layer[0][split_spot[0]][copy_counter] = number_of_split_found_in_game;
		copy_counter++;
		layer[0][split_spot[0]][copy_counter] = cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER];
		copy_counter++;
		// Inserting the associates Round and played_hands.
		for (uint8_t i = 0; i < number_of_split_found_in_game; i++)
		{
			layer[0][split_spot[0]][copy_counter] = split_spots_for_next_layer[2 * i];
			copy_counter++;
			layer[0][split_spot[0]][copy_counter] = split_spots_for_next_layer[2 * i + 1];
			copy_counter++;
		}

		// Inserting the data associate to Round. Remember to set back hands per player of dealer to 1.
		//-------------------------------------------BiggestRound.
		for (uint8_t i = 0; i <= split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)]; i++)
		{
#if(PLAYER != 1)
			for (uint16_t i1 = 0; i1 < PLAYER; i1++)
			{
				layer[0][split_spot[0]][copy_counter] = cumulated_hands_per_turn[i*(PLAYER + 1) + i1];
				copy_counter++;
			}
			layer[0][split_spot[0]][copy_counter] = cumulated_hands_per_turn[i*(PLAYER + 1) + PLAYER];
			copy_counter++;
#else
			layer[0][split_spot[0]][copy_counter] = cumulated_hands_per_turn[i << 1];
			copy_counter++;
			layer[0][split_spot[0]][copy_counter] = cumulated_hands_per_turn[(i << 1) + 1];
			copy_counter++;
#endif
			layer[0][split_spot[0]][copy_counter] = loosing_per_round[i];
			copy_counter++;
		}

		// Inserting the data assciated to played_hands
		//--------------------------------------------Biggestplayed_hands.
		for (uint8_t i = 0; i < cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER]; i++)
		{
			layer[0][split_spot[0]][copy_counter] = turn_per_hands[i];
			copy_counter++;

			layer[0][split_spot[0]][copy_counter] = card_end_of_decision[i];
			copy_counter++;

			layer[0][split_spot[0]][copy_counter] = distributed_cards[i << 1];
			copy_counter++;
			layer[0][split_spot[0]][copy_counter] = distributed_cards[(i << 1) + 1];
			copy_counter++;
			layer[0][split_spot[0]][copy_counter] = ace_counter[i];
			copy_counter++;
			layer[0][split_spot[0]][copy_counter] = hand_value[i];
			copy_counter++;

			layer[0][split_spot[0]][copy_counter] = hit_array[i];
			copy_counter++;
		}
		split_spot[0]++;
		number_of_split_found_in_game = 0;
	}
	else
	{
		no_split_found[1] = 1;// Should be in function of layer.
	}
	no_split_found[0] &= no_split_found[1];// if still equal to 1 after we tested a whole layer, we tested all the splits possibillitys.									   //delete[] temporary_data_holder;
}

inline void blackjack::find_split_n()
{
	if (number_of_split_found_in_game)
	{
		if (split_spot[layer_being_tested + 1] == split_spot_buffer[layer_being_tested + 1])// If another SplitSpot cant fit, resize.
		{

			// There will be 12 more array in the resized version of Layer0.
			split_spot_buffer[layer_being_tested + 1] += SPLIT_INIT;

			uint8_t** c;
			c = new(std::nothrow) uint8_t*[split_spot_buffer[layer_being_tested + 1]];

			for (uint32_t i = 0; i < split_spot[layer_being_tested + 1]; i++)
			{
				c[i] = layer[layer_being_tested + 1][i];
			}

			delete[] layer[layer_being_tested + 1];

			layer[layer_being_tested + 1] = c;
		}

		no_split_found[2 * (layer_being_tested + 1) + 1] = 0;

		// Creating a new splitspot array.

		if (modified_round[layer_being_tested])
		{
			//-------------------------------------------------------------------------------------round-hands----biggest hands------------------round---------------------------------round of precedent layer--------------------------------------highest hand found-----------------------------------------------------------------------------------quantity of hands in the precedent layer--------------------------------------------cards form the highestfound hand----------------------------------------------------------------------------------------------cards from the precedent modified round-----------------------------
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]] = new(std::nothrow) uint8_t[(2 * number_of_split_found_in_game + 2) + ((split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] + 1 - modified_round[layer_being_tested])*(2 + PLAYER) + 7 * (cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER] - cumulated_hands_per_turn[(modified_round[layer_being_tested] - 1) * (PLAYER + 1) + PLAYER]))];
		}
		else
		{
			//-------------------------------------------------------------------------------------round-hands----biggest hands------------------round---------------------------------round of precedent layer--------------------------------------highest hand found-----------------------------------------------------------------------------------quantity of hands in the precedent layer--------------------------------------------cards form the highestfound hand----------------------------------------------------------------------------------------------cards from the precedent modified round-----------------------------
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]] = new(std::nothrow) uint8_t[(2 * number_of_split_found_in_game + 2) + ((split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] + 1 - modified_round[layer_being_tested])*(2 + PLAYER) + 7 * (cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER]))];
		}


		//Insert splitspot coordinate
		uint32_t copy_counter = 0;
		// First data is the number of splits contained in the played game.
		layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = number_of_split_found_in_game;
		copy_counter++;
		layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER];
		copy_counter++;

		// Inserting the associates Round and played_hands.
		for (uint8_t i = 0; i < number_of_split_found_in_game; i++)
		{
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = split_spots_for_next_layer[2 * i];
			copy_counter++;
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = split_spots_for_next_layer[2 * i + 1];
			copy_counter++;
		}

		// Inserting the data associate to Round. Remember to set back hands per player of dealer to 1.
		for (uint8_t i = modified_round[layer_being_tested]; i <= split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)]; i++)
		{
#if(PLAYER != 1)
			for (uint16_t i1 = 0; i1 < PLAYER; i1++)
			{
				layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = cumulated_hands_per_turn[i*(PLAYER + 1) + i1];
				copy_counter++;
			}
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = cumulated_hands_per_turn[i*(PLAYER + 1) + PLAYER];
			copy_counter++;
#else
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = cumulated_hands_per_turn[i << 1];
			copy_counter++;
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = cumulated_hands_per_turn[(i << 1) + 1];
			copy_counter++;
#endif
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = loosing_per_round[i];
			copy_counter++;
		}

		// Inserting the data assciated to played_hands
		//--------------------------------------------Biggestplayed_hands.
		uint8_t start = 0;
		if (modified_round[layer_being_tested])
		{
			start = cumulated_hands_per_turn[(modified_round[layer_being_tested] - 1) * (PLAYER + 1) + PLAYER];
		}
		else
		{
			start = 0;
		}

		for (uint8_t i = start; i < cumulated_hands_per_turn[split_spots_for_next_layer[2 * (number_of_split_found_in_game - 1)] * (PLAYER + 1) + PLAYER]; i++)
		{
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = turn_per_hands[i];
			copy_counter++;

			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = card_end_of_decision[i];
			copy_counter++;

			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = distributed_cards[i << 1];
			copy_counter++;
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = distributed_cards[(i << 1) + 1];
			copy_counter++;
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = ace_counter[i];
			copy_counter++;
			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = hand_value[i];
			copy_counter++;

			layer[layer_being_tested + 1][split_spot[layer_being_tested + 1]][copy_counter] = hit_array[i];
			copy_counter++;
		}
		split_spot[layer_being_tested + 1]++;
		number_of_split_found_in_game = 0;
	}
	else
	{
		no_split_found[2 * (layer_being_tested + 1) + 1] = 1;// Should be in function of layer.
	}
	no_split_found[2 * (layer_being_tested + 1)] &= no_split_found[2 * (layer_being_tested + 1) + 1];// if still equal to 1 after we tested a whole layer, we tested all the splits possibillitys.

																									 //delete[] temporary_data_holder;
}


inline void blackjack::where_to_raise_hit_array()
{
	breaker = 0;

	round_copy = round - 1;
	played_hands_copy = played_hands - 2;// Converting quantity into coordinate.

	if (card_end_of_decision[played_hands_copy] == 52 * DECK)
	{
		do
		{
			played_hands_copy--;
		} while (card_end_of_decision[played_hands_copy] == 52 * DECK);
		if (turn_per_hands[played_hands_copy] == PLAYER)// a reviser
		{
			played_hands_copy--;
			round_copy--;
		}
	}


	if (played_hands_copy >= hit_limit_form_last_split)
	{
		do
		{
			if (hand_value[played_hands_copy] >= 21)
			{
				played_hands_copy--;
				if (turn_per_hands[played_hands_copy] == PLAYER)
				{
					played_hands_copy--;
					round_copy--;
				}
			}
			else
			{
				round = round_copy;
				find_played_hands();
				card_held = card_end_of_decision[played_hands_copy];
				turn = turn_per_hands[played_hands_copy];
				last_modified = round_copy;
				return;
			}
		} while (played_hands_copy >= hit_limit_form_last_split);
		all_hit_stand_have_been_tested = 1;
	}
	else
	{
		all_hit_stand_have_been_tested = 1;
	}
	last_modified = round_copy;
}


inline void blackjack::single_hit_no_split()
{
	uint8_t hands_counter_decision = played_hands_copy - played_hands;

	turn_copy = turn + 1;
	uint8_t temporary_u_c = played_hands_copy + 1;
#if(PLAYER != 1)
	while (turn_copy <= PLAYER)
	{
#endif
		reconstituting_one_hand(temporary_u_c);

		temporary_u_c++;
		turn_copy++;
#if(PLAYER != 1)
	}
#endif

	hand_value[played_hands + hands_counter_decision] += card[card_held];

	not_busted_if_soft_hand(hands_counter_decision);

	card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.

	hit_array[played_hands + hands_counter_decision]++;

	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
	hands_counter_decision++;
	if (card_held >= 52 * DECK)
	{
#if(PLAYER != 1)
		while (hands_counter_decision < PLAYER)
		{
			hit_array[played_hands + hands_counter_decision] = 0;
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			hands_counter_decision++;
		}
#endif
		hit_array[played_hands + hands_counter_decision] = 0;
		card_end_of_decision[played_hands + hands_counter_decision] = card_held;

		breaker = 1;
		return;
	}
#if(PLAYER != 1)
	while (hands_counter_decision < PLAYER)
	{
		hit_array[played_hands + hands_counter_decision] = 0;

		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);
			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;
			if (card_held >= 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;

				while (hands_counter_decision < PLAYER)
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
				}

				hit_array[played_hands + hands_counter_decision] = 0;
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;
		hands_counter_decision++;
	}
#endif
	//dealers turn

	hit_array[played_hands + hands_counter_decision] = 0;

	while (hand_value[played_hands + hands_counter_decision] < 17)	// Dealers turn
	{
		hand_value[played_hands + hands_counter_decision] += card[card_held];

		not_busted_if_soft_hand(hands_counter_decision);
		card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
		hit_array[played_hands + hands_counter_decision]++;
		if (card_held >= 52 * DECK)
		{
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			breaker = 1;
			return;
		}
	}

	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
}

inline void blackjack::single_hit_split()
{
	uint8_t hands_counter_decision = played_hands_copy - played_hands;
	turn_copy = turn_per_hands[played_hands_copy + 1];

	played_hands_copy++;
	while (played_hands_copy < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER])
	{
		reconstituting_one_hand((uint8_t&)played_hands_copy);
		played_hands_copy++;
	}

	hand_value[played_hands + hands_counter_decision] += card[card_held];

	not_busted_if_soft_hand(hands_counter_decision);

	card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.

	hit_array[played_hands + hands_counter_decision]++;

	card_end_of_decision[played_hands + hands_counter_decision] = card_held;

	hands_counter_decision++;

	if (card_held == 52 * DECK)
	{
		while (played_hands + hands_counter_decision < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER])
		{
			hit_array[played_hands + hands_counter_decision] = 0;
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			hands_counter_decision++;
		}
		breaker = 1;
		return;
	}

	while (played_hands + hands_counter_decision < (cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER] - 1))
	{
		hit_array[played_hands + hands_counter_decision] = 0;
		while (hand_value[played_hands + hands_counter_decision] < 12)
		{
			hand_value[played_hands + hands_counter_decision] += card[card_held];

			not_busted_if_soft_hand(hands_counter_decision);
			card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
			hit_array[played_hands + hands_counter_decision]++;
			if (card_held == 52 * DECK)
			{
				card_end_of_decision[played_hands + hands_counter_decision] = card_held;
				hands_counter_decision++;
				while (played_hands + hands_counter_decision < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER])
				{
					hit_array[played_hands + hands_counter_decision] = 0;
					card_end_of_decision[played_hands + hands_counter_decision] = card_held;
					hands_counter_decision++;
				}
				breaker = 1;
				return;
			}
		}

		card_end_of_decision[played_hands + hands_counter_decision] = card_held;

		hands_counter_decision++;
	}

	hit_array[played_hands + hands_counter_decision] = 0;

	while (hand_value[played_hands + hands_counter_decision] < 17)	// Dealers turn
	{
		hand_value[played_hands + hands_counter_decision] += card[card_held];

		not_busted_if_soft_hand(hands_counter_decision);

		card_held++;// We picked up a card, now the card_held variable hold the coordinate of the next card.
		hit_array[played_hands + hands_counter_decision]++;
		if (card_held >= 52 * DECK)
		{
			card_end_of_decision[played_hands + hands_counter_decision] = card_held;
			breaker = 1;
			return;
		}
	}

	card_end_of_decision[played_hands + hands_counter_decision] = card_held;
}


inline uint8_t blackjack::organizing_split_0()
{
	find_played_hands();

	turn = turn_per_hands[played_hands_copy];
	hands_per_player[round*PLAYER + turn]++;
	modified_round[layer_being_tested] = round;

	all_hit_stand_have_been_tested = 0;

	if (round*PLAYER + turn)
	{
		temporary_hands_counter = cumulated_hands_per_turn[round*(PLAYER + 1) + turn - 1];
	}
	else
	{
		temporary_hands_counter = 0;
	}

	uint8_t hands_counter_decision = temporary_hands_counter - played_hands;

	card_held = card_end_of_decision[temporary_hands_counter] - hit_array[temporary_hands_counter];

	hit_limit_form_last_split = temporary_hands_counter;

	modified_turn[layer_being_tested] = turn;
	temporary_hands_counter = cumulated_hands_per_turn[round*(PLAYER + 1) + turn];
	turn_copy = turn_per_hands[temporary_hands_counter];

	// Creating back the distributed hands from the turns found after the turn of the currently splitted hand.
#if(PLAYER != 1)
	while (temporary_hands_counter < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER])
	{
#endif
		hand_value[temporary_hands_counter] = distributed_cards[temporary_hands_counter << 1];

		if (distributed_cards[temporary_hands_counter << 1] == 11)
		{
			ace_counter[temporary_hands_counter] = 1;
		}
		else
		{
			ace_counter[temporary_hands_counter] = 0;
		}

		hand_value[temporary_hands_counter] += distributed_cards[(temporary_hands_counter << 1) + 1];

		if (distributed_cards[(temporary_hands_counter << 1) + 1] == 11)
		{
			if (hand_value[temporary_hands_counter] == 22)
			{
				hand_value[temporary_hands_counter] = 12;
			}
			else
			{
				ace_counter[temporary_hands_counter]++;
			}
		}
		temporary_hands_counter++;
#if(PLAYER != 1)
	}
#endif
	// Done recreating the splitted hand.

#if(PLAYER != 1)
	while (temporary_hands_counter > cumulated_hands_per_turn[round*(PLAYER + 1) + turn])
	{
#endif
		hand_value[temporary_hands_counter] = hand_value[temporary_hands_counter - 1];
		ace_counter[temporary_hands_counter] = ace_counter[temporary_hands_counter - 1];

		distributed_cards[temporary_hands_counter << 1] = distributed_cards[(temporary_hands_counter - 1) << 1];
		distributed_cards[(temporary_hands_counter << 1) + 1] = distributed_cards[((temporary_hands_counter - 1) << 1) + 1];

		turn_per_hands[temporary_hands_counter] = turn_per_hands[temporary_hands_counter - 1];

		temporary_hands_counter--;
#if(PLAYER != 1)
	}
#endif
	// Done rearanging the hands after turn of the splitted hand.

	// TemporaryHandscounter now point to the last hand of the turn of the player with the hand wich is going to be splitted.
	temporary_hands_counter--;
	// This is to show to the programmer in an easy way where the splitted hands are

	modified_hand[layer_being_tested] = temporary_hands_counter;



	if (hands_per_player[round*PLAYER + turn] > 2)
	{
		//uint8_t temporary_u_c;
		// transfer the value of the hands and the ditributed card and the ace info so that the 2 last hands of the turn are the most rencently splitted ones.
		if (played_hands_copy != temporary_hands_counter)
		{
			uint8_t temporary_u_c = hand_value[temporary_hands_counter];// temporaryhandscounter point to the last available position in the turn of the newly splitted hand.
			hand_value[temporary_hands_counter] = hand_value[played_hands_copy];// played_handscopy point to the origin of the newly splitted hand, witch could differ by many position from the extra splitted hand.
			hand_value[played_hands_copy] = temporary_u_c;

			temporary_u_c = ace_counter[temporary_hands_counter];
			ace_counter[temporary_hands_counter] = ace_counter[played_hands_copy];
			ace_counter[played_hands_copy] = temporary_u_c;

			temporary_u_c = distributed_cards[temporary_hands_counter << 1];
			distributed_cards[temporary_hands_counter << 1] = distributed_cards[played_hands_copy << 1];
			distributed_cards[played_hands_copy << 1] = temporary_u_c;

			temporary_u_c = distributed_cards[(temporary_hands_counter << 1) + 1];
			distributed_cards[(temporary_hands_counter << 1) + 1] = distributed_cards[(played_hands_copy << 1) + 1];
			distributed_cards[(played_hands_copy << 1) + 1] = temporary_u_c;
		}
		//temporary_u_c = 0;
		for (uint16_t i = temporary_hands_counter + 2 - hands_per_player[round*PLAYER + turn]; i < temporary_hands_counter; i++)
		{
			if (distributed_cards[i << 1] == 11)
			{
				ace_counter[i] = 1;
				hand_value[i] = 11;
			}
			else
			{
				ace_counter[i] = 0;
				hand_value[i] = distributed_cards[i << 1];
			}


			if (distributed_cards[(i << 1) + 1] == 11)
			{
				if (hand_value[i] > 10)
				{
					hand_value[i] += 1;
				}
				else
				{
					ace_counter[i]++;
					hand_value[i] += 11;
				}
			}
			else
			{
				hand_value[i] += distributed_cards[(i << 1) + 1];
			}
			//	temporary_u_c++;
		}
		splited_hands[temporary_hands_counter + 1] = 1;
	}
	else
	{
		splited_hands[temporary_hands_counter] = 1;
		splited_hands[temporary_hands_counter + 1] = 1;
	}

	return hands_counter_decision;
}

inline uint8_t blackjack::organizing_split_n()
{
	find_played_hands();

	turn = turn_per_hands[played_hands_copy];
	hands_per_player[round*PLAYER + turn]++;

	modified_round[layer_being_tested] = round;

	all_hit_stand_have_been_tested = 0;

	if (round*PLAYER + turn)
	{
		temporary_hands_counter = cumulated_hands_per_turn[round*(PLAYER + 1) + turn - 1];
	}
	else
	{
		temporary_hands_counter = 0;
	}

	uint8_t hands_counter_decision = temporary_hands_counter - played_hands;

	card_held = card_end_of_decision[temporary_hands_counter] - hit_array[temporary_hands_counter];

	hit_limit_form_last_split = temporary_hands_counter;

	modified_turn[layer_being_tested] = turn;
	temporary_hands_counter = cumulated_hands_per_turn[round*(PLAYER + 1) + turn];
	turn_copy = turn_per_hands[temporary_hands_counter];

	// Creating back the distributed hands from the turns found after the turn of the currently splitted hand.
#if(PLAYER != 1)
	while (temporary_hands_counter < cumulated_hands_per_turn[round*(PLAYER + 1) + PLAYER])
	{
#endif
		hand_value[temporary_hands_counter] = distributed_cards[temporary_hands_counter << 1];

		if (distributed_cards[temporary_hands_counter << 1] == 11)
		{
			ace_counter[temporary_hands_counter] = 1;
		}
		else
		{
			ace_counter[temporary_hands_counter] = 0;
		}

		hand_value[temporary_hands_counter] += distributed_cards[(temporary_hands_counter << 1) + 1];

		if (distributed_cards[(temporary_hands_counter << 1) + 1] == 11)
		{
			if (hand_value[temporary_hands_counter] == 22)
			{
				hand_value[temporary_hands_counter] = 12;
			}
			else
			{
				ace_counter[temporary_hands_counter]++;
			}
		}
		temporary_hands_counter++;
#if(PLAYER != 1)
	}
#endif
	// Done recreating the splitted hand.

#if(PLAYER != 1)
	while (temporary_hands_counter > cumulated_hands_per_turn[round*(PLAYER + 1) + turn])
	{
#endif
		hand_value[temporary_hands_counter] = hand_value[temporary_hands_counter - 1];
		ace_counter[temporary_hands_counter] = ace_counter[temporary_hands_counter - 1];

		distributed_cards[temporary_hands_counter << 1] = distributed_cards[(temporary_hands_counter - 1) << 1];
		distributed_cards[(temporary_hands_counter << 1) + 1] = distributed_cards[((temporary_hands_counter - 1) << 1) + 1];

		turn_per_hands[temporary_hands_counter] = turn_per_hands[temporary_hands_counter - 1];

		temporary_hands_counter--;
#if(PLAYER != 1)
	}
#endif
	// Done rearanging the hands after turn of the splitted hand.

	// TemporaryHandscounter now point to the last hand of the turn of the player with the hand wich is going to be splitted.
	temporary_hands_counter--;
	// This is to show to the programmer in an easy way where the splitted hands are

	modified_hand[layer_being_tested] = temporary_hands_counter;



	if (hands_per_player[round*PLAYER + turn] > 2)
	{
		//uint8_t temporary_u_c;
		// transfer the value of the hands and the ditributed card and the ace info so that the 2 last hands of the turn are the most rencently splitted ones.
		if (played_hands_copy != temporary_hands_counter)
		{
			uint8_t temporary_u_c = hand_value[temporary_hands_counter];// temporaryhandscounter point to the last available position in the turn of the newly splitted hand.
			hand_value[temporary_hands_counter] = hand_value[played_hands_copy];// played_handscopy point to the origin of the newly splitted hand, witch could differ by many position from the extra splitted hand.
			hand_value[played_hands_copy] = temporary_u_c;

			temporary_u_c = ace_counter[temporary_hands_counter];
			ace_counter[temporary_hands_counter] = ace_counter[played_hands_copy];
			ace_counter[played_hands_copy] = temporary_u_c;

			temporary_u_c = distributed_cards[temporary_hands_counter << 1];
			distributed_cards[temporary_hands_counter << 1] = distributed_cards[played_hands_copy << 1];
			distributed_cards[played_hands_copy << 1] = temporary_u_c;

			temporary_u_c = distributed_cards[(temporary_hands_counter << 1) + 1];
			distributed_cards[(temporary_hands_counter << 1) + 1] = distributed_cards[(played_hands_copy << 1) + 1];
			distributed_cards[(played_hands_copy << 1) + 1] = temporary_u_c;
		}
		//temporary_u_c = 0;
		for (uint16_t i = temporary_hands_counter + 2 - hands_per_player[round*PLAYER + turn]; i < temporary_hands_counter; i++)
		{
			if (distributed_cards[i << 1] == 11)
			{
				ace_counter[i] = 1;
				hand_value[i] = 11;
			}
			else
			{
				ace_counter[i] = 0;
				hand_value[i] = distributed_cards[i << 1];
			}


			if (distributed_cards[(i << 1) + 1] == 11)
			{
				if (hand_value[i] > 10)
				{
					hand_value[i] += 1;
				}
				else
				{
					ace_counter[i]++;
					hand_value[i] += 11;
				}
			}
			else
			{
				hand_value[i] += distributed_cards[(i << 1) + 1];
			}
			//	temporary_u_c++;
		}
		splited_hands[temporary_hands_counter + 1] = 1;
	}
	else
	{
		splited_hands[temporary_hands_counter] = 1;
		splited_hands[temporary_hands_counter + 1] = 1;
	}

	return hands_counter_decision;
}

inline void blackjack::test_a_game()
{
	/*
	insert_hit_array_and_turns();
	if (split_spot[0] == 0)
	{
	if (TT_counter[played_hands - 1] > 64)
	{
	std::thread t[31];
	uint64_t temp = TT_counter[played_hands - 1] / 32;
	for (uint8_t i = 0; i < 31; i++)
	{
	t[i] = tcheck_for_dulicate_thread(i * temp, (i + 1) * temp);
	}

	tcheck_for_dulicate(31 * temp, TT_counter[played_hands - 1] - 1);
	for (uint8_t i = 0; i < 31; i++)
	{
	t[i].join();
	}
	}
	else
	{
	tcheck_for_dulicate(0, TT_counter[played_hands - 1] - 1);
	}
	}
	*/
	uint8_t t_played_hands = 0, turn_c = 0, t_hands_counter = 0, t_hands_within = 0, t_round = 0;
	uint8_t kc[52 * DECK] = {}, t_hand_value[26 * DECK] = {}, t_hit_array[26 * DECK] = {}, t_ace[26 * DECK] = {}, t_winning[26 * DECK] = {}, t_loosing[26 * DECK] = {};
	uint8_t t_distrib[52 * DECK] = {}, t_hit_stand[26 * DECK] = {}, t_before_split[26 * DECK] = {}, t_for_hit[26 * DECK] = {};
	uint8_t t_turn_per_hands[26 * DECK] = {}, t_b_pppr[26 * DECK] = {};
	uint8_t kc_counter = 0, card_counter = 0;
	uint8_t t_hands_per_round[26 * DECK] = {}, t_splitted_hands[26 * DECK] = {}, t_cumulated[26 * DECK] = {};

	while (t_round < round)
	{
		// distribution
		turn_c = 0;
		while (turn_c <= PLAYER)
		{
			t_hand_value[t_played_hands + turn_c] = card[card_counter];
			t_ace[t_played_hands + turn_c] = (card[card_counter] == 11);
			kc[kc_counter] = card[card_counter];

			t_distrib[2 * (t_played_hands + turn_c)] = card[card_counter];

			kc_counter++;
			card_counter++;
			if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
			{
				uint32_t a1 = 0;
			}
			if (card[card_counter] == 11)
			{
				if (t_hand_value[t_played_hands + turn_c] == 11)
				{
					t_hand_value[t_played_hands + turn_c] += 1;
				}
				else
				{
					t_hand_value[t_played_hands + turn_c] += 11;
					t_ace[t_played_hands + turn_c] = 1;

				}
			}
			else
			{
				t_hand_value[t_played_hands + turn_c] += card[card_counter];
			}

			if (turn_c != PLAYER)
			{
				kc[kc_counter] = card[card_counter];
				kc_counter++;
			}

			t_distrib[2 * (t_played_hands + turn_c) + 1] = card[card_counter];
			card_counter++;
			if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
			{
				uint32_t a1 = 0;
			}

			turn_c++;
		}
		turn_c = 0;
		t_hands_counter = 0;

		//decision
		while (turn_c < PLAYER)
		{
			if (hands_per_player[t_round*PLAYER + turn_c] == 1)
			{
				if (t_distrib[2 * (t_played_hands + t_hands_counter)] != distributed_cards[2 * (t_played_hands + t_hands_counter)] || t_distrib[2 * (t_played_hands + t_hands_counter) + 1] != distributed_cards[2 * (t_played_hands + t_hands_counter) + 1])
				{
					uint32_t a1 = 0;
				}

				for (uint8_t hit_counter = 0; hit_counter < hit_array[t_played_hands + t_hands_counter]; hit_counter++)
				{

					t_hand_value[t_played_hands + t_hands_counter] += card[card_counter];
					t_ace[t_played_hands + t_hands_counter] += (card[card_counter] == 11);
					if (t_hand_value[t_played_hands + t_hands_counter] > 21)
					{
						if (t_ace[t_played_hands + t_hands_counter] != 0)
						{
							t_hand_value[t_played_hands + t_hands_counter] -= 10;
							t_ace[t_played_hands + t_hands_counter]--;
						}
					}

					kc[kc_counter] = card[card_counter];

					kc_counter++;
					card_counter++;
					if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
					{
						uint32_t a1 = 0;
					}
				}

				t_hit_stand[t_played_hands + t_hands_counter] = card_counter;
				t_for_hit[t_played_hands + t_hands_counter] = card_counter;

				if (t_for_hit[t_played_hands + t_hands_counter] != card_end_of_decision[t_played_hands + t_hands_counter])
				{
					uint32_t a1 = 0;
				}
				if (t_hand_value[t_played_hands + t_hands_counter] != hand_value[t_played_hands + t_hands_counter] || t_ace[t_played_hands + t_hands_counter] != ace_counter[t_played_hands + t_hands_counter])
				{
					uint32_t a1 = 0;
				}

				t_turn_per_hands[t_played_hands + t_hands_counter] = turn_c;

				if (t_turn_per_hands[t_played_hands + t_hands_counter] != turn_per_hands[t_played_hands + t_hands_counter])
				{
					uint32_t a1 = 0;
				}
				t_hands_counter++;
			}
			else
			{
				for (uint8_t move = cumulated_hands_per_turn[t_round*(PLAYER + 1) + PLAYER] - 1; move > (t_played_hands + t_hands_counter); move--)// moving all of the superior hands to make room for the hands to be splitted
				{
					t_hand_value[move + hands_per_player[t_round*PLAYER + turn_c] - 1] = t_hand_value[move];
					t_ace[move + hands_per_player[t_round*PLAYER + turn_c] - 1] = t_ace[move];

					t_distrib[2 * (move + hands_per_player[t_round*PLAYER + turn_c] - 1)] = t_distrib[2 * move];
					t_distrib[2 * (move + hands_per_player[t_round*PLAYER + turn_c] - 1) + 1] = t_distrib[2 * move + 1];
				}

				t_hands_within = 1;

				while (t_hands_within < hands_per_player[t_round*PLAYER + turn_c])// splitting the ahnds
				{
					t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];
					t_hand_value[t_played_hands + t_hands_within + t_hands_counter] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];
					t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];



					t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)] == 11);
					t_ace[t_played_hands + t_hands_within + t_hands_counter] = (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)] == 11);




					t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] += card[card_counter];

					if (t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] == 22)
					{
						t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] -= 10;
					}
					else if (card[card_counter] == 11)
					{
						t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = 1;
					}

					kc[kc_counter] = card[card_counter];
					t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] = card[card_counter];
					card_counter++;
					kc_counter++;
					if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
					{
						uint32_t a1 = 0;
					}

					t_hand_value[t_played_hands + t_hands_within + t_hands_counter] += card[card_counter];
					if (t_hand_value[t_played_hands + t_hands_within + t_hands_counter] == 22)
					{
						t_hand_value[t_played_hands + t_hands_within + t_hands_counter] -= 10;
					}
					else if (card[card_counter] == 11)
					{
						t_ace[t_played_hands + t_hands_within + t_hands_counter] = 1;
					}

					kc[kc_counter] = card[card_counter];
					t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] = card[card_counter];
					card_counter++;
					kc_counter++;
					if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
					{
						uint32_t a1 = 0;
					}

					if (hands_per_player[t_round*PLAYER + turn_c] > (t_hands_within + 1))
					{
						if (t_hands_counter < (hands_per_player[t_round*PLAYER + turn_c] - 1))
						{
							if (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] == t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)])
							{
								uint8_t temp;
								temp = t_ace[t_played_hands + t_hands_within + t_hands_counter - 1];
								t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = t_ace[t_played_hands + t_hands_within + t_hands_counter];
								t_ace[t_played_hands + t_hands_within + t_hands_counter] = temp;

								temp = t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1];
								t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] = t_hand_value[t_played_hands + t_hands_within + t_hands_counter];
								t_hand_value[t_played_hands + t_hands_within + t_hands_counter] = temp;

								temp = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1];
								t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1];
								t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] = temp;
							}
						}
					}

					t_hands_within++;
				}
				t_hands_within = 0;

				while (t_hands_within < hands_per_player[t_round*PLAYER + turn_c])// playing the hands
				{
					if (t_distrib[2 * (t_played_hands + t_hands_counter)] != distributed_cards[2 * (t_played_hands + t_hands_counter)] || t_distrib[2 * (t_played_hands + t_hands_counter) + 1] != distributed_cards[2 * (t_played_hands + t_hands_counter) + 1])
					{
						uint32_t a1 = 0;
					}
					for (uint8_t hit_counter = 0; hit_counter < hit_array[t_played_hands + t_hands_counter]; hit_counter++)
					{

						t_hand_value[t_played_hands + t_hands_counter] += card[card_counter];
						t_ace[t_played_hands + t_hands_counter] += (card[card_counter] == 11);

						if (t_hand_value[t_played_hands + t_hands_counter] > 21)
						{
							if (t_ace[t_played_hands + t_hands_counter] != 0)
							{
								t_hand_value[t_played_hands + t_hands_counter] -= 10;
								t_ace[t_played_hands + t_hands_counter]--;
							}
						}
						kc[kc_counter] = card[card_counter];


						kc_counter++;
						card_counter++;
						if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
						{
							uint32_t a1 = 0;
						}
					}
					t_hit_stand[t_played_hands + t_hands_counter] = card_counter;
					t_for_hit[t_played_hands + t_hands_counter] = card_counter;
					if (t_for_hit[t_played_hands + t_hands_counter] != card_end_of_decision[t_played_hands + t_hands_counter])
					{
						uint32_t a1 = 0;
					}
					if (t_hand_value[t_played_hands + t_hands_counter] != hand_value[t_played_hands + t_hands_counter] || t_ace[t_played_hands + t_hands_counter] != ace_counter[t_played_hands + t_hands_counter])
					{
						uint32_t a1 = 0;
					}
					t_splitted_hands[t_played_hands + t_hands_counter] = 1;
					if (t_splitted_hands[t_played_hands + t_hands_counter] != splited_hands[t_played_hands + t_hands_counter])
					{
						uint32_t a1 = 0;
					}
					t_turn_per_hands[t_played_hands + t_hands_counter] = turn_c;
					if (t_turn_per_hands[t_played_hands + t_hands_counter] != turn_per_hands[t_played_hands + t_hands_counter])
					{
						uint32_t a1 = 0;
					}
					t_hands_within++;
					t_hands_counter++;
				}
			}

			t_hit_stand[t_played_hands + t_hands_within] = card_counter;
			t_for_hit[t_played_hands + t_hands_within] = card_counter;

			turn_c++;
		}

		//dealers trun
		kc[kc_counter] = t_distrib[2 * (t_played_hands + t_hands_counter) + 1];
		kc_counter++;
		if (kc_counter > 52 * DECK)
		{
			uint32_t a1 = 0;
		}

		for (uint8_t hit_counter = 0; hit_counter < hit_array[t_played_hands + t_hands_counter]; hit_counter++)
		{
			t_hand_value[t_played_hands + t_hands_counter] += card[card_counter];
			t_ace[t_played_hands + t_hands_counter] += (card[card_counter] == 11);
			if (t_hand_value[t_played_hands + t_hands_counter] > 21)
			{
				if (t_ace[t_played_hands + t_hands_counter] != 0)
				{
					t_hand_value[t_played_hands + t_hands_counter] -= 10;
					t_ace[t_played_hands + t_hands_counter]--;
				}
			}

			kc[kc_counter] = card[card_counter];

			kc_counter++;
			card_counter++;
			if (card_counter > 52 * DECK || kc_counter > 52 * DECK)
			{
				uint32_t a1 = 0;
			}
		}
		t_turn_per_hands[t_played_hands + t_hands_counter] = turn_c;
		t_hands_counter++;

		//verification
		turn_c = 0;
		t_hands_counter = 0;
		if (t_round)
		{
			t_loosing[t_round] = t_loosing[t_round - 1];
		}
		else
		{
			t_loosing[0] = 0;
		}

		while (turn_c < PLAYER)
		{
			t_hands_within = 0;
			while (t_hands_within < hands_per_player[t_round*PLAYER + turn_c])
			{

				if (t_hand_value[t_played_hands + t_hands_counter] > 21)
				{
					t_loosing[t_round]++;
				}
				else
				{
					if (t_hand_value[t_played_hands + t_hands_counter] < t_hand_value[cumulated_hands_per_turn[t_round*(PLAYER + 1) + PLAYER] - 1] &&
						t_hand_value[cumulated_hands_per_turn[t_round*(PLAYER + 1) + PLAYER] - 1] <= 21)	// create a  dealers  hand coordinate variable...
					{
						t_loosing[t_round]++;
					}
				}

				t_hands_within++;
				t_hands_counter++;
			}

			t_cumulated[t_round*(PLAYER + 1) + turn_c] = t_played_hands + t_hands_counter;
			if (t_cumulated[t_round*(PLAYER + 1) + turn_c] != cumulated_hands_per_turn[t_round*(PLAYER + 1) + turn_c])
			{
				uint32_t a1 = 0;
			}
			turn_c++;
		}
		t_played_hands += t_hands_counter + 1;

		if (t_loosing[t_round] != loosing_per_round[t_round])
		{
			int a = 0;
		}

		t_cumulated[t_round*(PLAYER + 1) + turn_c] = t_played_hands;
		if (t_cumulated[t_round*(PLAYER + 1) + turn_c] != cumulated_hands_per_turn[t_round*(PLAYER + 1) + turn_c])
		{
			uint32_t a1 = 0;
		}
		t_turn_per_hands[t_played_hands - 1] = PLAYER;
		if (turn_per_hands[t_played_hands - 1] != PLAYER)
		{
			uint32_t a1 = 0;
		}
		t_round++;
	}
}


inline void blackjack::shuffling()
{

	//std::cout << "Shuffling \n" << endl;
	uint64_t Reminder, Reference;
	uint8_t CardBeingSwitched;
	uint8_t pseudo[104 * DECK] = {};
	random = 4294967295 / random;

	pseudo[0] = random & 255;
	random += ~random;
	if (random == 0)
	{
		random = (tested_possibillity*game_simulated*game_simulated + tested_possibillity)*(worker_id + 1) + card[0] * card[1] * card[2] * card[3] + card[51] * card[40] * card[30] * card[20] * card[10] * card[9];
	}
	Reminder = 4294967295 % random;
	Reference = pseudo[0] & (pseudo[0] * pseudo[0]);
	pseudo[0] = pseudo[0] % (52 * DECK);
#if(DECK > 2)
	for (uint16_t i = 1; i < 2 * DECK * 52; i++)
	{
		if (i & 1)
		{
			pseudo[i] = ((i | Reminder) & pseudo[i - 1]) & 255;
		}
		else
		{
			Reminder = Reminder << 8;
			pseudo[i] = Reminder / random;
			Reminder = Reminder % random;
		}
		pseudo[i] = pseudo[i] % (52 * DECK);
	}

	for (uint16_t i = 0; i < 104 * DECK; i++)
	{
		if (i < 52 * DECK)
		{
			CardBeingSwitched = card[pseudo[i]];
			card[pseudo[i]] = card[i];
			card[i] = CardBeingSwitched;
		}
		else
		{
			CardBeingSwitched = card[pseudo[i]];
			card[pseudo[i]] = card[i - 52 * DECK];
			card[i - 52 * DECK] = CardBeingSwitched;
		}
	}
#else
	for (uint8_t i = 1; i < 2 * DECK * 52; i++)
	{
		if (i & 1)
		{
			pseudo[i] = ((i | Reminder) & pseudo[i - 1]) & 255;
		}
		else
		{
			Reminder = Reminder << 8;
			pseudo[i] = Reminder / random;
			Reminder = Reminder % random;
		}
		pseudo[i] = pseudo[i] % (52 * DECK);
	}

	for (uint8_t i = 0; i < 104 * DECK; i++)
	{
		if (i < 52 * DECK)
		{
			CardBeingSwitched = card[pseudo[i]];
			card[pseudo[i]] = card[i];
			card[i] = CardBeingSwitched;
		}
		else
		{
			CardBeingSwitched = card[pseudo[i]];
			card[pseudo[i]] = card[i - 52 * DECK];
			card[i - 52 * DECK] = CardBeingSwitched;
		}
	}
#endif

	/*
	card[0] = 10;
	card[1] = 10;
	card[2] = 5;
	card[3] = 4;
	card[4] = 8;
	card[5] = 7;
	card[6] = 5;
	card[7] = 7;
	card[8] = 10;
	card[9] = 10;
	card[10] = 10;
	card[11] = 2;
	card[12] = 5;
	card[13] = 2;
	card[14] = 10;
	card[15] = 10;
	card[16] = 4;
	card[17] = 6;
	card[18] = 3;
	card[19] = 1;
	card[20] = 9;
	card[21] = 10;
	card[22] = 10;
	card[23] = 5;
	card[24] = 2;
	card[25] = 3;
	card[26] = 9;
	card[27] = 6;
	card[28] = 7;
	card[29] = 4;
	card[30] = 4;
	card[31] = 1;
	card[32] = 10;
	card[33] = 10;
	card[34] = 6;
	card[35] = 2;
	card[36] = 10;
	card[37] = 7;
	card[38] = 8;
	card[39] = 6;
	card[40] = 9;
	card[41] = 9;
	card[42] = 1;
	card[43] = 10;
	card[44] = 10;
	card[45] = 10;
	card[46] = 8;
	card[47] = 8;
	card[48] = 10;
	card[49] = 1;
	card[50] = 3;
	card[51] = 3;
	*/
}

inline void blackjack::testing_all_possibilitys()
{
	// First game ... stand only
	while (true)
	{
		first_game_distribution();
		first_game_verification();

		if (breaker)
		{
#if(DEBUG)
			test_a_game();
#endif
			tested_possibillity++;
#if(DEBUG && STOP_AT)
			stop_at();
#endif
			breaker = 0;
			break;
		}
	}

	find_optimization();
	find_split_0();

	no_split_found[0] = 1;
	random += game_simulated * (random & tested_possibillity);

	insert_best_games();

	goto first_simulation;

	//Testing all the hit stand possibillitys, no splits.

	while (true)// Game
	{
		while (breaker)
		{
#if(DEBUG)
			test_a_game();
#endif
			// Another possibillity have been tested.
			tested_possibillity++;
#if(DEBUG && STOP_AT)
			stop_at();
#endif

#if(DISPLAY)
			display_at_each();
#endif
			random += game_simulated * (random & tested_possibillity);
			random += card[0] * card[1] * card[2] * card[3] + (card[round*PLAYER - 1])*round*played_hands;

			find_optimization();
			find_split_0();

			insert_best_games();

		first_simulation:

			where_to_raise_hit_array();


			if (all_hit_stand_have_been_tested)
			{
				if (no_split_found[0])
				{
					return;
				}
				else
				{
					extract_data_mem2_0();
					goto TestingLayer0;
				}
			}
			else
			{
				single_hit_no_split();
				verefication_in_loop();

				if (optimization[3 * card_held] != 255)
				{
					find_split_0();
					breaker = 1;
					tested_possibillity++;
#if(DEBUG && STOP_AT)
					stop_at();
#endif

					insert_best_games_optimisation();
					goto first_simulation;
				}
			}
		}
		distribution_in_loop();
		verefication_in_loop();
	}
	//Testing all of the first layer splits, and all the hit stand between the splitted hand and the last possible played hand.

	if (!layer_being_tested)
	{
		while (true)
		{
			// Organizing splits and playing the round where the split happens.
			if (all_hit_stand_have_been_tested)
			{
				if (all_splits_have_been_tested)
				{
					return;
				}
				else
				{
					split_0();
					if (all_splits_have_been_tested)
					{
						return;
					}
					else
					{
						if (layer_being_tested)
						{
							goto TestingLayerN;
						}
						else
						{
							goto TestingLayer0;
						}
					}
				}

			TestingLayer0:

				distribution_split(organizing_split_0());

				verefication_split();
			}

			// Normal hit/stand in loop.
			if (!breaker)
			{
				distribution_in_loop();
				verefication_in_loop();
			}
			//analyse the winning and tcheck for splits.
			while (breaker)
			{
#if(DEBUG)
				test_a_game();
#endif
				// Another possibillity have been tested.
				tested_possibillity++;
#if(DEBUG && STOP_AT)
				stop_at();
#endif
#if(DISPLAY)
				display_at_each();
#endif

				random += card[0] * card[1] * card[2] * card[3] + (card[round*PLAYER - 1])*round*played_hands;

				random += game_simulated * (random & tested_possibillity);


				//winning_data(round, hands_per_player, winning, played_hands, winning_per_player_per_round, hit_array, turn_per_hands, distributed_cards, hand_value, card_before_split_layer, layer_being_tested);
				find_optimization();
				find_split_n();

				insert_best_games();

			dd:
				where_to_raise_hit_array();
				//Playing the round where the hit have been modified.
				if (!all_hit_stand_have_been_tested)
				{
					if (round == modified_round[layer_being_tested])
					{
						single_hit_split();
						verefication_split();
					}
					else
					{
						single_hit_no_split();
						verefication_in_loop();
					}


					if (optimization[3 * card_held] != 255)
					{
						find_split_n();
						breaker = 1;
						tested_possibillity++;
#if(DEBUG && STOP_AT)
						stop_at();
#endif

						insert_best_games_optimisation();

						goto dd;
					}
				}
			}
		}
	}


	if (layer_being_tested)
	{
		while (true)
		{
			// Organizing splits and playing the round where the split happens.
			if (all_hit_stand_have_been_tested)
			{
				if (all_splits_have_been_tested)
				{
					return;
				}
				else
				{
					split_n();
					if (all_splits_have_been_tested)
					{
						return;
					}
					else
					{
						if (layer_being_tested)
						{
							goto TestingLayerN;
						}
						else
						{
							goto TestingLayer0;
						}
					}
				}

			TestingLayerN:


				distribution_split(organizing_split_n());
				//decision_split();
				verefication_split();
			}

			// Normal hit/stand in loop.
			if (!breaker)
			{
				distribution_in_loop();
				verefication_in_loop();
			}
			// Organizing hits/stand, analyse the winning and tcheck for splits.
			while (breaker)
			{
#if(DEBUG)
				test_a_game();
#endif
				// Another possibillity have been tested.
				tested_possibillity++;
#if(DEBUG && STOP_AT)
				stop_at();
#endif
#if(DISPLAY)
				display_at_each();
#endif

				random += card[0] * card[1] * card[2] * card[3] + (card[round*PLAYER - 1])*round*played_hands;

				random += game_simulated * (random & tested_possibillity);

				find_optimization();
				find_split_n();

				insert_best_games();

			qd:
				where_to_raise_hit_array();
				//Playing the round where the hit have been modified.
				if (!all_hit_stand_have_been_tested)
				{
					if (round == modified_round[layer_being_tested])
					{
						single_hit_split();

						verefication_split();
					}
					else
					{
						single_hit_no_split();
						verefication_in_loop();
					}

					if (optimization[3 * card_held] != 255)
					{
						find_split_n();
						breaker = 1;
						tested_possibillity++;

#if(DEBUG && STOP_AT)
						stop_at();
#endif

						insert_best_games_optimisation();

						goto qd;
					}
				}
			}
		}
	}
}

inline void blackjack::simulation_()
{
	while (game_simulated < max_simulation)
	{
		testing_all_possibilitys();

		uint32_t ha_size = 26 * sizeof(best_hands_per_player[0]), hi_size = 26 * sizeof(best_hit_array[0]);

		for (uint8_t i = 0; i < 26 * DECK; i++)
		{
			hi_size += best_hit_array[i].counter;
			ha_size += best_hands_per_player[i].counter;
		}


		//vereficating if there is enough space left into the mem_blocks
		if (current_card_size[mem_block_assigned] + 52 * DECK + 2 >= card_size)
		{
			memory_zone_is_full[mem_block_assigned] = 1;

			uint16_t mem_zone = 0;
			while (mem_zone < memory_blocks)
			{
				if ((!memory_zone_is_beeing_accesed[mem_zone]) && (!memory_zone_is_full[mem_zone]))
				{
					memory_zone_is_beeing_accesed[mem_zone] = 1;
					mem_block_assigned = mem_zone;
					break;
				}
				mem_zone++;
				if (mem_zone == memory_blocks)
				{
					mem_zone = 0;
				}
			}

			goto save_data_in_mem_block;
		}

		if (current_hand_size[mem_block_assigned] + ha_size >= hand_size)
		{
			memory_zone_is_full[mem_block_assigned] = 1;

			uint16_t mem_zone = 0;
			while (mem_zone < memory_blocks)
			{
				if ((!memory_zone_is_beeing_accesed[mem_zone]) && (!memory_zone_is_full[mem_zone]))
				{
					memory_zone_is_beeing_accesed[mem_zone] = 1;
					mem_block_assigned = mem_zone;
					break;
				}
				mem_zone++;
				if (mem_zone == memory_blocks)
				{
					mem_zone = 0;
				}
			}

			goto save_data_in_mem_block;
		}
		if (current_hit_size[mem_block_assigned] + hi_size >= hit_size)
		{
			memory_zone_is_full[mem_block_assigned] = 1;

			uint16_t mem_zone = 0;
			while (mem_zone < memory_blocks)
			{
				if ((!memory_zone_is_beeing_accesed[mem_zone]) && (!memory_zone_is_full[mem_zone]))
				{
					memory_zone_is_beeing_accesed[mem_zone] = 1;
					mem_block_assigned = mem_zone;
					break;
				}
				mem_zone++;
				if (mem_zone == memory_blocks)
				{
					mem_zone = 0;
				}
			}

			goto save_data_in_mem_block;
		}

	save_data_in_mem_block:

		uint16_t games_saved = produced_hit[mem_block_assigned].counter;

		for (uint8_t i = 0; i < 52 * DECK; i++)
		{
			produced_card[produced_card_counter][i] = card[i];
			produced_card_counter++;
		}
		for (uint8_t i = 0; i < 26 * DECK; i++)
		{
			if (best_hit_array[i].counter)
			{
				produced_hit[mem_block_assigned].container[produced_hit[mem_block_assigned].counter].counter = best_hit_array[i].counter;

				best_hit_array[i].convert_to_array(produced_hit[mem_block_assigned].container[produced_hit[mem_block_assigned].counter].container, (size_t&)produced_hit[mem_block_assigned].container[produced_hit[mem_block_assigned].counter].counter);
			}
			if (best_hands_per_player[i].counter)
			{
				produced_hands[mem_block_assigned].container[produced_hands[mem_block_assigned].counter].counter = best_hands_per_player[i].counter;

				best_hands_per_player[i].convert_to_array(produced_hands[mem_block_assigned].container[produced_hands[mem_block_assigned].counter].container, (size_t&)produced_hands[mem_block_assigned].container[produced_hands[mem_block_assigned].counter].counter);
			}
			produced_hit[mem_block_assigned].counter++;
			produced_hands[mem_block_assigned].counter++;
		}

		uint16_t *temp = (uint16_t*)(produced_card[mem_block_assigned] + produced_card_counter);
		temp[0] = produced_hit[mem_block_assigned].counter - games_saved;
		produced_card_counter += 2;
		current_card_size[mem_block_assigned] += 52 * DECK + 2;
		current_hand_size[mem_block_assigned] += ha_size;
		current_hit_size[mem_block_assigned] += hi_size;

		game_simulated++;
		reset_for_next_game();

		insert_order_of_cards();
		shuffling();
		if (!(game_simulated & 63))
		{
			std::cout << "thread : " << (uint16_t)worker_id << " game simulated : " << game_simulated << " tested possibillity : " << tested_possibillity << endl;
		}
	}
}

inline void blackjack::evaluate_performance(uint32_t hit_table, uint32_t split_table)
{

}

inline void blackjack::insert_into_table()
{

	produced_hit[consummer_reading].reading_counter = 0;
	produced_hands[consummer_reading].reading_counter = 0;

	while (produced_hit[consummer_reading].reading_counter < produced_hit[consummer_reading].counter)
	{
		produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter = 0;
		produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].reading_counter = 0;

		while (produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter <
			produced_hit[consummer_reading].container[produced_hit[consummer_reading].counter].counter)
		{
			uint8_t t_played_hands = 0, turn_c = 0, t_hands_counter = 0, t_hands_within = 0, t_round = 0;
			uint8_t t_hand_value[26 * DECK] = {}, t_ace[26 * DECK] = {};
			uint8_t t_distrib[52 * DECK] = {}, dealers_card;
			uint8_t card_counter = 0, highest_hand = 0;

			uint8_t sum_of_identical[10] = {};

			while (card_counter < 52 * DECK - 2 * (PLAYER + 1))
			{
				// distribution
				turn_c = 0;
				while (turn_c <= PLAYER)
				{
					t_hand_value[t_played_hands + turn_c] = produced_card[consummer_reading][produced_card_counter + card_counter];
					t_ace[t_played_hands + turn_c] = (produced_card[consummer_reading][produced_card_counter + card_counter] == 11);

					t_distrib[2 * (t_played_hands + turn_c)] = produced_card[consummer_reading][produced_card_counter + card_counter];
					sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
					produced_card_counter++;

					if (produced_card[consummer_reading][produced_card_counter + card_counter] == 11)
					{
						if (t_hand_value[t_played_hands + turn_c] == 11)
						{
							t_hand_value[t_played_hands + turn_c] += 1;
						}
						else
						{
							t_hand_value[t_played_hands + turn_c] += 11;
							t_ace[t_played_hands + turn_c] = 1;

						}
					}
					else
					{
						t_hand_value[t_played_hands + turn_c] += produced_card[consummer_reading][produced_card_counter + card_counter];
					}


					t_distrib[2 * (t_played_hands + turn_c) + 1] = produced_card[consummer_reading][produced_card_counter + card_counter];
					sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
					produced_card_counter++;

					turn_c++;
				}

				highest_hand += t_hands_counter;
				turn_c = 0;
				t_hands_counter = 0;

				//decision
				while (turn_c < PLAYER)
				{
					if (produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
						container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] == 1)
					{

						if (t_distrib[2 * (t_played_hands + t_hands_counter) + 1] == t_distrib[2 * (t_played_hands + t_hands_counter)])
						{
							//insert into split table
							hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
								+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] + 125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] +
								5000 * sum_of_identical[2] + 1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] - 2)
								+ ((dealers_card - 2) << 1) + 0]++;
						}

						uint8_t hit_counter = 0;

						while (hit_counter < produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].
							container[produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter + t_played_hands + t_hands_counter])
						{
							t_hand_value[t_played_hands + t_hands_counter] += produced_card[consummer_reading][produced_card_counter + card_counter];
							t_ace[t_played_hands + t_hands_counter] += (produced_card[consummer_reading][produced_card_counter + card_counter] == 11);
							if (t_hand_value[t_played_hands + t_hands_counter] > 21)
							{
								if (t_ace[t_played_hands + t_hands_counter] != 0)
								{
									t_hand_value[t_played_hands + t_hands_counter] -= 10;
									t_ace[t_played_hands + t_hands_counter]--;
								}
							}
							//insert into hit table
							if (t_hand_value[t_played_hands + t_hands_counter] > 11 && t_hand_value[t_played_hands + t_hands_counter] < 21)
							{
								hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
									+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] +
									125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] + 5000 * sum_of_identical[2] +
									1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_hand_value[t_played_hands + t_hands_counter] - 11) + ((dealers_card - 2) << 1) + 1]++;
							}
							sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
							produced_card_counter++;
							hit_counter++;
						}
						//insert into hit table
						if (t_hand_value[t_played_hands + t_hands_counter] > 11 && t_hand_value[t_played_hands + t_hands_counter] < 21)
						{
							hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
								+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] +
								125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] + 5000 * sum_of_identical[2] +
								1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_hand_value[t_played_hands + t_hands_counter] - 11) + ((dealers_card - 2) << 1) + 0]++;
						}


						t_hands_counter++;
					}
					else
					{
						for (uint8_t move = highest_hand - 1; move > (t_played_hands + t_hands_counter); move--)// moving all of the superior hands to make room for the hands to be splitted
						{
							t_hand_value[move + produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] - 1] = t_hand_value[move];
							t_ace[move + produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] - 1] = t_ace[move];

							t_distrib[2 * (move + produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] - 1)] = t_distrib[2 * move];
							t_distrib[2 * (move + produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] - 1) + 1] = t_distrib[2 * move + 1];
						}

						t_hands_within = 1;

						while (t_hands_within < produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
							container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c])// splitting the ahnds
						{
							t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];
							t_hand_value[t_played_hands + t_hands_within + t_hands_counter] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];
							t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)];



							t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)] == 11);
							t_ace[t_played_hands + t_hands_within + t_hands_counter] = (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)] == 11);


							//insert into split table
							hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
								+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] + 125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] +
								5000 * sum_of_identical[2] + 1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] - 2)
								+ ((dealers_card - 2) << 1) + 1]++;


							t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] += produced_card[consummer_reading][produced_card_counter + card_counter];

							if (t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] == 22)
							{
								t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] -= 10;
							}
							else if (produced_card[consummer_reading][produced_card_counter + card_counter] == 11)
							{
								t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = 1;
							}

							t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] = produced_card[consummer_reading][produced_card_counter + card_counter];
							sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
							produced_card_counter++;


							t_hand_value[t_played_hands + t_hands_within + t_hands_counter] += produced_card[consummer_reading][produced_card_counter + card_counter];
							if (t_hand_value[t_played_hands + t_hands_within + t_hands_counter] == 22)
							{
								t_hand_value[t_played_hands + t_hands_within + t_hands_counter] -= 10;
							}
							else if (produced_card[consummer_reading][produced_card_counter + card_counter] == 11)
							{
								t_ace[t_played_hands + t_hands_within + t_hands_counter] = 1;
							}

							t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] = produced_card[consummer_reading][produced_card_counter + card_counter];

							sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
							produced_card_counter++;

							if (produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] > (t_hands_within + 1))
							{
								if (t_hands_counter < (produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
									container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c] - 1))
								{
									if (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] == t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1)])
									{
										uint8_t temp;
										temp = t_ace[t_played_hands + t_hands_within + t_hands_counter - 1];
										t_ace[t_played_hands + t_hands_within + t_hands_counter - 1] = t_ace[t_played_hands + t_hands_within + t_hands_counter];
										t_ace[t_played_hands + t_hands_within + t_hands_counter] = temp;

										temp = t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1];
										t_hand_value[t_played_hands + t_hands_within + t_hands_counter - 1] = t_hand_value[t_played_hands + t_hands_within + t_hands_counter];
										t_hand_value[t_played_hands + t_hands_within + t_hands_counter] = temp;

										temp = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1];
										t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter - 1) + 1] = t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1];
										t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] = temp;
									}
								}
							}

							t_hands_within++;
						}
						t_hands_within--;
						if (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] == t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)])
						{
							//insert into split table
							hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
								+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] + 125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] +
								5000 * sum_of_identical[2] + 1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] - 2)
								+ ((dealers_card - 2) << 1) + 0]++;
						}
						t_hands_within--;
						if (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter) + 1] == t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)])
						{
							//insert into split table
							hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
								+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] + 125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] +
								5000 * sum_of_identical[2] + 1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_distrib[2 * (t_played_hands + t_hands_within + t_hands_counter)] - 2)
								+ ((dealers_card - 2) << 1) + 0]++;
						}
						t_hands_within = 0;

						while (t_hands_within < produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
							container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c])// playing the hands
						{
							uint8_t hit_counter = 0;
							while (hit_counter < produced_hands[consummer_reading].container[produced_hands[consummer_reading].reading_counter].container[produced_hands[consummer_reading].
								container[produced_hands[consummer_reading].reading_counter].reading_counter + t_round * PLAYER + turn_c])
							{

								t_hand_value[t_played_hands + t_hands_counter] += produced_card[consummer_reading][produced_card_counter + card_counter];
								t_ace[t_played_hands + t_hands_counter] += (produced_card[consummer_reading][produced_card_counter + card_counter] == 11);

								if (t_hand_value[t_played_hands + t_hands_counter] > 21)
								{
									if (t_ace[t_played_hands + t_hands_counter] != 0)
									{
										t_hand_value[t_played_hands + t_hands_counter] -= 10;
										t_ace[t_played_hands + t_hands_counter]--;
									}
								}
								//insert into hit table
								if (t_hand_value[t_played_hands + t_hands_counter] > 11 && t_hand_value[t_played_hands + t_hands_counter] < 21)
								{
									hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
										+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] +
										125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] + 5000 * sum_of_identical[2] +
										1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_hand_value[t_played_hands + t_hands_counter] - 11) + ((dealers_card - 2) << 1) + 1]++;
								}
								sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
								produced_card_counter++;
								hit_counter++;
							}
							//insert into hit table
							if (t_hand_value[t_played_hands + t_hands_counter] > 11 && t_hand_value[t_played_hands + t_hands_counter] < 21)
							{
								hit_table[390625000 * sum_of_identical[9] + 78125000 * sum_of_identical[8] + 15625000 * sum_of_identical[7]
									+ 3125000 * sum_of_identical[6] + 625000 * sum_of_identical[5] +
									125000 * sum_of_identical[4] + 25000 * sum_of_identical[3] + 5000 * sum_of_identical[2] +
									1000 * sum_of_identical[1] + 200 * sum_of_identical[0] + 20 * (t_hand_value[t_played_hands + t_hands_counter] - 11) + ((dealers_card - 2) << 1) + 0]++;
							}
							t_hands_within++;
							t_hands_counter++;
						}
						highest_hand++;
					}

					turn_c++;
				}

				//dealers trun
				for (uint8_t hit_counter = 0; hit_counter < produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].
					container[produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter + t_played_hands + t_hands_counter]; hit_counter++)
				{
					sum_of_identical[produced_card[consummer_reading][produced_card_counter + card_counter] - 2]++;
					produced_card_counter++;
				}
				t_hands_counter++;

				//verification
				t_played_hands += t_hands_counter;
				t_round++;
			}
			produced_hit[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter += t_played_hands;
			produced_hands[consummer_reading].container[produced_hit[consummer_reading].reading_counter].reading_counter += t_round + 1;
		}
		produced_hit[consummer_reading].reading_counter++;
		produced_hands[consummer_reading].reading_counter++;
	}

}

inline void blackjack::consume()
{

}
inline void blackjack::insert_game_data(array_management<uint32_t, uint32_t, uint32_t> &v, array_management<uint8_t, uint32_t, uint32_t> &t)
{
	v.container[v.counter] = t.max;
	v.counter++;
	v.if_too_big_resize(t.max + 1);
	for (uint8_t i = 0; i <= t.max; i++)
	{
		v.container[v.counter] = t.container[i];
		v.counter++;
	}
	t.reset_state();
}

inline void blackjack::insert_order_of_cards()
{
	played_order_of_cards.if_too_big_resize(13 * DECK);

	for (uint16_t i = 0; i < 13 * DECK; i++)
	{
		played_order_of_cards.container[played_order_of_cards.counter] = card[i << 2] << 12;
		played_order_of_cards.container[played_order_of_cards.counter] |= card[(i << 2) + 1] << 8;
		played_order_of_cards.container[played_order_of_cards.counter] |= card[(i << 2) + 2] << 4;
		played_order_of_cards.container[played_order_of_cards.counter] |= card[(i << 2) + 3];
		played_order_of_cards.counter++;
	}
}

int main()
{
	blackjack::hit_table = new uint32_t[6640625000]();
	blackjack::split_table = new uint32_t[6640625000]();
	blackjack *game;

	game = new(std::nothrow) blackjack[WORKERS + CONSUMMER];//THREAD_LIMIT + 1 because of te main thread
	for (uint8_t i = 0; i < WORKERS + CONSUMMER; i++)
	{
		game[i].worker_id = i;
		game[i].random = rand() % 1000000000 + 1;
	}



	for (uint8_t i = 0; i < WORKERS; i++)
	{
		game[i].shuffling();
	}
	for (uint8_t i = 0; i < WORKERS + CONSUMMER + 1; i++)
	{
		for (uint8_t j = 0; j < i; j++)
		{
			uint8_t k = 0, l = 0;
			while (l < 52 * DECK)
			{
			start:
				if (game[i].card[l] == game[j].card[l])
				{
					k++;
					if (k >= 25)
					{
						game[i].random += rand() % 1000000000 + 1;
						for (uint32_t a = 0; game[i].random == 0; a++)
						{
							game[i].random += rand() % 1000000000 + 1;
						}
						game[i].shuffling();
						l = 0;
						k = 0;
						goto start;
					}
				}
				else
				{
					k = 0;
				}
				l++;
			}
			std::cout << "game: " << (uint16_t)i << " is correctly shuffled" << endl;
		}
	}

#if(WORKERS > 1)
	std::thread T[WORKERS];

	for (uint16_t i = 0; i < WORKERS; i++)
	{
		T[i] = game[i].simulation_thread();
	}
#endif
	game[WORKERS + CONSUMMER - 1].simulation_();

#if(WORKERS > 1)
	for (uint16_t i = 0; i < WORKERS; i++)
	{
		T[i].join();
	}
#endif

	std::cout << "end" << endl;
	return 0;
}