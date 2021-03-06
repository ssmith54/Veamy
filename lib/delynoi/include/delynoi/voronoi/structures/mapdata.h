#ifndef DELYNOI_MAPDATA_H
#define DELYNOI_MAPDATA_H

#include <functional>
#include <delynoi/models/basic/IndexSegment.h>

/*
 * Structure used as a key for the EdgeData map (used for the Voronoi diagram generation algorithm)
 */
struct Key {
    int first;
    int second;

    Key(int f, int s){first=f; second=s;}

    bool operator==(const Key &other) const{
        return first == other.first && second == other.second ||
               first == other.second && second == other.first;
    }
};

/*
 * Structure that is used to compute the hash value of the Key structure
 */
struct KeyHasher {
    std::size_t operator()(const Key &k) const {
        using std::size_t;
        using std::hash;

        return utilities::hash32(k.first) + utilities::hash32(k.second);
    }
};

/*
 * Structure that is used to compute the hash value of an IndexSegment, so that it can be used as key for
 * the SegmentMap
 */
struct SegmentHasher {
    std::size_t operator()(const IndexSegment &k) const {
        using std::size_t;
        using std::hash;

        return utilities::hash32(k.getFirst()) + utilities::hash32(k.getSecond());
    }
};

#endif