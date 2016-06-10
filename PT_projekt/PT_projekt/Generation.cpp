#include "Generation.hpp"

#include <functional>
#include <queue>


Generation::Generation(pieces_t &pieces)
{
	createInitialIndyviduals(pieces);
	rating = calculateRating();
}

double Generation::switchToNext(const unsigned mutationCount, pieces_t &pieces)
{
	createNewIndyviduals(mutationCount, pieces);
	doSelection();
	const double newRating = calculateRating();
	const double ratingImprovement = newRating - rating;
	rating = newRating;
	return ratingImprovement;
}

double Generation::calculateRating() const
{
	double currentRating = -std::numeric_limits<double>::infinity();
	for (const auto &indyvidual : indyviduals)
		if (currentRating < indyvidual->getRating())
			currentRating = indyvidual->getRating();
	return currentRating;
}

void Generation::createInitialIndyviduals(pieces_t &pieces)
{
	for (size_t i = 0; i != INITIAL_SIZE; ++i)
		indyviduals.push_back(indyvidual_prt_t(new Indyvidual(pieces)));
}

void Generation::createNewIndyviduals(const unsigned mutationCount, pieces_t &pieces)
{
	const size_t initialSize = indyviduals.size();
	for (size_t i = 0; i != CROSSES_NUMBER; ++i)
	{
		const size_t x = rand() % initialSize;
		size_t y = rand() % (initialSize - 1);
		if (y >= x)
			++y;
		Indyvidual indyvidualCopy1 = *indyviduals[x];
		Indyvidual indyvidualCopy2 = *indyviduals[y];
		indyvidualCopy1.mutate(mutationCount);
		indyvidualCopy2.mutate(mutationCount);
		Indyvidual newIndyvidual = indyvidualCopy1 * indyvidualCopy2;
		indyviduals.push_back(indyvidual_prt_t(new Indyvidual(newIndyvidual)));
	}
	for (size_t i = 0; i != MUTATION_NUMBER; ++i)
	{
		const size_t x = rand() % initialSize;
		Indyvidual newIndyvidual = *indyviduals[x];
		newIndyvidual.mutate(mutationCount);
		indyviduals.push_back(indyvidual_prt_t(new Indyvidual(newIndyvidual)));
	}
	for (size_t i = 0; i != RANDOM_NUMBER; ++i)
		indyviduals.push_back(indyvidual_prt_t(new Indyvidual(pieces)));
}

//TODO leave a few random (not best) indyviduals
void Generation::doSelection()
{
	std::sort(indyviduals.begin(), indyviduals.end(), [](const indyvidual_prt_t &i1, const indyvidual_prt_t &i2) -> bool { return i1->getRating() < i2->getRating(); });
	indyviduals.erase(indyviduals.begin(), indyviduals.end() - MAX_SIZE);
}