#include <random>
#include <iostream>
#include <array>
#include <algorithm>
#include <chrono>
#include <vector>
#include "bitset2.h"
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::array;
using std::sort;
using std::random_device;
using std::bernoulli_distribution;
using std::uniform_int_distribution;
using std::mt19937;
using clk = std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::duration;

// Changes could be done to bitset2.h to avoid having to compute & and ~ into temporary space

// Return the float seconds represented by the input duration t
inline auto seconds(auto t) {
    return duration_cast<duration<float>>(t).count();
}

// Get an initial time point asap
const auto start = clk::now();

const double TIME_LIMIT = 3.8;
const size_t PASS_THROUGH = 8;
const size_t POPULATION_SIZE = 20;
const size_t NUM_PEOPLE = 1000;
const size_t MAX_NUM_SKILLS = 1200;

random_device rd;
mt19937 gen(rd());

using I = std::bitset<MAX_NUM_SKILLS>;

size_t N,K;
array<I,NUM_PEOPLE> person_2_skills;
array<I,MAX_NUM_SKILLS> skill_2_people;
array<I,POPULATION_SIZE> population;

#define set _Unchecked_set
#define test _Unchecked_test
#define first _Find_first
#define next _Find_next
#define first_and _Find_first_and
#define next_and _Find_next_and
#define first_or _Find_first_or
#define next_or _Find_next_or

void make_valid_cover(I& people_included, I& skills_included) {
    // Incude people who have skills that are unsatisfied
    I people_avail = ~people_included;
    I skills_not_included = ~skills_included;
    for (size_t k = skills_not_included.first(); k < K; k = skills_not_included.next(k)) {
        size_t max_person_value = 0;
        size_t max_person = 0;
        for (size_t j = people_avail.first_and(skill_2_people[k]); j < NUM_PEOPLE; j = people_avail.next_and(skill_2_people[k],j)) {
            size_t contrib = skills_not_included.count_and(person_2_skills[j]);
            if (contrib > max_person_value) {
                max_person_value = contrib;
                max_person = j;
            }
        }
        skills_not_included &= ~person_2_skills[max_person];
        people_included.set(max_person);
    }
}

I generate_by_rarity() {
    I people_avail = ~I(0);
    I skills_not_included = ~((~I(0))<<K);

    // add a bit of randomness
    bernoulli_distribution dist(2./NUM_PEOPLE);
    for (int i = 0; i < N; ++i) {
        if (dist(gen)) {
            people_avail.reset(i);
            skills_not_included &= ~person_2_skills[i];
       }
    }

    // Incude people who have skills that are unsatisfied
    while (skills_not_included.count()) {
        double max_person_value = 0;
        size_t max_person = 0;
        I max_person_contrib_skills;
        for (size_t j = people_avail.first(); j < NUM_PEOPLE; j = people_avail.next(j)) {
            double contrib = 0;
            I skills = person_2_skills[j] & skills_not_included;
            for (int k = skills.first(); k < K; k=skills.next(k))
                contrib += 1./people_avail.count_and(skill_2_people[k]);
            if (contrib > max_person_value) {
                max_person_value = contrib;
                max_person = j;
                max_person_contrib_skills = skills;
            }
        }
        skills_not_included ^= max_person_contrib_skills;
        people_avail.reset(max_person);
    }

    return ~people_avail;
}

I mate(I& x, I& y, size_t best) {
    I people_included;
    I skills_included;
    bernoulli_distribution bdist(.5*best/float(NUM_PEOPLE));
    bernoulli_distribution sdist(2./NUM_PEOPLE);
    //for (int j = 0; j < NUM_PEOPLE; ++j) {
    for (int j = x.first_or(y); j < NUM_PEOPLE; j = x.next_or(y,j)) {
        if (bdist(gen))
            people_included.set(j,x.test(j));
        if (bdist(gen))
            people_included.set(j,y.test(j));
        if (sdist(gen))
            people_included.set(j);
        if (people_included.test(j))
            skills_included |= person_2_skills[j];
    }
    make_valid_cover(people_included, skills_included);
    return people_included;
};

inline size_t fitness(I const & x) {
    return x.count();
}

inline bool less(I const & l, I const & r) {
    return fitness(l) < fitness(r);
}

int main() {
    cin >> N >> K;
    vector<I> s2p(K);
    for (size_t i = 0; i < NUM_PEOPLE; ++i) {
        size_t number_of_skills;
        cin >> number_of_skills;
        for (size_t j = 0; j < number_of_skills; ++j) {
            size_t skill;
            cin >> skill;
            s2p[skill].set(i);
        }
    }

    // Sort skills by rarity
    // TODO explain
    sort(s2p.begin(), s2p.end(), less);

    // Construct skill vs person matrix and its transpose
    for (size_t k = 0; k < K; ++k) {
        skill_2_people[k] = s2p[k];
        for (size_t i = s2p[k].first(); i < NUM_PEOPLE; i = s2p[k].next(i))
            person_2_skills[i].set(k);
    }

    // Create initial population
    for (int i = 0; i < PASS_THROUGH; ++i) {
        population[i] = generate_by_rarity();
    }
    sort(population.begin(), population.begin()+PASS_THROUGH, less);
    size_t best = fitness(population[0]);

    uniform_int_distribution<size_t> idist;
    while (seconds(clk::now() - start) < TIME_LIMIT) {
        // Mate from the best individuals to create the rest of the population
        for (int i = PASS_THROUGH; i < POPULATION_SIZE; ++i) {
            I& x = population[idist(gen) % PASS_THROUGH];
            I& y = population[idist(gen) % PASS_THROUGH];
            population[i] = mate(x,y,best);
        }
        sort(population.begin(), population.end(), less);
        best = fitness(population[0]);
    }

    cout << best << endl;
    for (int i = population[0].first(); i < NUM_PEOPLE; i = population[0].next(i))
        cout << i << " ";
    cout << endl;
}
