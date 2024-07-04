#include <iostream>
#include <vector>
#include <algorithm>

/// <summary>
/// Fill the provided vector with randomly generated values in [1,6] representing dice rolls.
/// </summary>
/// <param name="vector"> The vector whose values we are to fill. </param>
void fill_vector_with_dice_rolls( std::vector<uint8_t>& vector )
{
	for ( size_t i = 0; i < vector.size(); ++i )
	{
		// Randomly generate a result in the range [1,6]
		vector[i] = 1 + (rand() % 6);
	}
}

/// <summary>
/// Calculates the result of a single battle by comparing the attackers vs defenders dice rolls. Applies the resulting
/// casualties to each side.
/// </summary>
/// <param name="defender_dice_rolls"> The dice rolls generated for the defenders. </param>
/// <param name="attacker_dice_rolls"> The dice rolls generated for the attackers. </param>
/// <param name="num_defenders_left"> Total  number of defenders remaining. </param>
/// <param name="num_attackers_left"> Total number of attacers remaining. </param>
void calculate_and_apply_single_battle_result(const std::vector<uint8_t>& defender_dice_rolls,
	const std::vector<uint8_t>& attacker_dice_rolls,
	int32_t& num_defenders_left,
	int32_t& num_attackers_left)
{
	// Calculate how many dice rolls to ignore
	const size_t num_battles = std::min(defender_dice_rolls.size(), attacker_dice_rolls.size());
	for (size_t i = 0; i < num_battles; ++i)
	{
		// The attackers dice roll must be higher than the defenders to inflict a casualty
		if (attacker_dice_rolls[i] > defender_dice_rolls[i])
		{
			--num_defenders_left;
		}
		else
		{
			--num_attackers_left;
		}
	}
}

/// <summary>
/// Run the provided number of simulations to calculate an approximate probability of victory for the attacker by
/// generating a sequence of random dice rolls.
/// </summary>
/// <param name="num_simulations"> The number of simulations to run, a higher result will be more accurate but slower. </param>
/// <param name="num_defenders"> The number of defenders present at the start of the battle. </param>
/// <param name="num_attackers"> The number of attackers present at the start of the balle. </param>
/// <returns> Probability that the attackers will win. </returns>
float calculate_probability_of_victory( const int32_t num_simulations, const int32_t num_defenders, const int32_t num_attackers)
{
	// Run a number of simulations, and see how many result in a victor for the attackers
	int32_t num_victories{ 0 };
	for (int32_t i = 0; i < num_simulations; ++i)
	{
		int32_t num_defenders_left = num_defenders;
		int32_t num_attackers_left = num_attackers;

		// Run a single simulation, until one side is victorious
		while (num_defenders_left > 0 && num_attackers_left > 0)
		{
			// Calculate number of dice to roll
			const uint8_t num_attackers_dice = std::clamp(num_attackers_left, 0, 3);
			const uint8_t num_defenders_dice = std::clamp(num_defenders_left, 0, 2);

			// Generate attacker dice rolls, sorted in descending order
			std::vector<uint8_t> attacker_dice_rolls(num_attackers_dice);
			fill_vector_with_dice_rolls(attacker_dice_rolls);
			std::sort(attacker_dice_rolls.begin(), attacker_dice_rolls.end(), std::greater<>());

			// Generate defender dice rolls, sorted in descending order
			std::vector<uint8_t> defender_dice_rolls(num_defenders_dice);
			fill_vector_with_dice_rolls(defender_dice_rolls);
			std::sort(defender_dice_rolls.begin(), defender_dice_rolls.end(), std::greater<>());

			// Calculate losses to defenders and attackers for these dice rolls
			calculate_and_apply_single_battle_result(defender_dice_rolls, attacker_dice_rolls, num_defenders_left, num_attackers_left);

		}

		// Check if this was a victory for the attackers or the defenders
		if (num_attackers_left > 0)
		{
			++num_victories;
		}
	}

	const float probability_of_victory = static_cast<float>(num_victories) / static_cast<float>(num_simulations);
	return probability_of_victory;
}

int main() {

	srand(uint32_t(time(0)));

	std::printf("Enter number of simulations to run: \n");
	int32_t num_simulations{};
	std::cin >> num_simulations;

	int8_t round_num{ 1 };
	while (true)
	{
		std::printf("========================BATTLE %i ======================== \n", round_num);
		std::printf("Enter number of attackers: \n");
		int32_t num_attackers{};
		std::cin >> num_attackers;
		std::printf("Enter number of defenders: \n");
		int32_t num_defenders{};
		std::cin >> num_defenders;

		const float victory_probability = calculate_probability_of_victory(num_simulations, num_attackers, num_defenders);
		std::printf("Probability of victory is %f \n", victory_probability);

		++round_num;
	}
	
	return 0;
}
