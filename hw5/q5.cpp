#include <cassert>
#include <iostream>
#include <algorithm>
#include <array>
#include "bitset2.h"
using std::fill;
using std::min;
using std::max;
using std::array;
using std::cout;
using std::cin;
using std::endl;
using I = std::bitset<200>;

#define ERR 200

size_t N,M;
array<I,200> adj{};

// I'm submitting the fastest code I came up with

// A bipartite graph can be solved in poly time by using Kőnig's theorem
// unfortunately the average size for a bipartite graph that we find is only ~20

I color;
I visited;
// This function will:
//   find what connected component we are in
//   determine if our connected component is bipartite
bool dfs(size_t i, const I& ud) { // assume dfs tree root is colored black (0)
   bool is_bipartite = true;
   I ud_nbrs = adj[i] & ud;
   visited._Unchecked_set(i);
   for (size_t nbr = ud_nbrs._Find_first(); nbr < ERR; nbr = ud_nbrs._Find_next(nbr))
       if (!visited._Unchecked_test(nbr)) {
           color._Unchecked_set(nbr, !color._Unchecked_test(i));
           is_bipartite &= dfs(nbr, ud);
       } else if (color._Unchecked_test(nbr) == color._Unchecked_test(i)) {
           is_bipartite = false;
       }
   return is_bipartite;
}

I bp_black;
I bp_white;
I bp_visited;
array<int,200> match;
// From geeks for geeks
bool bpm(const size_t u) {
   const I nbrs = adj[u] & bp_white;
   for (size_t v = nbrs._Find_first(); v < ERR; v = nbrs._Find_next(v)) {
       if (!bp_visited._Unchecked_test(v)) {
           bp_visited._Unchecked_set(v);
           if (match[v] < 0 || bpm(match[v])) {
               match[v] = u;
               return true;
           }
       }
   }
   return false;
}
size_t maxBPM() {
   fill(match.begin(),match.end(),-1);
   size_t result = 0;
   for (size_t u = bp_black._Find_first(); u < ERR; u = bp_black._Find_next(u)) {
       bp_visited.reset();
       result += bpm(u);
   }
   return result;
}

size_t solve(I r,  // red / included
             I ud, // undecided
             size_t& upper_bound)
{
    size_t rcount = r.count();
    if (rcount >= upper_bound)
        return upper_bound;

    size_t n = 0;

    bool changed = true;
    while (changed) {
       changed = false;
        size_t maxcontrib = 0;
        for (size_t i = ud._Find_first(); i < ERR; i = ud._Find_next(i)) {
            const size_t contrib = adj[i].count_shared(ud);

            if (contrib == 0) {
               ud._Unchecked_reset(i);
               changed = true;
               continue;
            }

            if (contrib == 1) {
                rcount += 1;
                if (rcount >= upper_bound)
                    return upper_bound;
                size_t nbr = ud.first_shared(adj[i]);
                r._Unchecked_set(nbr);
                ud._Unchecked_reset(nbr);
                ud._Unchecked_reset(i); // decide node i and its nbr
                //changed = true;
                continue;
            }

            if (contrib == 2) {
               const I ud_nbrs = adj[i] & ud;
               size_t nbr1 = ud_nbrs._Find_first();
               size_t nbr2 = ud_nbrs._Find_next(nbr1);
               if (adj[nbr1]._Unchecked_test(nbr2)) {
                   rcount += 2;
                   if (rcount >= upper_bound)
                       return upper_bound;
                   r._Unchecked_set(nbr1);
                   r._Unchecked_set(nbr2);
                   ud._Unchecked_reset(nbr1);
                   ud._Unchecked_reset(nbr2);
                   ud._Unchecked_reset(i); // decide node i and its nbr
                   changed = true;
                   continue;
               }
            }

            if (contrib > maxcontrib) {
                n = i;
                maxcontrib = contrib;
            }
        }
    }

    if (ud.none()) {
        if (rcount < upper_bound)
            upper_bound = rcount;
        return rcount;
    }

    // Do we find bipartite graphs (of size > 30) when choosing minimum contributing vertex?
    // Yes, but nothing on the cases I'm not solving.
    //      size ~50 on case 7
    //      size ~125 on case 10
    //      size ~50 on case 10
    // How about when choosing maximum contributing vertex?
    // Not really

    color.reset();
    visited.reset();
    bool is_bipartite = dfs(n,ud);
    bool disconnected = visited != ud;
    if (is_bipartite) {
       bp_white = color;
       bp_black = visited & ~color;
       rcount += maxBPM();
       if (rcount >= upper_bound)
           return rcount;
       if (!disconnected) {
           if (rcount < upper_bound)
               upper_bound = rcount;
           return rcount;
       }
    }

    // Solve disconnected components separately
    // Instead of enumerating all components in this recursive call, it is easier to just
    // solve one of the connected components and then recurse on the rest.
    if (disconnected) {
       I nv = ud ^ visited;
       size_t _upper_bound = upper_bound; // an upperbound in the subcomponents is not an upperbound in the whole graph
       if (!is_bipartite) // if we are bipartite then we've already solved this component
           rcount += solve(I(0),visited,_upper_bound);

       if (rcount >= upper_bound)
           return rcount;

       _upper_bound = upper_bound;
       rcount += solve(I(0),nv,_upper_bound);

       if (rcount < upper_bound)
           upper_bound = rcount;
       return rcount;
    }

    size_t with = ERR;
    size_t without = ERR;

    ud._Unchecked_reset(n);

    // guess red if not all nbrs are red
    if ((adj[n]&r)!=adj[n]) {
        r._Unchecked_set(n);
        with = solve(r, ud, upper_bound);
        r._Unchecked_reset(n);
    }

    // guess white
    r |= adj[n];
    ud &= ~adj[n];
    without = solve(r, ud, upper_bound);

    return min(with, without);
}

int main() {
    cin >> N >> M;
    size_t v1,v2;
    for (size_t i = 0; i < M; ++i) {
        cin >> v1 >> v2;
        adj[v1].set(v2);
        adj[v2].set(v1);
    }

    // The size of a maximal matching is at most twice the size of the optimal solution.
    // This gives an initial upper bound and lower bound.
    I mm(0);
    for (size_t i = 0; i < ERR; i++) {
        if (mm._Unchecked_test(i))
            continue;
        for (size_t j = adj[i]._Find_first(); j < ERR; j = adj[i]._Find_next(j))
            if (!mm._Unchecked_test(j)) {
                mm._Unchecked_set(i);
                mm._Unchecked_set(j);
                break;
            }
    }

    I r;
    I ud = ~((~I(0))<<N);

    size_t upper_bound = mm.count();
    cout << solve(r,ud,upper_bound) << endl;
}
