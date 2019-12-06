#include <cassert>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <map>
#include <algorithm>
#include <bitset>
#include <cmath>
using I = std::bitset<200>;
using std::vector;
using std::array;
using std::string;
using std::swap;
using std::cout;
using std::endl;
using std::cin;
using std::map;
using std::min;
using std::max;
using std::sort;

void remove(vector<I>& p, int i) {
    p.erase(p.begin()+i);
}

void find_covered_and_subsets(vector<I>& p, I& included, I& needed, int& N, int K) {
    for (int i = 0; i < int(p.size()) && p.size() > 0; ++i) {
        bool removed = false;
        const auto my_contrib = p[i] & needed;
        if ((my_contrib & included) == my_contrib) { // if union of prev chosen covers me then exclude me
            removed = true;
        } else {
            for (int j = i+1; j < p.size(); ++j) {
                if ((my_contrib & p[j]) == my_contrib) {
                    removed = true;
                    break;
                }
            }
        }
        if (removed) {
            remove(p,i);
            i -= 1;
        }
    }
}

void calc_unique(vector<I>& p, I& needed, I& seen, I& unique) {
    for (int j = 0; j < p.size(); ++j) {
        auto pjn = p[j]&needed;
        // This comes from solving a karnaugh map
        unique = ((~pjn)&unique) | (pjn&(~seen));
        seen |= pjn;
    }
}

void include_nec(vector<I>& p, I& included, I& needed, I& unique, int& np, int& N, int K) {
    for (int j = 0; j < (int)p.size() && p.size() > 0;++j) {
        auto pjn = p[j]&needed;
        if ((pjn & unique).any()) {
            included |= p[j];
            needed &= ~pjn;
            //unique &= ~pjn; // don't need to do this
            remove(p,j);
            np += 1;
            j -= 1;
        }
    }
}

int solve(vector<I> p, I included, I needed, int N, int K, int np, int& best) {
    #define ERR N

    if (np >= best)
        return np;
    if (needed.none()) {
        if (np < best)
            best = np;
        return np;
    }
    if (p.size() <= 0)
        return ERR;

    find_covered_and_subsets(p,included,needed,N,K);
    I seen;
    I unique;
    calc_unique(p,needed,seen,unique);
    // Include a person if they must be included
    include_nec(p,included,needed,unique,np,N,K);

    // Check if some skill is unsatisfied
    if ((needed&seen) != needed) { // seen is always subset of needed
        return ERR;
    }

    if (needed.none()) {
        if (np < best)
            best = np;
        return np;
    }
    if (p.size() <= 0)
        return ERR;

    int max_i = 0;
    auto max_num_satisfied = 0;
    for (int j = 0; j < (int)p.size(); ++j) {
        auto pjn = (p[j]&needed);
        auto num_satisfied = pjn.count();
        if (num_satisfied >= max_num_satisfied ) {
            max_i = j;
            max_num_satisfied = num_satisfied;
        }
    }
    if (max_num_satisfied != 0) {
        int min_num_additions = needed.count() / max_num_satisfied + ((needed.count() % max_num_satisfied) == 0 ? 0 : 1);
        if (np + min_num_additions >= best)
            return best;
    }  else {
        return ERR;
    }

    I next = p[max_i];
    remove(p,max_i);
    int with = solve(p, included | next, needed & (~next), N, K, np+1, best);
    int without = solve(p, included, needed, N, K, np, best);
    best = min(with,without);
    return best;
}

int main() {
    int N,K;
    cin >> N >> K;

    vector<I> persons;
    for (int i = 0; i < N; ++i) {
        int Pi;
        cin >> Pi;
        if (Pi == 0)
            continue;
        I my_skills;
        for (int j = 0; j < Pi; ++j) {
            string skill;
            cin >> skill;
            my_skills.set(skill);
        }
        persons.push_back(my_skills);
    }

    auto all = ~((~I(0))<<K);
    auto needed = ~((~I(0))<<K);

    sort(persons.begin(), persons.end(), less);

    auto best = fitness(population[0]);

    cout << solve(persons, all, needed&(~all), N, K, 0, best) << endl;
}
